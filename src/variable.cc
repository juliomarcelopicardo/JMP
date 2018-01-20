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
  number_value_ = VARIABLE_INITIAL_VALUE;
}

Variable::~Variable() {}

Variable::Variable(const Variable& copy) {
  type_ = copy.type_;
  is_registered_ = copy.is_registered_;
  name_ = copy.name_;
  pointer_to_the_original_ = copy.pointer_to_the_original_;
  text_value_ = copy.text_value_;
  number_value_ = copy.number_value_;
}

Variable & Variable::operator=(const Variable& copy) {
  type_ = copy.type_;
  is_registered_ = copy.is_registered_;
  name_ = copy.name_;
  pointer_to_the_original_ = copy.pointer_to_the_original_;
  text_value_ = copy.text_value_;
  number_value_ = copy.number_value_;
  return *this;
}





}; /* JMP */