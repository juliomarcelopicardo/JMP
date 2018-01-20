/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#include "variable.h"

namespace JMP {

/*******************************************************************************
***                       CONSTRUCTOR & DESTRUCTOR                           ***
*******************************************************************************/

Variable::Variable() {
  type_ = kVariableType_None;
  is_registered_ = false;
  name_ = "";
  pointer_to_the_original_ = nullptr;
  text_value_ = "";
  float_value_ = VARIABLE_INITIAL_VALUE;
  integer_value_ = VARIABLE_INITIAL_VALUE;
}

Variable::Variable(const char* name, int32 integer_value) {
  type_ = kVariableType_Integer;
  is_registered_ = false;
  name_ = name;
  pointer_to_the_original_ = nullptr;
  text_value_ = "";
  float_value_ = VARIABLE_INITIAL_VALUE;
  integer_value_ = integer_value;
}

Variable::Variable(const char* name, float32 float_value) {
  type_ = kVariableType_Float;
  is_registered_ = false;
  name_ = name;
  pointer_to_the_original_ = nullptr;
  text_value_ = "";
  float_value_ = float_value;
  integer_value_ = VARIABLE_INITIAL_VALUE;
}

Variable::Variable(const char* name, const char* text_value) {
  type_ = kVariableType_Text;
  is_registered_ = false;
  name_ = name;
  pointer_to_the_original_ = nullptr;
  text_value_ = text_value;
  float_value_ = VARIABLE_INITIAL_VALUE;
  integer_value_ = VARIABLE_INITIAL_VALUE;
}

Variable::Variable(const char* name, VariableType type, void* ptr_to_the_original) {
  type_ = type;
  is_registered_ = true;
  name_ = name;
  pointer_to_the_original_ = ptr_to_the_original;
  text_value_ = "";
  float_value_ = VARIABLE_INITIAL_VALUE;
  integer_value_ = VARIABLE_INITIAL_VALUE;
}

Variable::~Variable() {}

Variable::Variable(const Variable& copy) {
  type_ = copy.type_;
  is_registered_ = copy.is_registered_;
  name_ = copy.name_;
  pointer_to_the_original_ = copy.pointer_to_the_original_;
  text_value_ = copy.text_value_;
  float_value_ = copy.float_value_;
  integer_value_ = copy.integer_value_;
}

Variable & Variable::operator=(const Variable& copy) {
  type_ = copy.type_;
  is_registered_ = copy.is_registered_;
  name_ = copy.name_;
  pointer_to_the_original_ = copy.pointer_to_the_original_;
  text_value_ = copy.text_value_;
  float_value_ = copy.float_value_;
  integer_value_ = copy.integer_value_;
  return *this;
}





}; /* JMP */