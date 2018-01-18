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
  tag_list_.reserve(10);
}

TextParser::~TextParser() {
  current_token_.text.clear();
  sentence_.clear();
  tag_list_.clear();
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
  int32 id = token_manager.getHighestPriorityTokenIndex();
  Token token = token_manager.getToken(id);
  
  if (token.type == kTokenType_Keyword) {
    return compileKeywordToken(machine, token_manager, id);
  }

  // If theres any } or ( we will compile this separator first.
  if (token.priority == CLOSE_BRACKETS_PRIORITY) { // "}"
    return compileCloseBracketsSeparatorToken(machine, token_manager, id);
  }
  if (token.text == "(") { // We dont use priority becasue it changes to solve parenthethical grouping problems. 
    compileOpenParenthesisSeparatorToken(machine, token_manager, id);
  }

  // If theres any comma we will compile the content at both sides of it and will
  // step to the next line of code.
  if (checkIfAndCompileCommasContent(machine, token_manager)) {
    return kReport_NoErrors;
  }

  // Once checked that there arent commas, then we will compile the other separators.
  if (token.type == kTokenType_Separator && token.text != "(") {
    compileSeparatorToken(machine, token_manager, id);
  }

  // Compiling recursively what is not compiled yet.
  if (token_manager.numTokens() > 1) {
    return compileTokens(machine, token_manager);
  }

  return kReport_NoErrors;
}

Report TextParser::compileKeywordToken(Machine* machine, 
                                       TokenManager& token_manager, 
                                       int32& token_index) {
  Token token = token_manager.getToken(token_index);

  if (token.text == "if" || token.text == "else") {
    return compileConditionalKeywordToken(machine, token_manager, token_index);
  }
  if (token.text == "return") {
    return compileReturnKeywordToken(machine, token_manager, token_index);
  }
  if (token.text == "func") {
    return compileFunctionKeywordToken(machine, token_manager, token_index);
  }
  if (token.text == "do" || token.text == "while" || token.text == "for") {
    return compileLoopKeywordToken(machine, token_manager, token_index);
  }
  if (token.text == "var") {
    return compileVariableKeywordToken(machine, token_manager, token_index);
  }

  return kReport_UnexpectedKeyword;
}

Report TextParser::compileSeparatorToken(Machine* machine,
                                         TokenManager& token_manager,
                                         int32& token_index) {

  Token token = token_manager.getToken(token_index);
      
  switch (token.priority) {
    case ADDITION_OPERATION_PRIORITY: {
      compileAdditionOperationSeparatorToken(machine, token_manager, token_index);
    } break;
    case MULTIPLY_OPERATION_PRIORITY: {
      compileMultiplyOperationSeparatorToken(machine, token_manager, token_index);
    } break;
    case POWER_OPERATION_PRIORITY: {
      compilePowerOperationSeparatorToken(machine, token_manager, token_index);
    } break;
    case COMPARISON_PRIORITY: {
      compileComparisonOperationSeparatorToken(machine, token_manager, token_index);
    } break;
    case EQUAL_PRIORITY: {
      compileEqualSeparatorToken(machine, token_manager, token_index);
    } break;
    case DEFAULT_PRIORITY: {
      // Pushing variables, numbers and other stuff non compilable.
      machine->addCommand(kCommandType_PushToTheStack, token.text);
      token_manager.transferContentBetweenIDsInclusive(token_index, token_index);
      token_index--;
    } break;
  }

  return kReport_NoErrors;
}

Report TextParser::compileNumberToken(Machine* machine,
                                      TokenManager& token_manager,
                                      int32& token_index) {

  // TODO:
  return kReport_NoErrors;
}

Report TextParser::compileVariableToken(Machine* machine,
                                        TokenManager& token_manager,
                                        int32& token_index) {

  // TODO:
  return kReport_NoErrors;
}

