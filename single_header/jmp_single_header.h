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
#include <vector>
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

// Priorities
#define KEYWORD_PRIORITY 500
#define CLOSE_BRACKETS_PRIORITY 200
#define OPEN_PARENTHESIS_PRIORITY 100
#define POWER_OPERATION_PRIORITY 99
#define MULTIPLY_OPERATION_PRIORITY 98
#define ADDITION_OPERATION_PRIORITY 97
#define COMPARISON_PRIORITY 96
#define EQUAL_PRIORITY 95
#define FUNCTION_NAME_PRIORITY 10
#define DEFAULT_PRIORITY 0
#define COMMA_PRIORITY -10
#define MAX_PARAMETERS_PER_FUNCTION 30

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

  /// Token type.
  enum TokenType {
    kTokenType_None = 0,
    kTokenType_Keyword,
    kTokenType_Separator,
    kTokenType_Number,
    kTokenType_Variable,
  };

  /// Compiler tags to know if we are inside a function body, a loop body... 
  enum Tag {
    kTag_None = 0,
    kTag_Loop,
    kTag_Conditional,
    kTag_Function,
    kTag_VariablePack,
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

/*******************************************************************************
/*******************************************************************************
***                                                                          ***
***                                REPORTS                                   ***
***                                                                          ***
/*******************************************************************************
*******************************************************************************/

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

/*******************************************************************************
***                                 VALUE                                    ***
*******************************************************************************/

#pragma region VALUE_CLASS 

/// Class used to manage the different values used in the variables and the stack
class Value {

 public:
  
/******************************   ATTRIBUTES   ********************************/
  /// Type of value.
  ValueType type_;
  /// String variable value.
  std::string text_;
  /// Float variable value.
  float32 float_;
  /// Integer variable value.
  int32 integer_;

/*******************************   METHODS   **********************************/
  Value() {
    type_ = kValueType_None;
    text_ = "";
    float_ = INITIALIZATION_VALUE;
    integer_ = INITIALIZATION_VALUE;
  }

  Value(const bool condition_result) {

    type_ = kValueType_Integer;
    text_ = "BOOLEAN";
    float_ = INITIALIZATION_VALUE;
    if (condition_result) {
      integer_ = CONDITION_RESULT_TRUE;
    }
    else {
      integer_ = 0;
    }
  }

  Value(const int32 integer_value) {
    type_ = kValueType_Integer;
    text_ = "";
    float_ = INITIALIZATION_VALUE;
    integer_ = integer_value;
  }

  Value(const float32 float_value) {
    type_ = kValueType_Float;
    text_ = "";
    float_ = float_value;
    integer_ = INITIALIZATION_VALUE;
  }

  Value(const char* text_value) {
    type_ = kValueType_Text;
    text_ = text_value;
    float_ = INITIALIZATION_VALUE;
    integer_ = INITIALIZATION_VALUE;
  }

  ~Value() {}

  Value(const Value& copy) {
    type_ = copy.type_;
    text_ = copy.text_;
    float_ = copy.float_;
    integer_ = copy.integer_;
  }

  Value& operator=(const Value& copy) {
    type_ = copy.type_;
    text_ = copy.text_;
    float_ = copy.float_;
    integer_ = copy.integer_;
    return *this;
  }

  void print() {
    switch (type_) {
    case kValueType_Float: { printf("%f\n", float_); }  break;
    case kValueType_Integer: { printf("%d\n", integer_); } break;
    case kValueType_Text: { printf("%s\n", text_.c_str()); } break;
    }
  }

}; /* Value */


#pragma endregion

/*******************************************************************************
***                            VALUE OPERATORS                               ***
*******************************************************************************/

#pragma region VALUE_CLASS_OPERATORS

Value operator+(const Value& a, const Value& b) {
  // integer + integer = integer 
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Integer) {
    return{ a.integer_ + b.integer_ };
  }

  // float + float = float 
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Float) {
    return{ a.float_ + b.float_ };
  }

  // text + text = text 
  if (a.type_ == kValueType_Text && b.type_ == kValueType_Text) {
    return{ (a.text_ + b.text_).c_str() };
  }

  // integer + float = float
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Float) {
    return{ (float32)a.integer_ + b.float_ };
  }

  // float + integer = float
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Integer) {
    return{ a.float_ + (float32)b.integer_ };
  }

  // integer + text = text
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Text) {
    return{ (std::to_string(a.integer_) + b.text_).c_str() };
  }

  // float + text = text
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Text) {
    return{ (std::to_string(a.float_) + b.text_).c_str() };
  }

  // text + integer = text
  if (b.type_ == kValueType_Integer && a.type_ == kValueType_Text) {
    return{ (a.text_ + std::to_string(b.integer_)).c_str() };
  }

  // text + float = text
  if (b.type_ == kValueType_Float && a.type_ == kValueType_Text) {
    return{ (a.text_ + std::to_string(b.float_)).c_str() };
  }

  ReportWarning("Value Addition Operation Failed.");
  return Value();
}


