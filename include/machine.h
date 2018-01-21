/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#ifndef __JMP_MACHINE_H__
#define __JMP_MACHINE_H__ 1

#include "types.h"
#include "report.h"
#include "command.h"
#include "variable.h"
#include "function.h"
#include "token_manager.h"

namespace JMP {

/// Core of the language, this machine will be used to process .jpm script files.
class Machine {

 public: 

/*******************************************************************************          
***                             PUBLIC METHODS                               ***
*******************************************************************************/

  /// Default class constructor.
  Machine();
  /// Default class destructor.
  ~Machine();

/***************************  MAIN MACHINE METHODS  ***************************/

  /**
  * @brief Loads a script file so the machine can process it. Extension must be .jmp
  *
  * @param script Script file path.
  * @return Report with the function result.
  */
  Report processFile(std::string script_filename);

  /**
  * @brief Checks if the extension of the file is .jpm
  *
  * @param filename File to check.
  * @return InvalidExtension if failed.
  */
  Report checkExtension(std::string filename);

  /**
  * @brief Runs or executes a function from a previous loaded script.
  *
  * Example: to call this function the correct way is << runFunction("FuncName(param1,param2...)"); >>
  *
  * @param function_call_sentence Code of the function we want to run.
  * @return Report with the function result.
  */
  Report runFunction(std::string function_call_sentence = "main()");



/***************************  COMMAND LIST METHODS  ***************************/

  /**
  * @brief Adds a new command to the list.
  *
  * @param type Type of the command.
  */
  void addCommand(const CommandType type);

  /**
  * @brief Adds a new command to the list.
  *
  * @param type Type of the command.
  * @param name Name of the command.
  */
  void addCommand(const CommandType type, const char* name);

  /**
  * @brief Adds a new command to the list.
  *
  * @param type Type of the command.
  * @param name Name of the command.
  */
  void addCommand(const CommandType type, const std::string name);

  /**
  * @brief Adds a new command to the list.
  *
  * @param command Command to add.
  */
  void addCommand(const Command& command);

  /// Clears the command list.
  void clearCommandList();

  /**
  * @brief Removes a command from the list.
  *
  * @param id Index of the element in the command list.
  */
  void removeCommand(int32 id);

  /**
  * @brief Gets a command from the command manager list.
  *
  * @param list_index Index of the command in the list.
  * @return Command from the list.
  */
  Command getCommand(const int32 list_index);

  /**
  * @brief Getter of the number of commands allocated in the command manager list.
  *
  * @return Command list length.
  */
  const int32 numCommands();

  /**
  * @brief Concatenates the other machine command list, to the original one.
  *
  * @param other_machine Machine which command list will be added to this one.
  * @return Command from the list.
  */
  void pushBackOtherMachineCommandList(Machine* other_machine);

/***********************  VARIABLE REGISTRY METHODS  **************************/

  /**
  * @brief Adds a variable to the registry.
  *
  * @param name Name of the variable in the script (Case sensitive).
  * @param type Value type, necessary to allocate enough memory.
  * @param ptr_to_var Address of the variable of c++ that we want to register.
  * @return Report with the result of the instruction. NoErrors if succesful.
  */
  Report registerVariable(const char* name, ValueType type, void* ptr_to_var);

  /**
  * @brief removes a variable from the registry.
  *
  * @param name Name of the variable in the registry (Case sensitive).
  */
  void unregisterVariable(const char* name);

  /**
  * @brief removes a variable from the registry.
  *
  * @param id ID of the variable in the registry vector.
  */
  void unregisterVariable(const int32 id);

  /**
  * @brief Variable getter, will look for it in the function lists and in the variable registry.
  *
  * @param Variable pointer, nullptr if not found.
  */
  Variable* getVariable(const std::string& variable_name);

/**********************  DEFINED FUNCTION LIST METHODS  ***********************/

  /**
  * @brief Adds a new function to the list.
  *
  * @param name Name of the function.
  * @param command_index Position in the list of commands.
  * @return kReport_NoErrors if succesful.
  */
  Report addDefinedFunction(const char* name, const int32 command_index);

  /**
  * @brief removes a function from the defined function list.
  *
  * @param name Name of the function in the defined function list (Case Sensitive).
  */
  void removeDefinedFunction(const char* name);

  /**
  * @brief removes a function from the defined function list.
  *
  * @param id ID of the function in the defined function list.
  */
  void removeDefinedFunction(const int32 id);


/**********************  EXECUTION FUNCTION LIST METHODS  **********************/

  /**
  * @brief Last function added getter.
  *
  * @return Function pointer, or nullptr if not found.
  */
  Function* getCurrentFunction();

/*******************************************************************************
***                           PUBLIC ATTRIBUTES                              ***
*******************************************************************************/

  /**
  * @brief Push back a value into the stack.
  *
  * @param value Value to push back.
  */
  void addValueToTheStack(const Value value);

  /**
  * @brief Gets and removes the last value added to the stack.
  *
  * @return Last value added to the stack, (default value if stack is empty).
  */
  Value getAndRemoveTheLastAddedStackValue();

/*******************************************************************************
***                           PUBLIC ATTRIBUTES                              ***
*******************************************************************************/





 private:

/*******************************************************************************
***                            PRIVATE METHODS                               ***
*******************************************************************************/


  /// Default copy constructor.
  Machine(const Machine& copy);
  /// Assignment operator.
  Machine& operator=(const Machine& copy);

/*******************************************************************************
***                          PRIVATE ATTRIBUTES                              ***
*******************************************************************************/

/*****************************  COMMAND LIST **********************************/

/// List of all the compiling commands.
  std::vector<Command> cmd_list_;
  /// Number of elements of the list.
  int32 cmd_list_length_;

/************************* VARIABLE REGISTRY LIST *****************************/

  /// List of all the variable registered.
  std::vector<Variable> variable_registry_;
  /// Number of elements of the list.
  int32 variable_registry_length_;


/***************************** FUNCTION LISTS *********************************/

  /** Everytime we declare a function in the script, we will add here its command
      As we will need its position in the command list to execute it. */
  struct DefinedFunction {
    std::string name;
    int32 command_index;
  };

  /// List of functions defined in the script using the syntax "func name(params)"
  std::vector<DefinedFunction> defined_function_list_;
  /// Defined function list length -> number of elements.
  int32 defined_function_list_length_;

  /// List of functions used in the execution.
  std::vector<Function> function_list_;
  /// Number of functions.
  int32 function_list_length_;

/***************************** STACK OF VALUES ********************************/

  /// Stack of inmediate values.
  std::vector<Value> stack_;
  /// Number of elements of the stack.
  int32 stack_length_;

}; /* Machine */
}; /* JMP */

#endif /* __JMP_MACHINE_H__ */