const bool TextParser::checkIfAndCompileCommasContent(Machine * machine, 
                                                      TokenManager & token_manager) {

  if (token_manager.areAnyCommaTokenInList()) {
    int32 num_tokens = token_manager.numTokens();
    // Compile tokens with comas recursively.
    for (int32 i = 0; i < num_tokens; i++) {
      if (token_manager.getToken(i).text == ",") {
        TokenManager temp;
        // In case that we find any comma, we will take the previous content to compile it.
        // Example:   a b c d, e f     -> then we will transfer a b c d
        token_manager.transferContentBetweenIDsInclusive(0, i - 1, &temp);
        // Then we will delete the , and the RESULT token.
        token_manager.removeToken(1);
        token_manager.removeToken(0);
        // Once compiled, the previous content, we will compile the following one 
        compileTokens(machine, temp);
        compileTokens(machine, token_manager);
        // We will finish then the loop and we will end the compile of this line.
        return true;
      }
    }
    return true;
  }
  return false;
}

/*******************************************************************************
***                   SEPARATORS TOKEN COMPILER METHODS                      ***
*******************************************************************************/

Report TextParser::compileOpenParenthesisSeparatorToken(Machine* machine,
                                                        TokenManager& token_manager,
                                                        int32& token_index) {

  Report report = kReport_NoErrors;

  // First step will be to find its correspondant closing parenthesis.
  int32 close_parenthesis_id = token_manager.getNextCloseParenthesisIndex(token_index);
  if (close_parenthesis_id == -1) { 
    return kReport_NoMatchingCloseParenthesis; 
  }

  // We will extract the content inside the parenthesis in a new token manager, 
  // And paste it in a new token manager.
  TokenManager parenthesis_content;
  token_manager.transferParenthesisContent(token_index, close_parenthesis_id, &parenthesis_content);

  // Compile the content of the parenthesis.
  report = compileTokens(machine, parenthesis_content);

  // In case that its a function, then call it.
  token_index--;
  if (token_index >= 0) { // means that theres something behind the parenthesis.
    Token token = token_manager.getToken(token_index);
    if (token.type != kTokenType_Keyword &&
        token.type != kTokenType_Separator &&
        token.priority != 0) {      
      machine->addCommand(kCommandType_FunctionCall, token.text);
      token_manager.removeToken(token_index);
    }
  }


  return report;
}

Report TextParser::compileCloseBracketsSeparatorToken(Machine* machine, 
                                                      TokenManager& token_manager, 
                                                      int32& token_index) {
  
  // Will untag the end of a loop, conditional or function.
  Tag tag = getAndRemoveLastTag();
  switch (tag) {
    case JMP::kTag_None:{ } break;
    case JMP::kTag_Loop: { machine->addCommand(kCommandType_Finished, "loop"); } break;
    case JMP::kTag_Conditional: { machine->addCommand(kCommandType_Finished, "conditional"); } break;
    case JMP::kTag_Function:{ } break;
  } 
  return kReport_NoErrors;
}

Report TextParser::compileAdditionOperationSeparatorToken(Machine* machine,
                                                          TokenManager& token_manager, 
                                                          int32& token_index) {
  Token operator_token = token_manager.getToken(token_index);
  Token right_operand = token_manager.getToken(token_index + 1);
  Token left_operand = token_manager.getToken(token_index - 1);

  // Push the operands to the stack
  machine->addCommand(kCommandType_PushToTheStack, left_operand.text);
  machine->addCommand(kCommandType_PushToTheStack, right_operand.text);

  // Then the next command will set the action to be applied to the previous operands.
  if (operator_token.text == "-") {
    machine->addCommand(kCommandType_Substraction);
  }
  else {
    machine->addCommand(kCommandType_Addition);
  }

  // Deleting the tokens from the list and adding a "RESULT" Temporary one.
  token_manager.transferContentBetweenIDsInclusive(token_index - 1, token_index + 1);
  // After deleting from the list the three elements, we step back two positions.
  token_index -= 2; 

  return kReport_NoErrors;
}

