/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP_PROJECT - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#include "token.h"
#include "compiler.h"

namespace JMP {

/*******************************************************************************
***                       CONSTRUCTOR & DESTRUCTOR                           ***
*******************************************************************************/

Token::Token() {
  type_ = kTokenType_None;
  text_ = "";
  priority_ = DEFAULT_PRIORITY;
}

Token::Token(const char* text, const TokenType type, const int32 priority) {
  type_ = type;
  text_ = text;
  priority_ = priority;
}

Token::~Token() {}

Token::Token(const Token& copy) {
  type_ = copy.type_;
  text_ = copy.text_;
  priority_ = copy.priority_;
}

Token& Token::operator=(const Token& copy) {
  type_ = copy.type_;
  text_ = copy.text_;
  priority_ = copy.priority_;
  return *this;
}





}; /* JMP_PROJECT */