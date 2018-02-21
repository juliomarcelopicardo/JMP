/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP_PROJECT - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#include "value.h"
#include "report.h"
#include <cmath>

namespace JMP {


/*******************************************************************************
***                       CONSTRUCTOR & DESTRUCTOR                           ***
*******************************************************************************/

Value::Value() {
  type_ = kValueType_None;
  text_ = "";
  float_ = INITIALIZATION_VALUE;
  integer_ = INITIALIZATION_VALUE;
}

Value::Value(const bool condition_result) {

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

Value::Value(const int32 integer_value) {
  type_ = kValueType_Integer;
  text_ = "";
  float_ = INITIALIZATION_VALUE;
  integer_ = integer_value;
}

Value::Value(const float32 float_value) {
  type_ = kValueType_Float;
  text_ = "";
  float_ = float_value;
  integer_ = INITIALIZATION_VALUE;
}

Value::Value(const char* text_value) {
  type_ = kValueType_Text;
  text_ = text_value;
  float_ = INITIALIZATION_VALUE;
  integer_ = INITIALIZATION_VALUE;
}

Value::~Value() {}

Value::Value(const Value& copy) {
  type_ = copy.type_;
  text_ = copy.text_;
  float_ = copy.float_;
  integer_ = copy.integer_;
}

Value& Value::operator=(const Value& copy) {
  type_ = copy.type_;
  text_ = copy.text_;
  float_ = copy.float_;
  integer_ = copy.integer_;
  return *this;
}

void Value::print() const {
  switch (type_) {
    case JMP::kValueType_Float: { printf("%f\n", float_); }  break;
    case JMP::kValueType_Integer: { printf("%d\n", integer_); } break;
    case JMP::kValueType_Text: { printf("%s\n", text_.c_str()); } break;
  default: {}
  }
}

float32 Value::getAsFloat() const {
  if (type_ == kValueType_Float) { return float_; }
  if (type_ == kValueType_Integer) { return (float32)integer_; }
  ReportWarning(" Trying to get a float from a non-number value");
  return INITIALIZATION_VALUE;
}

int32 Value::getAsInteger() const {
  if (type_ == kValueType_Integer) { return integer_; }
  if (type_ == kValueType_Float) { return (int32)float_; }
  ReportWarning(" Trying to get an integer from a non-number value");
  return INITIALIZATION_VALUE;
}

/*******************************************************************************
***                              MATH METHODS                                ***
*******************************************************************************/

float32 Value::Sin() const {
  if (type_ == kValueType_Float) { return sinf(float_); }
  if (type_ == kValueType_Integer) { return sinf((float32)integer_); }
  ReportWarning(" Trying to calculate a SIN of a non number value");
  return INITIALIZATION_VALUE;
}

float32 Value::Cos() const {
  if (type_ == kValueType_Float) { return cosf(float_); }
  if (type_ == kValueType_Integer) { return cosf((float32)integer_); }
  ReportWarning(" Trying to calculate a COS of a non number value");
  return INITIALIZATION_VALUE;
}

float32 Value::Tan() const {
  if (type_ == kValueType_Float) { return tanf(float_); }
  if (type_ == kValueType_Integer) { return tanf((float32)integer_); }
  ReportWarning(" Trying to calculate a TAN of a non number value");
  return INITIALIZATION_VALUE;
}

float32 Value::ASin() const {
  if (type_ == kValueType_Float) { return asinf(float_); }
  if (type_ == kValueType_Integer) { return asinf((float32)integer_); }
  ReportWarning(" Trying to calculate a ASIN of a non number value");
  return INITIALIZATION_VALUE;
}

float32 Value::ACos() const {
  if (type_ == kValueType_Float) { return acosf(float_); }
  if (type_ == kValueType_Integer) { return acosf((float32)integer_); }
  ReportWarning(" Trying to calculate a ACOS of a non number value");
  return INITIALIZATION_VALUE;
}

float32 Value::ATan() const {
  if (type_ == kValueType_Float) { return atanf(float_); }
  if (type_ == kValueType_Integer) { return atanf((float32)integer_); }
  ReportWarning(" Trying to calculate a ATAN of a non number value");
  return INITIALIZATION_VALUE;
}



/*******************************************************************************
***                               OPERATORS                                  ***
*******************************************************************************/

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

}; /* JMP_PROJECT */