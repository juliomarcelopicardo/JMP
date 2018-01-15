/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#include "command.h"
#include "report.h"
#include <Windows.h>
#include <string>

namespace JMP {

/*******************************************************************************
***                       CONSTRUCTOR & DESTRUCTOR                           ***
*******************************************************************************/

Command::Command() {
  type_ = kCommandType_None;
  name_ = "";
}

Command::Command(const CommandType type, const char* name) {
  type_ = type;
  name_ = name;
}

Command::~Command() {}

Command::Command(const Command& copy) {
  type_ = copy.type_;
  name_ = copy.name_;
}

Command & Command::operator=(const Command& copy) {
  type_ = copy.type_;
  name_ = copy.name_;
  return *this;
}





}; /* JMP */