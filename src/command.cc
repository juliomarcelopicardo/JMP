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
    case JMP::kCommandType_Substraction: { return executeSubstraction(machine, id); }
    case JMP::kCommandType_Multiply: { return executeMultiply(machine, id); }
    case JMP::kCommandType_Division: { return executeDivision(machine, id); }
    case JMP::kCommandType_Power: { return executePower(machine, id); }
    case JMP::kCommandType_EqualAssigment: { return executeEqualAssignment(machine, id); }
    case JMP::kCommandType_GreaterThanComparison: { return executeGreaterThan(machine, id); }
    case JMP::kCommandType_LowerThanComparison: { return executeLowerThan(machine, id); }
    case JMP::kCommandType_FunctionDefinition: { return executeFunctionDefinition(machine, id); }
    case JMP::kCommandType_FunctionCall: {  }
    case JMP::kCommandType_FunctionReturn: { return executeFunctionReturn(machine, id); }
    case JMP::kCommandType_FunctionNumParameters: {  }
    case JMP::kCommandType_FunctionParameter: {  }
    case JMP::kCommandType_FinishedConditionalOrLoop: { return executeFinishedConditionalOrLoop(machine, id); }
    case JMP::kCommandType_Started: {  }
    case JMP::kCommandType_ConditionToEvaluate: {  }
    case JMP::kCommandType_VariableDefinition: {  }
    case JMP::kCommandType_LoopStartPoint: { return executeLoopStartPoint(machine, id); }
  }

  return kReport_InvalidCommandType;
}

Report Command::executeAddition(Machine* machine, int32& next_cmd_id) {
  // Take the last members pushed in the stack
  Value second = machine->getAndRemoveTheLastAddedStackValue();
  Value first = machine->getAndRemoveTheLastAddedStackValue();
  machine->addValueToTheStack(first + second);
  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}

Report Command::executeSubstraction(Machine* machine, int32& next_cmd_id) {
  // Take the last members pushed in the stack
  Value second = machine->getAndRemoveTheLastAddedStackValue();
  Value first = machine->getAndRemoveTheLastAddedStackValue();
  machine->addValueToTheStack(first - second);
  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}
Report Command::executeMultiply(Machine* machine, int32& next_cmd_id) {
  // Take the last members pushed in the stack
  Value second = machine->getAndRemoveTheLastAddedStackValue();
  Value first = machine->getAndRemoveTheLastAddedStackValue();
  machine->addValueToTheStack(first * second);
  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}
Report Command::executeDivision(Machine* machine, int32& next_cmd_id) {
  // Take the last members pushed in the stack
  Value second = machine->getAndRemoveTheLastAddedStackValue();
  Value first = machine->getAndRemoveTheLastAddedStackValue();
  machine->addValueToTheStack(first / second);
  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}
Report Command::executePower(Machine* machine, int32& next_cmd_id) {
  // Take the last members pushed in the stack
  Value second = machine->getAndRemoveTheLastAddedStackValue();
  Value first = machine->getAndRemoveTheLastAddedStackValue();
  machine->addValueToTheStack(first ^ second);
  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}
Report Command::executeEqualAssignment(Machine* machine, int32& next_cmd_id) {
  Variable* variable = machine->getVariable(name_);
  if (!variable) {
    ReportError(" Unable to find variable name: " + name_);
    return kReport_ExpectingNameOfVariable;
  }
  // Assign the last value of the stack to the variable.
  variable->setValue(machine->getAndRemoveTheLastAddedStackValue());
  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}
Report Command::executeGreaterThan(Machine* machine, int32& next_cmd_id) {
  // Take the last members pushed in the stack
  Value second = machine->getAndRemoveTheLastAddedStackValue();
  Value first = machine->getAndRemoveTheLastAddedStackValue();
  machine->addValueToTheStack(first > second);
  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}
Report Command::executeLowerThan(Machine* machine, int32& next_cmd_id) {
  // Take the last members pushed in the stack
  Value second = machine->getAndRemoveTheLastAddedStackValue();
  Value first = machine->getAndRemoveTheLastAddedStackValue();
  machine->addValueToTheStack(first < second);
  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}
