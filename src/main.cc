/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP_PROJECT - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#if 1 // ORIGINAL
#include "jmp.h"
#include <Windows.h>

void RegFunc(std::vector<JMP_PROJECT::Value>& params) {

  for (JMP_PROJECT::uint32 i = 0; i < params.size(); i++) {
    params[i].print();
  }
}

JMP_PROJECT::int32 main() {

  OutputDebugString("\n Starting application... \n\n");

  JMP_PROJECT::Machine machine;

  JMP_PROJECT::float32 variable = 3.02f;
  machine.processFile("../scripts/script.jmp");
  std::string pedro = machine.getString("w", "camera");
  machine.registerVariable("variable", JMP_PROJECT::kValueType_Float, &variable);
  machine.registerFunction("ExternalFunction", &RegFunc);
  //machine.runFunction("PrintExample(40, \"texto\", 40.34)");
  //machine.runFunction("Example(11111,22222)");
  //machine.runFunction("Example2()");
  machine.runFunction();

  /*
  system("pause");

  machine.reload();
  machine.runFunction();
  machine.unregisterFunction("ExternalFunction");
  machine.unregisterVariable("variable");
  */


  OutputDebugString("\n Ending application... \n\n");
  system("pause");
  return 0;
}
#else // SINGLE HEADER VERSION

#include "../single_header/jmp.h"
#include <Windows.h>

void RegFunc(std::vector<JMP::Value>& params) {

  for (JMP::uint32 i = 0; i < params.size(); i++) {
    params[i].print();
  }
}

JMP::int32 main() {

  OutputDebugString("\n Starting application... \n\n");

  JMP::Machine machine;

  JMP::float32 variable = 3.02f;
  machine.processFile("../scripts/script.jmp");
  std::string textico = machine.getString("w", "camera");
  JMP::int32 integer = machine.getInteger("x", "camera");
  JMP::float32 floaat = machine.getFloat("z", "camera");
  machine.registerVariable("variable", JMP::kValueType_Float, &variable);
  machine.registerFunction("ExternalFunction", &RegFunc);
  //machine.runFunction("PrintExample(40, \"texto\", 40.34)");
  //machine.runFunction("Example(11111,22222)");
  //machine.runFunction("Example2()");
  machine.runFunction();

  /*
  system("pause");

  machine.reload();
  machine.runFunction();
  machine.unregisterFunction("ExternalFunction");
  machine.unregisterVariable("variable");
  */


  OutputDebugString("\n Ending application... \n\n");
  system("pause");
  return 0;
}
#endif

