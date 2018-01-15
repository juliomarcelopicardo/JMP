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
  Report process(std::string script_filename);

  /**
  * @brief Checks if the extension of the file is .jpm
  *
  * @param filename File to check.
  * @return InvalidExtension if failed.
  */
  Report checkExtension(std::string filename);


/***************************  COMMAND LIST METHODS  ***************************/

  /**
  * @brief Adds a new command to the list.
  *
  * @param name Name of the command.
  * @param type Type of the command.
  */
  void addCommand(const char* name, const CommandType type = kCommandType_None);

  /**
  * @brief Adds a new command to the list.
  *
  * @param name Name of the command.
  * @param type Type of the command.
  */
  void addCommand(const std::string name, const CommandType type = kCommandType_None);

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
  const uint32 numCommands();

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

/// List of all the compiling commands.
  std::vector<Command> cmd_list_;
  /// Number of elements of the list.
  int32 cmd_list_length_;

}; /* Machine */
}; /* JMP */

#endif /* __JMP_MACHINE_H__ */