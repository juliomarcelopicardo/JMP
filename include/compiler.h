/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP_PROJECT - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#ifndef __JMP_COMPILER_H__
#define __JMP_COMPILER_H__ 1

// Priorities
#define KEYWORD_PRIORITY 500
#define CLOSE_BRACKETS_PRIORITY 200
#define OPEN_PARENTHESIS_PRIORITY 100
#define POWER_OPERATION_PRIORITY 99
#define MULTIPLY_OPERATION_PRIORITY 98
#define ADDITION_OPERATION_PRIORITY 97
#define COMPARISON_PRIORITY 96
#define EQUAL_PRIORITY 95
#define FUNCTION_NAME_PRIORITY 10
#define DEFAULT_PRIORITY 0
#define COMMA_PRIORITY (-10)
#define MAX_PARAMETERS_PER_FUNCTION 30


#include "types.h"
#include "token_manager.h"
#include "machine.h"

namespace JMP {

enum Tag {
  kTag_None = 0,
  kTag_Loop,
  kTag_Conditional,
  kTag_Function,
  kTag_VariablePack,
};

/// Text parser and compiler used to save and manage all the tokens from .jmp files.
class Compiler {

 public: 

/*******************************************************************************          
***                             PUBLIC METHODS                               ***
*******************************************************************************/

  /// Default class constructor.
  Compiler();
  /// Default class destructor.
  ~Compiler();
  /// Default copy constructor.
  Compiler(const Compiler& copy) = delete;
  /// Assignment operator.
  Compiler& operator=(const Compiler& copy) = delete;

/***************************  PARSER CORE METHODS  ****************************/

  /**
  * @brief Main function of the compiler, will compile a sentence or expression.
  *
  * @param machine Machine where all the process will be made.
  * @param sentence Text, sentence or expression to compile.
  * @param line_number Line number of the sentence in the full script (for errors).
  * @return Report with the compiling results.
  */
  Report compile(Machine* machine, std::string sentence, uint32 line_number);

/******************* ******  TOKEN COMPILER METHODS  ***************************/

  /**
  * @brief Compile all the tokens of this manager, using the machine.
  *
  * @param machine Machine where all the process will be made.
  * @param token_manager Manager where all the tokens are allocated.
  * @return Report with the compiling results.
  */
  Report compileTokens(Machine* machine, TokenManager& token_manager);

  /**
  * @brief Compile the keyword type token.
  *
  * @param machine Machine where all the process will be made.
  * @param token_manager Manager where all the tokens are allocated.
  * @param token_index index of the token manager list.
  * @return Report with the compiling results.
  */
  Report compileKeywordToken(Machine* machine, TokenManager& token_manager, int32& token_index);

  /**
  * @brief Compile the separator type token.
  *
  * @param machine Machine where all the process will be made.
  * @param token_manager Manager where all the tokens are allocated.
  * @param token_index index of the token manager list.
  * @return Report with the compiling results.
  */
  Report compileSeparatorToken(Machine* machine, TokenManager& token_manager, int32& token_index);

  /**
  * @brief Checks if there are "," tokens in the list and compiles the content in both sides of the comma.
  *
  * @param machine Machine where all the process will be made.
  * @param token_manager Manager where all the tokens are allocated.
  * @return True if any comma is found, false otherwise.
  */
  bool checkIfAndCompileCommasContent(Machine* machine, TokenManager& token_manager);

/*******************  SEPARATORS TOKEN COMPILER METHODS  **********************/

  /**
  * @brief Compile the open parenthesis separator type token.
  *
  * @param machine Machine where all the process will be made.
  * @param token_manager Manager where all the tokens are allocated.
  * @param token_index index of the token manager list.
  * @return Report with the compiling results.
  */
  Report compileOpenParenthesisSeparatorToken(Machine* machine, 
                                              TokenManager& token_manager, 
                                              int32& token_index);

  /**
  * @brief Compile the close brackets separator type token.
  *
  * @param machine Machine where all the process will be made.
  * @param token_manager Manager where all the tokens are allocated.
  * @param token_index index of the token manager list.
  * @return Report with the compiling results.
  */
  Report compileCloseBracketsSeparatorToken(Machine* machine, 
                                            TokenManager& token_manager, 
                                            int32& token_index);

  /**
  * @brief Compile the mathematical addition operation separator type token.
  *
  * @param machine Machine where all the process will be made.
  * @param token_manager Manager where all the tokens are allocated.
  * @param token_index index of the token manager list.
  * @return Report with the compiling results.
  */
  Report compileAdditionOperationSeparatorToken(Machine* machine,
                                                TokenManager& token_manager,
                                                int32& token_index) const;

  /**
  * @brief Compile the mathematical multiply operation separator type token.
  *
  * @param machine Machine where all the process will be made.
  * @param token_manager Manager where all the tokens are allocated.
  * @param token_index index of the token manager list.
  * @return Report with the compiling results.
  */
  Report compileMultiplyOperationSeparatorToken(Machine* machine,
                                                TokenManager& token_manager,
                                                int32& token_index) const;

  /**
  * @brief Compile the mathematical power operation separator type token.
  *
  * @param machine Machine where all the process will be made.
  * @param token_manager Manager where all the tokens are allocated.
  * @param token_index index of the token manager list.
  * @return Report with the compiling results.
  */
  Report compilePowerOperationSeparatorToken(Machine* machine,
                                             TokenManager& token_manager,
                                             int32& token_index) const;

