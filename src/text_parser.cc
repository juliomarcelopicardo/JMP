/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#include "text_parser.h"

namespace JMP {

/*******************************************************************************
***                       CONSTRUCTOR & DESTRUCTOR                           ***
*******************************************************************************/

TextParser::TextParser() {
  current_token_.text.clear();
  current_token_.type = kTokenType_None;
  sentence_.clear();
  sentence_index_ = 0;
}

TextParser::~TextParser() {
  current_token_.text.clear();
  sentence_.clear();
}

/*******************************************************************************
***                           TOKEN GENERATION                               ***
*******************************************************************************/

void TextParser::generateTokens(std::string& sentence, TokenManager& token_manager) {
  
  replaceSpacesFromQuotes(sentence, '_'); // Replace ' ' ==> '_'

  sentence_index_ = 0;
  sentence_ = sentence;

  // TESTING
  generateNextToken();

  while(current_token_.text != "") {
    token_manager.addToken(current_token_);
    generateNextToken();
  }
  
}

Token TextParser::generateNextToken() {

  uint32 sentence_length = sentence_.length();

  // Restarts the current token.
  current_token_.text = "";
  current_token_.type = kTokenType_None;

  // To analyze sentences, spaces will be ignored.
  while (sentence_index_ < sentence_length && 
         isBlankSpace(sentence_[sentence_index_])) {
    sentence_index_++;
  }

  // Checking end of line.
  if (sentence_index_ >= sentence_length) {
    current_token_.text = "\0";
    return current_token_;
  }

  // TOKEN ANALYZE: We will get the token type and the token text.
  
  // Separators
  if (isSeparator(sentence_[sentence_index_])) {
    current_token_.text.push_back(sentence_[sentence_index_]);
    current_token_.type = kTokenType_Separator;
    sentence_index_++;
    return current_token_;
  }

  // Numbers
  if (isDigit(sentence_[sentence_index_])) {
    while (!isSeparator(sentence_[sentence_index_]) && sentence_index_ < sentence_length) {
      current_token_.text.push_back(sentence_[sentence_index_]);
      sentence_index_++;
    }
    current_token_.type = kTokenType_Number;
    return current_token_;
  }

  // Vars and Keywords
  if (isLetter(sentence_[sentence_index_])) {
    while (!isSeparator(sentence_[sentence_index_]) && sentence_index_ < sentence_length) {
      current_token_.text.push_back(sentence_[sentence_index_]);
      sentence_index_++;
    }

    current_token_.type = kTokenType_Variable;
    if (isKeyword(current_token_.text)) { current_token_.type = kTokenType_Keyword; }
    return current_token_;
  }

  return current_token_;
}


/*******************************************************************************
***                            TEXT MODIFIERS                                ***
*******************************************************************************/

void TextParser::replaceSpacesFromQuotes(std::string& sentence, char8 replacement) {

  uint32 length = sentence.length();
  
  for (uint32 i = 0; i < length; ++i) {
    if (sentence[i] == '"') { // Begin of the quote.
      ++i;
      for (uint32 j = i; j < length; ++j) {
        ++i;
        if (sentence[j] == '"') { break; } // End of the quote.
        if (sentence[j] == ' ') { sentence[j] = replacement; }
      }
    }
  }
  
}


/*******************************************************************************
***                        TEXT ANALYZING GETTERS                            ***
*******************************************************************************/

const bool TextParser::isSeparator(const char8& character) {
  switch (character) {
    case ' ': case ',': case ';': case '?': case '!': case '^':
    case '+': case '-': case '*': case '/': case '=': case '%':
    case '(': case ')': case '{': case '}': case '<': case '>':
    return true;
  }
  return false;
}

const bool TextParser::isBlankSpace(const char8& character) {
  if (character == ' ') { 
    return true; 
  }
  return false;
}

const bool TextParser::isLetter(const char8& character) { 
  if ((character >= 'a' && character <= 'z') || 
      (character >= 'A' && character <= 'Z') || 
      (character == '"') || (character == '_')) { // needed for quotes.
    return true;
  }
  return false;
}

const bool TextParser::isDigit(const char8& character) {
  switch (character) {
    case '0': case '1': case '2': case '3': case '4':  
    case '5': case '6': case '7': case '8': case '9':
    return true;
  }
  return false;
}

const bool TextParser::isKeyword(const std::string& word) {

  if (word == "if" || word == "else" ||                   // Conditionals
      word == "func" || word == "return" ||               // Functions
      word == "do" || word == "while" || word == "for" || // Loops
      word == "var") {                                    // Variable
    return true;
  }
  return false;
}

}; /* JMP */