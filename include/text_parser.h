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
#include "machine.h"

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

/***************************  PARSER CORE METHODS  ****************************/

  /**
  * @brief Main function of the text parser, will compile a sentence or expression.
  *
  * @param machine Machine where all the process will be made.
  * @param sentence Text, sentence or expression to compile.
  * @param line_number Line number of the sentence in the full script (for errors).
  * @return Report with the compiling results.
  */
  Report compile(Machine* machine, std::string sentence, uint32 line_number);

  /**
  * @brief Compile all the tokens of this manager, using the machine.
  *
  * @param machine Machine where all the process will be made.
  * @param token_manager Manager where all the tokens are allocated.
  * @return Report with the compiling results.
  */
  Report compileTokens(Machine* machine, TokenManager& token_manager);


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
  */
  void generateNextToken();

  /**
  * @brief Generates the initial priority of the token depending on its type.
  */
  void generateCurrentTokenInitialPriority();


/****************************  TEXT MODIFIERS  ********************************/

  /**
  * @brief Removes all the ' ' in quotes, to avoid separate quotes in many tokens.
  *
  * Example: var x = "a b c d"  ->  result: var x = "a_b_c_d"
  * @param sentence Text to analyze.
  * @param replacement Char used to replace blank spaces.
  */
  void replaceSpacesFromQuotes(std::string& sentence, char8 replacement = '_');

  /**
  * @brief Inverse of replaceSpacesFromQuotes function. 
  *
  * Example: var x = "a_b_c_d"  ->  result: var x = "a b c d"
  * @param sentence Text to analyze.
  * @param replacement Char that will be replaced for blank spaces.
  */
  void recoverSpacesFromQuotes(std::string& sentence, char8 replacement = '_');


/************************  TEXT ANALYZING GETTERS *****************************/

  /**
  * @brief Character analyzer getter, tells you if the character is a separator.
  *
  * @param character Character to analyze.
  * @return true if it is a separator.
  */
  const bool isSeparator(const char8& character);
  /**
  * @brief Character analyzer getter, tells you if the character is a blank space.
  *
  * @param character Character to analyze.
  * @return true if it is a blank space.
  */
  const bool isBlankSpace(const char8& character);
  /**
  * @brief Character analyzer getter, tells you if the character is a letter.
  *
  * '"' and '_' will be considered as letters because we need them for quotes.
  * @param character Character to analyze.
  * @return true if it is a letter.
  */
  const bool isLetter(const char8& character);
  /**
  * @brief Character analyzer getter, tells you if the character is a digit.
  *
  * @param character Character to analyze.
  * @return true if it is a digit.
  */
  const bool isDigit(const char8& character);
  /**
  * @brief String analyzer getter, tells you if the string is a keyword.
  *
  * @param word String to analyze.
  * @return true if it is a keyword.
  */
  const bool isKeyword(const std::string& word);


/*******************************************************************************
***                           PUBLIC ATTRIBUTES                              ***
*******************************************************************************/

  /// Current sentence that we will try to compile.
  std::string sentence_;
  /// Current sentence index, to know which character are we compiling.
  uint32 sentence_index_;
  /// Current token that we are analyzing.
  Token current_token_;


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