Report TextParser::compileMultiplyOperationSeparatorToken(Machine* machine,
                                                          TokenManager& token_manager,
                                                          int32& token_index) {

  Token operator_token = token_manager.getToken(token_index);
  Token right_operand = token_manager.getToken(token_index + 1);
  Token left_operand = token_manager.getToken(token_index - 1);

  // Push the operands to the stack
  machine->addCommand(kCommandType_PushToTheStack, left_operand.text);
  machine->addCommand(kCommandType_PushToTheStack, right_operand.text);

  // Then the next command will set the action to be applied to the previous operands.
  if (operator_token.text == "/") {
    machine->addCommand(kCommandType_Division);
  }
  else {
    machine->addCommand(kCommandType_Multiply);
  }

  // Deleting the tokens from the list and adding a "RESULT" Temporary one.
  token_manager.transferContentBetweenIDsInclusive(token_index - 1, token_index + 1);
  // After deleting from the list the three elements, we step back two positions.
  token_index -= 2;

  return kReport_NoErrors;
}

Report TextParser::compilePowerOperationSeparatorToken(Machine* machine,
                                                       TokenManager& token_manager,
                                                       int32& token_index) {

  Token operator_token = token_manager.getToken(token_index);
  Token right_operand = token_manager.getToken(token_index + 1);
  Token left_operand = token_manager.getToken(token_index - 1);

  // Push the operands to the stack
  machine->addCommand(kCommandType_PushToTheStack, left_operand.text);
  machine->addCommand(kCommandType_PushToTheStack, right_operand.text);

  // Then the next command will set the action to be applied to the previous operands
  machine->addCommand(kCommandType_Power);

  // Deleting the tokens from the list and adding a "RESULT" Temporary one.
  token_manager.transferContentBetweenIDsInclusive(token_index - 1, token_index + 1);
  // After deleting from the list the three elements, we step back two positions.
  token_index -= 2;

  return kReport_NoErrors;
}

Report TextParser::compileComparisonOperationSeparatorToken(Machine* machine,
                                                            TokenManager& token_manager,
                                                            int32& token_index) {

  Token operator_token = token_manager.getToken(token_index);
  Token right_operand = token_manager.getToken(token_index + 1);
  Token left_operand = token_manager.getToken(token_index - 1);

  // Push the operands to the stack
  machine->addCommand(kCommandType_PushToTheStack, left_operand.text);
  machine->addCommand(kCommandType_PushToTheStack, right_operand.text);

  // Then the next command will set the action to be applied to the previous operands.
  if (operator_token.text == "<") {
    machine->addCommand(kCommandType_LowerThanComparison);
  }
  else {
    machine->addCommand(kCommandType_GreaterThanComparison);
  }

  // Deleting the tokens from the list and adding a "RESULT" Temporary one.
  token_manager.transferContentBetweenIDsInclusive(token_index - 1, token_index + 1);
  // After deleting from the list the three elements, we step back two positions.
  token_index -= 2;

  return kReport_NoErrors;
}

Report TextParser::compileEqualSeparatorToken(Machine* machine,
                                              TokenManager& token_manager,
                                              int32& token_index) {



  // TODO:
  return kReport_NoErrors;
}

/*******************************************************************************
***                    KEYWORDS TOKEN COMPILER METHODS                       ***
*******************************************************************************/

Report TextParser::compileConditionalKeywordToken(Machine* machine,
                                                  TokenManager& token_manager,
                                                  int32& token_index) {

  if (token_manager.getToken(token_index).text == "if") {
    // removing the "if" keyword.
    token_manager.removeToken(token_index);
    // Checks if the last token of the line sentence is a "{" to start the body.
    if (token_manager.getToken(token_manager.numTokens() - 1).text != "{") {
      return kReport_ExpectingOpenBrackets;
    }
    // Removing "{"
    token_manager.removeToken(token_manager.numTokens() - 1);
    addTag(kTag_Conditional);
    // Compiling the condition.
    Report report = compileTokens(machine, token_manager);
    machine->addCommand(kCommandType_ConditionToEvaluate);
    return report; 
  }
  
  ReportError("\"else\" keyword functionality not programmed yet in this version");
  return kReport_KeywordFunctionalityNotProgrammedYet;
}

Report TextParser::compileReturnKeywordToken(Machine* machine,
                                             TokenManager& token_manager,
                                             int32& token_index) {

  Report report = kReport_NoErrors;

  if (token_index != 0) {
    return kReport_ReturnShouldBeTheFirstToken;
  }

  if (token_manager.numTokens() > 1) {
    TokenManager temp; // use to save the content after return.
    token_manager.transferContentBetweenIDsInclusive(token_index + 1, token_manager.numTokens() - 1, &temp);
    // Compiling the right of return
    report = compileTokens(machine, temp);
    // After it the token_manager should be like: return "RESULT"
  }
  machine->addCommand(kCommandType_FunctionReturn);
  return report;
}

