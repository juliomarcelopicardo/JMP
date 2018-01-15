/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#ifndef __JMP_COMMAND_H__
#define __JMP_COMMAND_H__ 1

#include "types.h"
#include <string>

namespace JMP {

/// Token type.
enum CommandType {
  kCommandType_None = 0,
  kCommandType_Addition,
  kCommandType_Substraction,
  kCommandType_Multiply,
  kCommandType_Division,
  kCommandType_Power,
  kCommandType_FunctionCall,
};


/// Class used to save and manage the compiling commands.
class Command {

 public: 

/*******************************************************************************          
***                             PUBLIC METHODS                               ***
*******************************************************************************/

  /// Default class constructor.
  Command();
  /// Default class constructor.
  Command(const char* name, const CommandType type);
  /// Default class destructor.
  ~Command();
  /// Default copy constructor.
  Command(const Command& copy);
  /// Assignment operator.
  Command& operator=(const Command& copy);



/********************************  GETTERS  ***********************************/

 

/*******************************************************************************
***                           PUBLIC ATTRIBUTES                              ***
*******************************************************************************/

  /// Command type.
  CommandType type_;
  /// Name of the command.
  std::string name_;

 private:

/*******************************************************************************
***                            PRIVATE METHODS                               ***
*******************************************************************************/




/*******************************************************************************
***                          PRIVATE ATTRIBUTES                              ***
*******************************************************************************/



}; /* Command */
}; /* JMP */

#endif /* __JMP_COMMAND_H__ */