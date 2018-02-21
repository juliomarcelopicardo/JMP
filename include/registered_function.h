/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP_PROJECT - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#ifndef __JMP_REGISTERED_FUNCTION_H__
#define __JMP_REGISTERED_FUNCTION_H__ 1

#include "value.h"
#include <string>
#include <vector>

namespace JMP {

/// Class used to manage the different functions registered from c++
class RegisteredFunction {

 public: 

/*******************************************************************************          
***                             PUBLIC METHODS                               ***
*******************************************************************************/

  /// Default class constructor.
  RegisteredFunction();
  /// Default class constructor.
  RegisteredFunction(const char* name, void(*ptr)(std::vector<Value>&));
  /// Default class destructor.
  ~RegisteredFunction();
  /// Default copy constructor.
  RegisteredFunction(const RegisteredFunction& copy);
  /// Assignment operator.
  RegisteredFunction& operator=(const RegisteredFunction& copy);


/*******************************************************************************
***                           PUBLIC ATTRIBUTES                              ***
*******************************************************************************/

  /// Name of the function
  std::string name_;
  /// Pointer to the function defined in C++
  void(*function_pointer_)(std::vector<Value>&);
  /// Parameters of the function when its called from the script.
  std::vector<Value> params_;



}; /* RegisteredFunction */



}; /* JMP_PROJECT */

#endif /* __JMP_REGISTERED_FUNCTION_H__ */