Report TextParser::compileFunctionKeywordToken(Machine* machine,
                                               TokenManager& token_manager,
                                               int32& token_index) {



  // TODO:
  return kReport_NoErrors;
}

Report TextParser::compileLoopKeywordToken(Machine* machine,
                                           TokenManager& token_manager,
                                           int32& token_index) {

  if (token_index != 0) {
    return kReport_LoopKeywordShouldBeTheFirstToken;
  }

  if (token_manager.getToken(token_index).text == "while") {
    machine->addCommand(kCommandType_Loop);
    // removing the loop ("while") keyword.
    token_manager.removeToken(token_index);

    // Checks if the last token of the line sentence is a "{" to start the body.
    if (token_manager.getToken(token_manager.numTokens() - 1).text != "{") {
      return kReport_ExpectingOpenBrackets;
    }
    // Removing "{"
    token_manager.removeToken(token_manager.numTokens() - 1);
    addTag(kTag_Loop);
    // Compiling the condition sentence.
    Report report = compileTokens(machine, token_manager);
    // Once compiled, we assign a command to evaluate the condition.
    machine->addCommand(kCommandType_ConditionToEvaluate);
    return report;
  }

  ReportError("\"do\" and \"for\" keywords functionality not programmed yet in this version");
  return kReport_KeywordFunctionalityNotProgrammedYet;
}

Report TextParser::compileVariableKeywordToken(Machine* machine,
                                               TokenManager& token_manager,
                                               int32& token_index) {

  // Gets the variable name.
  Token var_name = token_manager.getToken(token_index + 1);
  if (var_name.type != kTokenType_Variable) {
    return kReport_ExpectingNameOfVariable;
  }
  // Delete de "var" keyword   
  token_manager.removeToken(token_index);  // "var"
  // Adds a command who defines the variable name.
  machine->addCommand(kCommandType_VariableDefinition, var_name.text);
  // Compile the rest of the line once the variable name is defined.
  return compileTokens(machine, token_manager);
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
  else if (isDigit(sentence_[sentence_index_])) {
    while (!isSeparator(sentence_[sentence_index_]) && sentence_index_ < sentence_length) {
      current_token_.text.push_back(sentence_[sentence_index_]);
      sentence_index_++;
    }
    current_token_.type = kTokenType_Number;
  }

  // Vars and Keywords
  else if (isLetter(sentence_[sentence_index_])) {
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
      if (separator == "}") { current_token_.priority = CLOSE_BRACKETS_PRIORITY; }
      if (separator == "(") { current_token_.priority = OPEN_PARENTHESIS_PRIORITY; }
      if (separator == "^") { current_token_.priority = POWER_OPERATION_PRIORITY; }
      if (separator == "*" || separator == "/") { current_token_.priority = MULTIPLY_OPERATION_PRIORITY; }
      if (separator == "+" || separator == "-") { current_token_.priority = ADDITION_OPERATION_PRIORITY; }
      if (separator == ">" || separator == "<") { current_token_.priority = COMPARISON_PRIORITY; }
      if (separator == "=") { current_token_.priority = EQUAL_PRIORITY; }
      if (separator == ",") { current_token_.priority = COMMA_PRIORITY; }
    }break;
    default: { current_token_.priority = DEFAULT_PRIORITY; } break;
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



/*******************************************************************************
***                        TAG SYSTEM LIST METHODS                           ***
*******************************************************************************/

void TextParser::addTag(const Tag tag) {
  tag_list_.push_back(tag);
}

const Tag TextParser::getAndRemoveLastTag() {
  int32 num_tags = tag_list_.size();
  if (num_tags <= 0) {
    Report report = kReport_NoTagsToDelete;
    PrintReport(report);
    return kTag_None;
  }
  Tag tag = tag_list_[num_tags - 1];
  tag_list_.pop_back();
  return tag;
}






}; /* JMP */