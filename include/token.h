/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP_PROJECT - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#ifndef __JMP_TOKEN__
#define __JMP_TOKEN__ 1

#include "types.h"
#include <string>

namespace JMP {

/// Token type.
enum TokenType {
  kTokenType_None = 0,
  kTokenType_Keyword,
  kTokenType_Separator,
  kTokenType_Number,
  kTokenType_Variable,
};


/// Class used to manage the different tokens or keys of the scripts.
class Token {

 public: 

/*******************************************************************************          
***                             PUBLIC METHODS                               ***
*******************************************************************************/

  /// Default class constructor.
  Token();
  /// Default class constructor.
  Token(const char* text, TokenType type, int32 priority);
  /// Default class destructor.
  ~Token();
  /// Default copy constructor.
  Token(const Token& copy);
  /// Assignment operator.
  Token& operator=(const Token& copy);



/********************************  GETTERS  ***********************************/

 

/*******************************************************************************
***                           PUBLIC ATTRIBUTES                              ***
*******************************************************************************/

  /// Token characters.
  std::string text_;
  /// Token type.
  TokenType type_;
  /// Token priority
  int32 priority_;


}; /* Token */
}; /* JMP_PROJECT */

#endif /* __JMP_TOKEN__ */