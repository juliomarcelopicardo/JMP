/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#include "value.h"

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





}; /* JMP */