  /**
  * @brief Compile the mathematical comparison operation separator type token.
  *
  * @param machine Machine where all the process will be made.
  * @param token_manager Manager where all the tokens are allocated.
  * @param token_index index of the token manager list.
  * @return Report with the compiling results.
  */
  Report compileComparisonOperationSeparatorToken(Machine* machine,
                                                  TokenManager& token_manager,
                                                  int32& token_index) const;

  /**
  * @brief Compile the equal separator type token.
  *
  * @param machine Machine where all the process will be made.
  * @param token_manager Manager where all the tokens are allocated.
  * @param token_index index of the token manager list.
  * @return Report with the compiling results.
  */
  Report compileEqualSeparatorToken(Machine* machine,
                                    TokenManager& token_manager,
                                    int32& token_index);

/********************  KEYWORDS TOKEN COMPILER METHODS  ***********************/

  /**
  * @brief Compile the conditional keyword type token.
  *
  * @param machine Machine where all the process will be made.
  * @param token_manager Manager where all the tokens are allocated.
  * @param token_index index of the token manager list.
  * @return Report with the compiling results.
  */
  Report compileConditionalKeywordToken(Machine* machine,
                                        TokenManager& token_manager,
                                        int32& token_index);

  /**
  * @brief Compile the return keyword type token.
  *
  * @param machine Machine where all the process will be made.
  * @param token_manager Manager where all the tokens are allocated.
  * @param token_index index of the token manager list.
  * @return Report with the compiling results.
  */
  Report compileReturnKeywordToken(Machine* machine,
                                   TokenManager& token_manager,
                                   int32& token_index);

  /**
  * @brief Compile the function keyword type token.
  *
  * @param machine Machine where all the process will be made.
  * @param token_manager Manager where all the tokens are allocated.
  * @param token_index index of the token manager list.
  * @return Report with the compiling results.
  */
  Report compileFunctionKeywordToken(Machine* machine,
                                     TokenManager& token_manager,
                                     int32& token_index);

  /**
  * @brief Compile the loop keyword type token.
  *
  * @param machine Machine where all the process will be made.
  * @param token_manager Manager where all the tokens are allocated.
  * @param token_index index of the token manager list.
  * @return Report with the compiling results.
  */
  Report compileLoopKeywordToken(Machine* machine,
                                 TokenManager& token_manager,
                                 int32& token_index);

  /**
  * @brief Compile the variable keyword type token.
  *
  * @param machine Machine where all the process will be made.
  * @param token_manager Manager where all the tokens are allocated.
  * @param token_index index of the token manager list.
  * @return Report with the compiling results.
  */
  Report compileVariableKeywordToken(Machine* machine,
                                     TokenManager& token_manager,
                                     int32& token_index);

  /**
  * @brief Compile the variable pack keyword type token.
  *
  * @param machine Machine where all the process will be made.
  * @param token_manager Manager where all the tokens are allocated.
  * @param token_index index of the token manager list.
  * @return Report with the compiling results.
  */
  Report compileVariablePackKeywordToken(Machine* machine,
                                         TokenManager& token_manager,
                                         int32& token_index);

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
  void replaceSpacesFromQuotes(std::string& sentence, char8 replacement = '_') const;

  /**
  * @brief Inverse of replaceSpacesFromQuotes function. 
  *
  * Example: var x = "a_b_c_d"  ->  result: var x = "a b c d"
  * @param sentence Text to analyze.
  * @param replacement Char that will be replaced for blank spaces.
  */
  void recoverSpacesFromQuotes(std::string& sentence, char8 replacement = '_') const;


/************************  TEXT ANALYZING GETTERS *****************************/

  /**
  * @brief Character analyzer getter, tells you if the character is a separator.
  *
  * @param character Character to analyze.
  * @return true if it is a separator.
  */
  bool isSeparator(const char8& character) const;
  /**
  * @brief Character analyzer getter, tells you if the character is a blank space.
  *
  * @param character Character to analyze.
  * @return true if it is a blank space.
  */
  bool isBlankSpace(const char8& character) const;
  /**
  * @brief Character analyzer getter, tells you if the character is a letter.
  *
  * '"' and '_' will be considered as letters because we need them for quotes.
  * @param character Character to analyze.
  * @return true if it is a letter.
  */
  bool isLetter(const char8& character) const;
  /**
  * @brief Character analyzer getter, tells you if the character is a digit.
  *
  * @param character Character to analyze.
  * @return true if it is a digit.
  */
  bool isDigit(const char8& character) const;
  /**
  * @brief String analyzer getter, tells you if the string is a keyword.
  *
  * @param word String to analyze.
  * @return true if it is a keyword.
  */
  bool isKeyword(const std::string& word) const;



/****************************  TAG SYSTEM LIST ********************************/

  /**
  * @brief Create a new tag as marking a new function, conditional or loop beginning.
  *
  * @param tag Sets if the new tag will be a conditional, loop or function.
  */
  void addTag(Tag tag);

  /**
  * @brief Will return the last tag added, and will delete it from the tag list.
  *
  * @return Last tag added to the list.
  */
  Tag getAndRemoveLastTag();


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
***                          PRIVATE ATTRIBUTES                              ***
*******************************************************************************/

  /// List of tags used to group different bodys of conditionals, loops or functions.
  std::vector<Tag> tag_list_;

}; /* Compiler */
}; /* JMP_PROJECT */

#endif /* __JMP_COMPILER_H__ */