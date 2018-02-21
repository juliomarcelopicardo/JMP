/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP_PROJECT - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#include "report.h"


namespace JMP {

void ReportError(const std::string& error) {
  ReportMsg(" ERROR: " + error + ".\n");
}

void ReportWarning(const std::string& warning) {
  ReportMsg(" WARNING: " + warning + ".\n");
}

void PrintReport(Report& report, uint32 line_number) {
  switch (report) {
    case JMP::kReport_EmptyLine: {
      //ReportWarning("Line " + std::to_string(line_number) + ": Nothing to compile..");
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
    case JMP::kReport_ExpectingOpenBrackets: {
      ReportError(": \"{\" separator expected at the end of the line..");
    } break;
    case JMP::kReport_ExpectingNameOfVariable: {
      ReportError(": Expecting name of variable after the \"var\" keyword..");
    } break;
    case JMP::kReport_ReturnShouldBeTheFirstToken: {
      ReportError(": Expecting \"return\" to be the first word of the line..");
    } break;
    case JMP::kReport_LoopKeywordShouldBeTheFirstToken: {
      ReportError(": Expecting loop keyword to be the first of the line..");
    } break;
    case JMP::kReport_FunctionKeywordShouldBeTheFirstToken: {
      ReportError(": Expecting function keyword \"func\" to be the first of the line..");
    } break;
    case JMP::kReport_FunctionDefinitionIncorrect: {
      ReportError(": Function definition incorrect, expecting at least \"func name() {\"..");
    } break;
    case JMP::kReport_EqualNeedTokensBeforeAndAfter: {
      ReportError(": Expecting tokens before and after the \"=\" token..");
    } break;
    case JMP::kReport_UnexpectedFunctionParameters: {
      ReportError(": Unexpected function parameters, only strings allowed..");
    } break;
    case JMP::kReport_ExceededNumParamsAllowedPerFunction: {
      ReportError(": Too many parameters, the maximum allowed are 30..");
    } break;
    case JMP::kReport_InvalidNameOfFunction: {
      ReportError(": Invalid function name..");
    } break;
    case JMP::kReport_ReturnCalledWithoutAnyActiveFunction: {
      ReportError(": Return was called, but there isnt any active function..");
    } break;
    case JMP::kReport_ConditionOutsideOfAFunction: {
      ReportError(": Conditions can only be evaluated inside a function body..");
    } break;
    case JMP::kReport_LoopOutsideOfAFunction: {
      ReportError(": Loops can only be executed inside a function body..");
    } break;
  default: {}
  }
}

void ReportMsg(const std::string& msg) {
  printf(msg.c_str());
}

}; /* JMP_PROJECT */