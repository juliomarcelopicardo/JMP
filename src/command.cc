/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#include "machine.h" // Before because of the forward declaration used in .h
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


/*******************************************************************************
***                              EXECUTION                                   ***
*******************************************************************************/

Report Command::execute(Machine* machine, int32& next_command_to_execute) {

  // TODO: Functions to execute each type of command.
  switch (type_) {
    case JMP::kCommandType_None: {} break;
    case JMP::kCommandType_Addition: {} break;
    case JMP::kCommandType_Substraction: {} break;
    case JMP::kCommandType_Multiply: {} break;
    case JMP::kCommandType_Division: {} break;
    case JMP::kCommandType_Power: {} break;
    case JMP::kCommandType_EqualAssigment: {} break;
    case JMP::kCommandType_GreaterThanComparison: {} break;
    case JMP::kCommandType_LowerThanComparison: {} break;
    case JMP::kCommandType_PushToTheStack: {} break;
    case JMP::kCommandType_FunctionDefinition: {} break;
    case JMP::kCommandType_FunctionCall: {} break;
    case JMP::kCommandType_FunctionReturn: {} break;
    case JMP::kCommandType_FunctionNumParameters: {} break;
    case JMP::kCommandType_FunctionParameter: {} break;
    case JMP::kCommandType_Finished: {} break;
    case JMP::kCommandType_Started: {} break;
    case JMP::kCommandType_ConditionToEvaluate: {} break;
    case JMP::kCommandType_VariableDefinition: {} break;
    case JMP::kCommandType_Loop: {} break;
  }

  return kReport_InvalidCommandType;
}





}; /* JMP */