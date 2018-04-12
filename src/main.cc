/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP_PROJECT - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/



#if 1 // 1 - Complete version. 0 - Single header version.


#include "jmp.h"
#include <Windows.h>

void RegFunc(std::vector<JMP::Value>& params) {

  for (auto& param : params) {
    param.print();
  }
}

JMP::int32 main() {

  OutputDebugString("\n Starting application... \n\n");

  JMP::Machine machine;

  machine.processFile("../scripts/script.jmp");
  const float test = machine.getFloat("cos");
  printf("%f\n", test);
  JMP::float32 variable = 3.02f;
  machine.registerVariable("variable", JMP::kValueType_Float, &variable);
  machine.registerFunction("ExternalFunction", &RegFunc);
  machine.runFunction("Example2()");


  OutputDebugString("\n Ending application... \n\n");
  system("pause");
  return 0;
}




#else // SINGLE HEADER VERSION

#include "../single_header/jmp.h"
#include <Windows.h>

void RegFunc(std::vector<JMP_SH::Value>& params) {

  for (auto& param : params) {
    param.print();
  }
}

JMP_SH::int32 main() {

  OutputDebugString("\n Starting application... \n\n");

  JMP_SH::Machine machine;

  machine.processFile("../scripts/script.jmp");
  const float test = machine.getFloat("cos");
  printf("%f\n", test);
  JMP_SH::float32 variable = 3.02f;
  machine.registerVariable("variable", JMP_SH::kValueType_Float, &variable);
  machine.registerFunction("ExternalFunction", &RegFunc);
  machine.runFunction("Example2()");


  OutputDebugString("\n Ending application... \n\n");
  system("pause");
  return 0;
}

#endif

