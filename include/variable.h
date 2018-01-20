/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#ifndef __JMP_VARIABLE_H__
#define __JMP_VARIABLE_H__ 1

#include "types.h"
#include "value.h"
#include <string>

namespace JMP {

/// Class used to manage the variables that will be added to the registger.
/// Depending on if the variable is a external registered one (created in C++) or
/// if its one defined in the script, the attributes used will be different.
class Variable {

 public: 

/*******************************************************************************          
***                             PUBLIC METHODS                               ***
*******************************************************************************/

  /// Default class constructor.
  Variable();
  /// Overloaded class constructor.
  Variable(const char* name, int32 integer_value);
  /// Overloaded class constructor.
  Variable(const char* name, float32 float_value);
  /// Overloaded class constructor.
  Variable(const char* name, const char* text_value);
  /// Overloaded class constructor.
  Variable(const char* name, ValueType type, void* ptr_to_the_original);
  /// Default class destructor.
  ~Variable();
  /// Default copy constructor.
  Variable(const Variable& copy);
  /// Assignment operator.
  Variable& operator=(const Variable& copy);



/********************************  GETTERS  ***********************************/

 

/*******************************************************************************
***                           PUBLIC ATTRIBUTES                              ***
*******************************************************************************/

  /// Will define if the variable is a external one or a script native one.
  bool is_registered_;
  /// Name of the variable.
  std::string name_;

/******************************  REGISTERED  **********************************/

  /// Pointer to the original variable defined in c++
  void* pointer_to_the_original_;

/****************************  NOT REGISTERED  ********************************/
  /// Value of the variable
  Value value_;

 private:

/*******************************************************************************
***                            PRIVATE METHODS                               ***
*******************************************************************************/




/*******************************************************************************
***                          PRIVATE ATTRIBUTES                              ***
*******************************************************************************/



}; /* Variable */
}; /* JMP */

#endif /* __JMP_VARIABLE_H__ */