Value operator-(const Value& a, const Value& b) {
  // integer - integer = integer 
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Integer) {
    return{ a.integer_ - b.integer_ };
  }

  // float - float = float 
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Float) {
    return{ a.float_ - b.float_ };
  }

  // integer - float = float
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Float) {
    return{ (float32)a.integer_ - b.float_ };
  }

  // float - integer = float
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Integer) {
    return{ a.float_ - (float32)b.integer_ };
  }

  ReportWarning("Value Substraction Operation Failed.");
  return Value();
}

Value operator*(const Value& a, const Value& b) {
  // integer * integer = integer 
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Integer) {
    return{ a.integer_ * b.integer_ };
  }

  // float * float = float 
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Float) {
    return{ a.float_ * b.float_ };
  }

  // integer * float = float
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Float) {
    return{ (float32)a.integer_ * b.float_ };
  }

  // float * integer = float
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Integer) {
    return{ a.float_ * (float32)b.integer_ };
  }

  ReportWarning("Value Multiply Operation Failed.");
  return Value();
}

Value operator/(const Value& a, const Value& b) {
  // integer / integer = integer 
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Integer) {
    if (b.integer_ == 0) {
      ReportWarning("Division by 0 avoided"); return Value();
    }
    return{ a.integer_ / b.integer_ };
  }

  // float / float = float 
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Float) {
    if (b.float_ == 0.0f) {
      ReportWarning("Division by 0 avoided"); return Value();
    }
    return{ a.float_ / b.float_ };
  }

  // integer / float = float
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Float) {
    if (b.float_ == 0.0f) {
      ReportWarning("Division by 0 avoided"); return Value();
    }
    return{ (float32)a.integer_ / b.float_ };
  }

  // float / integer = float
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Integer) {
    if (b.integer_ == 0) {
      ReportWarning("Division by 0 avoided"); return Value();
    }
    return{ a.float_ / (float32)b.integer_ };
  }

  ReportWarning("Value Division Operation Failed.");
  return Value();
}

Value operator^(const Value& a, const Value& b) {
  // integer ^ integer = integer 
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Integer) {
    return{ (int32)pow(a.integer_, b.integer_) };
  }

  // float ^ float = float 
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Float) {
    return{ powf(a.float_, b.float_) };
  }

  // integer ^ float = float
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Float) {
    return{ (float32)pow(a.integer_, b.float_) };
  }

  // float ^ integer = float
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Integer) {
    return{ pow(a.float_, b.integer_) };
  }

  ReportWarning("Value Power Operation Failed.");
  return Value();
}

