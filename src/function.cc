/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP_PROJECT - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#include "function.h"

namespace JMP {

/*******************************************************************************
***                       CONSTRUCTOR & DESTRUCTOR                           ***
*******************************************************************************/

Function::Function() {
  variable_list_.reserve(10);
  origin_id_ = INVALID_FUNCTION_ID;
  variable_list_length_ = 0;
}

Function::Function(const int32 origin_id) {
  variable_list_.reserve(10);
  origin_id_ = origin_id;
  variable_list_length_ = 0;
}

Function::~Function() {
  variable_list_.clear();
  variable_list_length_ = 0;
}

Function::Function(const Function& copy) {
  variable_list_ = copy.variable_list_;
  origin_id_ = copy.origin_id_;
  variable_list_length_ = copy.variable_list_length_;
}

Function& Function::operator=(const Function& copy) {
  variable_list_ = copy.variable_list_;
  origin_id_ = copy.origin_id_;
  variable_list_length_ = copy.variable_list_length_;
  return *this;
}



/*******************************************************************************
***                             MAIN METHODS                                 ***
*******************************************************************************/

Report Function::addVariable(const Variable& variable) {
  variable_list_.push_back(variable);
  variable_list_length_++;
  return kReport_NoErrors;
}

Report Function::addVariable(const char* name, const Value& value) {
  variable_list_.push_back({ name, value });
  variable_list_length_++;
  return kReport_NoErrors;
}

/*******************************************************************************
***                               GETTERS                                    ***
*******************************************************************************/

  int32 Function::getVariableID(const std::string& variable_name) {
  for (int32 i = 0; i < variable_list_length_; ++i) {
    if (variable_list_[i].name_ == variable_name) {
      return i;
    }
  }
  //ReportWarning(" Couldnt get variable from function. Variable name: " + variable_name);
  return INVALID_FUNCTION_ID;
}

Variable* Function::getVariable(const std::string& variable_name) {
  int32 id = getVariableID(variable_name);
  if (id != INVALID_FUNCTION_ID) {
    return &variable_list_[id];
  }
  return nullptr;
}

  int32 Function::numVariables() const
  {
  return variable_list_length_;
}

  int32 Function::originID() const
  {
  return origin_id_;
}



}; /* JMP_PROJECT */