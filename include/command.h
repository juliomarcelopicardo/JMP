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

namespace JMP {

/// Token type.
enum CommandType {
  kTokenType_None = 0,
  kTokenType_Keyword,
  kTokenType_Separator,
  kTokenType_Number,
  kTokenType_Variable,
};


/// Class used to save and manage the compiling commands.
class Command {

 public: 

/*******************************************************************************          
***                             PUBLIC METHODS                               ***
*******************************************************************************/

  /// Default class constructor.
  Command();
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