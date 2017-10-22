/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#ifndef __JMP_TOKENMANAGER_H__
#define __JMP_TOKENMANAGER_H__ 1

#include "types.h"
#include <vector>

namespace JMP {

/// Token type.
enum TokenType {
  kTokenType_None = 0,
  kTokenType_Keyword,
  kTokenType_Separator,
  kTokenType_Number,
  kTokenType_Variable,
};


/// Allocates the basic info of each token.
struct Token {
  /// Token characters.
  std::string text;
  /// Token type.
  TokenType type;
};

/// Class used to save and manage the tokens.
class TokenManager {

 public: 

/*******************************************************************************          
***                             PUBLIC METHODS                               ***
*******************************************************************************/

  /// Default class constructor.
  TokenManager();
  /// Default class destructor.
  ~TokenManager();

/*****************************  LIST METHODS  *********************************/

  /**
  * @brief Adds a new token to the list.
  *
  * @param text Text of the token.
  * @param type Type of the token.
  */
  void addToken(const char* text, const TokenType type = kTokenType_None);

  /**
  * @brief Adds a new token to the list.
  *
  * @param token Token to add.
  */
  void addToken(const Token& token);

  /// Clears the token list.
  void clear();

  /**
  * @brief Removes a token from the list.
  *
  * @param id Index of the element in the token list.
  */
  void removeToken(int32 id);


/*******************************  OPERATORS  **********************************/



/********************************  GETTERS  ***********************************/

  /**
  * @brief Gets a token from the token manager list.
  *
  * @param list_index Index of the token in the list.
  * @return Token from the list.
  */
  Token getToken(const uint32 list_index);

  /**
  * @brief Getter of the number of tokens allocated in the token manager list.
  *
  * @return Token list length.
  */
  const uint32 numTokens();


/*******************************  DEBUGGING  **********************************/
  
  /**
  * @brief Print the info of a token.
  *
  * Format: [TYPE] - "[TokenText]"
  * @param id Index of the element in the token list.
  */
  void printToken(int32 id);
  /// Print all the tokens from the list.
  void printTokenList();


/*******************************************************************************
***                           PUBLIC ATTRIBUTES                              ***
*******************************************************************************/




 private:

/*******************************************************************************
***                            PRIVATE METHODS                               ***
*******************************************************************************/


  /// Default copy constructor.
  TokenManager(const TokenManager& copy);
  /// Assignment operator.
  TokenManager& operator=(const TokenManager& copy);

/*******************************************************************************
***                          PRIVATE ATTRIBUTES                              ***
*******************************************************************************/

  /// List of all the tokens.
  std::vector<Token> token_list_;
  /// Number of elements of the list.
  int32 token_list_length_;

}; /* TokenManager */
}; /* JMP */

#endif /* __JMP_TOKENMANAGER_H__ */