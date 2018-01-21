/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#include "machine.h"
#include "text_parser.h"
#include <Windows.h>
#include <string>
#include <fstream>


namespace JMP {


/*******************************************************************************
***                         CONSTRUCTOR & DESTRUCTOR                         ***
*******************************************************************************/

Machine::Machine() {
  cmd_list_length_ = 0;
  cmd_list_.reserve(50);
  stack_.reserve(50);
  function_list_.reserve(10);
  variable_registry_length_ = 0;
  defined_function_list_length_ = 0;
  stack_length_ = 0;
  function_list_length_ = 0;
}

Machine::~Machine() {
  cmd_list_.clear();
  variable_registry_.clear();
  defined_function_list_.clear();
  stack_.clear();
  function_list_.clear();
}


/*******************************************************************************
***                           MAIN MACHINE METHODS                           ***
*******************************************************************************/


Report Machine::processFile(std::string script_filename) {

  // To force .jmp extensions.
  if (checkExtension(script_filename) != kReport_NoErrors) {
    return kReport_InvalidFileExtension;
  }

  std::ifstream script; // ifstream because we just want to read the file.
  script.open(script_filename); // Default read mode is read only "in".

  if (!script) {
    ReportError("\"" + script_filename + "\" - Can't open file");
    return kReport_InvalidFilename;
  }

  TextParser parser;
  uint32 line_num = 1;
  Report report = kReport_NoErrors;

  // Reading the whole file, allocating it using lines of code.
  while (!script.eof() && report == kReport_NoErrors) {
    std::string code_line;
    std::getline(script, code_line);

    // To prevent errors I will remove all the tabs. Changing them to blank spaces
    uint32 string_length = code_line.length();
    for (uint32 i = 0; i < string_length; i++) {
      if (code_line[i] == '\t') { code_line[i] = ' '; }
    }

    /*
      PROCESS THE LINES - COMPILING THEM.
    */
    report = parser.compile(this, code_line, line_num);
    line_num++;
  }
  
  script.clear();
  script.close();

  return report;
}



Report Machine::checkExtension(std::string filename) {
  
  // We will take the last 4 characters to check the extension.
  uint32 index = filename.length() - 4;
  std::string pepe = filename.substr(index);
  if (index > 1 && filename.substr(index) == ".jmp") {
    return kReport_NoErrors;
  }
  
  // Name too short.
  if (index < 1) {
    ReportError("\"" + filename + "\" - Filename too short");
    return kReport_FilenameTooShort;
  }

  // Invalid extension.
  ReportError("\"" + filename + "\" - Extension must be .jmp");
  return kReport_InvalidFileExtension;
}

Report Machine::runFunction(std::string function_call_sentence) {

  // Create a machine and a parser to compile the code received by parameter.
  Machine other_machine;
  TextParser other_parser;
  Report report;
  report = other_parser.compile(&other_machine, function_call_sentence, -1); // -1 as theres no line number
  if (report != kReport_NoErrors) { return report; }

  // Once compiled we will concatenate both machines command lists.
  // The result will be, this_machine = this_machine + other_machine;
  int32 this_machine_initial_num_commands = numCommands();
  pushBackOtherMachineCommandList(&other_machine);
  int32 num_commands_added = other_machine.numCommands();

  // Now we will execute the complete command list
  int32 index = this_machine_initial_num_commands;
  while (index < cmd_list_length_) {
    // possible results of the execution "NoError", "return is called" or "Error"
    report = cmd_list_[index].execute(this, index);
    if (report != kReport_NoErrors) { break; }
  }

  if (report == kReport_ReturnCalled) { report = kReport_NoErrors; }

  // Delete the commands added from the other machine.
  for (int32 i = 0; i < num_commands_added; i++) {
    removeCommand(this_machine_initial_num_commands);
  }

  return report;
}

/*******************************************************************************
***                           COMMAND LIST METHODS                           ***
*******************************************************************************/


void Machine::addCommand(const CommandType type) {
  Command cmd = { type, "" };
  cmd_list_.push_back(cmd);
  cmd_list_length_++;
}

void Machine::addCommand(const CommandType type, const char* name) {
  Command cmd = { type, name };
  if (cmd.name_ != "RESULT") { // Temporal tokens waiting for result no added.
    cmd_list_.push_back(cmd);
    cmd_list_length_++;
    if (cmd.type_ == kCommandType_FunctionDefinition) {
      addDefinedFunction(name, cmd_list_length_ - 1);
    }
  }
}

void Machine::addCommand(const CommandType type, const std::string name) {
  Command cmd = { type, name.c_str() };
  if (cmd.name_ != "RESULT") {
    cmd_list_.push_back(cmd);
    cmd_list_length_++;
    if (cmd.type_ == kCommandType_FunctionDefinition) {
      addDefinedFunction(name.c_str(), cmd_list_length_ - 1);
    }
  }
}

void Machine::addCommand(const Command& command) {
  if (command.name_ != "RESULT") {
    cmd_list_.push_back(command);
    cmd_list_length_++;
    if (command.type_ == kCommandType_FunctionDefinition) {
      addDefinedFunction(command.name_.c_str(), cmd_list_length_ - 1);
    }
  }
}

void Machine::clearCommandList() {
  cmd_list_.clear();
  cmd_list_length_ = 0;
}

void Machine::removeCommand(int32 id) {
  cmd_list_.erase(cmd_list_.begin() + id);
  cmd_list_length_--;
}

Command Machine::getCommand(const int32 list_index) {
  if (list_index >= cmd_list_length_ || list_index < 0) {
    ReportError("Command list index \"" + std::to_string(list_index) + "\" is out of range.");
    return Command();
  }
  return cmd_list_[list_index];
}

const int32 Machine::numCommands() {
  return cmd_list_length_;
}

void Machine::pushBackOtherMachineCommandList(Machine* other_machine) {
  for (int32 i = 0; i < other_machine->numCommands(); i++) {
    addCommand(other_machine->getCommand(i));
  }
}


/*******************************************************************************
***                        VARIABLE REGISTRY METHODS                         ***
*******************************************************************************/

Report Machine::registerVariable(const char *name, 
                                 ValueType type, 
                                 void* ptr_to_var) {

  // Error checkings
  if (!name || !ptr_to_var) {
    return kReport_NullPointer;
  }
  
  // If the variable already exists, we will edit the existing one.
  for (int32 i = 0; i < variable_registry_length_; i++) {
    if (name == variable_registry_[i].name_) {
      char warning[64];
      sprintf_s(warning, 64, "\"%s\": Variable already registered", name);
      ReportWarning(warning);
      variable_registry_[i] = { name, type, ptr_to_var };
      return kReport_NoErrors;
    }
  }

  // if the variable is not registered yet, we will add a new one to the registry.
  variable_registry_.push_back({ name, type, ptr_to_var });
  variable_registry_length_++;

  return kReport_NoErrors;
}

void Machine::unregisterVariable(const char* name) {

  for (int32 i = 0; i < variable_registry_length_; i++) {
    if (name == variable_registry_[i].name_) {
      variable_registry_.erase(variable_registry_.begin() + i);
      variable_registry_length_--;
      return;
    }
  }
  char warning[64];
  sprintf_s(warning, 64, "\"%s\": Can't be found in the registry. Unregister failed", name);
  ReportWarning(warning);
}

void Machine::unregisterVariable(const int32 id) {
  if (id >= 0 && id < variable_registry_length_) {
    variable_registry_.erase(variable_registry_.begin() + id);
    variable_registry_length_--;
    return;
  }

  char warning[64];
  sprintf_s(warning, 64, "Invalid variable registry id. Unregister failed");
  ReportWarning(warning);
}


/*******************************************************************************
***                      DEFINED FUNCTION LIST METHODS                       ***
*******************************************************************************/

Report Machine::addDefinedFunction(const char* name, const int32 command_index) {
  // Error checkings
  if (!name) {
    return kReport_NullPointer;
  }

  // If the variable already exists, we will edit the existing one.
  for (int32 i = 0; i < defined_function_list_length_; i++) {
    if (name == defined_function_list_[i].name) {
      char error[64];
      sprintf_s(error, 64, "\"%s\": Function already defined", name);
      ReportError(error);
      return kReport_FunctionDefinedMoreThanOnce;
    }
  }

  // if the variable is not registered yet, we will add a new one to the registry.
  defined_function_list_.push_back({ name, command_index });
  defined_function_list_length_++;

  return kReport_NoErrors;
}

void Machine::removeDefinedFunction(const char* name) {
  for (int32 i = 0; i < defined_function_list_length_; i++) {
    if (name == defined_function_list_[i].name) {
      defined_function_list_.erase(defined_function_list_.begin() + i);
      defined_function_list_length_--;
      return;
    }
  }
  char warning[64];
  sprintf_s(warning, 64, "\"%s\": Is not a defined function. Remove Defined Function failed", name);
  ReportWarning(warning);
}

void Machine::removeDefinedFunction(const int32 id) {
  if (id >= 0 && id < defined_function_list_length_) {
    defined_function_list_.erase(defined_function_list_.begin() + id);
    defined_function_list_length_--;
    return;
  }

  char warning[64];
  sprintf_s(warning, 64, "Invalid function id. Remove Defined Function failed");
  ReportWarning(warning);
}

/*******************************************************************************
***                             STACK METHODS                                ***
*******************************************************************************/

void Machine::addValueToTheStack(const Value value) {
  stack_.push_back(value);
  stack_length_++;
}

Value Machine::getAndRemoveTheLastAddedStackValue() {
  Value value;
  if (stack_length_ <= 0) {
    ReportWarning(" Trying to extract a value from an empty stack.");
    return value;
  }
  value = stack_[stack_length_ - 1];
  stack_.erase(stack_.begin() + stack_length_ - 1);
  stack_length_--;
  return value;
}





}; /* JMP */