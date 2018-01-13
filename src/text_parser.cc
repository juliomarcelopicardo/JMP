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
***                          PARSER CORE METHODS                             ***
*******************************************************************************/

Report TextParser::compile(Machine* machine, 
                           std::string sentence, 
                           uint32 line_number) {

  // Error checkings.
  if (!machine) {
    ReportError("Machine Null Pointer, can't compile.");
    return kReport_NullPointer;
  }

  // Allocates all the tokens in the manager.
  TokenManager token_manager;
  generateTokens(sentence, token_manager); 
  token_manager.printTokenList();

  // Compile all these tokens.
  Report report = compileTokens(machine, token_manager);

  PrintReport(report, line_number);
  return report;
}


/*******************************************************************************
***                        TOKEN COMPILER METHODS                            ***
*******************************************************************************/

Report TextParser::compileTokens(Machine* machine, TokenManager& token_manager) {
  
  // Error checkings.
  if (!machine) {
    ReportError("Machine Null Pointer, can't compile.");
    return kReport_NullPointer;
  }

  if (token_manager.numTokens() == 0) { return kReport_EmptyLine; }

  // Once we checked the basic errors, lets start with the highest priority token.
  int32 highest_priority_id = token_manager.getHighestPriorityToken();

  Token token = token_manager.getToken(highest_priority_id);

  switch (token.type) {
    case JMP::kTokenType_None: { return kReport_NoTokensToCompile; } break;
    case JMP::kTokenType_Keyword: { } break;
    case JMP::kTokenType_Separator: { } break;
    case JMP::kTokenType_Number: { } break;
    case JMP::kTokenType_Variable: { } break;
    default: { } break;
  }
  return kReport_NoErrors;
}

Report TextParser::compileKeywordToken(Machine * machine, 
                                       TokenManager & token_manager, 
                                       int32 token_index) {
  // TODO:

  return kReport_NoErrors;
}

Report TextParser::compileSeparatorToken(Machine * machine,
                                         TokenManager & token_manager,
                                         int32 token_index) {

  // TODO:
  return kReport_NoErrors;
}

Report TextParser::compileNumberToken(Machine * machine,
                                      TokenManager & token_manager,
                                      int32 token_index) {

  // TODO:
  return kReport_NoErrors;
}

Report TextParser::compileVariableToken(Machine * machine,
                                        TokenManager & token_manager,
                                        int32 token_index) {

  // TODO:
  return kReport_NoErrors;
}


/*******************************************************************************
***                           TOKEN GENERATION                               ***
*******************************************************************************/


void TextParser::generateTokens(std::string& sentence, TokenManager& token_manager) {
  
  int32 num_parenthesis_opened = 0;

  replaceSpacesFromQuotes(sentence, '_'); // Replace ' ' ==> '_'

  sentence_index_ = 0;
  sentence_ = sentence;

  generateNextToken(); // Setting the first token as current.

  while(current_token_.text != "") {

    // Everytime a parenthesis is opened, we increase the priority.
    // This way we will avoid the problem with multiple parenthetical groups.
    if (current_token_.text == "(") {
      current_token_.priority += num_parenthesis_opened;
      num_parenthesis_opened++;
    }

    recoverSpacesFromQuotes(current_token_.text); // Replace '_' ==> ' ' in quotes
    token_manager.addToken(current_token_);
    generateNextToken();
  }
  
}

void TextParser::generateNextToken() {

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
  }

  // TOKEN ANALYZE: We will get the token type and the token text.

  // Separators
  if (isSeparator(sentence_[sentence_index_])) {
    current_token_.text.push_back(sentence_[sentence_index_]);
    current_token_.type = kTokenType_Separator;
    sentence_index_++;
  }

  // Numbers
  if (isDigit(sentence_[sentence_index_])) {
    while (!isSeparator(sentence_[sentence_index_]) && sentence_index_ < sentence_length) {
      current_token_.text.push_back(sentence_[sentence_index_]);
      sentence_index_++;
    }
    current_token_.type = kTokenType_Number;
  }

  // Vars and Keywords
  if (isLetter(sentence_[sentence_index_])) {
    while (!isSeparator(sentence_[sentence_index_]) && sentence_index_ < sentence_length) {
      current_token_.text.push_back(sentence_[sentence_index_]);
      sentence_index_++;
    }

    current_token_.type = kTokenType_Variable;
    if (isKeyword(current_token_.text)) { current_token_.type = kTokenType_Keyword; }
  }

  // Once the type is set, we will assign an initial priority depending on the type.
  generateCurrentTokenInitialPriority();
}

void TextParser::generateCurrentTokenInitialPriority() {
  switch (current_token_.type) {
    case JMP::kTokenType_Keyword: { current_token_.priority = 500; } break;
    case JMP::kTokenType_Separator: {
      std::string separator = current_token_.text;
      if (separator == "}") { current_token_.priority = 200; }
      if (separator == "(") { current_token_.priority = 100; }
      if (separator == "^") { current_token_.priority = 99; }
      if (separator == "*" || separator == "/") { current_token_.priority = 98; }
      if (separator == "+" || separator == "-") { current_token_.priority = 97; }
      if (separator == ">" || separator == "<") { current_token_.priority = 96; }
      if (separator == "=") { current_token_.priority = 95; }
      if (separator == ",") { current_token_.priority = -10; }
    }break;
    default: { current_token_.priority = 0; } break;
  }
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

void TextParser::recoverSpacesFromQuotes(std::string & sentence, char8 replacement) {
  uint32 length = sentence.length();

  if (sentence[0] == '"') {	
    for (uint32 i = 0; i < length; ++i) {
      if (sentence[i] == replacement) { sentence[i] = ' '; }
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