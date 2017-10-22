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

TextParser::TextParser() {}

TextParser::~TextParser() {}

void TextParser::generateTokens(std::string& sentence, TokenManager& token_manager) {
  
  replaceSpacesFromQuotes(sentence, '_'); // Replace ' ' ==> '_'

  sentence_index_ = 0;
  sentence_ = sentence;

  generateNextToken();
}

Token TextParser::generateNextToken() {

  uint32 sentence_length = sentence_.length();

  // Restarts the current token.
  current_token.text.clear();
  current_token.type = TokenType_None;

  // To analyze sentences, spaces will be ignored.
  while (sentence_index_ < sentence_length && sentence_[sentence_index_] == ' ') {
    sentence_index_++;
  }

  // Checking end of line.
  if (sentence_index_ >= sentence_length) {
    current_token.text = "\0";
    return current_token;
  }


  // TOKEN ANALYZE: We will

  return current_token;
}

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




}; /* JMP */