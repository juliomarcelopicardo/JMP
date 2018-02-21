/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP_PROJECT - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#ifndef __JMP_REPORT_H__
#define __JMP_REPORT_H__ 1

#include <string>
#include "types.h"

namespace JMP {

/*******************************************************************************
***                           RESULTS OR REPORTS                             ***
*******************************************************************************/

  enum Report {
    kReport_NoErrors = 0,
    kReport_InvalidFileExtension,
    kReport_InvalidFilename,
    kReport_FilenameTooShort,
    kReport_NullPointer,
    kReport_EmptyContainer,
    kReport_ListIndexOutOfRange,
    kReport_EmptyLine,
    kReport_NoTokensToCompile,
    kReport_NoMatchingCloseParenthesis,
    kReport_NoTagsToDelete,
    kReport_UnexpectedKeyword,
    kReport_KeywordFunctionalityNotProgrammedYet,
    kReport_ExpectingOpenBrackets,
    kReport_ExpectingNameOfVariable,
    kReport_ReturnShouldBeTheFirstToken,
    kReport_LoopKeywordShouldBeTheFirstToken,
    kReport_FunctionKeywordShouldBeTheFirstToken,
    kReport_FunctionDefinitionIncorrect,
    kReport_EqualNeedTokensBeforeAndAfter,
    kReport_UnexpectedFunctionParameters,
    kReport_InvalidNameOfFunction,
    kReport_ExceededNumParamsAllowedPerFunction,
    kReport_FunctionDefinedMoreThanOnce,
    kReport_InvalidCommandType,
    kReport_LastActiveFunctionReturnCalled,
    kReport_InvalidValueType,
    kReport_ReturnCalledWithoutAnyActiveFunction,
    kReport_NotEnoughFunctionParamsInTheStack,
    kReport_ParamsNeedAFunctionToBeExecuted,
    kReport_EndOfConditionalOrLoopNotFound,
    kReport_CallingUndefinedFunction,
    kReport_VariablePackKeywordShouldBeTheFirstToken,
    kReport_VariablePackDefinitionIncorrect,
    kReport_VariablePackCantBeDefinedInsideAFunction,
    kReport_InvalidNameOfVariablePack,
    kReport_VariableDefinedTwice,
    kReport_ConditionOutsideOfAFunction,
    kReport_LoopOutsideOfAFunction,
    kReport_UnexpectedComparisonToken,
  };

  void ReportMsg(const std::string& msg);
  void ReportError(const std::string& error);
  void ReportWarning(const std::string& warning);
  void PrintReport(Report& report, uint32 line_number = 0);

}; /* JMP_PROJECT */

#endif /* __JMP_REPORT_H__ */