/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/


/* SINGLE HEADER LIBRARY */

#ifndef __JMP_SINGLE_HEADER_H__
#define __JMP_SINGLE_HEADER_H__ 1

#include "types.h"
#include <string>
#include <math.h>
#include <Windows.h>

namespace JMP_SINGLE_HEADER {


/*******************************************************************************
/*******************************************************************************
***                                                                          ***
***                                DEFINES                                   ***                           
***                                                                          ***
/*******************************************************************************
*******************************************************************************/

#pragma region DEFINES

#define INITIALIZATION_VALUE -99999
#define CONDITION_RESULT_TRUE 30 // Random number

#pragma endregion


/*******************************************************************************
/*******************************************************************************
***                                                                          ***
***                                 TYPES                                    ***
***                                                                          ***
/*******************************************************************************
*******************************************************************************/
#pragma region TYPES

  /// char
  typedef char char8;
  typedef char16_t char16;
  typedef char32_t char32;

  /// unsigned char
  typedef unsigned char uchar8;

  /// integer
  typedef int8_t int8;
  typedef int16_t int16;
  typedef int32_t int32;
  typedef int64_t int64;

  /// unsigned integer
  typedef uint8_t uint8;
  typedef uint16_t uint16;
  typedef uint32_t uint32;
  typedef uint64_t uint64;

  /// precision
  typedef float float32;
  typedef double float64;

#pragma endregion


/*******************************************************************************
/*******************************************************************************
***                                                                          ***
***                                 ENUMS                                    ***
***                                                                          ***
/*******************************************************************************
*******************************************************************************/
#pragma region ENUMS

  /// Value type.
  enum ValueType {
    kValueType_None = 0,
    kValueType_Float,
    kValueType_Integer,
    kValueType_Text,
  };

  /// Reports. Can be errors, warnings or simple status.
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
#pragma endregion

#pragma region REPORTS

void ReportMsg(std::string msg) {
  OutputDebugString(msg.c_str());
}

void ReportError(std::string error) {
  ReportMsg(" ERROR: " + error + ".\n");
}

void ReportWarning(std::string warning) {
  ReportMsg(" WARNING: " + warning + ".\n");
}

void PrintReport(Report& report, uint32 line_number) {
  switch (report) {
  case kReport_EmptyLine: {
    ReportWarning("Line " + std::to_string(line_number) + ": Nothing to compile..");
    report = kReport_NoErrors;
  } break;
  case kReport_NoTokensToCompile: {
    ReportError("Line " + std::to_string(line_number) + ": Can't compile, no compilable tokens available..");
  } break;
  case kReport_NoMatchingCloseParenthesis: {
    ReportError("Line " + std::to_string(line_number) + ": No matching close parenthesis found..");
  } break;
  case kReport_NoTagsToDelete: {
    ReportError(": No matching tag to remove was found, more conditional, functions or loops closed than created..");
  } break;
  case kReport_UnexpectedKeyword: {
    ReportError(": Unexpected or unkwown keyword used..");
  } break;
  case kReport_ExpectingOpenBrackets: {
    ReportError(": \"{\" separator expected at the end of the line..");
  } break;
  case kReport_ExpectingNameOfVariable: {
    ReportError(": Expecting name of variable after the \"var\" keyword..");
  } break;
  case kReport_ReturnShouldBeTheFirstToken: {
    ReportError(": Expecting \"return\" to be the first word of the line..");
  } break;
  case kReport_LoopKeywordShouldBeTheFirstToken: {
    ReportError(": Expecting loop keyword to be the first of the line..");
  } break;
  case kReport_FunctionKeywordShouldBeTheFirstToken: {
    ReportError(": Expecting function keyword \"func\" to be the first of the line..");
  } break;
  case kReport_FunctionDefinitionIncorrect: {
    ReportError(": Function definition incorrect, expecting at least \"func name() {\"..");
  } break;
  case kReport_EqualNeedTokensBeforeAndAfter: {
    ReportError(": Expecting tokens before and after the \"=\" token..");
  } break;
  case kReport_UnexpectedFunctionParameters: {
    ReportError(": Unexpected function parameters, only strings allowed..");
  } break;
  case kReport_ExceededNumParamsAllowedPerFunction: {
    ReportError(": Too many parameters, the maximum allowed are 30..");
  } break;
  case kReport_InvalidNameOfFunction: {
    ReportError(": Invalid function name..");
  } break;
  case kReport_ReturnCalledWithoutAnyActiveFunction: {
    ReportError(": Return was called, but there isnt any active function..");
  } break;
  case kReport_ConditionOutsideOfAFunction: {
    ReportError(": Conditions can only be evaluated inside a function body..");
  } break;
  case kReport_LoopOutsideOfAFunction: {
    ReportError(": Loops can only be executed inside a function body..");
  } break;
  }
}


#pragma endregion

/*******************************************************************************
/*******************************************************************************
***                                                                          ***
***                                CLASSES                                   ***
***                                                                          ***
/*******************************************************************************
*******************************************************************************/

#pragma region VALUE_CLASS 
/*******************************************************************************
***                                 VALUE                                    ***
*******************************************************************************/

#pragma endregion

}; /* JMP_SINGLE_FILE */

#endif /* __JMP_SINGLE_HEADER_H__ */