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
  is_registered_ = false;
  name_ = "";
  pointer_to_the_original_ = nullptr;
}

Variable::Variable(const char* name, int32 integer_value) {
  is_registered_ = false;
  name_ = name;
  pointer_to_the_original_ = nullptr;
  value_ = { integer_value };
}

Variable::Variable(const char* name, float32 float_value) {
  is_registered_ = false;
  name_ = name;
  pointer_to_the_original_ = nullptr;
  value_ = { float_value };
}

Variable::Variable(const char* name, const char* text_value) {
  is_registered_ = false;
  name_ = name;
  pointer_to_the_original_ = nullptr;
  value_ = { text_value };
}

Variable::Variable(const char* name, ValueType type, void* ptr_to_the_original) {
  is_registered_ = true;
  name_ = name;
  pointer_to_the_original_ = ptr_to_the_original;
  value_.type_ = type;
}

Variable::~Variable() {}

Variable::Variable(const Variable& copy) {
  is_registered_ = copy.is_registered_;
  name_ = copy.name_;
  pointer_to_the_original_ = copy.pointer_to_the_original_;
  value_ = copy.value_;
}

Variable & Variable::operator=(const Variable& copy) {
  is_registered_ = copy.is_registered_;
  name_ = copy.name_;
  pointer_to_the_original_ = copy.pointer_to_the_original_;
  value_ = copy.value_;
  return *this;
}





}; /* JMP */