/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#include "report.h"
#include <Windows.h>


namespace JMP {

  void ReportError(std::string error) {
    ReportMsg(" ERROR: " + error + ".\n");
  }

  void ReportMsg(std::string msg) {
    OutputDebugString(msg.c_str());
  }

}; /* JMP */