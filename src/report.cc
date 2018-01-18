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

void ReportWarning(std::string warning) {
  ReportMsg(" WARNING: " + warning + ".\n");
}

void PrintReport(Report& report, uint32 line_number) {
  switch (report) {
    case JMP::kReport_EmptyLine: {
      ReportWarning("Line " + std::to_string(line_number) + ": Nothing to compile..");
      report = kReport_NoErrors;
    } break;
    case JMP::kReport_NoTokensToCompile: {
      ReportError("Line " + std::to_string(line_number) + ": Can't compile, no compilable tokens available..");
    } break;
    case JMP::kReport_NoMatchingCloseParenthesis: {
      ReportError("Line " + std::to_string(line_number) + ": No matching close parenthesis found..");
    } break;
    case JMP::kReport_NoTagsToDelete: {
      ReportError(": No matching tag to remove was found, more conditional, functions or loops closed than created..");
    } break;
    case JMP::kReport_UnexpectedKeyword: {
      ReportError(": Unexpected or unkwown keyword used..");
    } break;
  }
}

void ReportMsg(std::string msg) {
  OutputDebugString(msg.c_str());
}

}; /* JMP */