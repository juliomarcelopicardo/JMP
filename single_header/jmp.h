/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP_PROJECT - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/


/* SINGLE HEADER LIBRARY */

#ifndef __JMP_SINGLE_HEADER_H__
#define __JMP_SINGLE_HEADER_H__ 1

#include <stdint.h>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>
#include <Windows.h>

namespace JMP {

// Forward declaration
class Machine;

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
#define INVALID_FUNCTION_ID -9999

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

/// Command type.
enum CommandType {
  kCommandType_None = 0,
  // Separators -> mathematical operations
  kCommandType_Addition,
  kCommandType_Substraction,
  kCommandType_Multiply,
  kCommandType_Division,
  kCommandType_Power,
  // Separator -> "=" Assignment.
  kCommandType_EqualAssigment,
  // Separator -> Comparisons.
  kCommandType_GreaterThanComparison,
  kCommandType_LowerThanComparison,
  kCommandType_GreaterOrEqualThanComparison,
  kCommandType_LowerOrEqualThanComparison,
  kCommandType_EqualThanComparison,
  kCommandType_NotEqualThanComparison,
  // Stack Actions
  kCommandType_PushToTheStack,
  // Function
  kCommandType_FunctionDefinition,
  kCommandType_FunctionCall,
  kCommandType_FunctionReturn,
  kCommandType_FunctionEnd,
  kCommandType_FunctionNumParameters,
  kCommandType_FunctionParameter,
  // Body state -> Conditional, loop iteration or function body ended.
  kCommandType_FinishedConditionalOrLoop,
  // Condition Evaluation, can be the condition of a loop or an if.
  kCommandType_ConditionToEvaluate,
  // Variable definition
  kCommandType_VariableDefinition,
  // Variable pack
  kCommandType_VariablePackDefinition,
  kCommandType_VariablePackEnd,
  // Loop
  kCommandType_LoopStartPoint,
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

void PrintReport(Report& report, uint32 line_number = 0) {
  switch (report) {
    case kReport_EmptyLine: {
      //ReportWarning("Line " + std::to_string(line_number) + ": Nothing to compile..");
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

  float32 Sin() {
    if (type_ == kValueType_Float) { return sinf(float_); }
    if (type_ == kValueType_Integer) { return sinf((float32)integer_); }
    ReportWarning(" Trying to calculate a SIN of a non number value");
    return INITIALIZATION_VALUE;
  }

  float32 Cos() {
    if (type_ == kValueType_Float) { return cosf(float_); }
    if (type_ == kValueType_Integer) { return cosf((float32)integer_); }
    ReportWarning(" Trying to calculate a COS of a non number value");
    return INITIALIZATION_VALUE;
  }

  float32 Tan() {
    if (type_ == kValueType_Float) { return tanf(float_); }
    if (type_ == kValueType_Integer) { return tanf((float32)integer_); }
    ReportWarning(" Trying to calculate a TAN of a non number value");
    return INITIALIZATION_VALUE;
  }

  float32 ASin() {
    if (type_ == kValueType_Float) { return asinf(float_); }
    if (type_ == kValueType_Integer) { return asinf((float32)integer_); }
    ReportWarning(" Trying to calculate a ASIN of a non number value");
    return INITIALIZATION_VALUE;
  }

  float32 ACos() {
    if (type_ == kValueType_Float) { return acosf(float_); }
    if (type_ == kValueType_Integer) { return acosf((float32)integer_); }
    ReportWarning(" Trying to calculate a ACOS of a non number value");
    return INITIALIZATION_VALUE;
  }

  float32 ATan() {
    if (type_ == kValueType_Float) { return atanf(float_); }
    if (type_ == kValueType_Integer) { return atanf((float32)integer_); }
    ReportWarning(" Trying to calculate a ATAN of a non number value");
    return INITIALIZATION_VALUE;
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

/// Class used to manage the different functions registered from c++
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

/// Class used to manage the different tokens or keys of the scripts.
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

/*******************************************************************************
***                            TOKEN_MANAGER                                 ***
*******************************************************************************/

#pragma region TOKEN_MANAGER_CLASS 

/// Class used to save and manage the tokens.
class TokenManager {

 private:

/******************************   ATTRIBUTES   ********************************/

  /// List of all the tokens.
  std::vector<Token> token_list_;
  /// Number of elements of the list.
  int32 token_list_length_;

/*******************************   METHODS   **********************************/

  /// Default copy constructor.
  TokenManager(const TokenManager& copy);
  /// Assignment operator.
  TokenManager& operator=(const TokenManager& copy);

 public:

/*******************************   METHODS   **********************************/

  TokenManager() {
    token_list_length_ = 0;
    token_list_.reserve(10);
  }

  ~TokenManager() {
    token_list_.clear();
  }

  void addToken(const char * text, 
                const TokenType type = kTokenType_None, 
                int32 priority = DEFAULT_PRIORITY) {

    token_list_.push_back({ text, type, priority });
    token_list_length_++;

    // To determine if there's a function name behind the token.
    if (token_list_length_ >= 2) {
      if (token_list_[token_list_length_ - 1].text_ == "(" &&
        token_list_[token_list_length_ - 2].type_ == kTokenType_Variable) {
        token_list_[token_list_length_ - 2].priority_ = FUNCTION_NAME_PRIORITY;
      }
    }
  }

  void addToken(const Token& token) {
    token_list_.push_back(token);
    token_list_length_++;

    // To determine if there's a function name behind the token.
    if (token_list_length_ >= 2) {
      if (token_list_[token_list_length_ - 1].text_ == "(" &&
        token_list_[token_list_length_ - 2].type_ == kTokenType_Variable) {
        token_list_[token_list_length_ - 2].priority_ = FUNCTION_NAME_PRIORITY;
      }
    }
  }

  void clear() {
    token_list_.clear();
    token_list_length_ = 0;
  }

  void removeToken(int32 id) {
    token_list_.erase(token_list_.begin() + id);
    token_list_length_--;
  }

  void transferParenthesisContent(int32 open_parenthesis_id,
    int32 close_parenthesis_id,
    TokenManager* transfer_output) {
    // Copying the content into the output.
    int32 num_tokens_transfered = 0;
    for (int32 i = open_parenthesis_id + 1; i < close_parenthesis_id; i++) {
      transfer_output->addToken(token_list_[i]);
      num_tokens_transfered++;
    }

    // Changing the open parenthesis token to text "RESULT", None type, priority 0.
    token_list_[open_parenthesis_id] = { "RESULT", kTokenType_None, 0 };

    // Deleting the rest of the tokens. including the ")" one.
    for (int32 i = 0; i < num_tokens_transfered + 1; i++) {
      removeToken(open_parenthesis_id + 1);
    }
  }

  void transferContentBetweenIDsInclusive(int32 initial_id,
                                          int32 final_id,
                                          TokenManager* transfer_output = nullptr) {
    // Copying the content into the output.
    int32 num_tokens_transfered = 0;
    for (int32 i = initial_id; i <= final_id; i++) {
      if (transfer_output) { transfer_output->addToken(token_list_[i]); }
      num_tokens_transfered++;
    }

    // Changing the open parenthesis token to text "RESULT", None type, priority 0.
    token_list_[initial_id] = { "RESULT", kTokenType_None, 0 };

    // Deleting the rest of the tokens. including the ")" one.
    for (int32 i = 0; i < num_tokens_transfered - 1; i++) {
      removeToken(initial_id + 1);
    }
  }

  Token getToken(const int32 list_index) {
    if (list_index >= token_list_length_ || list_index < 0) {
      ReportError("Token list index \"" + std::to_string(list_index) + "\" is out of range.");
      return{ "", kTokenType_None, 0 };
    }
    return token_list_[list_index];
  }

  int32 getHighestPriorityTokenIndex() {

    int32 maximum_priority = -1;
    int32 index = -1;

    for (int32 i = 0; i < token_list_length_; i++) {
      if (token_list_[i].priority_ > maximum_priority) {
        maximum_priority = token_list_[i].priority_;
        index = i;
      }
    }

    return index;
  }



  int32 getNextCloseParenthesisIndex(int32 open_parenthesis_index) {

    for (int32 i = open_parenthesis_index; i < token_list_length_; ++i) {
      if (token_list_[i].text_ == ")") {
        return i;
      }
    }

    return -1; // No maatching close parenthesis found.
  }

  const uint32 numTokens() {
    return token_list_length_;
  }

  const bool areAnyCommaTokenInList() {
    for (int32 i = 0; i < token_list_length_; i++) {
      if (token_list_[i].text_ == ",") {
        return true;
      }
    }
    return false;
  }

  void printToken(int32 id) {

    OutputDebugString(" Type: ");
    switch (token_list_[id].type_) {
    case kTokenType_None: { OutputDebugString("None \t\t"); }break;
    case kTokenType_Keyword: { OutputDebugString("Keyword \t\t"); }break;
    case kTokenType_Number: { OutputDebugString("Number \t\t"); }break;
    case kTokenType_Separator: { OutputDebugString("Separator \t"); }break;
    case kTokenType_Variable: { OutputDebugString("Variable \t"); }break;
    default: { OutputDebugString("None \t\t"); }break;
    }

    char priority[32] = "";
    if (token_list_[id].priority_ == 0) {
      sprintf_s(priority, 32, "Priority: %d\t\t", token_list_[id].priority_);
    }
    else {
      sprintf_s(priority, 32, "Priority: %d\t", token_list_[id].priority_);
    }
    OutputDebugString(priority);

    OutputDebugString("Token: \"");
    OutputDebugString(token_list_[id].text_.c_str());
    OutputDebugString("\"\n");

  }

  void printTokenList() {
    for (int32 i = 0; i < token_list_length_; i++) {
      printToken(i);
    }
  }

}; /* TokenManager */

#pragma endregion

/*******************************************************************************
***                              FUNCTION                                    ***
*******************************************************************************/

#pragma region FUNCTION_CLASS 

/// Now we create a list of functions, that will be used to jump between different
/// function calls, getting it's variabes and being able to go back to the step
/// id where the function was called */
class Function {

 private:

/******************************   ATTRIBUTES   ********************************/
  /// List of variables allocated in the function scope.
  std::vector<Variable> variable_list_;
  /// Origin command id, where the function is called.
  int32 origin_id_;
  /// Variable list length
  int32 variable_list_length_;

 public:

/*******************************   METHODS   **********************************/

  Function() {
    variable_list_.reserve(10);
    origin_id_ = INVALID_FUNCTION_ID;
    variable_list_length_ = 0;
  }

  Function(const int32 origin_id) {
    variable_list_.reserve(10);
    origin_id_ = origin_id;
    variable_list_length_ = 0;
  }

  ~Function() {
    variable_list_.clear();
    variable_list_length_ = 0;
  }

  Function(const Function& copy) {
    variable_list_ = copy.variable_list_;
    origin_id_ = copy.origin_id_;
    variable_list_length_ = copy.variable_list_length_;
  }

  Function& operator=(const Function& copy) {
    variable_list_ = copy.variable_list_;
    origin_id_ = copy.origin_id_;
    variable_list_length_ = copy.variable_list_length_;
    return *this;
  }

  Report addVariable(const Variable variable) {
    variable_list_.push_back(variable);
    variable_list_length_++;
    return kReport_NoErrors;
  }

  Report addVariable(const char* name, const Value value) {
    variable_list_.push_back({ name, value });
    variable_list_length_++;
    return kReport_NoErrors;
  }

  const int32 getVariableID(const std::string& variable_name) {
    for (int32 i = 0; i < variable_list_length_; i++) {
      if (variable_list_[i].name_ == variable_name) {
        return i;
      }
    }
    //ReportWarning(" Couldnt get variable from function. Variable name: " + variable_name);
    return INVALID_FUNCTION_ID;
  }

  Variable* getVariable(const std::string& variable_name) {
    int32 id = getVariableID(variable_name);
    if (id != INVALID_FUNCTION_ID) {
      return &variable_list_[id];
    }
    return nullptr;
  }

  const int32 numVariables() {
    return variable_list_length_;
  }

  const int32 originID() {
    return origin_id_;
  }


}; /* Function */

#pragma endregion

/*******************************************************************************
***                              COMMAND                                     ***
*******************************************************************************/

#pragma region COMMAND_CLASS 

/// Class used to save and manage the compiling commands or instructions.
class Command {

 public:

/******************************   ATTRIBUTES   ********************************/

  /// Command type.
  CommandType type_;
  /// Name of the command.
  std::string name_;

/*******************************   METHODS   **********************************/

  Command() {
    type_ = kCommandType_None;
    name_ = "";
  }

  Command(const CommandType type, const char* name) {
    type_ = type;
    name_ = name;
  }

  ~Command() {}

  Command(const Command& copy) {
    type_ = copy.type_;
    name_ = copy.name_;
  }

  Command & operator=(const Command& copy) {
    type_ = copy.type_;
    name_ = copy.name_;
    return *this;
  }

  ValueType getNameDataType() {
    int32 name_length = name_.size();
    if (name_length == 0) { return kValueType_None; }
    if (name_[0] == '"' && name_[name_length - 1] == '"') { return kValueType_Text; }
    if (name_[0] == '-' || isDigit(name_[0])) { // Check type of number
      int32 num_dots = 0;
      for (int32 i = 1; i < name_length; i++) {
        if (!isDigit(name_[i])) {
          if (name_[i] == '.' && num_dots == 0) {
            num_dots++;
          }
          else {
            ReportWarning(" Incorrect name of command.");
            return kValueType_None;
          }
        }
      }
      if (num_dots == 1) { return kValueType_Float; }
      else { return kValueType_Integer; }
    }
    return kValueType_None;
  }

  const bool isDigit(const char8& character) {
    switch (character) {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      return true;
    }
    return false;
  }

/****************************  METHODS PROTOTYPES  ****************************/

  Report execute(Machine* machine, int32& next_command_to_execute);
  Report executeAddition(Machine* machine, int32& next_command_to_execute);
  Report executeSubstraction(Machine* machine, int32& next_command_to_execute);
  Report executeMultiply(Machine* machine, int32& next_command_to_execute);
  Report executeDivision(Machine* machine, int32& next_command_to_execute);
  Report executePower(Machine* machine, int32& next_command_to_execute);
  Report executeEqualAssignment(Machine* machine, int32& next_command_to_execute);
  Report executeGreaterThanComparison(Machine* machine, int32& next_command_to_execute);
  Report executeLowerThanComparison(Machine* machine, int32& next_command_to_execute);
  Report executeGreaterOrEqualThanComparison(Machine* machine, int32& next_command_to_execute);
  Report executeLowerOrEqualThanComparison(Machine* machine, int32& next_command_to_execute);
  Report executeEqualThanComparison(Machine* machine, int32& next_command_to_execute);
  Report executeNotEqualThanComparison(Machine* machine, int32& next_command_to_execute);
  Report executePushToTheStack(Machine* machine, int32& next_command_to_execute);
  Report executeFunctionDefinition(Machine* machine, int32& next_command_to_execute);
  Report executeFunctionCall(Machine* machine, int32& next_command_to_execute);
  Report executeFunctionReturn(Machine* machine, int32& next_command_to_execute);
  Report executeFunctionEnd(Machine* machine, int32& next_command_to_execute);
  Report executeFunctionNumParams(Machine* machine, int32& next_command_to_execute);
  Report executeFunctionParam(Machine* machine, int32& next_command_to_execute);
  Report executeFinishedConditionalOrLoop(Machine* machine, int32& next_command_to_execute);
  Report executeConditionToEvaluate(Machine* machine, int32& next_command_to_execute);
  Report executeVariableDefinition(Machine* machine, int32& next_command_to_execute);
  Report executeVariablePackDefinition(Machine* machine, int32& next_command_to_execute);
  Report executeVariablePackEnd(Machine* machine, int32& next_command_to_execute);
  Report executeLoopStartPoint(Machine* machine, int32& next_command_to_execute);

}; /* Command */

#pragma endregion

/*******************************************************************************
***                              COMPILER                                    ***
*******************************************************************************/

#pragma region COMPILER_CLASS 

/// Text parser and compiler used to save and manage all the tokens from .jmp files.
class Compiler {

private:

  /******************************   ATTRIBUTES   ********************************/

    /// List of tags used to group different bodys of conditionals, loops or functions.
  std::vector<Tag> tag_list_;

  /*******************************   METHODS   **********************************/

    /// Default copy constructor.
  Compiler(const Compiler& copy);
  /// Assignment operator.
  Compiler& operator=(const Compiler& copy);

public:

  /******************************   ATTRIBUTES   ********************************/

    /// Current sentence that we will try to compile.
  std::string sentence_;
  /// Current sentence index, to know which character are we compiling.
  uint32 sentence_index_;
  /// Current token that we are analyzing.
  Token current_token_;

  /*******************************   METHODS   **********************************/

  Compiler() {
    current_token_.text_.clear();
    current_token_.type_ = kTokenType_None;
    sentence_.clear();
    sentence_index_ = 0;
    tag_list_.reserve(10);
  }

  ~Compiler() {
    current_token_.text_.clear();
    sentence_.clear();
    tag_list_.clear();
  }

  void replaceSpacesFromQuotes(std::string& sentence, char8 replacement = '_') {

    uint32 length = sentence.length();

    for (uint32 i = 0; i < length; ++i) {
      if (sentence[i] == '"') { // Begin of the quote.
        ++i;
        for (uint32 j = i; j < length; ++j) {
          ++i;
          if (sentence[j] == '"') { break; } // End of the quote.
          if (sentence[j] == ' ') { sentence[j] = replacement; }
        }
      }
    }

  }

  void recoverSpacesFromQuotes(std::string & sentence, char8 replacement = '_') {
    uint32 length = sentence.length();

    if (sentence[0] == '"') {
      for (uint32 i = 0; i < length; ++i) {
        if (sentence[i] == replacement) { sentence[i] = ' '; }
      }
    }

  }


  const bool isSeparator(const char8& character) {
    switch (character) {
    case ' ': case ',': case ';': case '?': case '!': case '^':
    case '+': case '-': case '*': case '/': case '=': case '%':
    case '(': case ')': case '{': case '}': case '<': case '>':
      return true;
    }
    return false;
  }

  const bool isBlankSpace(const char8& character) {
    if (character == ' ') {
      return true;
    }
    return false;
  }

  const bool isLetter(const char8& character) {
    if ((character >= 'a' && character <= 'z') ||
      (character >= 'A' && character <= 'Z') ||
      (character == '"') || (character == '_')) { // needed for quotes.
      return true;
    }
    return false;
  }

  const bool isDigit(const char8& character) {
    switch (character) {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      return true;
    }
    return false;
  }

  const bool isKeyword(const std::string& word) {

    if (word == "IF" || word == "ELSE" ||                   // Conditionals
      word == "FUNC" || word == "RETURN" ||               // Functions
      word == "DO" || word == "WHILE" || word == "FOR" || // Loops
      word == "VAR" || word == "VARPACK") {               // Variables and packs
      return true;
    }
    return false;
  }

  void addTag(const Tag tag) {
    tag_list_.push_back(tag);
  }

  const Tag getAndRemoveLastTag() {
    int32 num_tags = tag_list_.size();
    if (num_tags <= 0) {
      Report report = kReport_NoTagsToDelete;
      PrintReport(report);
      return kTag_None;
    }
    Tag tag = tag_list_[num_tags - 1];
    tag_list_.pop_back();
    return tag;
  }

  void generateTokens(std::string& sentence, TokenManager& token_manager) {

    int32 num_parenthesis_opened = 0;

    replaceSpacesFromQuotes(sentence, '_'); // Replace ' ' ==> '_'

    sentence_index_ = 0;
    sentence_ = sentence;

    generateNextToken(); // Setting the first token as current.

    while (current_token_.text_ != "") {

      // Everytime a parenthesis is opened, we increase the priority.
      // This way we will avoid the problem with multiple parenthetical groups.
      if (current_token_.text_ == "(") {
        current_token_.priority_ += num_parenthesis_opened;
        num_parenthesis_opened++;
      }

      recoverSpacesFromQuotes(current_token_.text_); // Replace '_' ==> ' ' in quotes
      token_manager.addToken(current_token_);
      generateNextToken();
    }

  }

  void generateNextToken() {

    uint32 sentence_length = sentence_.length();

    // Restarts the current token.
    current_token_.text_ = "";
    current_token_.type_ = kTokenType_None;

    // To analyze sentences, spaces will be ignored.
    while (sentence_index_ < sentence_length &&
      isBlankSpace(sentence_[sentence_index_])) {
      sentence_index_++;
    }

    // Checking end of line.
    if (sentence_index_ >= sentence_length) {
      current_token_.text_ = "\0";
    }

    // TOKEN ANALYZE: We will get the token type and the token text.

    // Text between quotes, then we will ignore separators, etc. 
    if (sentence_[sentence_index_] == '\"') {
      current_token_.text_.push_back(sentence_[sentence_index_]);
      sentence_index_++;
      while (sentence_index_ < sentence_length) {
        current_token_.text_.push_back(sentence_[sentence_index_]);
        if (sentence_[sentence_index_] == '\"') {
          sentence_index_++;
          break;
        }
        sentence_index_++;
      }

      current_token_.type_ = kTokenType_Variable;
    }

    // Separators 
    else if (isSeparator(sentence_[sentence_index_])) {
      // Operators "==", "!=", "<=", ">=" checking
      if (sentence_[sentence_index_] == '=' ||
        sentence_[sentence_index_] == '!' ||
        sentence_[sentence_index_] == '<' ||
        sentence_[sentence_index_] == '>') {

        if (sentence_index_ + 1 < sentence_length && sentence_[sentence_index_ + 1] == '=') {
          current_token_.text_.push_back(sentence_[sentence_index_]);
          sentence_index_++;
        }
      }
      current_token_.text_.push_back(sentence_[sentence_index_]);
      current_token_.type_ = kTokenType_Separator;
      sentence_index_++;
    }

    // Numbers
    else if (isDigit(sentence_[sentence_index_])) {
      while (!isSeparator(sentence_[sentence_index_]) && sentence_index_ < sentence_length) {
        current_token_.text_.push_back(sentence_[sentence_index_]);
        sentence_index_++;
      }
      current_token_.type_ = kTokenType_Number;
    }

    // Vars and Keywords
    else if (isLetter(sentence_[sentence_index_])) {
      while (!isSeparator(sentence_[sentence_index_]) && sentence_index_ < sentence_length) {
        current_token_.text_.push_back(sentence_[sentence_index_]);
        sentence_index_++;
      }

      current_token_.type_ = kTokenType_Variable;
      if (isKeyword(current_token_.text_)) { current_token_.type_ = kTokenType_Keyword; }
    }

    // Once the type is set, we will assign an initial priority depending on the type.
    generateCurrentTokenInitialPriority();
  }

  void generateCurrentTokenInitialPriority() {
    switch (current_token_.type_) {
    case kTokenType_Keyword: { current_token_.priority_ = KEYWORD_PRIORITY; } break;
    case kTokenType_Separator: {
      std::string separator = current_token_.text_;
      if (separator == "}") { current_token_.priority_ = CLOSE_BRACKETS_PRIORITY; }
      else if (separator == "(") { current_token_.priority_ = OPEN_PARENTHESIS_PRIORITY; }
      else if (separator == "^") { current_token_.priority_ = POWER_OPERATION_PRIORITY; }
      else if (separator == "*" || separator == "/") { current_token_.priority_ = MULTIPLY_OPERATION_PRIORITY; }
      else if (separator == "+" || separator == "-") { current_token_.priority_ = ADDITION_OPERATION_PRIORITY; }
      else if (separator == ">" || separator == "<") { current_token_.priority_ = COMPARISON_PRIORITY; }
      else if (separator == ">=" || separator == "<=") { current_token_.priority_ = COMPARISON_PRIORITY; }
      else if (separator == "==" || separator == "!=") { current_token_.priority_ = COMPARISON_PRIORITY; }
      else if (separator == "=") { current_token_.priority_ = EQUAL_PRIORITY; }
      else if (separator == ",") { current_token_.priority_ = COMMA_PRIORITY; }
    }break;
    default: { current_token_.priority_ = DEFAULT_PRIORITY; } break;
    }
  }

  /****************************  METHODS PROTOTYPES  ****************************/

  Report compile(Machine* machine, std::string sentence, uint32 line_number);
  Report compileTokens(Machine* machine, TokenManager& token_manager);
  Report compileKeywordToken(Machine* machine, TokenManager& token_manager, int32& token_index);
  Report compileSeparatorToken(Machine* machine, TokenManager& token_manager, int32& token_index);
  const bool checkIfAndCompileCommasContent(Machine* machine, TokenManager& token_manager);
  Report compileOpenParenthesisSeparatorToken(Machine* machine, TokenManager& token_manager, int32& token_index);
  Report compileCloseBracketsSeparatorToken(Machine* machine, TokenManager& token_manager, int32& token_index);
  Report compileAdditionOperationSeparatorToken(Machine* machine, TokenManager& token_manager, int32& token_index);
  Report compileMultiplyOperationSeparatorToken(Machine* machine, TokenManager& token_manager, int32& token_index);
  Report compilePowerOperationSeparatorToken(Machine* machine, TokenManager& token_manager, int32& token_index);
  Report compileComparisonOperationSeparatorToken(Machine* machine, TokenManager& token_manager, int32& token_index);
  Report compileEqualSeparatorToken(Machine* machine, TokenManager& token_manager, int32& token_index);
  Report compileConditionalKeywordToken(Machine* machine, TokenManager& token_manager, int32& token_index);
  Report compileReturnKeywordToken(Machine* machine, TokenManager& token_manager, int32& token_index);
  Report compileFunctionKeywordToken(Machine* machine, TokenManager& token_manager, int32& token_index);
  Report compileLoopKeywordToken(Machine* machine, TokenManager& token_manager, int32& token_index);
  Report compileVariableKeywordToken(Machine* machine, TokenManager& token_manager, int32& token_index);
  Report compileVariablePackKeywordToken(Machine* machine, TokenManager& token_manager, int32& token_index);


}; /* Compiler */

#pragma endregion

/*******************************************************************************
***                              MACHINE                                     ***
*******************************************************************************/

#pragma region MACHINE_CLASS 

/// Core of the language, this machine will be used to process .jpm script files.
class Machine {

public:

/******************************   ATTRIBUTES   ********************************/

  /// Stack of inmediate values. Need to be static as will be shared between machines.
  struct Stack {
    Stack() { length = 0; value.reserve(30); }
    /// Values list
    std::vector<Value> value;
    /// Number of elements of the stack.
    int32 length;
  };

  /// Stack of inmediate values.Need to be static as will be shared between machines.
  static Stack stack_;

private:

/******************************   ATTRIBUTES   ********************************/

/// List of all the compiling commands.
  std::vector<Command> cmd_list_;
  /// Number of elements of the list.
  int32 cmd_list_length_;

  /// List of all the variable registered from C++.
  std::vector<Variable> variable_registry_;
  /// Number of elements of the list.
  int32 variable_registry_length_;

  struct VariablePack {
    std::string name;
    std::vector<Variable> var;
  };
  /// List of global variables pack
  std::vector<VariablePack> global_variable_pack_list_;
  /// Global variable list length.
  int32 global_variable_pack_list_length_;
  /// Current global variable pack
  int32 current_global_variable_pack_;

  /// List of all the functions registered from C++.
  std::vector<RegisteredFunction> function_registry_;
  /// Number of elements of the list.
  int32 function_registry_length_;
  
  /** Everytime we declare a function in the script, we will add here its command
  As we will need its position in the command list to execute it. */
  struct DefinedFunction {
    std::string name;
    int32 command_index;
  };

  /// List of functions defined in the script using the syntax "func name(params)"
  std::vector<DefinedFunction> defined_function_list_;
  /// Defined function list length -> number of elements.
  int32 defined_function_list_length_;

  /// List of functions used in the execution.
  std::vector<Function> function_list_;
  /// Number of functions.
  int32 function_list_length_;

  /// Last script compiled file path.
  std::string last_script_compiled_path_;



public:

/*******************************   METHODS   **********************************/

  Machine::Machine() {
    cmd_list_.reserve(50);
    cmd_list_length_ = 0;
    function_list_.reserve(10);
    function_list_length_ = 0;
    function_registry_.reserve(5);
    function_registry_length_ = 0;
    variable_registry_.reserve(5);
    variable_registry_length_ = 0;
    defined_function_list_.reserve(10);
    defined_function_list_length_ = 0;
    global_variable_pack_list_.resize(1);
    global_variable_pack_list_[0].name = "";
    global_variable_pack_list_length_ = 1;
    current_global_variable_pack_ = 0;
    last_script_compiled_path_ = "";
  }

  Machine::~Machine() {
    cmd_list_.clear();
    variable_registry_.clear();
    function_registry_.clear();
    defined_function_list_.clear();
    function_list_.clear();
    cmd_list_length_ = 0;
    variable_registry_length_ = 0;
    function_registry_length_ = 0;
    defined_function_list_length_ = 0;
    function_list_length_ = 0;
    last_script_compiled_path_.clear();
    global_variable_pack_list_length_ = 0;
    current_global_variable_pack_ = 0;
    global_variable_pack_list_.clear();
  }

  Report Machine::processFile(std::string script_filename) {

    // To force .jmp extensions.
    if (checkExtension(script_filename) != kReport_NoErrors) {
      return kReport_InvalidFileExtension;
    }

    std::ifstream script; // ifstream because we just want to read the file.
    script.open(script_filename); // Default read mode is read only "in".

    if (!script) {
      ReportError("\"" + script_filename + "\" - Can't open file");
      return kReport_InvalidFilename;
    }

    Compiler compiler;
    uint32 line_num = 1;
    Report report = kReport_NoErrors;

    // Reading the whole file, allocating it using lines of code.
    while (!script.eof() && report == kReport_NoErrors) {
      std::string code_line;
      std::getline(script, code_line);

      // To prevent errors I will remove all the tabs. Changing them to blank spaces
      uint32 string_length = code_line.length();
      for (uint32 i = 0; i < string_length; i++) {
        if (code_line[i] == '\t') { code_line[i] = ' '; }
      }

      /*
      PROCESS THE LINES - COMPILING THEM.
      */
      report = compiler.compile(this, code_line, line_num);
      line_num++;
    }


    // If the file has been loaded properly, then we will save its path for possible reloadings.
    if (report == kReport_NoErrors) {
      last_script_compiled_path_ = script_filename;
    }

    report = runScriptToSaveGlobalVariables();

    script.clear();
    script.close();

    return report;
  }



  Report Machine::checkExtension(std::string filename) {

    // We will take the last 4 characters to check the extension.
    uint32 index = filename.length() - 4;
    std::string pepe = filename.substr(index);
    if (index > 1 && filename.substr(index) == ".jmp") {
      return kReport_NoErrors;
    }

    // Name too short.
    if (index < 1) {
      ReportError("\"" + filename + "\" - Filename too short");
      return kReport_FilenameTooShort;
    }

    // Invalid extension.
    ReportError("\"" + filename + "\" - Extension must be .jmp");
    return kReport_InvalidFileExtension;
  }

  Report Machine::runFunction(std::string function_call_sentence = "main()") {

    // Create a machine and a parser to compile the code received by parameter.
    Machine other_machine;
    Compiler other_compiler;
    Report report;
    report = other_compiler.compile(&other_machine, function_call_sentence, -1); // -1 as theres no line number
    if (report != kReport_NoErrors) { return report; }

    // Once compiled we will concatenate both machines command lists.
    // The result will be, this_machine = this_machine + other_machine;
    int32 this_machine_initial_num_commands = numCommands();
    pushBackOtherMachineCommandList(&other_machine);
    int32 num_commands_added = other_machine.numCommands();

    // Now we will execute the complete command list
    int32 index = this_machine_initial_num_commands;
    while (index < cmd_list_length_) {
      // possible results of the execution "NoError", "return is called" or "Error"
      report = cmd_list_[index].execute(this, index);
      if (report != kReport_NoErrors) { break; }
    }

    if (report == kReport_LastActiveFunctionReturnCalled) { report = kReport_NoErrors; }

    // Delete the commands added from the other machine.
    for (int32 i = 0; i < num_commands_added; i++) {
      removeCommand(this_machine_initial_num_commands);
    }

    return report;
  }

  Report Machine::runScriptToSaveGlobalVariables() {
    Report report = kReport_NoErrors;
    Command cmd;
    int32 index = 0;
    while (index < cmd_list_length_) {
      // possible results of the execution "NoError", "return is called" or "Error"
      cmd = cmd_list_[index];
      if (cmd.type_ == kCommandType_FunctionDefinition) { // To avoid functions
        while (index < cmd_list_length_ && cmd_list_[index].type_ != kCommandType_FunctionEnd) {
          index++;
        }
        index++;
      }
      else {
        report = cmd.execute(this, index);
        if (report != kReport_NoErrors) { break; }
      }
    }

    return report;
  }

  void Machine::reload() {

    // We will clean all the buffers used. We will reserve the same size it needed before.
    cmd_list_.clear();
    cmd_list_.reserve(cmd_list_length_);
    cmd_list_length_ = 0;
    function_list_.clear();
    function_list_.reserve(function_list_length_);
    function_list_length_ = 0;
    defined_function_list_.clear();
    defined_function_list_.reserve(defined_function_list_length_);
    defined_function_list_length_ = 0;
    global_variable_pack_list_.reserve(global_variable_pack_list_length_);
    addGlobalVariablePack("");

    /*
    We wont delete the registrys, as functions or variables registered from c++
    doesnt depend on the file, is the user the one who should manage what to
    register and unregister in each moment.
    */
    /*
    function_registry_.clear();
    function_registry_.reserve(function_registry_length_);
    function_registry_length_ = 0;
    variable_registry_.clear();
    variable_registry_.reserve(variable_registry_length_);
    variable_registry_length_ = 0;
    */

    // Then we proccess the file again.
    processFile(last_script_compiled_path_);
  }

  void Machine::addCommand(const CommandType type) {
    Command cmd = { type, "" };
    cmd_list_.push_back(cmd);
    cmd_list_length_++;
  }

  void Machine::addCommand(const CommandType type, const char* name) {
    Command cmd = { type, name };
    if (cmd.name_ != "RESULT") { // Temporal tokens waiting for result no added.
      cmd_list_.push_back(cmd);
      cmd_list_length_++;
      if (cmd.type_ == kCommandType_FunctionDefinition) {
        addDefinedFunction(name, cmd_list_length_ - 1);
      }
    }
  }

  void Machine::addCommand(const CommandType type, const std::string name) {
    Command cmd = { type, name.c_str() };
    if (cmd.name_ != "RESULT") {
      cmd_list_.push_back(cmd);
      cmd_list_length_++;
      if (cmd.type_ == kCommandType_FunctionDefinition) {
        addDefinedFunction(name.c_str(), cmd_list_length_ - 1);
      }
    }
  }

  void Machine::addCommand(const Command& command) {
    if (command.name_ != "RESULT") {
      cmd_list_.push_back(command);
      cmd_list_length_++;
      if (command.type_ == kCommandType_FunctionDefinition) {
        addDefinedFunction(command.name_.c_str(), cmd_list_length_ - 1);
      }
    }
  }

  void Machine::clearCommandList() {
    cmd_list_.clear();
    cmd_list_length_ = 0;
  }

  void Machine::removeCommand(int32 id) {
    cmd_list_.erase(cmd_list_.begin() + id);
    cmd_list_length_--;
  }

  Command Machine::getCommand(const int32 list_index) {
    if (list_index >= cmd_list_length_ || list_index < 0) {
      ReportError("Command list index \"" + std::to_string(list_index) + "\" is out of range.");
      return Command();
    }
    return cmd_list_[list_index];
  }

  const int32 Machine::numCommands() {
    return cmd_list_length_;
  }

  void Machine::pushBackOtherMachineCommandList(Machine* other_machine) {
    for (int32 i = 0; i < other_machine->numCommands(); i++) {
      addCommand(other_machine->getCommand(i));
    }
  }



  Report Machine::registerVariable(const char *name,
    ValueType type,
    void* ptr_to_var) {

    // Error checkings
    if (!name || !ptr_to_var) {
      return kReport_NullPointer;
    }

    // If the variable already exists, we will edit the existing one.
    for (int32 i = 0; i < variable_registry_length_; i++) {
      if (name == variable_registry_[i].name_) {
        char warning[64];
        sprintf_s(warning, 64, "\"%s\": Variable already registered", name);
        ReportWarning(warning);
        variable_registry_[i] = { name, type, ptr_to_var };
        return kReport_NoErrors;
      }
    }

    // if the variable is not registered yet, we will add a new one to the registry.
    variable_registry_.push_back({ name, type, ptr_to_var });
    variable_registry_length_++;

    return kReport_NoErrors;
  }

  void Machine::unregisterVariable(const char* name) {

    for (int32 i = 0; i < variable_registry_length_; i++) {
      if (name == variable_registry_[i].name_) {
        variable_registry_.erase(variable_registry_.begin() + i);
        variable_registry_length_--;
        return;
      }
    }
    char warning[64];
    sprintf_s(warning, 64, "\"%s\": Can't be found in the registry. Unregister failed", name);
    ReportWarning(warning);
  }

  void Machine::unregisterVariable(const int32 id) {
    if (id >= 0 && id < variable_registry_length_) {
      variable_registry_.erase(variable_registry_.begin() + id);
      variable_registry_length_--;
      return;
    }

    char warning[64];
    sprintf_s(warning, 64, "Invalid variable registry id. Unregister failed");
    ReportWarning(warning);
  }

  void Machine::addGlobalVariablePack(const char* name) {
    global_variable_pack_list_.push_back({ name });
    current_global_variable_pack_ = global_variable_pack_list_length_;
    global_variable_pack_list_length_++;
  }

  Report Machine::addGlobalVariableToCurrentPack(const Variable variable) {
    int32 length = global_variable_pack_list_[current_global_variable_pack_].var.size();
    for (int32 i = 0; i < length; i++) {
      if (global_variable_pack_list_[current_global_variable_pack_].var[i].name_ == variable.name_) {
        ReportError("Multiple definition of the variable: " + variable.name_);
        return kReport_VariableDefinedTwice;
      }
    }
    global_variable_pack_list_[current_global_variable_pack_].var.push_back(variable);
    return kReport_NoErrors;
  }

  Report Machine::addGlobalVariableToCurrentPack(const char* name, const Value value) {
    int32 length = global_variable_pack_list_[current_global_variable_pack_].var.size();
    for (int32 i = 0; i < length; i++) {
      if (global_variable_pack_list_[current_global_variable_pack_].var[i].name_ == name) {
        std::string n = name;
        ReportError("Multiple definition of the variable: " + n);
        return kReport_VariableDefinedTwice;
      }
    }
    global_variable_pack_list_[current_global_variable_pack_].var.push_back({ name, value });
    return kReport_NoErrors;
  }

  void Machine::restartCurrentGlobalVariablePackIndex() {
    current_global_variable_pack_ = 0;
  }

  std::string Machine::getCurrentGlobalVariablePackName() {
    return global_variable_pack_list_[current_global_variable_pack_].name;
  }

  Variable* Machine::getVariable(const std::string& variable_name) {

    // 1st step will be to look for the variable into the current function scope.
    Function* function = getCurrentFunction();
    if (function) {
      Variable* variable = function->getVariable(variable_name);
      if (variable) {
        return variable;
      }
    }

    // If not found, we will look for it in the variable registry.
    for (int32 i = 0; i < variable_registry_length_; i++) {
      if (variable_registry_[i].name_ == variable_name) {
        return &variable_registry_[i];
      }
    }

    // Looking for the variable in the global stack.
    // 1st step will be to separate the name in case that is part of a variable pack.
    // We will look for "."  --> example:  ->>>   camera.position
    int32 length = variable_name.length();
    int32 index = -1;
    for (int32 i = 0; i < length; i++) {
      if (variable_name[i] == '.') { index = i; break; }
    }
    if (index == -1) { // No "." found
                       // Look for in the default variable pack.
      length = global_variable_pack_list_[0].var.size();
      for (int32 i = 0; i < length; i++) {
        if (global_variable_pack_list_[0].var[i].name_ == variable_name) {
          return &global_variable_pack_list_[0].var[i];
        }
      }
    }
    else { // Separate "camera.position" ->>>> to "camera"(pack) and "position"(var)
      std::string pack_name = variable_name.substr(0, index); // not including "."
      std::string var_name = variable_name.substr(index + 1); // to ignore "."
      index = 0;
      // looking for the pack
      for (int32 i = 0; i < global_variable_pack_list_length_; i++) {
        if (global_variable_pack_list_[i].name == pack_name) {
          length = global_variable_pack_list_[i].var.size();
          for (int32 j = 0; j < length; j++) {
            if (global_variable_pack_list_[i].var[j].name_ == var_name) {
              return &global_variable_pack_list_[i].var[j];
            }
          }
        }
      }
    }


    return nullptr;
  }



  Report Machine::registerFunction(const char* name,
    void(*function_ptr)(std::vector<Value>&)) {

    // Error checkings
    if (!name || !function_ptr) {
      return kReport_NullPointer;
    }

    // If the function already exists, we will edit the existing one.
    for (int32 i = 0; i < function_registry_length_; i++) {
      if (name == function_registry_[i].name_) {
        char warning[64];
        sprintf_s(warning, 64, "\"%s\": Function already registered", name);
        ReportWarning(warning);
        function_registry_[i].name_ = name;
        function_registry_[i].function_pointer_ = function_ptr;
        return kReport_NoErrors;
      }
    }

    // if the variable is not registered yet, we will add a new one to the registry.
    function_registry_.push_back({ name, function_ptr });
    function_registry_length_++;

    return kReport_NoErrors;
  }

  void Machine::unregisterFunction(const char* name) {

    for (int32 i = 0; i < function_registry_length_; i++) {
      if (name == function_registry_[i].name_) {
        function_registry_.erase(function_registry_.begin() + i);
        function_registry_length_--;
        return;
      }
    }
    char warning[64];
    sprintf_s(warning, 64, "\"%s\": Can't be found in the registry. Unregister failed", name);
    ReportWarning(warning);
  }

  void Machine::unregisterFunction(const int32 id) {
    if (id >= 0 && id < function_registry_length_) {
      function_registry_.erase(function_registry_.begin() + id);
      function_registry_length_--;
      return;
    }

    char warning[64];
    sprintf_s(warning, 64, "Invalid variable registry id. Unregister failed");
    ReportWarning(warning);
  }

  RegisteredFunction* Machine::getRegisteredFunction(const std::string& function_name) {

    // If not found, we will look for it in the function registry.
    for (int32 i = 0; i < function_registry_length_; i++) {
      if (function_registry_[i].name_ == function_name) {
        return &function_registry_[i];
      }
    }

    return nullptr;
  }

  Report Machine::addDefinedFunction(const char* name, const int32 command_index) {
    // Error checkings
    if (!name) {
      return kReport_NullPointer;
    }

    // If the variable already exists, we will edit the existing one.
    for (int32 i = 0; i < defined_function_list_length_; i++) {
      if (name == defined_function_list_[i].name) {
        char error[64];
        sprintf_s(error, 64, "\"%s\": Function already defined", name);
        ReportError(error);
        return kReport_FunctionDefinedMoreThanOnce;
      }
    }

    // if the variable is not registered yet, we will add a new one to the registry.
    defined_function_list_.push_back({ name, command_index });
    defined_function_list_length_++;

    return kReport_NoErrors;
  }

  int32 Machine::getDefinedFunctionID(const char* name) {
    std::string function_name = name;
    for (int32 i = 0; i < defined_function_list_length_; i++) {
      if (defined_function_list_[i].name == function_name) {
        return defined_function_list_[i].command_index;
      }
    }

    ReportError("Function not defined, so theres no ID for function name: " + function_name);
    return INVALID_FUNCTION_ID;
  }

  void Machine::removeDefinedFunction(const char* name) {
    for (int32 i = 0; i < defined_function_list_length_; i++) {
      if (name == defined_function_list_[i].name) {
        defined_function_list_.erase(defined_function_list_.begin() + i);
        defined_function_list_length_--;
        return;
      }
    }
    char warning[64];
    sprintf_s(warning, 64, "\"%s\": Is not a defined function. Remove Defined Function failed", name);
    ReportWarning(warning);
  }

  void Machine::removeDefinedFunction(const int32 id) {
    if (id >= 0 && id < defined_function_list_length_) {
      defined_function_list_.erase(defined_function_list_.begin() + id);
      defined_function_list_length_--;
      return;
    }

    char warning[64];
    sprintf_s(warning, 64, "Invalid function id. Remove Defined Function failed");
    ReportWarning(warning);
  }


  void Machine::addFunction(const int32 origin_id) {
    function_list_.push_back({ origin_id });
    function_list_length_++;
  }

  Function* Machine::getCurrentFunction() {
    if (function_list_length_ == 0) {
      return nullptr;
    }
    return &function_list_[function_list_length_ - 1];
  }

  void Machine::removeFunction(const int32 id) {
    if (id >= 0 && id < function_list_length_) {
      function_list_.erase(function_list_.begin() + id);
      function_list_length_--;
    }
    else {
      ReportWarning(" Can't remove function from the execution function list.");
    }
  }

  void Machine::removeCurrentFunction() {
    removeFunction(function_list_length_ - 1);
  }

  const int32 Machine::numActiveFunctions() {
    return function_list_length_;
  }

  void Machine::addValueToTheStack(const Value value) {
    stack_.value.push_back(value);
    stack_.length++;
  }

  Value Machine::getAndRemoveTheLastAddedStackValue() {
    Value value;
    if (stack_.length <= 0) {
      ReportWarning(" Trying to extract a value from an empty stack.");
      return value;
    }
    value = stack_.value[stack_.length - 1];
    stack_.value.erase(stack_.value.begin() + stack_.length - 1);
    stack_.length--;
    return value;
  }

  int32 Machine::numStackValues() {
    return stack_.length;
  }

  int32 Machine::getInteger(const char* variable_name, const char* variable_pack_name = "") {
    VariablePack* varpack = nullptr;
    Variable* var = nullptr;
    int32 length = 0;
    int32 i = 0;
    for (i = 0; i < global_variable_pack_list_length_; i++) {
      if (global_variable_pack_list_[i].name == variable_pack_name) {
        varpack = &global_variable_pack_list_[i];
        break;
      }
    }

    if (!varpack) {
      ReportError("Unable to find variable pack name: " + std::string(variable_pack_name));
      return -99999;
    }

    length = varpack->var.size();
    for (i = 0; i < length; i++) {
      if (varpack->var[i].name_ == variable_name) {
        var = &varpack->var[i];
        break;
      }
    }

    if (!var) {
      ReportError("Unable to find variable name: " + std::string(variable_name));
      return -99999;
    }

    if (var->value_.type_ != kValueType_Integer) {
      ReportError("Varible name: " + std::string(variable_name) + " is not an integer");
      return -99999;
    }

    return var->getValue().integer_;
  }

  float32 Machine::getFloat(const char* variable_name, const char* variable_pack_name = "") {
    VariablePack* varpack = nullptr;
    Variable* var = nullptr;
    int32 length = 0;
    int32 i = 0;
    for (i = 0; i < global_variable_pack_list_length_; i++) {
      if (global_variable_pack_list_[i].name == variable_pack_name) {
        varpack = &global_variable_pack_list_[i];
        break;
      }
    }

    if (!varpack) {
      ReportError("Unable to find variable pack name: " + std::string(variable_pack_name));
      return -99999.0f;
    }

    length = varpack->var.size();
    for (i = 0; i < length; i++) {
      if (varpack->var[i].name_ == variable_name) {
        var = &varpack->var[i];
        break;
      }
    }

    if (!var) {
      ReportError("Unable to find variable name: " + std::string(variable_name));
      return -99999.0f;
    }

    if (var->value_.type_ != kValueType_Float) {
      ReportError("Varible name: " + std::string(variable_name) + " is not a float");
      return -99999.0f;
    }

    return var->getValue().float_;
  }


  std::string Machine::getString(const char* variable_name, const char* variable_pack_name = "") {
    VariablePack* varpack = nullptr;
    Variable* var = nullptr;
    int32 length = 0;
    int32 i = 0;
    for (i = 0; i < global_variable_pack_list_length_; i++) {
      if (global_variable_pack_list_[i].name == variable_pack_name) {
        varpack = &global_variable_pack_list_[i];
        break;
      }
    }

    if (!varpack) {
      ReportError("Unable to find variable pack name: " + std::string(variable_pack_name));
      return "ERROR";
    }

    length = varpack->var.size();
    for (i = 0; i < length; i++) {
      if (varpack->var[i].name_ == variable_name) {
        var = &varpack->var[i];
        break;
      }
    }

    if (!var) {
      ReportError("Unable to find variable name: " + std::string(variable_name));
      return "ERROR";
    }

    if (var->value_.type_ != kValueType_Text) {
      ReportError("Varible name: " + std::string(variable_name) + " is not a string");
      return "ERROR";
    }

    return var->getValue().text_;
  }

private:

/*******************************   METHODS   **********************************/

  /// Default copy constructor.
  Machine(const Machine& copy);
  /// Assignment operator.
  Machine& operator=(const Machine& copy);

}; /* Machine */

#pragma endregion

/*******************************************************************************
***                          COMMAND METHODS                                 ***
*******************************************************************************/

#pragma region COMMAND_CLASS_METHODS 

Report Command::execute(Machine* machine, int32& id) {

  switch (type_) {
  case kCommandType_PushToTheStack: { return executePushToTheStack(machine, id); }
  case kCommandType_Addition: { return executeAddition(machine, id); }
  case kCommandType_Substraction: { return executeSubstraction(machine, id); }
  case kCommandType_Multiply: { return executeMultiply(machine, id); }
  case kCommandType_Division: { return executeDivision(machine, id); }
  case kCommandType_Power: { return executePower(machine, id); }
  case kCommandType_EqualAssigment: { return executeEqualAssignment(machine, id); }
  case kCommandType_GreaterThanComparison: { return executeGreaterThanComparison(machine, id); }
  case kCommandType_LowerThanComparison: { return executeLowerThanComparison(machine, id); }
  case kCommandType_GreaterOrEqualThanComparison: { return executeGreaterOrEqualThanComparison(machine, id); }
  case kCommandType_LowerOrEqualThanComparison: { return executeLowerOrEqualThanComparison(machine, id); }
  case kCommandType_EqualThanComparison: { return executeEqualThanComparison(machine, id); }
  case kCommandType_NotEqualThanComparison: { return executeNotEqualThanComparison(machine, id); }
  case kCommandType_FunctionDefinition: { return executeFunctionDefinition(machine, id); }
  case kCommandType_FunctionCall: { return executeFunctionCall(machine, id); }
  case kCommandType_FunctionReturn: { return executeFunctionReturn(machine, id); }
  case kCommandType_FunctionEnd: { return executeFunctionEnd(machine, id); }
  case kCommandType_FunctionNumParameters: { return executeFunctionNumParams(machine, id); }
  case kCommandType_FunctionParameter: { return executeFunctionParam(machine, id); }
  case kCommandType_FinishedConditionalOrLoop: { return executeFinishedConditionalOrLoop(machine, id); }
  case kCommandType_ConditionToEvaluate: { return executeConditionToEvaluate(machine, id); }
  case kCommandType_VariableDefinition: { return executeVariableDefinition(machine, id); }
  case kCommandType_VariablePackDefinition: { return executeVariablePackDefinition(machine, id); }
  case kCommandType_VariablePackEnd: { return executeVariablePackEnd(machine, id); }
  case kCommandType_LoopStartPoint: { return executeLoopStartPoint(machine, id); }
  }

  return kReport_InvalidCommandType;
}

Report Command::executeAddition(Machine* machine, int32& next_cmd_id) {
  // Take the last members pushed in the stack
  Value second = machine->getAndRemoveTheLastAddedStackValue();
  Value first = machine->getAndRemoveTheLastAddedStackValue();
  machine->addValueToTheStack(first + second);
  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}

Report Command::executeSubstraction(Machine* machine, int32& next_cmd_id) {
  // Take the last members pushed in the stack
  Value second = machine->getAndRemoveTheLastAddedStackValue();
  Value first = machine->getAndRemoveTheLastAddedStackValue();
  machine->addValueToTheStack(first - second);
  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}

Report Command::executeMultiply(Machine* machine, int32& next_cmd_id) {
  // Take the last members pushed in the stack
  Value second = machine->getAndRemoveTheLastAddedStackValue();
  Value first = machine->getAndRemoveTheLastAddedStackValue();
  machine->addValueToTheStack(first * second);
  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}

Report Command::executeDivision(Machine* machine, int32& next_cmd_id) {
  // Take the last members pushed in the stack
  Value second = machine->getAndRemoveTheLastAddedStackValue();
  Value first = machine->getAndRemoveTheLastAddedStackValue();
  machine->addValueToTheStack(first / second);
  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}

Report Command::executePower(Machine* machine, int32& next_cmd_id) {
  // Take the last members pushed in the stack
  Value second = machine->getAndRemoveTheLastAddedStackValue();
  Value first = machine->getAndRemoveTheLastAddedStackValue();
  machine->addValueToTheStack(first ^ second);
  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}

Report Command::executeEqualAssignment(Machine* machine, int32& next_cmd_id) {
  // Check if the variable is being assigned inside a pack definition.
  std::string pack_name = machine->getCurrentGlobalVariablePackName();
  std::string var_name = name_;
  // If its inside a VARPACK, then we will attach the pack name.
  // Example, name_ = "x". If its inside "cam" varpack, the result will be "cam.x"
  if (pack_name != "") {
    var_name = pack_name + "." + name_;
  }
  Variable* variable = machine->getVariable(var_name);
  if (!variable) {
    ReportError(" Unable to find variable name: " + var_name);
    return kReport_ExpectingNameOfVariable;
  }
  // Assign the last value of the stack to the variable.
  variable->setValue(machine->getAndRemoveTheLastAddedStackValue());
  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}

Report Command::executeGreaterThanComparison(Machine* machine, int32& next_cmd_id) {
  // Take the last members pushed in the stack
  Value second = machine->getAndRemoveTheLastAddedStackValue();
  Value first = machine->getAndRemoveTheLastAddedStackValue();
  machine->addValueToTheStack(first > second);
  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}

Report Command::executeLowerThanComparison(Machine* machine, int32& next_cmd_id) {
  // Take the last members pushed in the stack
  Value second = machine->getAndRemoveTheLastAddedStackValue();
  Value first = machine->getAndRemoveTheLastAddedStackValue();
  machine->addValueToTheStack(first < second);
  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}

Report Command::executeGreaterOrEqualThanComparison(Machine* machine, int32& next_cmd_id) {
  // Take the last members pushed in the stack
  Value second = machine->getAndRemoveTheLastAddedStackValue();
  Value first = machine->getAndRemoveTheLastAddedStackValue();
  machine->addValueToTheStack(first >= second);
  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}

Report Command::executeLowerOrEqualThanComparison(Machine* machine, int32& next_cmd_id) {
  // Take the last members pushed in the stack
  Value second = machine->getAndRemoveTheLastAddedStackValue();
  Value first = machine->getAndRemoveTheLastAddedStackValue();
  machine->addValueToTheStack(first <= second);
  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}

Report Command::executeEqualThanComparison(Machine* machine, int32& next_cmd_id) {
  // Take the last members pushed in the stack
  Value second = machine->getAndRemoveTheLastAddedStackValue();
  Value first = machine->getAndRemoveTheLastAddedStackValue();
  machine->addValueToTheStack(first == second);
  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}

Report Command::executeNotEqualThanComparison(Machine* machine, int32& next_cmd_id) {
  // Take the last members pushed in the stack
  Value second = machine->getAndRemoveTheLastAddedStackValue();
  Value first = machine->getAndRemoveTheLastAddedStackValue();
  machine->addValueToTheStack(first != second);
  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}

Report Command::executePushToTheStack(Machine* machine, int32& next_cmd_id) {

  // We will check if its a quote.
  switch (getNameDataType()) {
  case kValueType_None: {
    // this means that its a name of possible variable, so we will check it.
    Variable* variable = machine->getVariable(name_);
    if (!variable) {
      ReportError(" Expecting name of variable, cannot find variable name: " + name_);
      return kReport_ExpectingNameOfVariable;
    }
    machine->addValueToTheStack(variable->getValue());
  } break;
  case kValueType_Float: {
    machine->addValueToTheStack((float32)atof(name_.c_str()));
  } break;
  case kValueType_Integer: {
    machine->addValueToTheStack((int32)atoi(name_.c_str()));
  } break;
  case kValueType_Text: {
    std::string temp = name_;
    // remove quotes.
    temp.erase(0, 1);
    temp.erase(temp.size() - 1, 1);
    machine->addValueToTheStack({ temp.c_str() });
  } break;
  }

  next_cmd_id++; // Jump to the next step.
  return kReport_NoErrors;
}


Report Command::executeFunctionDefinition(Machine* machine, int32& next_cmd_id) {
  next_cmd_id++; // Jump to the next command
  machine->addFunction(next_cmd_id);
  return kReport_NoErrors;
}

Report Command::executeFunctionCall(Machine* machine, int32& next_cmd_id) {

  // First step will be checking if the function is called PRINT.
  if (name_ == "PRINT") {
    machine->getAndRemoveTheLastAddedStackValue().print();
    next_cmd_id++; // Jump to the next command
    return kReport_NoErrors;
  }

  // First step will be checking if the function is called SIN.
  if (name_ == "SIN") {
    machine->addValueToTheStack(machine->getAndRemoveTheLastAddedStackValue().Sin());
    next_cmd_id++; // Jump to the next command
    return kReport_NoErrors;
  }

  // First step will be checking if the function is called COS.
  if (name_ == "COS") {
    machine->addValueToTheStack(machine->getAndRemoveTheLastAddedStackValue().Cos());
    next_cmd_id++; // Jump to the next command
    return kReport_NoErrors;
  }

  // First step will be checking if the function is called TAN.
  if (name_ == "TAN") {
    machine->addValueToTheStack(machine->getAndRemoveTheLastAddedStackValue().Tan());
    next_cmd_id++; // Jump to the next command
    return kReport_NoErrors;
  }

  // First step will be checking if the function is called ASIN.
  if (name_ == "ASIN") {
    machine->addValueToTheStack(machine->getAndRemoveTheLastAddedStackValue().ASin());
    next_cmd_id++; // Jump to the next command
    return kReport_NoErrors;
  }

  // First step will be checking if the function is called ACOS.
  if (name_ == "ACOS") {
    machine->addValueToTheStack(machine->getAndRemoveTheLastAddedStackValue().ACos());
    next_cmd_id++; // Jump to the next command
    return kReport_NoErrors;
  }

  // First step will be checking if the function is called ATAN.
  if (name_ == "ATAN") {
    machine->addValueToTheStack(machine->getAndRemoveTheLastAddedStackValue().ATan());
    next_cmd_id++; // Jump to the next command
    return kReport_NoErrors;
  }

  RegisteredFunction* function = machine->getRegisteredFunction(name_);
  if (function) {
    // Get the num of params. (do this because of the order of them.
    int32 num_params = 0;
    for (int32 i = next_cmd_id - 1; machine->getCommand(i).type_ == kCommandType_PushToTheStack; i--) {
      num_params++;
    }
    // Save the values in the registered function params list.
    function->params_.resize(num_params);
    for (int32 j = num_params - 1; j >= 0; j--) {
      function->params_[j] = machine->getAndRemoveTheLastAddedStackValue();
    }
    // Then executes the function.
    function->function_pointer_(function->params_);
    next_cmd_id++;
    return kReport_NoErrors;
  }

  // Look for the function in the defined function list to know where its body starts.
  int32 function_start_command_id = machine->getDefinedFunctionID(name_.c_str());
  if (function_start_command_id != INVALID_FUNCTION_ID) {
    // Then we will push a new function and we will assign the next command to 
    // be the returning point.
    machine->addFunction(next_cmd_id + 1); // To continue the execution after finishing it.
    next_cmd_id = function_start_command_id + 1; // To avoid the step of function definition.
    return kReport_NoErrors;
  }

  return kReport_CallingUndefinedFunction;
}

Report Command::executeFunctionReturn(Machine* machine, int32& next_cmd_id) {

  Report report = kReport_NoErrors;
  // We will check if theres any fu
  Function* function = machine->getCurrentFunction();
  if (function == nullptr) {
    report = kReport_ReturnCalledWithoutAnyActiveFunction;
    PrintReport(report);
    return report;
  }

  // if theres an active function we will come back to the origin command where it was called.
  next_cmd_id = function->originID();
  // if its the last active function, then we finish the execution.
  if (machine->numActiveFunctions() == 1) {
    report = kReport_LastActiveFunctionReturnCalled; // This will end the execution without errors.
  }
  machine->removeCurrentFunction();
  return report;
}

Report Command::executeFunctionEnd(Machine* machine, int32& next_cmd_id) {

  Report report = kReport_NoErrors;
  // We will check if theres any fu
  Function* function = machine->getCurrentFunction();
  if (function == nullptr) {
    report = kReport_ReturnCalledWithoutAnyActiveFunction;
    PrintReport(report);
    return report;
  }

  // if theres an active function we will come back to the origin command where it was called.
  next_cmd_id = function->originID();
  // if its the last active function, then we finish the execution.
  if (machine->numActiveFunctions() == 1) {
    report = kReport_LastActiveFunctionReturnCalled; // This will end the execution without errors.
  }
  machine->removeCurrentFunction();
  return report;
}

Report Command::executeFunctionNumParams(Machine* machine, int32& next_cmd_id) {

  if (machine->numStackValues() < atoi(name_.c_str())) {
    ReportError("Function couldn't take enough params from the stack");
    return kReport_NotEnoughFunctionParamsInTheStack;
  }
  next_cmd_id++; // Jump to the next command
  return kReport_NoErrors;
}

Report Command::executeFunctionParam(Machine* machine, int32& next_cmd_id) {

  Function* function = machine->getCurrentFunction();
  if (function) {
    // Adds a variable to the current function scope.
    function->addVariable(name_.c_str(), machine->getAndRemoveTheLastAddedStackValue());
    next_cmd_id++; // Jump to the next command
    return kReport_NoErrors;
  }

  ReportError(" Params need a function to be executed. Theres no active function");
  return kReport_ParamsNeedAFunctionToBeExecuted;
}



Report Command::executeFinishedConditionalOrLoop(Machine* machine, int32& next_cmd_id) {

  if (name_ == "conditional") {
    next_cmd_id++; // jump to the next command on the list
    return kReport_NoErrors;
  }
  if (name_ == "loop") {
    // if its the end of a loop, we will step back again to check the loop condition.
    for (int32 i = next_cmd_id; i >= 0; i--) {
      if (machine->getCommand(i).type_ == kCommandType_LoopStartPoint) {
        next_cmd_id = i; // Next step will be the init of the loop.
        return kReport_NoErrors;
      }
    }
  }

  // if theres any error, we will jump to the next instruction.
  next_cmd_id++;
  ReportWarning("Unable to execute finished conditional or loop command");
  return kReport_NoErrors;
}


Report Command::executeConditionToEvaluate(Machine* machine, int32& next_cmd_id) {

  if (!machine->getCurrentFunction()) {
    Report report = kReport_ConditionOutsideOfAFunction;
    PrintReport(report);
    return report;
  }

  // The condition is true, so we will enter in the loop or conditional.
  if (machine->getAndRemoveTheLastAddedStackValue().integer_ == CONDITION_RESULT_TRUE) {
    next_cmd_id++;
    return kReport_NoErrors;
  }

  // If not we will jump to the end of this conditional or loop.
  // To avoid the problem with nested loops and conditionals we will do the next:
  /*
  <<<< exit counter (EC) starts with value 0 >>>>

  LOOP (condition) { EC++        >>>> 1
  if (condition2) { EC++       >>>> 2
  loop(condition3) { EC++    >>>> 3
  } EC--                     >>>> 2
  } EC--                       >>>> 1
  } EC--                         >>>> 0 -> end of LOOP body or condition.
  NEXT COMMMAND
  Then we assign the next_cmd id to this next command.
  */

  int32 exit_counter = 0;
  Command cmd;
  for (int32 i = next_cmd_id; i < machine->numCommands(); i++) {
    cmd = machine->getCommand(i);
    if (cmd.type_ == kCommandType_ConditionToEvaluate) {
      exit_counter++;
    }
    else if (cmd.type_ == kCommandType_FinishedConditionalOrLoop) {
      exit_counter--;
    }

    // Then we check if we are at the end of the main conditionl or loop body.
    if (exit_counter == 0) {
      next_cmd_id = i + 1; // Next command after "}"
      return kReport_NoErrors;
    }
  }


  ReportError("Cant find the end of the conditional or loop");
  return kReport_EndOfConditionalOrLoopNotFound;
}


Report Command::executeVariableDefinition(Machine* machine, int32& next_cmd_id) {

  Function* function = machine->getCurrentFunction();
  // Allocate the variable in the current function scope list.
  if (function) {
    function->addVariable({ name_.c_str() });
    next_cmd_id++; // jump to the next command on the list
    return kReport_NoErrors;
  }
  // TODO: Error checking to avoid repeated names.
  // Adding a variable to the global variable stack.
  next_cmd_id++; // jump to the next command on the list
  return machine->addGlobalVariableToCurrentPack({ name_.c_str() });
}

Report Command::executeVariablePackDefinition(Machine* machine, int32& next_cmd_id) {
  if (machine->getCurrentFunction()) {
    ReportError("Variable packs cannot be defined inside functions. Pack name: " + name_);
    return kReport_VariablePackCantBeDefinedInsideAFunction;
  }

  machine->addGlobalVariablePack(name_.c_str());
  next_cmd_id++; // Just go to the next step that would be the condition check.
  return kReport_NoErrors;
}

Report Command::executeVariablePackEnd(Machine* machine, int32& next_cmd_id) {
  machine->restartCurrentGlobalVariablePackIndex();
  next_cmd_id++; // Just go to the next step that would be the condition check.
  return kReport_NoErrors;
}

Report Command::executeLoopStartPoint(Machine* machine, int32& next_cmd_id) {
  if (!machine->getCurrentFunction()) {
    Report report = kReport_LoopOutsideOfAFunction;
    PrintReport(report);
    return report;
  }
  next_cmd_id++; // Just go to the next step that would be the condition check.
  return kReport_NoErrors;
}

#pragma endregion

/*******************************************************************************
***                          COMPILER METHODS                                ***
*******************************************************************************/

#pragma region COMPILER_CLASS_METHODS

Report Compiler::compile(Machine* machine,
  std::string sentence,
  uint32 line_number) {

  // Error checkings.
  if (!machine) {
    ReportError("Machine Null Pointer, can't compile.");
    return kReport_NullPointer;
  }

  // Allocates all the tokens in the manager.
  TokenManager token_manager;
  generateTokens(sentence, token_manager);
  //token_manager.printTokenList();

  // Compile all these tokens.
  Report report = compileTokens(machine, token_manager);

  PrintReport(report, line_number);
  return report;
}


/*******************************************************************************
***                        TOKEN COMPILER METHODS                            ***
*******************************************************************************/

Report Compiler::compileTokens(Machine* machine, TokenManager& token_manager) {

  // Error checkings.
  if (!machine) {
    ReportError("Machine Null Pointer, can't compile.");
    return kReport_NullPointer;
  }

  if (token_manager.numTokens() == 0) { return kReport_EmptyLine; }

  // Once we checked the basic errors, lets start with the highest priority token.
  int32 id = token_manager.getHighestPriorityTokenIndex();
  Token token = token_manager.getToken(id);

  if (token.type_ == kTokenType_Keyword) {
    return compileKeywordToken(machine, token_manager, id);
  }

  // If theres any } or ( we will compile this separator first.
  if (token.priority_ == CLOSE_BRACKETS_PRIORITY) { // "}"
    return compileCloseBracketsSeparatorToken(machine, token_manager, id);
  }
  if (token.text_ == "(") { // We dont use priority becasue it changes to solve parenthethical grouping problems. 
    compileOpenParenthesisSeparatorToken(machine, token_manager, id);
  }

  // If theres any comma we will compile the content at both sides of it and will
  // step to the next line of code.
  if (checkIfAndCompileCommasContent(machine, token_manager)) {
    return kReport_NoErrors;
  }

  // Once checked that there arent commas, then we will compile the other separators.
  if (token.type_ == kTokenType_Separator && token.text_ != "(") {
    compileSeparatorToken(machine, token_manager, id);
  }

  if (token.priority_ == DEFAULT_PRIORITY) {
    // Pushing variables, numbers and other stuff non compilable.
    machine->addCommand(kCommandType_PushToTheStack, token.text_);
    token_manager.transferContentBetweenIDsInclusive(id, id);
  }

  // Compiling recursively what is not compiled yet.
  if (token_manager.numTokens() > 1) {
    return compileTokens(machine, token_manager);
  }

  return kReport_NoErrors;
}

Report Compiler::compileKeywordToken(Machine* machine,
  TokenManager& token_manager,
  int32& token_index) {
  Token token = token_manager.getToken(token_index);

  if (token.text_ == "IF" || token.text_ == "ELSE") {
    return compileConditionalKeywordToken(machine, token_manager, token_index);
  }
  if (token.text_ == "RETURN") {
    return compileReturnKeywordToken(machine, token_manager, token_index);
  }
  if (token.text_ == "FUNC") {
    return compileFunctionKeywordToken(machine, token_manager, token_index);
  }
  if (token.text_ == "DO" || token.text_ == "WHILE" || token.text_ == "FOR") {
    return compileLoopKeywordToken(machine, token_manager, token_index);
  }
  if (token.text_ == "VAR") {
    return compileVariableKeywordToken(machine, token_manager, token_index);
  }
  if (token.text_ == "VARPACK") {
    return compileVariablePackKeywordToken(machine, token_manager, token_index);
  }

  return kReport_UnexpectedKeyword;
}



Report Compiler::compileSeparatorToken(Machine* machine,
  TokenManager& token_manager,
  int32& token_index) {

  Token token = token_manager.getToken(token_index);

  switch (token.priority_) {
  case ADDITION_OPERATION_PRIORITY: {
    return compileAdditionOperationSeparatorToken(machine, token_manager, token_index);
  } break;
  case MULTIPLY_OPERATION_PRIORITY: {
    return compileMultiplyOperationSeparatorToken(machine, token_manager, token_index);
  } break;
  case POWER_OPERATION_PRIORITY: {
    return compilePowerOperationSeparatorToken(machine, token_manager, token_index);
  } break;
  case COMPARISON_PRIORITY: {
    return compileComparisonOperationSeparatorToken(machine, token_manager, token_index);
  } break;
  case EQUAL_PRIORITY: {
    return compileEqualSeparatorToken(machine, token_manager, token_index);
  } break;
  }

  return kReport_NoErrors;
}

const bool Compiler::checkIfAndCompileCommasContent(Machine * machine,
  TokenManager & token_manager) {

  if (token_manager.areAnyCommaTokenInList()) {
    int32 num_tokens = token_manager.numTokens();
    // Compile tokens with comas recursively.
    for (int32 i = 0; i < num_tokens; i++) {
      if (token_manager.getToken(i).text_ == ",") {
        TokenManager temp;
        // In case that we find any comma, we will take the previous content to compile it.
        // Example:   a b c d, e f     -> then we will transfer a b c d
        token_manager.transferContentBetweenIDsInclusive(0, i - 1, &temp);
        // Then we will delete the , and the RESULT token.
        token_manager.removeToken(1);
        token_manager.removeToken(0);
        // Once compiled, the previous content, we will compile the following one 
        compileTokens(machine, temp);
        compileTokens(machine, token_manager);
        // We will finish then the loop and we will end the compile of this line.
        return true;
      }
    }
    return true;
  }
  return false;
}

/*******************************************************************************
***                   SEPARATORS TOKEN COMPILER METHODS                      ***
*******************************************************************************/

Report Compiler::compileOpenParenthesisSeparatorToken(Machine* machine,
  TokenManager& token_manager,
  int32& token_index) {

  Report report = kReport_NoErrors;

  // First step will be to find its correspondant closing parenthesis.
  int32 close_parenthesis_id = token_manager.getNextCloseParenthesisIndex(token_index);
  if (close_parenthesis_id == -1) {
    return kReport_NoMatchingCloseParenthesis;
  }

  // We will extract the content inside the parenthesis in a new token manager, 
  // And paste it in a new token manager.
  TokenManager parenthesis_content;
  token_manager.transferParenthesisContent(token_index, close_parenthesis_id, &parenthesis_content);

  // Compile the content of the parenthesis.
  report = compileTokens(machine, parenthesis_content);

  // In case that its a function, then call it.
  token_index--;
  if (token_index >= 0) { // means that theres something behind the parenthesis.
    Token token = token_manager.getToken(token_index);
    if (token.type_ != kTokenType_Keyword &&
      token.type_ != kTokenType_Separator &&
      token.priority_ != 0) {
      machine->addCommand(kCommandType_FunctionCall, token.text_);
      token_manager.removeToken(token_index);
    }
  }


  return report;
}

Report Compiler::compileCloseBracketsSeparatorToken(Machine* machine,
  TokenManager& token_manager,
  int32& token_index) {

  // Will untag the end of a loop, conditional or function.
  Tag tag = getAndRemoveLastTag();
  switch (tag) {
  case kTag_None: { } break;
  case kTag_Loop: { machine->addCommand(kCommandType_FinishedConditionalOrLoop, "loop"); } break;
  case kTag_Conditional: { machine->addCommand(kCommandType_FinishedConditionalOrLoop, "conditional"); } break;
  case kTag_Function: { machine->addCommand(kCommandType_FunctionEnd, "endfunc"); } break;
  case kTag_VariablePack: { machine->addCommand(kCommandType_VariablePackEnd, "endvarpack"); } break;

  }
  return kReport_NoErrors;
}

Report Compiler::compileAdditionOperationSeparatorToken(Machine* machine,
  TokenManager& token_manager,
  int32& token_index) {
  Token operator_token = token_manager.getToken(token_index);
  Token right_operand = token_manager.getToken(token_index + 1);
  Token left_operand = token_manager.getToken(token_index - 1);

  // Push the operands to the stack
  machine->addCommand(kCommandType_PushToTheStack, left_operand.text_);
  machine->addCommand(kCommandType_PushToTheStack, right_operand.text_);

  // Then the next command will set the action to be applied to the previous operands.
  if (operator_token.text_ == "-") {
    machine->addCommand(kCommandType_Substraction);
  }
  else {
    machine->addCommand(kCommandType_Addition);
  }

  // Deleting the tokens from the list and adding a "RESULT" Temporary one.
  token_manager.transferContentBetweenIDsInclusive(token_index - 1, token_index + 1);
  // After deleting from the list the three elements, we step back two positions.
  token_index -= 2;

  return kReport_NoErrors;
}

Report Compiler::compileMultiplyOperationSeparatorToken(Machine* machine,
  TokenManager& token_manager,
  int32& token_index) {

  Token operator_token = token_manager.getToken(token_index);
  Token right_operand = token_manager.getToken(token_index + 1);
  Token left_operand = token_manager.getToken(token_index - 1);

  // Push the operands to the stack
  machine->addCommand(kCommandType_PushToTheStack, left_operand.text_);
  machine->addCommand(kCommandType_PushToTheStack, right_operand.text_);

  // Then the next command will set the action to be applied to the previous operands.
  if (operator_token.text_ == "/") {
    machine->addCommand(kCommandType_Division);
  }
  else {
    machine->addCommand(kCommandType_Multiply);
  }

  // Deleting the tokens from the list and adding a "RESULT" Temporary one.
  token_manager.transferContentBetweenIDsInclusive(token_index - 1, token_index + 1);
  // After deleting from the list the three elements, we step back two positions.
  token_index -= 2;

  return kReport_NoErrors;
}

Report Compiler::compilePowerOperationSeparatorToken(Machine* machine,
  TokenManager& token_manager,
  int32& token_index) {

  Token operator_token = token_manager.getToken(token_index);
  Token right_operand = token_manager.getToken(token_index + 1);
  Token left_operand = token_manager.getToken(token_index - 1);

  // Push the operands to the stack
  machine->addCommand(kCommandType_PushToTheStack, left_operand.text_);
  machine->addCommand(kCommandType_PushToTheStack, right_operand.text_);

  // Then the next command will set the action to be applied to the previous operands
  machine->addCommand(kCommandType_Power);

  // Deleting the tokens from the list and adding a "RESULT" Temporary one.
  token_manager.transferContentBetweenIDsInclusive(token_index - 1, token_index + 1);
  // After deleting from the list the three elements, we step back two positions.
  token_index -= 2;

  return kReport_NoErrors;
}

Report Compiler::compileComparisonOperationSeparatorToken(Machine* machine,
  TokenManager& token_manager,
  int32& token_index) {

  Token operator_token = token_manager.getToken(token_index);
  Token right_operand = token_manager.getToken(token_index + 1);
  Token left_operand = token_manager.getToken(token_index - 1);

  // Push the operands to the stack
  machine->addCommand(kCommandType_PushToTheStack, left_operand.text_);
  machine->addCommand(kCommandType_PushToTheStack, right_operand.text_);

  // Then the next command will set the action to be applied to the previous operands.
  if (operator_token.text_ == "<") {
    machine->addCommand(kCommandType_LowerThanComparison);
  }
  else if (operator_token.text_ == ">") {
    machine->addCommand(kCommandType_GreaterThanComparison);
  }
  else if (operator_token.text_ == "<=") {
    machine->addCommand(kCommandType_LowerOrEqualThanComparison);
  }
  else if (operator_token.text_ == ">=") {
    machine->addCommand(kCommandType_GreaterOrEqualThanComparison);
  }
  else if (operator_token.text_ == "==") {
    machine->addCommand(kCommandType_EqualThanComparison);
  }
  else if (operator_token.text_ == "!=") {
    machine->addCommand(kCommandType_NotEqualThanComparison);
  }
  else {
    ReportError("Unexpected comparison token: " + operator_token.text_);
    return kReport_UnexpectedComparisonToken;
  }

  // Deleting the tokens from the list and adding a "RESULT" Temporary one.
  token_manager.transferContentBetweenIDsInclusive(token_index - 1, token_index + 1);
  // After deleting from the list the three elements, we step back two positions.
  token_index -= 2;

  return kReport_NoErrors;
}

Report Compiler::compileEqualSeparatorToken(Machine* machine,
  TokenManager& token_manager,
  int32& token_index) {

  int32 num_tokens = token_manager.numTokens();
  if (num_tokens < 3 || token_index <= 0) {
    return kReport_EqualNeedTokensBeforeAndAfter;
  }
  // Transfer the content after the = to a temp token manager.
  TokenManager temp;
  token_manager.transferContentBetweenIDsInclusive(token_index + 1, num_tokens - 1, &temp);
  // Compile the content of this manager recursively.
  Report report = compileTokens(machine, temp);
  // Gets the token before the equal symbol.
  token_index--;
  Token token = token_manager.getToken(token_index);
  machine->addCommand(kCommandType_EqualAssigment, token.text_);
  // Then delete the content of from the one behind the "=", to the end of the sentence.
  token_manager.transferContentBetweenIDsInclusive(token_index, num_tokens - 1);
  token_index--;

  return kReport_NoErrors;
}

/*******************************************************************************
***                    KEYWORDS TOKEN COMPILER METHODS                       ***
*******************************************************************************/

Report Compiler::compileConditionalKeywordToken(Machine* machine,
  TokenManager& token_manager,
  int32& token_index) {

  if (token_manager.getToken(token_index).text_ == "IF") {
    // removing the "vayword.
    token_manager.removeToken(token_index);
    // Checks if the last token of the line sentence is a "{" to start the body.
    if (token_manager.getToken(token_manager.numTokens() - 1).text_ != "{") {
      return kReport_ExpectingOpenBrackets;
    }
    // Removing "{"
    token_manager.removeToken(token_manager.numTokens() - 1);
    addTag(kTag_Conditional);
    // Compiling the condition.
    Report report = compileTokens(machine, token_manager);
    machine->addCommand(kCommandType_ConditionToEvaluate);
    return report;
  }

  ReportError("\"else\" keyword functionality not programmed yet in this version");
  return kReport_KeywordFunctionalityNotProgrammedYet;
}

Report Compiler::compileReturnKeywordToken(Machine* machine,
  TokenManager& token_manager,
  int32& token_index) {

  Report report = kReport_NoErrors;

  if (token_index != 0) {
    return kReport_ReturnShouldBeTheFirstToken;
  }

  if (token_manager.numTokens() > 1) {
    TokenManager temp; // use to save the content after return.
    token_manager.transferContentBetweenIDsInclusive(token_index + 1, token_manager.numTokens() - 1, &temp);
    // Compiling the right of return
    report = compileTokens(machine, temp);
    // After it the token_manager should be like: return "RESULT"
  }
  machine->addCommand(kCommandType_FunctionReturn);
  return report;
}

Report Compiler::compileFunctionKeywordToken(Machine* machine,
  TokenManager& token_manager,
  int32& token_index) {
  // ERROR CHECKINGS
  // func keyword need to be the first word of the token_manager.
  if (token_index != 0) {
    return kReport_FunctionKeywordShouldBeTheFirstToken;
  }

  // Mininum function code... "FUNC name() {
  if (token_manager.numTokens() < 5) {
    return kReport_FunctionDefinitionIncorrect;
  }

  // Check that the last token is "{"
  if (token_manager.getToken(token_manager.numTokens() - 1).text_ != "{") {
    return kReport_ExpectingOpenBrackets;
  }

  // Check if the name is valid
  Token token = token_manager.getToken(token_index + 1);
  if (token.type_ != kTokenType_Variable) {
    return kReport_InvalidNameOfFunction;
  }

  // FUNCTION DEFINITION
  // Command to define the function.
  machine->addCommand(kCommandType_FunctionDefinition, token.text_);
  // Deleting "func" and "funtion_name" tokens and the "{" one.
  token_manager.removeToken(token_index);
  token_manager.removeToken(token_index);
  token_manager.removeToken(token_manager.numTokens() - 1);

  // PARAMETERS OF THE FUNCTION
  // Saving the parameters in strings.
  std::vector<std::string> params;
  int32 num_tokens = token_manager.numTokens();
  for (int32 i = 1; i < num_tokens - 1; i++) { // Ignoring "(" and ")"
    token = token_manager.getToken(i);
    if (token.text_ != ",") {
      if (token.type_ != kTokenType_Variable) {
        return kReport_UnexpectedFunctionParameters;
      }
      // Save the parameter
      params.push_back(token.text_);
    }
  }

  // Creating the commands -> Number of parameters, param1, param2...
  int32 num_params = params.size();
  if (num_params > MAX_PARAMETERS_PER_FUNCTION) {
    return kReport_ExceededNumParamsAllowedPerFunction;
  }
  char num_params_text[3];
  sprintf_s(num_params_text, 3, "%d", num_params);
  machine->addCommand(kCommandType_FunctionNumParameters, num_params_text);
  for (int32 i = 0; i < num_params; i++) {
    machine->addCommand(kCommandType_FunctionParameter, params[i]);
  }

  // Tagging the start of the definition of a function
  addTag(kTag_Function);

  return kReport_NoErrors;
}

Report Compiler::compileLoopKeywordToken(Machine* machine,
  TokenManager& token_manager,
  int32& token_index) {

  if (token_index != 0) {
    return kReport_LoopKeywordShouldBeTheFirstToken;
  }

  if (token_manager.getToken(token_index).text_ == "WHILE") {
    machine->addCommand(kCommandType_LoopStartPoint);
    // removing the loop ("WHILE") keyword.
    token_manager.removeToken(token_index);

    // Checks if the last token of the line sentence is a "{" to start the body.
    if (token_manager.getToken(token_manager.numTokens() - 1).text_ != "{") {
      return kReport_ExpectingOpenBrackets;
    }
    // Removing "{"
    token_manager.removeToken(token_manager.numTokens() - 1);
    addTag(kTag_Loop);
    // Compiling the condition sentence.
    Report report = compileTokens(machine, token_manager);
    // Once compiled, we assign a command to evaluate the condition.
    machine->addCommand(kCommandType_ConditionToEvaluate);
    return report;
  }

  ReportError("\"do\" and \"for\" keywords functionality not programmed yet in this version");
  return kReport_KeywordFunctionalityNotProgrammedYet;
}

Report Compiler::compileVariableKeywordToken(Machine* machine,
  TokenManager& token_manager,
  int32& token_index) {

  // Gets the variable name.
  Token var_name = token_manager.getToken(token_index + 1);
  if (var_name.type_ != kTokenType_Variable) {
    return kReport_ExpectingNameOfVariable;
  }
  // Delete de "var" keyword   
  token_manager.removeToken(token_index);  // "var"
                                           // Adds a command who defines the variable name.
  machine->addCommand(kCommandType_VariableDefinition, var_name.text_);
  // Compile the rest of the line once the variable name is defined.
  return compileTokens(machine, token_manager);
}

Report Compiler::compileVariablePackKeywordToken(Machine* machine,
  TokenManager& token_manager,
  int32& token_index) {

  // ERROR CHECKINGS
  // VARPACK keyword need to be the first word of the token_manager.
  if (token_index != 0) {
    return kReport_VariablePackKeywordShouldBeTheFirstToken;
  }

  // Mininum varpack code... "VARPACK name {"
  if (token_manager.numTokens() < 3) {
    return kReport_VariablePackDefinitionIncorrect;
  }

  // Check that the last token is "{"
  if (token_manager.getToken(token_manager.numTokens() - 1).text_ != "{") {
    return kReport_ExpectingOpenBrackets;
  }

  // Check if the name is valid
  Token token = token_manager.getToken(token_index + 1);
  if (token.type_ != kTokenType_Variable) {
    return kReport_InvalidNameOfVariablePack;
  }

  // VARIABLE PACK DEFINITION
  // Command to define the variable pack.
  machine->addCommand(kCommandType_VariablePackDefinition, token.text_);
  // Deleting "VARPACK" and "variable pack name" tokens and the "{" one.
  token_manager.removeToken(token_index);
  token_manager.removeToken(token_index);
  token_manager.removeToken(token_manager.numTokens() - 1);

  // Tagging the start of the definition of a variable pack
  addTag(kTag_VariablePack);

  return kReport_NoErrors;
}


#pragma endregion

// Declaration of the static stack
Machine::Stack Machine::stack_;


}; /* JMP */

#endif /* __JMP_SINGLE_HEADER_H__ */