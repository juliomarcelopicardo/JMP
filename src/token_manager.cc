/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#include "token_manager.h"
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




/*******************************************************************************
***                               DEBUGGING                                  ***
*******************************************************************************/

void TokenManager::printToken(int32 id) {

  OutputDebugString(" Type: ");
  switch (token_list_[id].type) {
    case JMP::TokenType_None: { OutputDebugString("None \t"); }break;
    case JMP::TokenType_Keyword: { OutputDebugString("Keyword \t"); }break;
    case JMP::TokenType_Number: { OutputDebugString("Number \t"); }break;
    case JMP::TokenType_text: { OutputDebugString("Text \t"); }break;
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