Value operator>(const Value& a, const Value& b) {
  // integer > integer = bool 
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Integer) {
    return{ a.integer_ > b.integer_ };
  }

  // float > float = bool 
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Float) {
    return{ a.float_ > b.float_ };
  }

  // text > text = bool 
  if (a.type_ == kValueType_Text && b.type_ == kValueType_Text) {
    return{ a.text_ > b.text_ };
  }

  // integer > float = bool
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Float) {
    return{ a.integer_ > b.float_ };
  }

  // float > integer = bool
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Integer) {
    return{ a.float_ > b.integer_ };
  }

  // integer > text = bool
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Text) {
    return{ std::to_string(a.integer_) > b.text_ };
  }

  // float > text = bool
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Text) {
    return{ std::to_string(a.float_) > b.text_ };
  }

  // text > integer = bool
  if (b.type_ == kValueType_Integer && a.type_ == kValueType_Text) {
    return{ a.text_ > std::to_string(b.integer_) };
  }

  // text > float = bool
  if (b.type_ == kValueType_Float && a.type_ == kValueType_Text) {
    return{ a.text_ > std::to_string(b.float_) };
  }

  ReportWarning("Value Greater Than Comparison Operation Failed.");
  return Value();
}

Value operator<(const Value& a, const Value& b) {
  // integer < integer = bool 
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Integer) {
    return{ a.integer_ < b.integer_ };
  }

  // float < float = bool 
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Float) {
    return{ a.float_ < b.float_ };
  }

  // text < text = bool 
  if (a.type_ == kValueType_Text && b.type_ == kValueType_Text) {
    return{ a.text_ < b.text_ };
  }

  // integer < float = bool
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Float) {
    return{ a.integer_ < b.float_ };
  }

  // float < integer = bool
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Integer) {
    return{ a.float_ < b.integer_ };
  }

  // integer < text = bool
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Text) {
    return{ std::to_string(a.integer_) < b.text_ };
  }

  // float < text = bool
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Text) {
    return{ std::to_string(a.float_) < b.text_ };
  }

  // text < integer = bool
  if (b.type_ == kValueType_Integer && a.type_ == kValueType_Text) {
    return{ a.text_ < std::to_string(b.integer_) };
  }

  // text < float = bool
  if (b.type_ == kValueType_Float && a.type_ == kValueType_Text) {
    return{ a.text_ < std::to_string(b.float_) };
  }

  ReportWarning("Value Lesser Than Comparison Operation Failed.");
  return Value();
}

Value operator>=(const Value& a, const Value& b) {
  // integer >= integer = bool 
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Integer) {
    return{ a.integer_ >= b.integer_ };
  }

  // float >= float = bool 
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Float) {
    return{ a.float_ >= b.float_ };
  }

  // text >= text = bool 
  if (a.type_ == kValueType_Text && b.type_ == kValueType_Text) {
    return{ a.text_ >= b.text_ };
  }

  // integer >= float = bool
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Float) {
    return{ a.integer_ >= b.float_ };
  }

  // float >= integer = bool
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Integer) {
    return{ a.float_ >= b.integer_ };
  }

  // integer >= text = bool
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Text) {
    return{ std::to_string(a.integer_) >= b.text_ };
  }

  // float >= text = bool
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Text) {
    return{ std::to_string(a.float_) >= b.text_ };
  }

  // text >= integer = bool
  if (b.type_ == kValueType_Integer && a.type_ == kValueType_Text) {
    return{ a.text_ >= std::to_string(b.integer_) };
  }

  // text >= float = bool
  if (b.type_ == kValueType_Float && a.type_ == kValueType_Text) {
    return{ a.text_ >= std::to_string(b.float_) };
  }

  ReportWarning("Value GreaterOrEqual Than Comparison Operation Failed.");
  return Value();
}

Value operator<=(const Value& a, const Value& b) {
  // integer <= integer = bool 
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Integer) {
    return{ a.integer_ <= b.integer_ };
  }

  // float <= float = bool 
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Float) {
    return{ a.float_ <= b.float_ };
  }

  // text <= text = bool 
  if (a.type_ == kValueType_Text && b.type_ == kValueType_Text) {
    return{ a.text_ <= b.text_ };
  }

  // integer <= float = bool
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Float) {
    return{ a.integer_ <= b.float_ };
  }

  // float <= integer = bool
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Integer) {
    return{ a.float_ <= b.integer_ };
  }

  // integer <= text = bool
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Text) {
    return{ std::to_string(a.integer_) <= b.text_ };
  }

  // float <= text = bool
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Text) {
    return{ std::to_string(a.float_) <= b.text_ };
  }

  // text <= integer = bool
  if (b.type_ == kValueType_Integer && a.type_ == kValueType_Text) {
    return{ a.text_ <= std::to_string(b.integer_) };
  }

  // text <= float = bool
  if (b.type_ == kValueType_Float && a.type_ == kValueType_Text) {
    return{ a.text_ <= std::to_string(b.float_) };
  }

  ReportWarning("Value LesserOrEqual Than Comparison Operation Failed.");
  return Value();
}

