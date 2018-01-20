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

Report Command::execute(Machine* machine, int32& id) {

  // TODO: Functions to execute each type of command.
  switch (type_) {
    case JMP::kCommandType_None: { return kReport_NoErrors; } 
    case JMP::kCommandType_PushToTheStack: { return executePushToTheStack(machine, id); }
    case JMP::kCommandType_Addition: { return executeAddition(machine, id); }
    case JMP::kCommandType_Substraction: { return executeAddition(machine, id); }
    case JMP::kCommandType_Multiply: { return executeAddition(machine, id); }
    case JMP::kCommandType_Division: { return executeAddition(machine, id); }
    case JMP::kCommandType_Power: { return executeAddition(machine, id); }
    case JMP::kCommandType_EqualAssigment: { return executeAddition(machine, id); }
    case JMP::kCommandType_GreaterThanComparison: { return executeAddition(machine, id); }
    case JMP::kCommandType_LowerThanComparison: { return executeAddition(machine, id); }
    case JMP::kCommandType_FunctionDefinition: { return executeAddition(machine, id); }
    case JMP::kCommandType_FunctionCall: { return executeAddition(machine, id); }
    case JMP::kCommandType_FunctionReturn: { return executeAddition(machine, id); }
    case JMP::kCommandType_FunctionNumParameters: { return executeAddition(machine, id); }
    case JMP::kCommandType_FunctionParameter: { return executeAddition(machine, id); }
    case JMP::kCommandType_Finished: { return executeAddition(machine, id); }
    case JMP::kCommandType_Started: { return executeAddition(machine, id); }
    case JMP::kCommandType_ConditionToEvaluate: { return executeAddition(machine, id); }
    case JMP::kCommandType_VariableDefinition: { return executeAddition(machine, id); }
    case JMP::kCommandType_Loop: {} 
  }

  return kReport_InvalidCommandType;
}

Report Command::executeAddition(Machine* machine, int32 & next_cmd_id) {
  return Report();
}

Report Command::executeSubstraction(Machine* machine, int32 & next_cmd_id) {
  return Report();
}
Report Command::executeMultiply(Machine* machine, int32 & next_cmd_id) {
  return Report();
}
Report Command::executeDivision(Machine* machine, int32 & next_cmd_id) {
  return Report();
}
Report Command::executePower(Machine* machine, int32 & next_cmd_id) {
  return Report();
}
Report Command::executeEqualAssignment(Machine* machine, int32 & next_cmd_id) {
  return Report();
}
Report Command::executeGreaterThan(Machine* machine, int32 & next_cmd_id) {
  return Report();
}
Report Command::executeLowerThan(Machine* machine, int32 & next_cmd_id) {
  return Report();
}
Report Command::executePushToTheStack(Machine* machine, int32 & next_cmd_id) {
  return Report();
}
Report Command::executeFunctionDefinition(Machine* machine, int32 & next_cmd_id) {
  return Report();
}
Report Command::executeFunctionCall(Machine* machine, int32 & next_cmd_id) {
  return Report();
}




}; /* JMP */