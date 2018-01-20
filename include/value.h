/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#ifndef __JMP_VALUE_H__
#define __JMP_VALUE_H__ 1

#include "types.h"
#include <string>

namespace JMP {

#define INITIALIZATION_VALUE -99999

/// Value type.
enum ValueType {
  kValueType_None = 0,
  kValueType_Float,
  kValueType_Integer,
  kValueType_Text,
};


/// Class used to manage the different values used in the variables and the stack
class Value {

 public: 

/*******************************************************************************          
***                             PUBLIC METHODS                               ***
*******************************************************************************/

  /// Default class constructor.
  Value();
  /// Overloaded class constructor.
  Value(const int32 integer_value);
  /// Overloaded class constructor.
  Value(const float32 float_value);
  /// Overloaded class constructor.
  Value(const char* text_value);
  /// Default class destructor.
  ~Value();
  /// Default copy constructor.
  Value(const Value& copy);
  /// Assignment operator.
  Value& operator=(const Value& copy);



/********************************  GETTERS  ***********************************/

 

/*******************************************************************************
***                           PUBLIC ATTRIBUTES                              ***
*******************************************************************************/

  /// Type of value.
  ValueType type_;
  /// String variable value.
  std::string text_;
  /// Float variable value.
  float32 float_;
  /// Integer variable value.
  int32 integer_;

 private:

/*******************************************************************************
***                            PRIVATE METHODS                               ***
*******************************************************************************/




/*******************************************************************************
***                          PRIVATE ATTRIBUTES                              ***
*******************************************************************************/



}; /* Value */
}; /* JMP */

#endif /* __JMP_VALUE_H__ */