/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP_PROJECT - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#ifndef __JMP_COMMAND_H__
#define __JMP_COMMAND_H__ 1

#include "types.h"
#include "value.h"
#include <string>

namespace JMP {

/// Command type.
enum CommandType {
  kCommandType_None = 0,
  // Separators -> mathematical operations
  kCommandType_Addition,
  kCommandType_Substraction,
  kCommandType_Multiply,
  kCommandType_Division,
  kCommandType_Power,
  // Separator -> "=" Assignment.
  kCommandType_EqualAssigment, 
  // Separator -> Comparisons.
  kCommandType_GreaterThanComparison,
  kCommandType_LowerThanComparison,
  kCommandType_GreaterOrEqualThanComparison,
  kCommandType_LowerOrEqualThanComparison,
  kCommandType_EqualThanComparison,
  kCommandType_NotEqualThanComparison,
  // Stack Actions
  kCommandType_PushToTheStack,
  // Function
  kCommandType_FunctionDefinition,
  kCommandType_FunctionCall,
  kCommandType_FunctionReturn,
  kCommandType_FunctionEnd,
  kCommandType_FunctionNumParameters,
  kCommandType_FunctionParameter,
  // Body state -> Conditional, loop iteration or function body ended.
  kCommandType_FinishedConditionalOrLoop,
  // Condition Evaluation, can be the condition of a loop or an if.
  kCommandType_ConditionToEvaluate,
  // Variable definition
  kCommandType_VariableDefinition,
  // Variable pack
  kCommandType_VariablePackDefinition,
  kCommandType_VariablePackEnd,
  // Loop
  kCommandType_LoopStartPoint,
};

/// Class used to save and manage the compiling commands or instructions.
class Command {

 public: 

/*******************************************************************************          
***                             PUBLIC METHODS                               ***
*******************************************************************************/

  /// Default class constructor.
  Command();
  /// Default class constructor.
  Command(CommandType type, const char* name);
  /// Default class destructor.
  ~Command();
  /// Default copy constructor.
  Command(const Command& copy);
  /// Assignment operator.
  Command& operator=(const Command& copy);



/*******************************  EXECUTION  **********************************/

  /**
  * @brief Executes a command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report execute(class Machine* machine, int32& next_cmd_id);

  /**
  * @brief Executes the Addition command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeAddition(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the Substraction command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeSubstraction(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the Multiply command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeMultiply(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the Division command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeDivision(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the Power command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executePower(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the EqualAssigment command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeEqualAssignment(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the GreaterThan command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeGreaterThanComparison(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the LowerThan command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeLowerThanComparison(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the GreaterOrEqualThan command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeGreaterOrEqualThanComparison(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the LowerOrEqualThan command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeLowerOrEqualThanComparison(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the EqualThan command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeEqualThanComparison(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the NotEqualThan command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeNotEqualThanComparison(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the PushToTheStack command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executePushToTheStack(class Machine* machine, int32& next_cmd_id);

  /**
  * @brief Executes the FunctionDefinition command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeFunctionDefinition(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the FunctionCall command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeFunctionCall(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the FunctionReturn command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeFunctionReturn(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the FunctionEnd command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeFunctionEnd(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the FunctionNumParams command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeFunctionNumParams(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the FunctionParam command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeFunctionParam(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the FinishedConditionalOrLoop command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeFinishedConditionalOrLoop(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the ConditionToEvaluate command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeConditionToEvaluate(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the VariableDefinition command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeVariableDefinition(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the VariablePackDefinition command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeVariablePackDefinition(class Machine* machine, int32& next_cmd_id) const;
  
  /**
  * @brief Executes the VariablePackEnd command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeVariablePackEnd(class Machine* machine, int32& next_cmd_id) const;

  /**
  * @brief Executes the LoopStartPoint command from the list.
  *
  * @param machine Machine where the command is allocated.
  * @param next_cmd_id Index of the next step of the execution list.
  * @return Report with possible errors, or NoErrors if succesful.
  */
  Report executeLoopStartPoint(class Machine* machine, int32& next_cmd_id) const;


/*****************************  TYPE CHECKING  ********************************/

  /**
  * @brief Check if the name is between quotes, which means that is a text.
  *
  * @return None (variable), Integer, Float or Text("string between quotes").
  */
  ValueType getNameDataType();

  /**
  * @brief Character analyzer getter, tells you if the character is a digit.
  *
  * @param character Character to analyze.
  * @return true if it is a digit.
  */
  bool isDigit(const char8& character) const;
/*******************************************************************************
***                           PUBLIC ATTRIBUTES                              ***
*******************************************************************************/

  /// Command type.
  CommandType type_;
  /// Name of the command.
  std::string name_;


}; /* Command */
}; /* JMP_PROJECT */

#endif /* __JMP_COMMAND_H__ */