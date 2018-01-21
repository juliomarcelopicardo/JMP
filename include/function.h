/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#ifndef __JMP_FUNCTION_H__
#define __JMP_FUNCTION_H__ 1

#include "types.h"
#include "variable.h"
#include <string>
#include <vector>
#include "report.h"

#define INVALID_FUNCTION_ID -9999

namespace JMP {

/// Now we create a list of functions, that will be used to jump between different
/// function calls, getting it's variabes and being able to go back to the step
/// id where the function was called */
class Function {

 public: 

/*******************************************************************************          
***                             PUBLIC METHODS                               ***
*******************************************************************************/

  /// Default class constructor.
  Function();
  /// Overloaded class constructor.
  Function(const int32 origin_id);
  /// Default class destructor.
  ~Function();
  /// Default copy constructor.
  Function(const Function& copy);
  /// Assignment operator.
  Function& operator=(const Function& copy);


/******************************  MAIN METHODS  *********************************/

  /**
  * @brief Push back a variable in the list.
  *
  * @param variable Variable to push back in the list
  * @return Report with the result of the function.
  */
  Report addVariable(const Variable variable);

/********************************  GETTERS  ***********************************/

  /**
  * @brief Looks for a variable in the list and return its id or position.
  *
  * @param variable_name Name of the variable that we are looking for.
  * @return ID of the variable (position in the list), or INVALID... if error.
  */
  const int32 getVariableID(const std::string& variable_name);

  /**
  * @brief Looks for a variable in the list.
  *
  * @param variable_name Name of the variable that we are looking for.
  * @return Pointer to the variable from the list. Nullptr if not found.
  */
  Variable* getVariable(const std::string& variable_name);

  /**
  * @brief Number of variables allocated in the list getter. List length.
  *
  * @return List length.
  */
  const int32 numVariables();

  /**
  * @brief Origin of the function id getter.
  *
  * @return Origin id of the function call, to continue executing.
  */
  const int32 originID();

/*******************************************************************************
***                           PUBLIC ATTRIBUTES                              ***
*******************************************************************************/



 private:

/*******************************************************************************
***                            PRIVATE METHODS                               ***
*******************************************************************************/

  /// List of variables allocated in the function scope.
  std::vector<Variable> variable_list_;
  /// Origin command id, where the function is called.
  int32 origin_id_;
  /// Variable list length
  int32 variable_list_length_;



/*******************************************************************************
***                          PRIVATE ATTRIBUTES                              ***
*******************************************************************************/
  


}; /* Function */
}; /* JMP */

#endif /* __JMP_FUNCTION_H__ */