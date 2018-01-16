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
  /// Token priority
  int32 priority;
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
  * @param priority Priority of the token, necessary for compiling in order.
  */
  void addToken(const char* text, const TokenType type = kTokenType_None, int32 priority = 0);

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

  /**
  * @brief Tranfers the content of a parenthesis to a new token manager.
  *
  * Transfer means that content will be cut & pasted to the new one and the 
  * parenthesis tokens from the original will be deleted.
  * In the original, everything once deleted is replaced by a unique token "RESULT"
  *
  * EXAMPLE: Original content ->        ( a / b )
  * RESULT:  Output content->             a / b
  *          Original content ->       1token: "RESULT" type "None" Priority "0"
  *
  * @param open_parenthesis_id Initial id.
  * @param close_parenthesis_id Final id.
  * @param transfer_output where to transfer the content.
  */
  void transferParenthesisContent(int32 open_parenthesis_id,
                                  int32 close_parenthesis_id,
                                  TokenManager* transfer_output);

  /**
  * @brief Tranfers the content between two IDs, inclusive these ids.
  *
  * Transfer means that content will be cut & pasted to the new one and the
  * tokens from the original will be deleted.
  * In the original, everything once deleted is replaced by a unique token "RESULT"
  *
  * EXAMPLE: Original content ->        ( a / b )
  * RESULT:  Output content->           ( a / b )
  *          Original content ->       1token: "RESULT" type "None" Priority "0"
  *
  * @param initial_id Initial id.
  * @param final_id Final id.
  * @param transfer_output where to transfer the content. if Null wont transfer.
  */
  void transferContentBetweenIDsInclusive(int32 initial_id,
                                          int32 final_id,
                                          TokenManager* transfer_output = nullptr);


/*******************************  OPERATORS  **********************************/


/********************************  GETTERS  ***********************************/

  /**
  * @brief Gets a token from the token manager list.
  *
  * @param list_index Index of the token in the list.
  * @return Token from the list.
  */
  Token getToken(const int32 list_index);

  /**
  * @brief Gets the index with the highest priority token.
  *
  * @return highest priority token index. -1 if theres no tokens.
  */
  int32 getHighestPriorityTokenIndex();

  /**
  * @brief Gets the index with the matching close parenthesis.
  *
  * @param open_parenthesis_index Open parenthesis index to find its mathcing close one.
  * @return Next matching close parenthesis index. -1 if theres no tokens.
  */
  int32 getNextCloseParenthesisIndex(int32 open_parenthesis_index);

  /**
  * @brief Getter of the number of tokens allocated in the token manager list.
  *
  * @return Token list length.
  */
  const uint32 numTokens();

  /**
  * @brief Gets if theres any comma as token in the list.
  *
  * @return True if any comma exists.
  */
  const bool areAnyCommaTokenInList();


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