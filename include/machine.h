/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP_PROJECT - A Scripting Language for Game Engines.
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
#include "registered_function.h"

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
  /// Default copy constructor.
  Machine(const Machine& copy) = delete;
  /// Assignment operator.
  Machine& operator=(const Machine& copy) = delete;

/***************************  MAIN MACHINE METHODS  ***************************/

  /**
  * @brief Loads a script file so the machine can process it. Extension must be .jmp
  *
  * @param script_filename Script file path.
  * @return Report with the function result.
  */
  Report processFile(const std::string& script_filename);

  /**
  * @brief Checks if the extension of the file is .jpm
  *
  * @param filename File to check.
  * @return InvalidExtension if failed.
  */
  Report checkExtension(const std::string& filename) const;

  /**
  * @brief Runs or executes a function from a previous loaded script.
  *
  * Example: to call this function the correct way is << runFunction("FuncName(param1,param2...)"); >>
  *
  * @param function_call_sentence Code of the function we want to run.
  * @return Report with the function result.
  */
  Report runFunction(std::string function_call_sentence = "main()");

  /**
  * @brief Runs or executes a script in order to take all the global variables.
  *
  * Execution will ignore loops, functions and conditionals. Just Variables.
  *
  * @return Report with the function result.
  */
  Report runScriptToSaveGlobalVariables();

  ///@brief Deletes the last compilation and process and compile the last file.
  void reload();

  /**
  * @brief Current script code getter.
  *
  * @return String with all the code allocated in the current script file.
  */
  std::string getCurrentScript() const;

/***************************  COMMAND LIST METHODS  ***************************/

  /**
  * @brief Adds a new command to the list.
  *
  * @param type Type of the command.
  */
  void addCommand(CommandType type);

  /**
  * @brief Adds a new command to the list.
  *
  * @param type Type of the command.
  * @param name Name of the command.
  */
  void addCommand(CommandType type, const char* name);

  /**
  * @brief Adds a new command to the list.
  *
  * @param type Type of the command.
  * @param name Name of the command.
  */
  void addCommand(CommandType type, const std::string& name);

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
  Command getCommand(int32 list_index);

  /**
  * @brief Getter of the number of commands allocated in the command manager list.
  *
  * @return Command list length.
  */
  int32 numCommands() const;

  /**
  * @brief Concatenates the other machine command list, to the original one.
  *
  * @param other_machine Machine which command list will be added to this one.
  * @return Command from the list.
  */
  void pushBackOtherMachineCommandList(Machine* other_machine);

/****************************  VARIABLE METHODS  ******************************/

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
  void unregisterVariable(int32 id);

  /**
  * @brief Push back a new global variable pack and set it as current.
  *
  * @param name Name of the variable pack.
  * @return Report with the result of the function.
  */
  void addGlobalVariablePack(const char* name);

  /**
  * @brief Push back a global variable in the current list.
  *
  * @param variable Variable to push back in the list
  * @return Report with the result of the function.
  */
  Report addGlobalVariableToCurrentPack(Variable variable);

  /**
  * @brief Push back a global type variable in the current pack list.
  *
  * @param name Name of the variable
  * @param value Value of the variable.
  * @return Report with the result of the function.
  */
  Report addGlobalVariableToCurrentPack(const char* name, const Value& value);

  /**
  * @brief Current pack index will be 0, which is the pack for non-packaged global variables.
  */
  void restartCurrentGlobalVariablePackIndex();

  /**
  * @brief Gets the current global variable pack name.
  *
  * @return Name of the pack.
  */
  std::string getCurrentGlobalVariablePackName();

  /**
  * @brief Variable getter, will look for it in the function lists and in the variable registry.
  *
  * @param variable_name Name of the variable in the registry (Case sensitive).
  * @return Variable from the list, null if not found.
  */
  Variable* getVariable(const std::string& variable_name);


  /***********************  FUNCTION REGISTRY METHODS  **************************/

  /**
  * @brief Adds a function to the registry.
  *
  * To register a function its declaration needs to be:
  * "void FunctionName(std::vector<Value>& params)"
  * Params is a list of the parameters used everytime we call it from the script.
  *
  * @param name Name of the function in the script (Case sensitive).
  * @param function_ptr Pointer to the function defined in c++.
  * @return Report with the result of the instruction, NoErrors if succesful.
  */
  Report registerFunction(const char* name, void(*function_ptr)(std::vector<Value>&));

  /**
  * @brief removes a function from the registry.
  *
  * @param name Name of the function in the registry (Case sensitive).
  */
  void unregisterFunction(const char* name);

  /**
  * @brief removes a function from the registry.
  *
  * @param id ID of the function in the registry vector.
  */
  void unregisterFunction(int32 id);

  /**
  * @brief Function getter, will look for it in the function lists and in the variable registry.
  *
  * @param function_name Name of the function in the registry (Case sensitive).
  * @return Pointer to the registered function. Nullptr if not found.
  */
  RegisteredFunction* getRegisteredFunction(const std::string& function_name);