Report Command::executePushToTheStack(Machine* machine, int32& next_cmd_id) {
  
  // We will check if its a quote.
  switch (getNameDataType()) {
    case JMP::kValueType_None: { 
      // this means that its a name of possible variable, so we will check it.
      Variable* variable = machine->getVariable(name_);
      if (!variable) {
        ReportError(" Expecting name of variable, cannot find variable name: " + name_);
        return kReport_ExpectingNameOfVariable;
      }
      machine->addValueToTheStack(variable->getValue());
    } break;
    case JMP::kValueType_Float: { 
      machine->addValueToTheStack((float32)atof(name_.c_str()));
    } break;
    case JMP::kValueType_Integer: { 
      machine->addValueToTheStack((int32)atoi(name_.c_str()));
    } break;
    case JMP::kValueType_Text: { 
      std::string temp = name_;
      // remove quotes.
      temp.erase(0, 1);
      temp.erase(temp.size() - 1, 1);
      machine->addValueToTheStack({ temp.c_str() });
    } break;
  }

  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}


Report Command::executeFunctionDefinition(Machine* machine, int32& next_cmd_id) {
  next_cmd_id++; // Jump to the next command
  machine->addFunction(next_cmd_id);
  return kReport_NoErrors;
}

Report Command::executeFunctionCall(Machine* machine, int32& next_cmd_id) {
  return Report();
}

Report Command::executeFunctionReturn(Machine* machine, int32& next_cmd_id) {

  Report report = kReport_NoErrors;
  // We will check if theres any fu
  Function* function = machine->getCurrentFunction();
  if (function == nullptr) {
    report = kReport_ReturnCalledWithoutAnyActiveFunction;
    PrintReport(report);
    return report;
  }

  // if theres an active function we will come back to the origin command where it was called.
  next_cmd_id = function->originID();
  // if its the last active function, then we finish the execution.
  if (machine->numActiveFunctions() == 1) {
    report = kReport_LastActiveFunctionReturnCalled; // This will end the execution without errors.
  }
  machine->removeCurrentFunction();
  return report;
}



Report Command::executeFinishedConditionalOrLoop(Machine* machine, int32& next_cmd_id) {
  
  if (name_ == "conditional") {
    next_cmd_id++; // jump to the next command on the list
    return kReport_NoErrors;
  }
  if (name_ == "loop") {
    // if its the end of a loop, we will step back again to check the loop condition.
    for (int32 i = next_cmd_id; i >= 0; i--) {
      if (machine->getCommand(i).type_ == kCommandType_LoopStartPoint) {
        next_cmd_id = i; // Next step will be the init of the loop.
        return kReport_NoErrors;
      }
    }
  }

  // if theres any error, we will jump to the next instruction.
  next_cmd_id++;
  ReportWarning("Unable to execute finished conditional or loop command");
  return kReport_NoErrors;
}

Report Command::executeLoopStartPoint(Machine* machine, int32& next_cmd_id) {
  next_cmd_id++; // Just go to the next step that would be the condition check.
  return kReport_NoErrors;
}

/*******************************************************************************
***                            TYPE CHECKINGS                                ***
*******************************************************************************/


ValueType Command::getNameDataType() {
  int32 name_length = name_.size();
  if (name_length == 0) { return kValueType_None; }
  if (name_[0] == '"' && name_[name_length - 1] == '"') { return kValueType_Text; }
  if (name_[0] == '-' || isDigit(name_[0])) { // Check type of number
    int32 num_dots = 0;
    for (int32 i = 1; i < name_length; i++) {
      if (!isDigit(name_[i])) {
        if (name_[i] == '.' && num_dots == 0) {
          num_dots++;
        }
        else {
          ReportWarning(" Incorrect name of command.");
          return kValueType_None;
        }
      }
    }
    if (num_dots == 1) { return kValueType_Float; }
    else { return kValueType_Integer; }
  }
  return kValueType_None;
}

const bool Command::isDigit(const char8& character) {
  switch (character) {
  case '0': case '1': case '2': case '3': case '4':
  case '5': case '6': case '7': case '8': case '9':
    return true;
  }
  return false;
}




}; /* JMP */