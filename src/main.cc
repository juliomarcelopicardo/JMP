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
  OutputDebugString("\n Starting application... \n\n");

  JMP::TokenManager token_manager;
  JMP::TextParser text_parser;
  JMP::Machine machine;

  JMP::float32 variable = 3.02f;
  machine.processFile("../scripts/script.jmp");
  machine.registerVariable("variable", JMP::kValueType_Float, &variable);
  machine.runFunction("PrintExample(40, \"texto\", 40.34)");
  machine.runFunction("Example(11111,22222)");
  machine.unregisterVariable("variable");
  OutputDebugString("\n Ending application... \n\n");
  return 0;
}

