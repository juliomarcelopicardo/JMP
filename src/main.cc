/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#include "jmp.h"
#include <Windows.h>

JMP::int32 main() {
  OutputDebugString("\n Starting application... \n\n\n\n\n");

  JMP::TokenManager token_manager;
  JMP::TextParser text_parser;

  token_manager.addToken("token", JMP::TokenType_Keyword);
  token_manager.addToken("token1");

  token_manager.printTokenList();

  token_manager.removeToken(0);
  token_manager.printTokenList();

  OutputDebugString("\n Ending application... \n\n");
  return 0;
}

