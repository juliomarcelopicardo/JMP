/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#ifndef __JMP_TEXTPARSER_H__
#define __JMP_TEXTPARSER_H__ 1

#include "types.h"
#include "token_manager.h"

namespace JMP {

/// Text parser used to save all the tokens from .jmp files.
class TextParser {

 public: 

/*******************************************************************************          
***                             PUBLIC METHODS                               ***
*******************************************************************************/

  /// Default class constructor.
  TextParser();
  /// Default class destructor.
  ~TextParser();



/*******************************************************************************
***                           PUBLIC ATTRIBUTES                              ***
*******************************************************************************/





 private:

/*******************************************************************************
***                            PRIVATE METHODS                               ***
*******************************************************************************/


  /// Default copy constructor.
  TextParser(const TextParser& copy);
  /// Assignment operator.
  TextParser& operator=(const TextParser& copy);

/*******************************************************************************
***                          PRIVATE ATTRIBUTES                              ***
*******************************************************************************/



}; /* TextParser */
}; /* JMP */

#endif /* __JMP_TEXTPARSER_H__ */