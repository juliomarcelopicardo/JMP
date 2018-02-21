/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP_PROJECT - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#include "registered_function.h"

namespace JMP {


/*******************************************************************************
***                       CONSTRUCTOR & DESTRUCTOR                           ***
*******************************************************************************/

RegisteredFunction::RegisteredFunction() {
  name_ = "";
  function_pointer_ = nullptr;
  params_.reserve(5);
}

RegisteredFunction::RegisteredFunction(const char* name, 
                                       void(*ptr)(std::vector<Value>&)) {
  name_ = name;
  function_pointer_ = ptr;
}

RegisteredFunction::~RegisteredFunction() {
  params_.clear();
  function_pointer_ = nullptr;
}

RegisteredFunction::RegisteredFunction(const RegisteredFunction& copy) {
  name_ = copy.name_;
  function_pointer_ = copy.function_pointer_;
  params_ = copy.params_;
}

RegisteredFunction& RegisteredFunction::operator=(const RegisteredFunction& copy) {
  name_ = copy.name_;
  function_pointer_ = copy.function_pointer_;
  params_ = copy.params_;
  return *this;
}





}; /* JMP_PROJECT */