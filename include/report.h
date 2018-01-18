/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/

#ifndef __JMP_REPORT_H__
#define __JMP_REPORT_H__ 1

#include <stdint.h>
#include <string>
#include "types.h"

namespace JMP {

/*******************************************************************************
***                           RESULTS OR REPORTS                             ***
*******************************************************************************/

  enum Report {
    kReport_NoErrors = 0,
    kReport_InvalidFileExtension,
    kReport_InvalidFilename,
    kReport_FilenameTooShort,
    kReport_NullPointer,
    kReport_EmptyContainer,
    kReport_ListIndexOutOfRange,
    kReport_EmptyLine,
    kReport_NoTokensToCompile,
    kReport_NoMatchingCloseParenthesis,
    kReport_NoTagsToDelete,
    kReport_UnexpectedKeyword,
    kReport_KeywordFunctionalityNotProgrammedYet,
    kReport_ExpectingOpenBrackets,
    kReport_ExpectingNameOfVariable,
  };

  void ReportMsg(std::string msg);
  void ReportError(std::string error);
  void ReportWarning(std::string warning);
  void PrintReport(Report& report, uint32 line_number = 0);

}; /* JMP */

#endif /* __JMP_REPORT_H__ */