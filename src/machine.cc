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
}

Machine::~Machine() {
  cmd_list_.clear();
}


/*******************************************************************************
***                           MAIN MACHINE METHODS                           ***
*******************************************************************************/


Report Machine::process(std::string script_filename) {

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


/*******************************************************************************
***                           COMMAND LIST METHODS                           ***
*******************************************************************************/

void Machine::addCommand(const char* name, const CommandType type) {
  Command cmd = { name, type };
  if (cmd.name_ != "RESULT") { // Temporal tokens waiting for result no added.
    cmd_list_.push_back(cmd);
    cmd_list_length_++;
    if (cmd.type_ == kCommandType_Function) {
      // TODO:: FUNCTION TABLES TO ALLOCATE THE DIFFERENT FUNCTIONS OF THE SCRIPT.
    }
  }
}

void Machine::addCommand(const std::string name, const CommandType type) {
  Command cmd = { name.c_str(), type };
  if (cmd.name_ != "RESULT") {
    cmd_list_.push_back(cmd);
    cmd_list_length_++;
    if (cmd.type_ == kCommandType_Function) {
      // TODO:: FUNCTION TABLES TO ALLOCATE THE DIFFERENT FUNCTIONS OF THE SCRIPT.
    }
  }
}

void Machine::addCommand(const Command& command) {
  if (command.name_ != "RESULT") {
    cmd_list_.push_back(command);
    cmd_list_length_++;
    if (command.type_ == kCommandType_Function) {
      // TODO:: FUNCTION TABLES TO ALLOCATE THE DIFFERENT FUNCTIONS OF THE SCRIPT.
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

const uint32 Machine::numCommands() {
  return cmd_list_length_;
}

}; /* JMP */