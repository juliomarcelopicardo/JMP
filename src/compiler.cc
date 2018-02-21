/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP_PROJECT - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#include "compiler.h"

namespace JMP {

/*******************************************************************************
***                       CONSTRUCTOR & DESTRUCTOR                           ***
*******************************************************************************/

Compiler::Compiler() {
  current_token_.text_.clear();
  current_token_.type_ = kTokenType_None;
  sentence_.clear();
  sentence_index_ = 0;
  tag_list_.reserve(10);
}

Compiler::~Compiler() {
  current_token_.text_.clear();
  sentence_.clear();
  tag_list_.clear();
}

/*******************************************************************************
***                          PARSER CORE METHODS                             ***
*******************************************************************************/

Report Compiler::compile(Machine* machine, 
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
  //token_manager.printTokenList();

  // Compile all these tokens.
  Report report = compileTokens(machine, token_manager);

  PrintReport(report, line_number);
  return report;
}


/*******************************************************************************
***                        TOKEN COMPILER METHODS                            ***
*******************************************************************************/

Report Compiler::compileTokens(Machine* machine, TokenManager& token_manager) {
  
  // Error checkings.
  if (!machine) {
    ReportError("Machine Null Pointer, can't compile.");
    return kReport_NullPointer;
  }

  if (token_manager.numTokens() == 0) { return kReport_EmptyLine; }

  // Once we checked the basic errors, lets start with the highest priority token.
  int32 id = token_manager.getHighestPriorityTokenIndex();
  const Token token = token_manager.getToken(id);
  
  if (token.type_ == kTokenType_Keyword) {
    return compileKeywordToken(machine, token_manager, id);
  }

  // If theres any } or ( we will compile this separator first.
  if (token.priority_ == CLOSE_BRACKETS_PRIORITY) { // "}"
    return compileCloseBracketsSeparatorToken(machine, token_manager, id);
  }
  if (token.text_ == "(") { // We dont use priority becasue it changes to solve parenthethical grouping problems. 
    compileOpenParenthesisSeparatorToken(machine, token_manager, id);
  }

  // If theres any comma we will compile the content at both sides of it and will
  // step to the next line of code.
  if (checkIfAndCompileCommasContent(machine, token_manager)) {
    return kReport_NoErrors;
  }

  // Once checked that there arent commas, then we will compile the other separators.
  if (token.type_ == kTokenType_Separator && token.text_ != "(") {
    compileSeparatorToken(machine, token_manager, id);
  }

  if (token.priority_ == DEFAULT_PRIORITY) {
    // Pushing variables, numbers and other stuff non compilable.
    machine->addCommand(kCommandType_PushToTheStack, token.text_);
    token_manager.transferContentBetweenIDsInclusive(id, id);
  }

  // Compiling recursively what is not compiled yet.
  if (token_manager.numTokens() > 1) {
    return compileTokens(machine, token_manager);
  }

  return kReport_NoErrors;
}

Report Compiler::compileKeywordToken(Machine* machine, 
                                     TokenManager& token_manager, 
                                     int32& token_index) {
  Token token = token_manager.getToken(token_index);

  if (token.text_ == "IF" || token.text_ == "ELSE") {
    return compileConditionalKeywordToken(machine, token_manager, token_index);
  }
  if (token.text_ == "RETURN") {
    return compileReturnKeywordToken(machine, token_manager, token_index);
  }
  if (token.text_ == "FUNC") {
    return compileFunctionKeywordToken(machine, token_manager, token_index);
  }
  if (token.text_ == "DO" || token.text_ == "WHILE" || token.text_ == "FOR") {
    return compileLoopKeywordToken(machine, token_manager, token_index);
  }
  if (token.text_ == "VAR") {
    return compileVariableKeywordToken(machine, token_manager, token_index);
  }
  if (token.text_ == "VARPACK") {
    return compileVariablePackKeywordToken(machine, token_manager, token_index);
  }
  
  return kReport_UnexpectedKeyword;
}



Report Compiler::compileSeparatorToken(Machine* machine,
                                       TokenManager& token_manager,
                                       int32& token_index) {
  const Token token = token_manager.getToken(token_index);
      
  switch (token.priority_) {
    case ADDITION_OPERATION_PRIORITY: {
      return compileAdditionOperationSeparatorToken(machine, token_manager, token_index);
    }
  case MULTIPLY_OPERATION_PRIORITY: {
      return compileMultiplyOperationSeparatorToken(machine, token_manager, token_index);
    } 
    case POWER_OPERATION_PRIORITY: {
      return compilePowerOperationSeparatorToken(machine, token_manager, token_index);
    } 
    case COMPARISON_PRIORITY: {
      return compileComparisonOperationSeparatorToken(machine, token_manager, token_index);
    } 
    case EQUAL_PRIORITY: {
      return compileEqualSeparatorToken(machine, token_manager, token_index);
    } 
    default: {}
  }

  return kReport_NoErrors;
}

