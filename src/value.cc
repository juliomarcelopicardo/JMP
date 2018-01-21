/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#include "value.h"
#include "report.h"

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

}; /* JMP */