/**********************  DEFINED FUNCTION LIST METHODS  ***********************/

  /**
  * @brief Adds a new function to the list.
  *
  * @param name Name of the function.
  * @param command_index Position in the list of commands.
  * @return kReport_NoErrors if succesful.
  */
  Report addDefinedFunction(const char* name, int32 command_index);

  /**
  * @brief Looks for a function in the defined function list and return its ID.
  *
  * @param name Name of the function.
  * @return The ID which is the command where the function body starts.
  */
  int32 getDefinedFunctionID(const char* name);

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
  void removeDefinedFunction(int32 id);


/**********************  EXECUTION FUNCTION LIST METHODS  **********************/


  /**
  * @brief Adds a new function to the list.
  *
  * @param origin_id Once finished the execution of the function will return to this command.
  */
  void addFunction(int32 origin_id);


  /**
  * @brief Last function added getter.
  *
  * @return Function pointer, or nullptr if not found.
  */
  Function* getCurrentFunction();

  /**
  * @brief Removes a function from the execution function list.
  *
  * @param id ID or position in the list.
  */
  void removeFunction(int32 id);

  /**
  * @brief Removes the last function from the execution function list.
  */
  void removeCurrentFunction();

  /**
  * @brief function list length getter.
  *
  * @return number of active functions, function list length.
  */
  int32 numActiveFunctions() const;

/***************************** STACK OF VALUES ********************************/

  /**
  * @brief Push back a value into the stack.
  *
  * @param value Value to push back.
  */
  void addValueToTheStack(const Value& value) const;

  /**
  * @brief Gets and removes the last value added to the stack.
  *
  * @return Last value added to the stack, (default value if stack is empty).
  */
  Value getAndRemoveTheLastAddedStackValue() const;

  /**
  * @brief Gets the number of values allocated in the stack.
  *
  * @return number of values.
  */
  int32 numStackValues() const;

/**************************** VARIABLE GETTERS ********************************/

  /**
  * @brief Global variable getter. Will extract an integer value from a variable.
  *
  * @param variable_name Name of the variable in the script.
  * @param variable_pack_name Name of the variable pack defined in the script.
  * @return The integer value of this variable.
  */
  int32 getInteger(const char* variable_name, const char* variable_pack_name = "");

  /**
  * @brief Global variable getter. Will extract an float value from a variable.
  *
  * @param variable_name Name of the variable in the script.
  * @param variable_pack_name Name of the variable pack defined in the script.
  * @return The float value of this variable.
  */
  float32 getFloat(const char* variable_name, const char* variable_pack_name = "");

  /**
  * @brief Global variable getter. Will extract an string value from a variable.
  *
  * @param variable_name Name of the variable in the script.
  * @param variable_pack_name Name of the variable pack defined in the script.
  * @return The string value of this variable.
  */
  std::string getString(const char* variable_name, const char* variable_pack_name = "");


/*******************************************************************************
***                           PUBLIC ATTRIBUTES                              ***
*******************************************************************************/


/***************************** STACK OF VALUES ********************************/

  /// Stack of inmediate values. Need to be static as will be shared between machines.
  struct Stack { 
    Stack() { length = 0; value.reserve(30); }
    /// Values list
    std::vector<Value> value;
    /// Number of elements of the stack.
    int32 length; 
  };

  /// Stack of inmediate values.Need to be static as will be shared between machines.
  static Stack stack_;

 private:



/*******************************************************************************
***                          PRIVATE ATTRIBUTES                              ***
*******************************************************************************/

/*****************************  COMMAND LIST **********************************/

/// List of all the compiling commands.
  std::vector<Command> cmd_list_;
  /// Number of elements of the list.
  int32 cmd_list_length_;

/************************* VARIABLE REGISTRY LIST *****************************/

  /// List of all the variable registered from C++.
  std::vector<Variable> variable_registry_;
  /// Number of elements of the list.
  int32 variable_registry_length_;

/***********************  GLOBAL VARIABLE PACK LIST ***************************/

  struct VariablePack {
    std::string name;
    std::vector<Variable> var;
  };
  /// List of global variables pack
  std::vector<VariablePack> global_variable_pack_list_;
  /// Global variable list length.
  int32 global_variable_pack_list_length_;
  /// Current global variable pack
  int32 current_global_variable_pack_;

/************************* FUNCTION REGISTRY LIST *****************************/

  /// List of all the functions registered from C++.
  std::vector<RegisteredFunction> function_registry_;
  /// Number of elements of the list.
  int32 function_registry_length_;


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

  /// Last script compiled file path.
  std::string last_script_compiled_path_;

}; /* Machine */
}; /* JMP_PROJECT */

#endif /* __JMP_MACHINE_H__ */