Value operator==(const Value& a, const Value& b) {
  // integer == integer = bool 
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Integer) {
    return{ a.integer_ == b.integer_ };
  }

  // float == float = bool 
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Float) {
    return{ a.float_ == b.float_ };
  }

  // text == text = bool 
  if (a.type_ == kValueType_Text && b.type_ == kValueType_Text) {
    return{ a.text_ == b.text_ };
  }

  // integer == float = bool
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Float) {
    return{ a.integer_ == b.float_ };
  }

  // float == integer = bool
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Integer) {
    return{ a.float_ == b.integer_ };
  }

  // integer == text = bool
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Text) {
    return{ std::to_string(a.integer_) == b.text_ };
  }

  // float == text = bool
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Text) {
    return{ std::to_string(a.float_) == b.text_ };
  }

  // text == integer = bool
  if (b.type_ == kValueType_Integer && a.type_ == kValueType_Text) {
    return{ a.text_ == std::to_string(b.integer_) };
  }

  // text == float = bool
  if (b.type_ == kValueType_Float && a.type_ == kValueType_Text) {
    return{ a.text_ == std::to_string(b.float_) };
  }

  ReportWarning("Value Equal Than Comparison Operation Failed.");
  return Value();
}

Value operator!=(const Value& a, const Value& b) {
  // integer != integer = bool 
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Integer) {
    return{ a.integer_ != b.integer_ };
  }

  // float != float = bool 
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Float) {
    return{ a.float_ != b.float_ };
  }

  // text != text = bool 
  if (a.type_ == kValueType_Text && b.type_ == kValueType_Text) {
    return{ a.text_ != b.text_ };
  }

  // integer != float = bool
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Float) {
    return{ a.integer_ != b.float_ };
  }

  // float != integer = bool
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Integer) {
    return{ a.float_ != b.integer_ };
  }

  // integer != text = bool
  if (a.type_ == kValueType_Integer && b.type_ == kValueType_Text) {
    return{ std::to_string(a.integer_) != b.text_ };
  }

  // float != text = bool
  if (a.type_ == kValueType_Float && b.type_ == kValueType_Text) {
    return{ std::to_string(a.float_) != b.text_ };
  }

  // text != integer = bool
  if (b.type_ == kValueType_Integer && a.type_ == kValueType_Text) {
    return{ a.text_ != std::to_string(b.integer_) };
  }

  // text != float = bool
  if (b.type_ == kValueType_Float && a.type_ == kValueType_Text) {
    return{ a.text_ != std::to_string(b.float_) };
  }

  ReportWarning("Value Not Equal Than Comparison Operation Failed.");
  return Value();
}

#pragma endregion

/*******************************************************************************
***                               VARIABLE                                   ***
*******************************************************************************/

#pragma region VARIABLE_CLASS 

   /// Class used to manage the variables that will be added to the registger.
   /// Depending on if the variable is a external registered one (created in C++) or
   /// if its one defined in the script, the attributes used will be different.
class Variable {

 public:

/******************************   ATTRIBUTES   ********************************/

  /// Will define if the variable is a external one or a script native one.
  bool is_registered_;
  /// Name of the variable.
  std::string name_;
  /// Pointer to the original variable defined in c++
  void* pointer_to_the_original_;
  /// Value of the variable
  Value value_;

/*******************************   METHODS   **********************************/

  Variable() {
    is_registered_ = false;
    name_ = "";
    pointer_to_the_original_ = nullptr;
  }

  Variable(const char* name) {
    is_registered_ = false;
    name_ = name;
    pointer_to_the_original_ = nullptr;
  }

