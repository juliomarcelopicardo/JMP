/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#include "machine.h"
#include <Windows.h>
#include <string>
#include <fstream>


namespace JMP {

Machine::Machine() {}

Machine::~Machine() {}




Report Machine::process(std::string script_filename) {

  // To force .jmp extensions.
  if (checkExtension(script_filename) != kReport_NoErrors) {
    return kReport_InvalidFileExtension;
  }

  std::ifstream script; // ifstream because we just want to read the file.
  script.open(script_filename); // Default read mode is read only "in".

  if (!script) {
    ReportError("\"" + script_filename + "\" - Can't open file");
    return kReport_InvalidFilename;
  }

  // Reading the whole file, allocating it using lines of code.
  while (!script.eof()) {
    std::string code_line;
    std::getline(script, code_line);

    // To prevent errors I will remove all the tabs. Changing them to blank spaces
    uint32 string_length = code_line.length();
    for (int32 i = 0; i < string_length; i++) {
      if (code_line[i] == '\t') { code_line[i] = ' '; }
    }

    OutputDebugString(code_line.c_str());

  }

  return kReport_NoErrors;
}

Report Machine::checkExtension(std::string filename) {
  
  // We will take the last 4 characters to check the extension.
  uint32 index = filename.length() - 4;
  std::string pepe = filename.substr(index);
  if (index > 1 && filename.substr(index) == ".jmp") {
    return kReport_NoErrors;
  }
  
  // Name too short.
  if (index < 1) {
    ReportError("\"" + filename + "\" - Filename too short");
    return kReport_FilenameTooShort;
  }

  // Invalid extension.
  ReportError("\"" + filename + "\" - Extension must be .jmp");
  return kReport_InvalidFileExtension;
}

}; /* JMP */