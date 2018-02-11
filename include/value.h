/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP_PROJECT - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#ifndef __JMP_VALUE_H__
#define __JMP_VALUE_H__ 1

#include "types.h"
#include <string>

namespace JMP_PROJECT {

#define INITIALIZATION_VALUE -99999
#define CONDITION_RESULT_TRUE 30 // Random number

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
  Value(const bool condition_result);
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


  /// @brief Prints the value.
  void print();

/*****************************  MATH METHODS  *********************************/

  /**
  * @brief Calculates the Sin if the value is a number.
  *
  * @return Sin of the number or INITIALIZATION_VALUE if its not a number.
  */
  float32 Sin();

  /**
  * @brief Calculates the Cos if the value is a number.
  *
  * @return Cos of the number or INITIALIZATION_VALUE if its not a number.
  */
  float32 Cos();

  /**
  * @brief Calculates the Tan if the value is a number.
  *
  * @return Tan of the number or INITIALIZATION_VALUE if its not a number.
  */
  float32 Tan();

  /**
  * @brief Calculates the ASin if the value is a number.
  *
  * @return ASin of the number or INITIALIZATION_VALUE if its not a number.
  */
  float32 ASin();

  /**
  * @brief Calculates the ACos if the value is a number.
  *
  * @return ACos of the number or INITIALIZATION_VALUE if its not a number.
  */
  float32 ACos();

  /**
  * @brief Calculates the ATan if the value is a number.
  *
  * @return ATan of the number or INITIALIZATION_VALUE if its not a number.
  */
  float32 ATan();




 

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

/*******************************  OPERATORS  **********************************/

Value operator+(const Value&, const Value&);
Value operator-(const Value&, const Value&);
Value operator*(const Value&, const Value&);
Value operator/(const Value&, const Value&);
Value operator^(const Value&, const Value&);
Value operator>(const Value&, const Value&);
Value operator<(const Value&, const Value&);
Value operator>=(const Value&, const Value&);
Value operator<=(const Value&, const Value&);
Value operator==(const Value&, const Value&);
Value operator!=(const Value&, const Value&);

}; /* JMP_PROJECT */

#endif /* __JMP_VALUE_H__ */