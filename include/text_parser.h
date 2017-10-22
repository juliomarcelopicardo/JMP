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

/****************************  TOKEN GENERATION  ******************************/

  /**
  * @brief Adds a new token to the list.
  *
  * @param sentence Text of the token.
  * @param token_manager Type of the token.
  */
  void generateTokens(std::string& sentence, TokenManager& token_manager);

  /**
  * @brief Analyzes the current sentence to get the next token available.
  *
  * Function will allocate also the token into the current token variable.
  * @return Next token.
  */
  Token generateNextToken();


/****************************  TEXT MODIFIERS  ********************************/

  /**
  * @brief Removes all the ' ' in quotes, to avoid separate quotes in many tokens.
  *
  * Example: var x = "a b c d"  ->  result: var x = "a_b_c_d"
  * @param sentence Text to analyze.
  * @param replacement Char used to replace blank spaces.
  */
  void replaceSpacesFromQuotes(std::string& sentence, char8 replacement = '_');


/*******************************  ANALYZING  **********************************/



/*******************************************************************************
***                           PUBLIC ATTRIBUTES                              ***
*******************************************************************************/

  /// Current sentence that we will try to compile.
  std::string sentence_;
  /// Current sentence index, to know which character are we compiling.
  uint32 sentence_index_;
  /// Current token that we are analyzing.
  Token current_token;



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