  Variable(const char* name, int32 integer_value) {
    is_registered_ = false;
    name_ = name;
    pointer_to_the_original_ = nullptr;
    value_ = { integer_value };
  }

  Variable(const char* name, float32 float_value) {
    is_registered_ = false;
    name_ = name;
    pointer_to_the_original_ = nullptr;
    value_ = { float_value };
  }

  Variable(const char* name, const char* text_value) {
    is_registered_ = false;
    name_ = name;
    pointer_to_the_original_ = nullptr;
    value_ = { text_value };
  }

  Variable(const char* name, Value value) {
    is_registered_ = false;
    name_ = name;
    pointer_to_the_original_ = nullptr;
    value_ = value;
  }

  Variable(const char* name, ValueType type, void* ptr_to_the_original) {
    is_registered_ = true;
    name_ = name;
    pointer_to_the_original_ = ptr_to_the_original;
    value_.type_ = type;
  }

  ~Variable() {}

  Variable(const Variable& copy) {
    is_registered_ = copy.is_registered_;
    name_ = copy.name_;
    pointer_to_the_original_ = copy.pointer_to_the_original_;
    value_ = copy.value_;
  }

  Variable & operator=(const Variable& copy) {
    is_registered_ = copy.is_registered_;
    name_ = copy.name_;
    pointer_to_the_original_ = copy.pointer_to_the_original_;
    value_ = copy.value_;
    return *this;
  }

  Report setValue(const Value value) {

    if (is_registered_) {

      // Original value is a TEXT.
      if (value_.type_ == kValueType_Text && value.type_ == kValueType_Text) {
        std::string* ptr = static_cast<std::string*>(pointer_to_the_original_);
        *ptr = value.text_;
        return kReport_NoErrors;
      }

      if (value_.type_ == kValueType_Text && value.type_ == kValueType_Float) {
        std::string* ptr = static_cast<std::string*>(pointer_to_the_original_);
        *ptr = std::to_string(value.float_);
        return kReport_NoErrors;
      }

      if (value_.type_ == kValueType_Text && value.type_ == kValueType_Integer) {
        std::string* ptr = static_cast<std::string*>(pointer_to_the_original_);
        *ptr = std::to_string(value.integer_);
        return kReport_NoErrors;
      }

      // Original value is an INTEGER.
      if (value_.type_ == kValueType_Integer && value.type_ == kValueType_Integer) {
        int32* ptr = static_cast<int32*>(pointer_to_the_original_);
        *ptr = value.integer_;
        return kReport_NoErrors;
      }

      if (value_.type_ == kValueType_Integer && value.type_ == kValueType_Float) {
        int32* ptr = static_cast<int32*>(pointer_to_the_original_);
        *ptr = (int32)value.float_;
        return kReport_NoErrors;
      }

      // Original value is a FLOAT.
      if (value_.type_ == kValueType_Float && value.type_ == kValueType_Float) {
        float32* ptr = static_cast<float32*>(pointer_to_the_original_);
        *ptr = value.float_;
        return kReport_NoErrors;
      }

      if (value_.type_ == kValueType_Float && value.type_ == kValueType_Integer) {
        float32* ptr = static_cast<float32*>(pointer_to_the_original_);
        *ptr = (float32)value.integer_;
        return kReport_NoErrors;
      }

      ReportError(" Cant set variable value, types not match. " + name_);
      return kReport_InvalidValueType;
    }

    // if its not a registered variable.

    // Original value is a TEXT.
    if (value_.type_ == kValueType_Text && value.type_ == kValueType_Text) {
      value_.text_ = value.text_;
      return kReport_NoErrors;
    }

    if (value_.type_ == kValueType_Text && value.type_ == kValueType_Float) {
      value_.text_ = std::to_string(value.float_);
      return kReport_NoErrors;
    }

    if (value_.type_ == kValueType_Text && value.type_ == kValueType_Integer) {
      value_.text_ = std::to_string(value.integer_);
      return kReport_NoErrors;
    }

    // Original value is an INTEGER.
    if (value_.type_ == kValueType_Integer && value.type_ == kValueType_Integer) {
      value_.integer_ = value.integer_;
      return kReport_NoErrors;
    }

    if (value_.type_ == kValueType_Integer && value.type_ == kValueType_Float) {
      value_.integer_ = (int32)value.float_;
      return kReport_NoErrors;
    }

    // Original value is a FLOAT.
    if (value_.type_ == kValueType_Float && value.type_ == kValueType_Float) {
      value_.float_ = value.float_;
      return kReport_NoErrors;
    }

    if (value_.type_ == kValueType_Float && value.type_ == kValueType_Integer) {
      value_.float_ = (float32)value.integer_;
      return kReport_NoErrors;
    }

    // If the variable doesnt have a type, then we assign the external value type.
    if (value_.type_ == kValueType_None) {
      value_ = value;
      return kReport_NoErrors;
    }

    ReportError(" Cant set variable value, types not match. " + name_);
    return kReport_InvalidValueType;
  }

