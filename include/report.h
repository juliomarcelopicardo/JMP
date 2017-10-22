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

namespace JMP {

/*******************************************************************************
***                           RESULTS OR REPORTS                             ***
*******************************************************************************/

  enum Report {
    kReport_NoErrors = 0,
    kReport_InvalidFileExtension,
    kReport_InvalidFilename,
    kReport_FilenameTooShort,
  };

  void ReportMsg(std::string msg);
  void ReportError(std::string error);

}; /* JMP */

#endif /* __JMP_REPORT_H__ */