/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#ifndef __JMP_MACHINE_H__
#define __JMP_MACHINE_H__ 1

#include "types.h"
#include "report.h"
#include "token_manager.h"

namespace JMP {

/// Core of the language, this machine will be used to process .jpm script files.
class Machine {

 public: 

/*******************************************************************************          
***                             PUBLIC METHODS                               ***
*******************************************************************************/

  /// Default class constructor.
  Machine();
  /// Default class destructor.
  ~Machine();


  /**
  * @brief Loads a script file so the machine can process it. Extension must be .jmp
  *
  * @param script Script file path.
  * @return Report with the function result.
  */
  Report process(std::string script_filename);

  /**
  * @brief Checks if the extension of the file is .jpm
  *
  * @param filename File to check.
  * @return InvalidExtension if failed.
  */
  Report checkExtension(std::string filename);


/*******************************************************************************
***                           PUBLIC ATTRIBUTES                              ***
*******************************************************************************/





 private:

/*******************************************************************************
***                            PRIVATE METHODS                               ***
*******************************************************************************/


  /// Default copy constructor.
  Machine(const Machine& copy);
  /// Assignment operator.
  Machine& operator=(const Machine& copy);

/*******************************************************************************
***                          PRIVATE ATTRIBUTES                              ***
*******************************************************************************/



}; /* Machine */
}; /* JMP */

#endif /* __JMP_MACHINE_H__ */