  const Value getValue() {
    if (!is_registered_) {
      return value_;
    }

    if (value_.type_ == kValueType_Text) {
      std::string* ptr = static_cast<std::string*>(pointer_to_the_original_);
      return{ ptr->c_str() };
    }

    if (value_.type_ == kValueType_Float) {
      float32* ptr = static_cast<float32*>(pointer_to_the_original_);
      return{ *ptr };
    }

    if (value_.type_ == kValueType_Integer) {
      int32* ptr = static_cast<int32*>(pointer_to_the_original_);
      return{ *ptr };
    }

    ReportError("Error getting the value of the variable which name is : " + name_);
    return Value();
  }


}; /* Variable */

#pragma endregion

/*******************************************************************************
***                         REGISTERED_FUNCTION                              ***
*******************************************************************************/

#pragma region REGISTERED_FUNCTION_CLASS 

class RegisteredFunction {

 public:

/******************************   ATTRIBUTES   ********************************/

  /// Name of the function
  std::string name_;
  /// Pointer to the function defined in C++
  void(*function_pointer_)(std::vector<Value>&);
  /// Parameters of the function when its called from the script.
  std::vector<Value> params_;

/*******************************   METHODS   **********************************/

  RegisteredFunction() {
    name_ = "";
    function_pointer_ = nullptr;
    params_.reserve(5);
  }

  RegisteredFunction(const char* name,
    void(*ptr)(std::vector<Value>&)) {
    name_ = name;
    function_pointer_ = ptr;
  }

  ~RegisteredFunction() {
    params_.clear();
    function_pointer_ = nullptr;
  }

  RegisteredFunction(const RegisteredFunction& copy) {
    name_ = copy.name_;
    function_pointer_ = copy.function_pointer_;
    params_ = copy.params_;
  }

  RegisteredFunction& operator=(const RegisteredFunction& copy) {
    name_ = copy.name_;
    function_pointer_ = copy.function_pointer_;
    params_ = copy.params_;
    return *this;
  }


}; /* RegisteredFunction */

#pragma endregion

/*******************************************************************************
***                                TOKEN                                     ***
*******************************************************************************/

#pragma region TOKEN_CLASS 

class Token {

 public:

/******************************   ATTRIBUTES   ********************************/

  /// Token characters.
  std::string text_;
  /// Token type.
  TokenType type_;
  /// Token priority
  int32 priority_;

/*******************************   METHODS   **********************************/

  Token() {
    type_ = kTokenType_None;
    text_ = "";
    priority_ = DEFAULT_PRIORITY;
  }

  Token(const char* text, const TokenType type, const int32 priority) {
    type_ = type;
    text_ = text;
    priority_ = priority;
  }

  ~Token() {}

  Token(const Token& copy) {
    type_ = copy.type_;
    text_ = copy.text_;
    priority_ = copy.priority_;
  }

  Token& operator=(const Token& copy) {
    type_ = copy.type_;
    text_ = copy.text_;
    priority_ = copy.priority_;
    return *this;
  }

}; /* Token */

#pragma endregion







}; /* JMP_SINGLE_HEADER */

#endif /* __JMP_SINGLE_HEADER_H__ */