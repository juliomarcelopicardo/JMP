/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP_PROJECT - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#ifndef __JMP_H__
#define __JMP_H__ 1

#include "machine.h"

#endif /* __JMP_H__ */




/**********************  SCRIPTING LANGUAGE USER'S GUIDE  **********************

--------------------------------------------------------------------------------


  INTRODUCTION

    1. SYNTAX BASICS

      1.1 CHARACTERS
      1.2 KEYWORDS
      1.3 OPERATORS
      1.4 COMMENTS

    2. MEMORY AND SCOPE

    3. VARIABLES

      3.1 VARIABLE PACKS
      3.2 GET SCRIPT VARIABLE VALUES FROM C++
      3.3 REGISTER C++ VARIABLES TO USE THEM IN A JMP SCRIPT

    4. CONDITIONALS
    
    5. LOOPS

    6. FUNCTIONS

      6.1 LANGUAGE NATIVE FUNCTIONS
      6.2 RUN JMP SCRIPT FUNCTIONS FROM C++
      6.3 REGISTER C++ FUNCTIONS TO CALL THEM IN A JMP SCRIPT


--------------------------------------------------------------------------------

  INTRODUCTION

    The only file from the whole library that we will need to include in our
    code is the "jmp.h" as it will contain all the includes that we will need.

    To start using JMP, the first step is to create a "Machine" object, that will
    be the one in charge of doing everything related with our script.
    For each script that we want to use during the execution loop, is recommended
    to create a different machine, so it's easier to control them, and faster
    than reassign the script file to compile it in every frame.

    MACHINE CLASS WILL ONLY PROCESS SCRIPTS OR FILES WITH ".jmp" EXTENSION.

    Once created the machine, we will load a script, which will automaticly
    compile it, generate the global variables and commands necessary to execute
    it faster everytime we want to.

    There are three type of VALUES that we will use, they are the FLOAT numbers, 
    the INTEGER ones and the STRING.

    At the end of the guide, a small example will be shown, so will be easier to
    realize how simple is to use it.


--------------------------------------------------------------------------------

  1. SYNTAX BASICS.

    The compiler is CASE SENSITIVE, so we need to be careful with it.

    JMP is a scripting language COMPILED LINE BY LINE, aswell as others like
    "Python", which means that each separate line is considered a new instruction.
    There are no separators (like ";") which allow us to concatenate different
    instructions in the same line as we do with C++ and others.

    CORRECT USE:                  variable = 8 + 2

    INCORRECT USE:                variable =
                                  8 + 2


  1.1 CHARACTERS

    Digits and letters can be used in this language, special characters from
    concrete languages are not allowed.

    DIGITS: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9.
    CHARACTERS: From ASCII table -> a to z and A to Z.
    '_' Character is not recommended to be used, as compiler will delete it.


  1.2 KEYWORDS

    There are some keywords that we can use, they are already defined so this
    symbols or tokens shouldn't be used as variable or function names. ALL THE
    KEYWORDS ARE WRITTEN IN UPPER CASE.

    FUNC     -   Function definition.
    RETURN   -   Function return.
    VAR      -   Variable definition.
    VARPACK  -   Variable pack definition.
    IF       -   Conditional.
    WHILE    -   Loop.
    PRINT    -   Predefined language function name.
    SIN      -   Predefined language function name.
    COS      -   Predefined language function name.
    TAN      -   Predefined language function name.
    ASIN     -   Predefined language function name.
    ACOS     -   Predefined language function name.
    ATAN     -   Predefined language function name.

    ELSE, DO, FOR... are keywords that will be working in future updates.


  1.3 OPERATORS.

    MATHS:       (Strings can also be concatenated using this operand +)

    '+'      -   Addition.                         Example: a = b + c
    '*'      -   Multiply.
    '-'      -   Substract.
    '/'      -   Division.
    '^'      -   Power or potency.
    '='      -   Assignment.

    COMPARISON:

    '>'      -   Greater than.                     Example: IF(a>b){
    '>='     -   Greater or equal than.
    '<'      -   Lower than.
    '<='     -   Lower or equal than.
    '=='     -   Equal than.
    '!='     -   Not equal than.

    OTHERS:

    ','      -   Separate parameters.             Example: PRINT("Hello", 6)
    '"'      -   To define strings.               Example: VAR a = "Hello World"
    '.'      -   To access sub categories.        Example: Var a = Window.width
    '#'      -   Comments                         Example: #This is a comment

    There are separators that works exactly the same as C++, like brackets, keys...


  1.4 COMMENTS.

    As a good way to make your code easier to be understood, you can write
    comments that will be ignored by the compiler.

    To start a comment just use the symbol '#' and the rest of the line won't
    be compiled.

    EXAMPLE:   VAR a = 5 #This is a comment
    So the compiler will just read "VAR a = 5"

    As the code is compiled line by line, there's no character designed for
    multiline comments, you will need to write the comment initializer '#' in
    each of the lines designed as comments.



--------------------------------------------------------------------------------



  2. MEMORY AND SCOPE.

    This language uses stacks and memory to allocate the variable used in the
    script, so the scope system used in many languagues as C is the one chosen
    for JMP.

    There are two scopes:

    Local variables: They are defined inside the definition of a function,
    so you can only access them locally, when the function
    scope ends, the variables are deleted.

    Global variables: Are not defined inside any scope, so we can access to
    their values, even from C++.



    EXAMPLE:
                  VAR global = "This is a global variable"

                  FUNC example() {
                    VAR local = 0
                    PRINT(global)     # Compile (as it's a global variable)
                    PRINT(local)      # Compile (as it's inside the scope)
                  }

                  FUNC example2() {
                    PRINT(global)     # Compile (as it's a global variable)
                    PRINT(local)      # COMPILE ERROR (local out of scope).
                  }



--------------------------------------------------------------------------------


  3. VARIABLES.

    
    Variable system has been implemented, in order to create for the user, a 
    easy way to manage data. It's a non-typed language, so we won't need to 
    tell the type of variable we are going to define.

    To define a local or global variable we will use the keyword "VAR", and WE
    NEED TO ASSIGN AN INITIAL VALUE to them, in order to help the compiler.

      EXAMPLE:     VAR string = "value"    # To save a string.
                   VAR integer = 24        # To save an integer.
                   VAR float = 3.545       # To save a float.  

    REMEMBER: keywords are UPPER CASE, and the language is CASE SENSITIVE.


  3.1 VARIABLE PACKS.

    After using LUA, and practicing with JMP, I realized that sometimes, just for
    making the code easier to read, creating a way to package some variables 
    that are relating to the same object or entity, was very useful.

    VARIABLE PACKS CAN ONLY BE DEFINED IN THE GLOBAL SCOPE.

    To define them we need to open scope "{" in the first line, and close it "}"
    in the last line, after the last variable is declared.

      EXAMPLE:    VARPACK nameofthepack {
                        ...
                      }
           
    Inside the varpack scope, we can create as many variables as we want. And 
    to access them, we will just need to use the '.' character to reference it.

    EXAMPLE:      VARPACK Window {
                    VAR width = 1024
                    VAR height = 978
                  }
     
                  VAR example = Window.width   # example = 1024 



  3.2 GET SCRIPT VARIABLE VALUES FROM C++.

    JMP is a language that can be used to set configuration files and read them
    easily. That's why I thought that was very important to make an easy way to 
    comunicate the values that you can have in the script, with the existing
    framework where we are using the language (usually a C++ application).

    As I told before, "Machine" is the base class of our language, and it will 
    manage all the utils that we need for using the language.

    There are three functions that will return us the value of existing global
    variables that we can find in the script. As this way, we can look for a
    value just by knowing it's variable name in the script. 

    Here comes an example of the these functions. The first parameter of the
    function is the name of the variable, and the second (optional) is the name
    of the varpack where the variable is allocated.

    PROTOTYPE:   
      
    float Machine::getFloat(const char* var_name, const char* varpack_name = "");
    int Machine::getInteger(const char* var_name, const char* varpack_name = "");
    std::string Machine::getString(const char* var_name, const char* varpack_name = "");

    EXAMPLE:  
    
                  -------- JMP script example ---------
        
                  VAR number = 1.5

                  VARPACK Object {
                    VAR name = "example object"
                    VAR id = 3
                  }

                  --------- C++ code example ----------

                  JMP::Machine jmp;  // Creates a machine
                  jmp.processFile("examplepath.jmp"); // Loads the script
                     
                  float a = jmp.getFloat("number"); 
                  std::string b = jmp.getString("name", "Object");
                  int c = jmp.getInteger("id", "Object");

                  // RESULT: a = 1.5     b = "example object"    c = 3 
  


  3.3 REGISTER C++ VARIABLES TO USE THEM IN A JMP SCRIPT.

    We also have the chance to communicate our C++ application with the JMP Script
    so we can register variables from C++ to use and modify them in the script.

    We will think that when we are registering a variable, we are giving like a 
    reference to the script, so everytime we want to access to this variable in 
    the script, its value WILL BE ALWAYS UP TO DATE, and if we modify the value
    in the script, its correspondent in C++ will be updated to.

    PROTOTYPE: 

    Report Machine::registerVariable(const char *name_in_the_script, 
                                     JMP::ValueType type, 
                                     void* variable_pointer);

    EXAMPLE:

                  --------- C++ code example ----------

                  JMP::Machine jmp;  // Creates a machine
                  jmp.processFile("examplepath.jmp"); // Loads the script
                     
                  int number = 9; 
                  jmp.registerVariable("variable", kValueType_Integer, &number);
    

                  -------- JMP script example ---------
        
                  FUNC example() {
                    PRINT(variable)    # Output will be ---- 9 ----
                  }


--------------------------------------------------------------------------------


  4. CONDITIONALS.


    Conditionals are the as in C++, if the result of the comparison is true, it 
    will enter, if false, then not. The keyword IF will be used to start a 
    conditional instruction.

    FUNCTIONS CAN'T BE CALLED AS PART OF THE CONDITIONS.

    REMEMBER: keywords are UPPER CASE, and the language is CASE SENSITIVE.

    EXAMPLE:    
                      VARPACK Window {
                        VAR width = 1024
                        VAR height = 978
                      }
     
                      IF (Window.width >= 1000) {
                        PRINT("yeah")
                      }

                      # Output will be "yeah" as the condition is true.



--------------------------------------------------------------------------------


  5. LOOPS.


    Loops are the as in C++, if the result of the comparison is true, it
    will enter, if false, then not. The keyword WHILE will be used to start a
    Loop instruction.

    FUNCTIONS CAN'T BE CALLED AS PART OF THE CONDITIONS.

    REMEMBER: keywords are UPPER CASE, and the language is CASE SENSITIVE.

    EXAMPLE:    
                      VAR i = 0

                      WHILE (i < 5) {
                        PRINT(i)
                        i = i + 1
                      }

                      # Output will be "01234" 


--------------------------------------------------------------------------------


  6. FUNCTIONS.


    Functions system has been implemented, in order to create for the user, a
    easy way to create code. It's a non-typed language, so we won't need to
    tell the type of variable we are going to return.

    Imagine all the functions as VOID functions, because we are not allowed to
    return values. The RETURN keyword, will be used just as an exit point.

    The keyword "FUNC" will be use to define new functions. Parameter names will
    be assigned separated with commas.

    REMEMBER: keywords are UPPER CASE, and the language is CASE SENSITIVE.

    EXAMPLE:    
                  FUNC example(a,b) {
                    PRINT(a)
                    PRINT(b)
                  }

                  FUNC Main() {
                    example("Hello", 3.5)     # Example of a function call
                  }

                  # If we run the function Main, the output will be "Hello3.5"



  6.1 LANGUAGE NATIVE FUNCTIONS

    There are some functions that we can use, which are native from the proper
    language. There are two main categories, Output and Trigonometric functions.

    PRINT - Function used to output a string (like the std::cout or the printf)
    SIN, ASIN, COS, ACOS, TAN, ATAN - Tringonometric functions.

    REMEMBER: keywords are UPPER CASE, and the language is CASE SENSITIVE.

    EXAMPLE:    
              FUNC Main() {
                VAR cosinus = COS(0.0)   # ALWAYS RADIANS
                PRINT(cosinus)
              }

              # If we run the function Main, the output will be "1.0" 



  6.2 RUN JMP SCRIPT FUNCTIONS FROM C++

    This is the most important part to be able to use the language using its full
    power. Calling JMP script functions from C++ will give us the chance to
    execute code that can be modified in real time without compiling.
    
    To run a function we just need to do as in the example.

    PROTOTYPE: 

    Report Machine::runFunction(std::string function_call_sentence);

    If we don't call any function, the language will look for any function in the
    script which name is main(), to execute it by default.

    EXAMPLE:

              --------- C++ code example ----------

              JMP::Machine jmp;  // Creates a machine
              jmp.processFile("examplepath.jmp"); // Loads the script
                     
              jmp.runFunction(Init("Hello World"));
    

              -------- JMP script example ---------
        
              FUNC Init(param) {
                PRINT(param)       # Output will be ---- Hello world ----
              }




  6.3 REGISTER C++ FUNCTIONS TO CALL THEM IN A JMP SCRIPT

    This is possibly the hardest part to understand, but once you get used to it,
    you start using it very simply.

    For communicating C++ with JMP you need to follow these steps.

    - Defining a function in C++ with THIS PROTOTYPE:
      
      void NameOfFunction(std::vector<JMP::Value>& params) {}

    - Register the created function so the JMP script can call it. To register
      it we will do similar as with the variables.

      Report Machine::registerFunction(const char* func_name, void(*function_ptr)(std::vector<Value>&)) {

    - Call the function from the script.

    Everytime this function is called from the script, in C++ the function that
    we registered will be called, with the number of parameters that we used in
    the script, allocated in the vector of values of the c++ function. This 
    means that you are the only responsable to give the correct parameters in the
    script and also to use them correctly in C++, as the language won't detect 
    errors for you.

    EXAMPLE:
    
          --------- C++ code example ----------

          void ExampleFunction(std::vector<JMP::Value>& param) {
            printf(%s, param[0].text_.c_str());    // "Hello"
            printf(%d, param[1].getAsInteger());   // 4
            printf(%f, param[2].getAsFloat());     // 5.6
          }

          int main() {

            JMP::Machine jmp;  // Creates a machine
            jmp.processFile("examplepath.jmp"); // Loads the script

            jmp.registerFunction("Example", &ExampleFunction);
            jmp.runFunction("Init()");
            return 0;
          }


          -------- JMP script example ---------

          FUNC Init() {
            Example("Hello", 4, 5.6)
          }
   
        # FINAL OUTPUT will be      Hello45.6

*/