  bool Compiler::checkIfAndCompileCommasContent(Machine * machine, 
                                                    TokenManager & token_manager) {

  if (token_manager.areAnyCommaTokenInList()) {
    const int32 num_tokens = token_manager.numTokens();
    // Compile tokens with comas recursively.
    for (int32 i = 0; i < num_tokens; ++i) {
      if (token_manager.getToken(i).text_ == ",") {
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

Report Compiler::compileOpenParenthesisSeparatorToken(Machine* machine,
                                                      TokenManager& token_manager,
                                                      int32& token_index) {

  // First step will be to find its correspondant closing parenthesis.
  const int32 close_parenthesis_id = token_manager.getNextCloseParenthesisIndex(token_index);
  if (close_parenthesis_id == -1) { 
    return kReport_NoMatchingCloseParenthesis; 
  }

  // We will extract the content inside the parenthesis in a new token manager, 
  // And paste it in a new token manager.
  TokenManager parenthesis_content;
  token_manager.transferParenthesisContent(token_index, close_parenthesis_id, &parenthesis_content);

  // Compile the content of the parenthesis.
  const Report report = compileTokens(machine, parenthesis_content);

  // In case that its a function, then call it.
  token_index--;
  if (token_index >= 0) { // means that theres something behind the parenthesis.
    const Token token = token_manager.getToken(token_index);
    if (token.type_ != kTokenType_Keyword &&
        token.type_ != kTokenType_Separator &&
        token.priority_ != 0) {      
      machine->addCommand(kCommandType_FunctionCall, token.text_);
      token_manager.removeToken(token_index);
    }
  }


  return report;
}

Report Compiler::compileCloseBracketsSeparatorToken(Machine* machine, 
                                                    TokenManager& token_manager, 
                                                    int32& token_index) {
  
  // Will untag the end of a loop, conditional or function.
  const Tag tag = getAndRemoveLastTag();
  switch (tag) {
    case JMP::kTag_None:{ } break;
    case JMP::kTag_Loop: { machine->addCommand(kCommandType_FinishedConditionalOrLoop, "loop"); } break;
    case JMP::kTag_Conditional: { machine->addCommand(kCommandType_FinishedConditionalOrLoop, "conditional"); } break;
    case JMP::kTag_Function: { machine->addCommand(kCommandType_FunctionEnd, "endfunc"); } break;
    case JMP::kTag_VariablePack: { machine->addCommand(kCommandType_VariablePackEnd, "endvarpack"); } break;
  
  } 
  return kReport_NoErrors;
}

Report Compiler::compileAdditionOperationSeparatorToken(Machine* machine,
                                                        TokenManager& token_manager, 
                                                        int32& token_index) const {
  const Token operator_token = token_manager.getToken(token_index);
  const Token right_operand = token_manager.getToken(token_index + 1);
  const Token left_operand = token_manager.getToken(token_index - 1);

  // Push the operands to the stack
  machine->addCommand(kCommandType_PushToTheStack, left_operand.text_);
  machine->addCommand(kCommandType_PushToTheStack, right_operand.text_);

  // Then the next command will set the action to be applied to the previous operands.
  if (operator_token.text_ == "-") {
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

Report Compiler::compileMultiplyOperationSeparatorToken(Machine* machine,
                                                        TokenManager& token_manager,
                                                        int32& token_index) const {
  const Token operator_token = token_manager.getToken(token_index);
  const Token right_operand = token_manager.getToken(token_index + 1);
  const Token left_operand = token_manager.getToken(token_index - 1);

  // Push the operands to the stack
  machine->addCommand(kCommandType_PushToTheStack, left_operand.text_);
  machine->addCommand(kCommandType_PushToTheStack, right_operand.text_);

  // Then the next command will set the action to be applied to the previous operands.
  if (operator_token.text_ == "/") {
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

Report Compiler::compilePowerOperationSeparatorToken(Machine* machine,
                                                     TokenManager& token_manager,
                                                     int32& token_index) const {

  const Token operator_token = token_manager.getToken(token_index);
  const Token right_operand = token_manager.getToken(token_index + 1);
  const Token left_operand = token_manager.getToken(token_index - 1);

  // Push the operands to the stack
  machine->addCommand(kCommandType_PushToTheStack, left_operand.text_);
  machine->addCommand(kCommandType_PushToTheStack, right_operand.text_);

  // Then the next command will set the action to be applied to the previous operands
  machine->addCommand(kCommandType_Power);

  // Deleting the tokens from the list and adding a "RESULT" Temporary one.
  token_manager.transferContentBetweenIDsInclusive(token_index - 1, token_index + 1);
  // After deleting from the list the three elements, we step back two positions.
  token_index -= 2;

  return kReport_NoErrors;
}

Report Compiler::compileComparisonOperationSeparatorToken(Machine* machine,
                                                          TokenManager& token_manager,
                                                          int32& token_index) const {

  const Token operator_token = token_manager.getToken(token_index);
  const Token right_operand = token_manager.getToken(token_index + 1);
  const Token left_operand = token_manager.getToken(token_index - 1);

  // Push the operands to the stack
  machine->addCommand(kCommandType_PushToTheStack, left_operand.text_);
  machine->addCommand(kCommandType_PushToTheStack, right_operand.text_);

  // Then the next command will set the action to be applied to the previous operands.
  if (operator_token.text_ == "<") {
    machine->addCommand(kCommandType_LowerThanComparison);
  }
  else if (operator_token.text_ == ">") {
    machine->addCommand(kCommandType_GreaterThanComparison);
  }
  else if (operator_token.text_ == "<=") {
    machine->addCommand(kCommandType_LowerOrEqualThanComparison);
  }
  else if (operator_token.text_ == ">=") {
    machine->addCommand(kCommandType_GreaterOrEqualThanComparison);
  }
  else if (operator_token.text_ == "==") {
    machine->addCommand(kCommandType_EqualThanComparison);
  }
  else if (operator_token.text_ == "!=") {
    machine->addCommand(kCommandType_NotEqualThanComparison);
  }
  else {
    ReportError("Unexpected comparison token: " + operator_token.text_);
    return kReport_UnexpectedComparisonToken;
  }

  // Deleting the tokens from the list and adding a "RESULT" Temporary one.
  token_manager.transferContentBetweenIDsInclusive(token_index - 1, token_index + 1);
  // After deleting from the list the three elements, we step back two positions.
  token_index -= 2;

  return kReport_NoErrors;
}

Report Compiler::compileEqualSeparatorToken(Machine* machine,
                                            TokenManager& token_manager,
                                            int32& token_index) {

  const int32 num_tokens = token_manager.numTokens();
  if (num_tokens < 3 || token_index <= 0) {
    return kReport_EqualNeedTokensBeforeAndAfter;
  }
  // Transfer the content after the = to a temp token manager.
  TokenManager temp;
  token_manager.transferContentBetweenIDsInclusive(token_index + 1, num_tokens - 1, &temp);
  // Compile the content of this manager recursively.
  compileTokens(machine, temp);
  // Gets the token before the equal symbol.
  token_index--;
  const Token token = token_manager.getToken(token_index);
  machine->addCommand(kCommandType_EqualAssigment, token.text_);
  // Then delete the content of from the one behind the "=", to the end of the sentence.
  token_manager.transferContentBetweenIDsInclusive(token_index, num_tokens - 1);
  token_index--;
  
  return kReport_NoErrors;
}

/*******************************************************************************
***                    KEYWORDS TOKEN COMPILER METHODS                       ***
*******************************************************************************/

Report Compiler::compileConditionalKeywordToken(Machine* machine,
                                                TokenManager& token_manager,
                                                int32& token_index) {

  if (token_manager.getToken(token_index).text_ == "IF") {
    // removing the "vayword.
    token_manager.removeToken(token_index);
    // Checks if the last token of the line sentence is a "{" to start the body.
    if (token_manager.getToken(token_manager.numTokens() - 1).text_ != "{") {
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

Report Compiler::compileReturnKeywordToken(Machine* machine,
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

Report Compiler::compileFunctionKeywordToken(Machine* machine,
                                             TokenManager& token_manager,
                                             int32& token_index) {
  // ERROR CHECKINGS
  // func keyword need to be the first word of the token_manager.
  if (token_index != 0) {
    return kReport_FunctionKeywordShouldBeTheFirstToken;
  }

  // Mininum function code... "FUNC name() {
  if (token_manager.numTokens() < 5) {
    return kReport_FunctionDefinitionIncorrect;
  }
  
  // Check that the last token is "{"
  if (token_manager.getToken(token_manager.numTokens() - 1).text_ != "{") {
    return kReport_ExpectingOpenBrackets;
  }

  // Check if the name is valid
  Token token = token_manager.getToken(token_index + 1);
  if (token.type_ != kTokenType_Variable) {
    return kReport_InvalidNameOfFunction;
  }

  // FUNCTION DEFINITION
  // Command to define the function.
  machine->addCommand(kCommandType_FunctionDefinition, token.text_);
  // Deleting "func" and "funtion_name" tokens and the "{" one.
  token_manager.removeToken(token_index);
  token_manager.removeToken(token_index);
  token_manager.removeToken(token_manager.numTokens() - 1);

  // PARAMETERS OF THE FUNCTION
  // Saving the parameters in strings.
  std::vector<std::string> params;
  int32 num_tokens = token_manager.numTokens();
  for (int32 i = 1; i < num_tokens - 1; ++i) { // Ignoring "(" and ")"
    token = token_manager.getToken(i);
    if (token.text_ != ",") {
      if (token.type_ != kTokenType_Variable) {
        return kReport_UnexpectedFunctionParameters;
      }
      // Save the parameter
      params.push_back(token.text_);
    }
  }

  // Creating the commands -> Number of parameters, param1, param2...
  int32 num_params = params.size();
  if (num_params > MAX_PARAMETERS_PER_FUNCTION) {
    return kReport_ExceededNumParamsAllowedPerFunction;
  }
  char num_params_text[3];
  sprintf_s(num_params_text, 3, "%d", num_params);
  machine->addCommand(kCommandType_FunctionNumParameters, num_params_text);
  for (int32 i = num_params - 1; i >= 0; --i) {
    machine->addCommand(kCommandType_FunctionParameter, params[i]);
  }

  // Tagging the start of the definition of a function
  addTag(kTag_Function);

  return kReport_NoErrors;
}

Report Compiler::compileLoopKeywordToken(Machine* machine,
                                         TokenManager& token_manager,
                                         int32& token_index) {

  if (token_index != 0) {
    return kReport_LoopKeywordShouldBeTheFirstToken;
  }

  if (token_manager.getToken(token_index).text_ == "WHILE") {
    machine->addCommand(kCommandType_LoopStartPoint);
    // removing the loop ("WHILE") keyword.
    token_manager.removeToken(token_index);

    // Checks if the last token of the line sentence is a "{" to start the body.
    if (token_manager.getToken(token_manager.numTokens() - 1).text_ != "{") {
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

Report Compiler::compileVariableKeywordToken(Machine* machine,
                                             TokenManager& token_manager,
                                             int32& token_index) {

  // Gets the variable name.
  Token var_name = token_manager.getToken(token_index + 1);
  if (var_name.type_ != kTokenType_Variable) {
    return kReport_ExpectingNameOfVariable;
  }
  // Delete de "var" keyword   
  token_manager.removeToken(token_index);  // "var"
  // Adds a command who defines the variable name.
  machine->addCommand(kCommandType_VariableDefinition, var_name.text_);
  // Compile the rest of the line once the variable name is defined.
  return compileTokens(machine, token_manager);
}

Report Compiler::compileVariablePackKeywordToken(Machine* machine,
                                                 TokenManager& token_manager,
                                                 int32& token_index) {

  // ERROR CHECKINGS
  // VARPACK keyword need to be the first word of the token_manager.
  if (token_index != 0) {
    return kReport_VariablePackKeywordShouldBeTheFirstToken;
  }

  // Mininum varpack code... "VARPACK name {"
  if (token_manager.numTokens() < 3) {
    return kReport_VariablePackDefinitionIncorrect;
  }

  // Check that the last token is "{"
  if (token_manager.getToken(token_manager.numTokens() - 1).text_ != "{") {
    return kReport_ExpectingOpenBrackets;
  }

  // Check if the name is valid
  Token token = token_manager.getToken(token_index + 1);
  if (token.type_ != kTokenType_Variable) {
    return kReport_InvalidNameOfVariablePack;
  }

  // VARIABLE PACK DEFINITION
  // Command to define the variable pack.
  machine->addCommand(kCommandType_VariablePackDefinition, token.text_);
  // Deleting "VARPACK" and "variable pack name" tokens and the "{" one.
  token_manager.removeToken(token_index);
  token_manager.removeToken(token_index);
  token_manager.removeToken(token_manager.numTokens() - 1);

  // Tagging the start of the definition of a variable pack
  addTag(kTag_VariablePack);

  return kReport_NoErrors;
}

/*******************************************************************************
***                           TOKEN GENERATION                               ***
*******************************************************************************/


void Compiler::generateTokens(std::string& sentence, TokenManager& token_manager) {
  
  int32 num_parenthesis_opened = 0;

  replaceSpacesFromQuotes(sentence, '_'); // Replace ' ' ==> '_'

  sentence_index_ = 0;
  sentence_ = sentence;

  generateNextToken(); // Setting the first token as current.

  while(current_token_.text_ != "\0") { // End of line or empty text ""

    // Everytime a parenthesis is opened, we increase the priority.
    // This way we will avoid the problem with multiple parenthetical groups.
    if (current_token_.text_ == "(") {
      current_token_.priority_ += num_parenthesis_opened;
      num_parenthesis_opened++;
    }

    recoverSpacesFromQuotes(current_token_.text_); // Replace '_' ==> ' ' in quotes
    token_manager.addToken(current_token_);
    generateNextToken();
  }
  
}

void Compiler::generateNextToken() {

  const uint32 sentence_length = sentence_.length();

  // Restarts the current token.
  current_token_.text_ = "\0";
  current_token_.type_ = kTokenType_None;

  // To analyze sentences, spaces will be ignored.
  while (sentence_index_ < sentence_length &&
    isBlankSpace(sentence_[sentence_index_])) {
    sentence_index_++;
  }

  // Checking end of line.
  if (sentence_index_ >= sentence_length) {
    current_token_.text_ = "\0";
    return;
  }

  // Checking comments.
  if (sentence_[sentence_index_] == '#') {
    current_token_.text_ = "\0";
    return;
  }

  // TOKEN ANALYZE: We will get the token type and the token text.

  // Text between quotes, then we will ignore separators, etc. 
  if (sentence_[sentence_index_] == '\"') {
    current_token_.text_.push_back(sentence_[sentence_index_]);
    sentence_index_++;
    while (sentence_index_ < sentence_length) {
      current_token_.text_.push_back(sentence_[sentence_index_]);
      if (sentence_[sentence_index_] == '\"') {
        sentence_index_++;
        break;
      }
      sentence_index_++;
    }

    current_token_.type_ = kTokenType_Variable;
  }

  // Separators 
  else if (isSeparator(sentence_[sentence_index_])) {
    // Operators "==", "!=", "<=", ">=" checking
    if (sentence_[sentence_index_] == '=' ||
        sentence_[sentence_index_] == '!' ||
        sentence_[sentence_index_] == '<' ||
        sentence_[sentence_index_] == '>') {

      if (sentence_index_ + 1 < sentence_length && sentence_[sentence_index_ + 1] == '=') {
        current_token_.text_.push_back(sentence_[sentence_index_]);
        sentence_index_++;
      }
    }
    current_token_.text_.push_back(sentence_[sentence_index_]);
    current_token_.type_ = kTokenType_Separator;
    sentence_index_++;
  }

  // Numbers
  else if (isDigit(sentence_[sentence_index_])) {
    while (!isSeparator(sentence_[sentence_index_]) && sentence_index_ < sentence_length) {
      current_token_.text_.push_back(sentence_[sentence_index_]);
      sentence_index_++;
    }
    current_token_.type_ = kTokenType_Number;
  }

  // Vars and Keywords
  else if (isLetter(sentence_[sentence_index_])) {
    while (!isSeparator(sentence_[sentence_index_]) && sentence_index_ < sentence_length) {
      current_token_.text_.push_back(sentence_[sentence_index_]);
      sentence_index_++;
    }

    current_token_.type_ = kTokenType_Variable;
    if (isKeyword(current_token_.text_)) { current_token_.type_ = kTokenType_Keyword; }
  }

  // Once the type is set, we will assign an initial priority depending on the type.
  generateCurrentTokenInitialPriority();
}

void Compiler::generateCurrentTokenInitialPriority() {
  switch (current_token_.type_) {
    case JMP::kTokenType_Keyword: { current_token_.priority_ = KEYWORD_PRIORITY; } break;
    case JMP::kTokenType_Separator: {
      const std::string separator = current_token_.text_;
      if (separator == "}") { current_token_.priority_ = CLOSE_BRACKETS_PRIORITY; }
      else if (separator == "(") { current_token_.priority_ = OPEN_PARENTHESIS_PRIORITY; }
      else if (separator == "^") { current_token_.priority_ = POWER_OPERATION_PRIORITY; }
      else if (separator == "*" || separator == "/") { current_token_.priority_ = MULTIPLY_OPERATION_PRIORITY; }
      else if (separator == "+" || separator == "-") { current_token_.priority_ = ADDITION_OPERATION_PRIORITY; }
      else if (separator == ">" || separator == "<") { current_token_.priority_ = COMPARISON_PRIORITY; }
      else if (separator == ">=" || separator == "<=") { current_token_.priority_ = COMPARISON_PRIORITY; }
      else if (separator == "==" || separator == "!=") { current_token_.priority_ = COMPARISON_PRIORITY; }
      else if (separator == "=") { current_token_.priority_ = EQUAL_PRIORITY; }
      else if (separator == ",") { current_token_.priority_ = COMMA_PRIORITY; }
    }break;
    default: { current_token_.priority_ = DEFAULT_PRIORITY; } break;
  }
}


/*******************************************************************************
***                            TEXT MODIFIERS                                ***
*******************************************************************************/

void Compiler::replaceSpacesFromQuotes(std::string& sentence, char8 replacement) const {

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

void Compiler::recoverSpacesFromQuotes(std::string & sentence, char8 replacement) const {
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

bool Compiler::isSeparator(const char8& character) const {
  switch (character) {
    case ' ': case ',': case ';': case '?': case '!': case '^':
    case '+': case '-': case '*': case '/': case '=': case '%':
    case '(': case ')': case '{': case '}': case '<': case '>':
    return true;
    default: {};
  }
  return false;
}

bool Compiler::isBlankSpace(const char8& character) const {
  if (character == ' ') { 
    return true; 
  }
  return false;
}

bool Compiler::isLetter(const char8& character) const { 
  if ((character >= 'a' && character <= 'z') || 
      (character >= 'A' && character <= 'Z') || 
      (character == '"') || (character == '_')) { // needed for quotes.
    return true;
  }
  return false;
}

bool Compiler::isDigit(const char8& character) const {
  switch (character) {
    case '0': case '1': case '2': case '3': case '4':  
    case '5': case '6': case '7': case '8': case '9':
      return true;
    default: {};
  }
  return false;
}

bool Compiler::isKeyword(const std::string& word) const {

  if (word == "IF" || word == "ELSE" ||                   // Conditionals
      word == "FUNC" || word == "RETURN" ||               // Functions
      word == "DO" || word == "WHILE" || word == "FOR" || // Loops
      word == "VAR" || word == "VARPACK") {               // Variables and packs
    return true;
  }
  return false;
}



/*******************************************************************************
***                        TAG SYSTEM LIST METHODS                           ***
*******************************************************************************/

void Compiler::addTag(const Tag tag) {
  tag_list_.push_back(tag);
}

  Tag Compiler::getAndRemoveLastTag() {
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






}; /* JMP_PROJECT */