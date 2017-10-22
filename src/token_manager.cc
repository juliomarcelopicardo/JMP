/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#include "token_manager.h"
#include "report.h"
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

void TokenManager::addToken(const char * text, const TokenType type) {
  token_list_.push_back({ text, type });
  token_list_length_++;
}

void TokenManager::addToken(const Token& token) {
  token_list_.push_back(token);
  token_list_length_++;
}

void TokenManager::clear() {
  token_list_.clear();
  token_list_length_ = 0;
}

void TokenManager::removeToken(int32 id) {
  token_list_.erase(token_list_.begin() + id);
  token_list_length_--;
}


/*******************************************************************************
***                                GETTERS                                   ***
*******************************************************************************/

Token TokenManager::getToken(const uint32 list_index) {
  if (list_index >= token_list_length_) {
    ReportError("Token list index out of range.");
    return{ "", kTokenType_None };
  }
  return token_list_[list_index];
}

const uint32 TokenManager::numTokens() {
  return token_list_length_;
}



/*******************************************************************************
***                               DEBUGGING                                  ***
*******************************************************************************/

void TokenManager::printToken(int32 id) {

  OutputDebugString(" Type: ");
  switch (token_list_[id].type) {
    case JMP::kTokenType_None: { OutputDebugString("None \t\t"); }break;
    case JMP::kTokenType_Keyword: { OutputDebugString("Keyword \t\t"); }break;
    case JMP::kTokenType_Number: { OutputDebugString("Number \t\t"); }break;
    case JMP::kTokenType_Separator: { OutputDebugString("Separator \t"); }break;
    case JMP::kTokenType_Variable: { OutputDebugString("Variable \t"); }break;
    default: { OutputDebugString("None \t\t"); }break;
  }
  OutputDebugString("Token: \"");
  OutputDebugString(token_list_[id].text.c_str());
  OutputDebugString("\"\n");
}

void TokenManager::printTokenList() {
  for (int32 i = 0; i < token_list_length_; i++) {
    printToken(i);
  }
}




}; /* JMP */