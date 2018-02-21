/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP_PROJECT - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#include "machine.h"
#include "compiler.h"
#include <Windows.h>
#include <string>
#include <fstream>
#include <utility>


namespace JMP {

// Declaration of the static stack
Machine::Stack Machine::stack_; 

/*******************************************************************************
***                         CONSTRUCTOR & DESTRUCTOR                         ***
*******************************************************************************/

Machine::Machine() {
  cmd_list_.reserve(50);
  cmd_list_length_ = 0;
  function_list_.reserve(10);
  function_list_length_ = 0;
  function_registry_.reserve(5);
  function_registry_length_ = 0;
  variable_registry_.reserve(5);
  variable_registry_length_ = 0;
  defined_function_list_.reserve(10);
  defined_function_list_length_ = 0;
  global_variable_pack_list_.resize(1);
  global_variable_pack_list_[0].name = "";
  global_variable_pack_list_length_ = 1;
  current_global_variable_pack_ = 0;
  last_script_compiled_path_ = "";
}

Machine::~Machine() {
  cmd_list_.clear();
  variable_registry_.clear();
  function_registry_.clear();
  defined_function_list_.clear();
  function_list_.clear();
  cmd_list_length_ = 0;
  variable_registry_length_ = 0;
  function_registry_length_ = 0;
  defined_function_list_length_ = 0;
  function_list_length_ = 0;
  last_script_compiled_path_.clear();
  global_variable_pack_list_length_ = 0;
  current_global_variable_pack_ = 0;
  global_variable_pack_list_.clear();
}


/*******************************************************************************
***                           MAIN MACHINE METHODS                           ***
*******************************************************************************/


Report Machine::processFile(const std::string& script_filename) {

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

  Compiler compiler;
  uint32 line_num = 1;
  Report report = kReport_NoErrors;

  // Reading the whole file, allocating it using lines of code.
  while (!script.eof() && report == kReport_NoErrors) {
    std::string code_line;
    std::getline(script, code_line);

    // To prevent errors I will remove all the tabs. Changing them to blank spaces
    const uint32 string_length = code_line.length();
    for (uint32 i = 0; i < string_length; ++i) {
      if (code_line[i] == '\t') { code_line[i] = ' '; }
    }

    /*
      PROCESS THE LINES - COMPILING THEM.
    */
    report = compiler.compile(this, code_line, line_num);
    line_num++;
  }


  // If the file has been loaded properly, then we will save its path for possible reloadings.
  if (report == kReport_NoErrors) {
    last_script_compiled_path_ = script_filename;
  }

  report = runScriptToSaveGlobalVariables();
  
  script.clear();
  script.close();

  return report;
}



Report Machine::checkExtension(const std::string& filename) const {
  
  // We will take the last 4 characters to check the extension.
  const uint32 index = filename.length() - 4;
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
  Compiler other_compiler;
  Report report = other_compiler.compile(&other_machine, std::move(function_call_sentence), -1); // -1 as theres no line number
  if (report != kReport_NoErrors) { return report; }

  // Once compiled we will concatenate both machines command lists.
  // The result will be, this_machine = this_machine + other_machine;
  const int32 this_machine_initial_num_commands = numCommands();
  pushBackOtherMachineCommandList(&other_machine);
  const int32 num_commands_added = other_machine.numCommands();

  // Now we will execute the complete command list
  int32 index = this_machine_initial_num_commands;
  while (index < cmd_list_length_) {
    // possible results of the execution "NoError", "return is called" or "Error"
    report = cmd_list_[index].execute(this, index);
    if (report != kReport_NoErrors) { break; }
  }

  if (report == kReport_LastActiveFunctionReturnCalled) { report = kReport_NoErrors; }

  // Delete the commands added from the other machine.
  for (int32 i = 0; i < num_commands_added; ++i) {
    removeCommand(this_machine_initial_num_commands);
  }

  return report;
}

Report Machine::runScriptToSaveGlobalVariables() {
  Report report = kReport_NoErrors;
  Command cmd;
  int32 index = 0;
  while (index < cmd_list_length_) {
    // possible results of the execution "NoError", "return is called" or "Error"
    cmd = cmd_list_[index];
    if (cmd.type_ == kCommandType_FunctionDefinition) { // To avoid functions
      while (index < cmd_list_length_ && cmd_list_[index].type_ != kCommandType_FunctionEnd ) {
        index++;
      }
      index++;
    }
    else {
      report = cmd.execute(this, index);
      if (report != kReport_NoErrors) { break; }
    }
  }

  return report;
}

void Machine::reload() {

  // We will clean all the buffers used. We will reserve the same size it needed before.
  cmd_list_.clear();
  cmd_list_.reserve(cmd_list_length_);
  cmd_list_length_ = 0;
  function_list_.clear();
  function_list_.reserve(function_list_length_);
  function_list_length_ = 0;
  defined_function_list_.clear();
  defined_function_list_.reserve(defined_function_list_length_);
  defined_function_list_length_ = 0;
  global_variable_pack_list_.reserve(global_variable_pack_list_length_);
  addGlobalVariablePack("");

  /* 
    We wont delete the registrys, as functions or variables registered from c++
    doesnt depend on the file, is the user the one who should manage what to
    register and unregister in each moment.
  */
  /*
  function_registry_.clear();
  function_registry_.reserve(function_registry_length_);
  function_registry_length_ = 0;
  variable_registry_.clear();
  variable_registry_.reserve(variable_registry_length_);
  variable_registry_length_ = 0;
  */

  // Then we proccess the file again.
  processFile(last_script_compiled_path_);
}

std::string Machine::getCurrentScript() const{
  std::ifstream script(last_script_compiled_path_);
  std::string script_code { std::istreambuf_iterator<char>(script), std::istreambuf_iterator<char>() };
  return script_code;
}

/*******************************************************************************
***                           COMMAND LIST METHODS                           ***
*******************************************************************************/


void Machine::addCommand(const CommandType type) {
  const Command cmd = { type, "" };
  cmd_list_.push_back(cmd);
  cmd_list_length_++;
}

void Machine::addCommand(const CommandType type, const char* name) {
  const Command cmd = { type, name };
  if (cmd.name_ != "RESULT") { // Temporal tokens waiting for result no added.
    cmd_list_.push_back(cmd);
    cmd_list_length_++;
    if (cmd.type_ == kCommandType_FunctionDefinition) {
      addDefinedFunction(name, cmd_list_length_ - 1);
    }
  }
}

void Machine::addCommand(const CommandType type, const std::string& name) {
  const Command cmd = { type, name.c_str() };
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

int32 Machine::numCommands() const {
  return cmd_list_length_;
}

void Machine::pushBackOtherMachineCommandList(Machine* other_machine) {
  for (int32 i = 0; i < other_machine->numCommands(); ++i) {
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
  for (int32 i = 0; i < variable_registry_length_; ++i) {
    if (name == variable_registry_[i].name_) {
      char warning[64];
      sprintf_s(warning, 64, "\"%s\": Variable already registered", name);
      ReportWarning(warning);
      variable_registry_[i] = { name, type, ptr_to_var };
      return kReport_NoErrors;
    }
  }

  // if the variable is not registered yet, we will add a new one to the registry.
  variable_registry_.emplace_back(name, type, ptr_to_var);
  variable_registry_length_++;

  return kReport_NoErrors;
}

void Machine::unregisterVariable(const char* name) {

  for (int32 i = 0; i < variable_registry_length_; ++i) {
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

void Machine::addGlobalVariablePack(const char* name) {
  global_variable_pack_list_.push_back({ name });
  current_global_variable_pack_ = global_variable_pack_list_length_;
  global_variable_pack_list_length_++;
}

Report Machine::addGlobalVariableToCurrentPack(const Variable variable) {
  const int32 length = global_variable_pack_list_[current_global_variable_pack_].var.size();
  for (int32 i = 0; i < length; ++i) {
    if (global_variable_pack_list_[current_global_variable_pack_].var[i].name_ == variable.name_) {
      ReportError("Multiple definition of the variable: " + variable.name_);
      return kReport_VariableDefinedTwice;
    }
  }
  global_variable_pack_list_[current_global_variable_pack_].var.push_back(variable);
  return kReport_NoErrors;
}

Report Machine::addGlobalVariableToCurrentPack(const char* name, const Value& value) {
  const int32 length = global_variable_pack_list_[current_global_variable_pack_].var.size();
  for (int32 i = 0; i < length; ++i) {
    if (global_variable_pack_list_[current_global_variable_pack_].var[i].name_ == name) {
      const std::string n = name;
      ReportError("Multiple definition of the variable: " + n);
      return kReport_VariableDefinedTwice;
    }
  }
  global_variable_pack_list_[current_global_variable_pack_].var.emplace_back(name, value);
  return kReport_NoErrors;
}

void Machine::restartCurrentGlobalVariablePackIndex() {
  current_global_variable_pack_ = 0;
}

std::string Machine::getCurrentGlobalVariablePackName() {
  return global_variable_pack_list_[current_global_variable_pack_].name;
}

Variable* Machine::getVariable(const std::string& variable_name) {
  
  // 1st step will be to look for the variable into the current function scope.
  Function* function = getCurrentFunction();
  if (function) {
    Variable* variable = function->getVariable(variable_name);
    if (variable) {
      return variable;
    }
  }

  // If not found, we will look for it in the variable registry.
  for (int32 i = 0; i < variable_registry_length_; ++i) {
    if (variable_registry_[i].name_ == variable_name) {
      return &variable_registry_[i];
    }
  }

  // Looking for the variable in the global stack.
  // 1st step will be to separate the name in case that is part of a variable pack.
  // We will look for "."  --> example:  ->>>   camera.position
  int32 length = variable_name.length();
  int32 index = -1;
  for (int32 i = 0; i < length; ++i) {
    if (variable_name[i] == '.') { index = i; break; }
  }
  if (index == -1) { // No "." found
    // Look for in the default variable pack.
    length = global_variable_pack_list_[0].var.size();
    for (int32 i = 0; i < length; ++i) {
      if (global_variable_pack_list_[0].var[i].name_ == variable_name) {
        return &global_variable_pack_list_[0].var[i];
      }
    }
  }
  else { // Separate "camera.position" ->>>> to "camera"(pack) and "position"(var)
    const std::string pack_name = variable_name.substr(0, index); // not including "."
    const std::string var_name = variable_name.substr(index + 1); // to ignore "."

    // looking for the pack
    for (int32 i = 0; i < global_variable_pack_list_length_; ++i) {
      if (global_variable_pack_list_[i].name == pack_name) {
        length = global_variable_pack_list_[i].var.size();
        for (int32 j = 0; j < length; j++) {
          if (global_variable_pack_list_[i].var[j].name_ == var_name) {
            return &global_variable_pack_list_[i].var[j];
          }
        }
      }
    }
  }


  return nullptr;
}


/*******************************************************************************
***                        FUNCTION REGISTRY METHODS                         ***
*******************************************************************************/

Report Machine::registerFunction(const char* name, 
                                 void(*function_ptr)(std::vector<Value>&)) {

  // Error checkings
  if (!name || !function_ptr) {
    return kReport_NullPointer;
  }

  // If the function already exists, we will edit the existing one.
  for (int32 i = 0; i < function_registry_length_; ++i) {
    if (name == function_registry_[i].name_) {
      char warning[64];
      sprintf_s(warning, 64, "\"%s\": Function already registered", name);
      ReportWarning(warning);
      function_registry_[i].name_ = name;
      function_registry_[i].function_pointer_ = function_ptr;
      return kReport_NoErrors;
    }
  }

  // if the variable is not registered yet, we will add a new one to the registry.
  function_registry_.emplace_back(name, function_ptr);
  function_registry_length_++;

  return kReport_NoErrors;
}

void Machine::unregisterFunction(const char* name) {

  for (int32 i = 0; i < function_registry_length_; ++i) {
    if (name == function_registry_[i].name_) {
      function_registry_.erase(function_registry_.begin() + i);
      function_registry_length_--;
      return;
    }
  }
  char warning[64];
  sprintf_s(warning, 64, "\"%s\": Can't be found in the registry. Unregister failed", name);
  ReportWarning(warning);
}

void Machine::unregisterFunction(const int32 id) {
  if (id >= 0 && id < function_registry_length_) {
    function_registry_.erase(function_registry_.begin() + id);
    function_registry_length_--;
    return;
  }

  char warning[64];
  sprintf_s(warning, 64, "Invalid variable registry id. Unregister failed");
  ReportWarning(warning);
}

RegisteredFunction* Machine::getRegisteredFunction(const std::string& function_name) {

  // If not found, we will look for it in the function registry.
  for (int32 i = 0; i < function_registry_length_; ++i) {
    if (function_registry_[i].name_ == function_name) {
      return &function_registry_[i];
    }
  }

  return nullptr;
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
  for (int32 i = 0; i < defined_function_list_length_; ++i) {
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

int32 Machine::getDefinedFunctionID(const char* name) {
  const std::string function_name = name;
  for (int32 i = 0; i < defined_function_list_length_; ++i) {
    if (defined_function_list_[i].name == function_name) {
      return defined_function_list_[i].command_index;
    }
  }

  ReportError("Function not defined, so theres no ID for function name: " + function_name);
  return INVALID_FUNCTION_ID;
}

void Machine::removeDefinedFunction(const char* name) {
  for (int32 i = 0; i < defined_function_list_length_; ++i) {
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
***                     EXECUTION FUNCTION LIST METHODS                      ***
*******************************************************************************/

void Machine::addFunction(const int32 origin_id) {
  function_list_.emplace_back(origin_id);
  function_list_length_++;
}

Function* Machine::getCurrentFunction() {
  if (function_list_length_ == 0) {
    return nullptr;
  }
  return &function_list_[function_list_length_ - 1];
}

void Machine::removeFunction(const int32 id) {
  if (id >= 0 && id < function_list_length_) {
    function_list_.erase(function_list_.begin() + id);
    function_list_length_--;
  }
  else {
    ReportWarning(" Can't remove function from the execution function list.");
  }
}

void Machine::removeCurrentFunction() {
  removeFunction(function_list_length_ - 1);
}

int32 Machine::numActiveFunctions() const {
  return function_list_length_;
}

/*******************************************************************************
***                             STACK METHODS                                ***
*******************************************************************************/

void Machine::addValueToTheStack(const Value& value) const {
    stack_.value.push_back(value);
    stack_.length++;
}

Value Machine::getAndRemoveTheLastAddedStackValue() const {
  Value value;
  if (stack_.length <= 0) {
    ReportWarning(" Trying to extract a value from an empty stack.");
    return value;
  }
  value = stack_.value[stack_.length - 1];
  stack_.value.erase(stack_.value.begin() + stack_.length - 1);
  stack_.length--;
  return value;
}

int32 Machine::numStackValues() const {
  return stack_.length;
}

/*******************************************************************************
***                           VARIABLE GETTERS                               ***
*******************************************************************************/

int32 Machine::getInteger(const char* variable_name, const char* variable_pack_name) {
  VariablePack* varpack = nullptr;
  Variable* var = nullptr;
  
  for (int32 i = 0; i < global_variable_pack_list_length_; ++i) {
    if (global_variable_pack_list_[i].name == variable_pack_name) {
      varpack = &global_variable_pack_list_[i];
      break;
    }
  }

  if (!varpack) {
    ReportError("Unable to find variable pack name: " + std::string(variable_pack_name));
    return -99999;
  }

  const int32 length = varpack->var.size();
  for (int32 i = 0; i < length; ++i) {
    if (varpack->var[i].name_ == variable_name) {
      var = &varpack->var[i];
      break;
    }
  }

  if (!var) {
    ReportError("Unable to find variable name: " + std::string(variable_name));
    return -99999;
  }

  if (var->value_.type_ != kValueType_Integer) {
    ReportError("Varible name: " + std::string(variable_name) + " is not an integer");
    return -99999;
  }

  return var->getValue().integer_;
}

float32 Machine::getFloat(const char* variable_name, const char* variable_pack_name) {
  VariablePack* varpack = nullptr;
  Variable* var = nullptr;

  for (int32 i = 0; i < global_variable_pack_list_length_; ++i) {
    if (global_variable_pack_list_[i].name == variable_pack_name) {
      varpack = &global_variable_pack_list_[i];
      break;
    }
  }

  if (!varpack) {
    ReportError("Unable to find variable pack name: " + std::string(variable_pack_name));
    return -99999.0f;
  }

  const int32 length = varpack->var.size();
  for (int32 i = 0; i < length; ++i) {
    if (varpack->var[i].name_ == variable_name) {
      var = &varpack->var[i];
      break;
    }
  }

  if (!var) {
    ReportError("Unable to find variable name: " + std::string(variable_name));
    return -99999.0f;
  }

  if (var->value_.type_ != kValueType_Float) {
    ReportError("Varible name: " + std::string(variable_name) + " is not a float");
    return -99999.0f;
  }

  return var->getValue().float_;
}


std::string Machine::getString(const char* variable_name, const char* variable_pack_name) {
  VariablePack* varpack = nullptr;
  Variable* var = nullptr;

  for (int32 i = 0; i < global_variable_pack_list_length_; ++i) {
    if (global_variable_pack_list_[i].name == variable_pack_name) {
      varpack = &global_variable_pack_list_[i];
      break;
    }
  }

  if (!varpack) {
    ReportError("Unable to find variable pack name: " + std::string(variable_pack_name));
    return "ERROR";
  }

  const int32 length = varpack->var.size();
  for (int32 i = 0; i < length; ++i) {
    if (varpack->var[i].name_ == variable_name) {
      var = &varpack->var[i];
      break;
    }
  }

  if (!var) {
    ReportError("Unable to find variable name: " + std::string(variable_name));
    return "ERROR";
  }

  if (var->value_.type_ != kValueType_Text) {
    ReportError("Varible name: " + std::string(variable_name) + " is not a string");
    return "ERROR";
  }

  return var->getValue().text_;
}




}; /* JMP_PROJECT */