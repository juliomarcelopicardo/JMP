/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP_PROJECT - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#include "token_manager.h"
#include "report.h"
#include "compiler.h"
#include <Windows.h>
#include <string>

namespace JMP {

/*******************************************************************************
***                       CONSTRUCTOR & DESTRUCTOR                           ***
*******************************************************************************/

TokenManager::TokenManager() {
  token_list_length_ = 0;
  token_list_.reserve(10);
}

TokenManager::~TokenManager() {
  token_list_.clear();
}

/*******************************************************************************
***                              LIST METHODS                                ***
*******************************************************************************/

void TokenManager::addToken(const char * text, const TokenType type, int32 priority) {
  token_list_.emplace_back(text, type, priority);
  token_list_length_++;

  // To determine if there's a function name behind the token.
  if (token_list_length_ >= 2) {
    if (token_list_[token_list_length_ - 1].text_ == "(" &&
      token_list_[token_list_length_ - 2].type_ == kTokenType_Variable) {
      token_list_[token_list_length_ - 2].priority_ = FUNCTION_NAME_PRIORITY;
    }
  }
}

void TokenManager::addToken(const Token& token) {
  token_list_.push_back(token);
  token_list_length_++;

  // To determine if there's a function name behind the token.
  if (token_list_length_ >= 2) {
    if (token_list_[token_list_length_ - 1].text_ == "(" &&
      token_list_[token_list_length_ - 2].type_ == kTokenType_Variable) {
      token_list_[token_list_length_ - 2].priority_ = FUNCTION_NAME_PRIORITY;
    }
  }
}

void TokenManager::clear() {
  token_list_.clear();
  token_list_length_ = 0;
}

void TokenManager::removeToken(int32 id) {
  token_list_.erase(token_list_.begin() + id);
  token_list_length_--;
}

void TokenManager::transferParenthesisContent(int32 open_parenthesis_id, 
                                              int32 close_parenthesis_id, 
                                              TokenManager* transfer_output) {
  // Copying the content into the output.
  int32 num_tokens_transfered = 0;
  for (int32 i = open_parenthesis_id + 1; i < close_parenthesis_id; ++i) {
    transfer_output->addToken(token_list_[i]);
    num_tokens_transfered++;
  }

  // Changing the open parenthesis token to text "RESULT", None type, priority 0.
  token_list_[open_parenthesis_id] = { "RESULT", kTokenType_None, 0 };
  
  // Deleting the rest of the tokens. including the ")" one.
  for (int32 i = 0; i < num_tokens_transfered + 1; ++i) {
    removeToken(open_parenthesis_id + 1);
  }
}

void TokenManager::transferContentBetweenIDsInclusive(int32 initial_id, 
                                                      int32 final_id, 
                                                      TokenManager* transfer_output) {
  // Copying the content into the output.
  int32 num_tokens_transfered = 0;
  for (int32 i = initial_id; i <= final_id; ++i) {
    if (transfer_output) { transfer_output->addToken(token_list_[i]); }
    num_tokens_transfered++;
  }

  // Changing the open parenthesis token to text "RESULT", None type, priority 0.
  token_list_[initial_id] = { "RESULT", kTokenType_None, 0 };

  // Deleting the rest of the tokens. including the ")" one.
  for (int32 i = 0; i < num_tokens_transfered - 1; ++i) {
    removeToken(initial_id + 1);
  }
}

/*******************************************************************************
***                                GETTERS                                   ***
*******************************************************************************/




Token TokenManager::getToken(const int32 list_index) {
  if (list_index >= token_list_length_ || list_index < 0) {
    ReportError("Token list index \"" + std::to_string(list_index) + "\" is out of range.");
    return{ "", kTokenType_None, 0 };
  }
  return token_list_[list_index];
}

int32 TokenManager::getHighestPriorityTokenIndex() {

  int32 maximum_priority = -1;
  int32 index = -1;

  for (int32 i = 0; i < token_list_length_; ++i) {
    if (token_list_[i].priority_ > maximum_priority) {
      maximum_priority = token_list_[i].priority_;
      index = i;
    }
  }

  return index;
}



int32 TokenManager::getNextCloseParenthesisIndex(int32 open_parenthesis_index) {
  
  for (int32 i = open_parenthesis_index; i < token_list_length_; ++i) {
    if (token_list_[i].text_ == ")") {
      return i;
    }
  }

  return -1; // No maatching close parenthesis found.
}

  uint32 TokenManager::numTokens() const
  {
  return token_list_length_;
}

  bool TokenManager::areAnyCommaTokenInList() {
  for (int32 i = 0; i < token_list_length_; ++i) {
    if (token_list_[i].text_ == ",") {
      return true;
    }
  }
  return false;
}



/*******************************************************************************
***                               DEBUGGING                                  ***
*******************************************************************************/

void TokenManager::printToken(int32 id) {

  OutputDebugString(" Type: ");
  switch (token_list_[id].type_) {
    case JMP::kTokenType_None: { OutputDebugString("None \t\t"); }break;
    case JMP::kTokenType_Keyword: { OutputDebugString("Keyword \t\t"); }break;
    case JMP::kTokenType_Number: { OutputDebugString("Number \t\t"); }break;
    case JMP::kTokenType_Separator: { OutputDebugString("Separator \t"); }break;
    case JMP::kTokenType_Variable: { OutputDebugString("Variable \t"); }break;
    default: { OutputDebugString("None \t\t"); }break;
  }

  char priority[32] = "";
  if (token_list_[id].priority_ == 0) {
    sprintf_s(priority, 32, "Priority: %d\t\t", token_list_[id].priority_);
  }
  else {
    sprintf_s(priority, 32, "Priority: %d\t", token_list_[id].priority_);
  }
  OutputDebugString(priority);
  
  OutputDebugString("Token: \"");
  OutputDebugString(token_list_[id].text_.c_str());
  OutputDebugString("\"\n");

}

void TokenManager::printTokenList() {
  for (int32 i = 0; i < token_list_length_; ++i) {
    printToken(i);
  }
}




}; /* JMP_PROJECT */