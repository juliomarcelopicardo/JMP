/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP_PROJECT - A Scripting Language for Game Engines.
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

Variable::Variable(const char* name) {
  is_registered_ = false;
  name_ = name;
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

Variable::Variable(const char* name, const Value& value) {
  is_registered_ = false;
  name_ = name;
  pointer_to_the_original_ = nullptr;
  value_ = value;
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

/*******************************************************************************
***                                SETTERS                                   ***
*******************************************************************************/

Report Variable::setValue(const Value value) {
  
  if (is_registered_) {

    // Original value is a TEXT.
    if (value_.type_ == kValueType_Text && value.type_ == kValueType_Text) {
      auto* ptr = static_cast<std::string*>(pointer_to_the_original_);
      *ptr = value.text_;
      return kReport_NoErrors;
    }

    if (value_.type_ == kValueType_Text && value.type_ == kValueType_Float) {
      auto* ptr = static_cast<std::string*>(pointer_to_the_original_);
      *ptr = std::to_string(value.float_);
      return kReport_NoErrors;
    }

    if (value_.type_ == kValueType_Text && value.type_ == kValueType_Integer) {
      auto* ptr = static_cast<std::string*>(pointer_to_the_original_);
      *ptr = std::to_string(value.integer_);
      return kReport_NoErrors;
    }

    // Original value is an INTEGER.
    if (value_.type_ == kValueType_Integer && value.type_ == kValueType_Integer) {
      auto* ptr = static_cast<int32*>(pointer_to_the_original_);
      *ptr = value.integer_;
      return kReport_NoErrors;
    }

    if (value_.type_ == kValueType_Integer && value.type_ == kValueType_Float) {
      auto* ptr = static_cast<int32*>(pointer_to_the_original_);
      *ptr = (int32)value.float_;
      return kReport_NoErrors;
    }

    // Original value is a FLOAT.
    if (value_.type_ == kValueType_Float && value.type_ == kValueType_Float) {
      auto* ptr = static_cast<float32*>(pointer_to_the_original_);
      *ptr = value.float_;
      return kReport_NoErrors;
    }

    if (value_.type_ == kValueType_Float && value.type_ == kValueType_Integer) {
      auto* ptr = static_cast<float32*>(pointer_to_the_original_);
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



/*******************************************************************************
***                                GETTERS                                   ***
*******************************************************************************/

Value Variable::getValue() const {
  if (!is_registered_) {
    return value_;
  }

  if (value_.type_ == kValueType_Text) {
    auto* ptr = static_cast<std::string*>(pointer_to_the_original_);
    return { ptr->c_str() };
  }

  if (value_.type_ == kValueType_Float) {
    auto* ptr = static_cast<float32*>(pointer_to_the_original_);
    return { *ptr };
  }

  if (value_.type_ == kValueType_Integer) {
    auto* ptr = static_cast<int32*>(pointer_to_the_original_);
    return { *ptr };
  }

  ReportError("Error getting the value of the variable which name is : " + name_);
  return Value();
}

}; /* JMP_PROJECT */