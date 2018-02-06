/** Copyright Julio Marcelo Picardo 2017-18, all rights reserved.
*
*  @project JMP - A Scripting Language for Game Engines.
*  @brief Final Project: BSc (Honours) Computer Science for Games.
*  @supervisor Dr. Paul Parry <p.w.parry@shu.ac.uk>
*  @author Julio Marcelo Picardo <juliomarcelopicardo@gmail.com>
*                                <b7026027@my.shu.ac.uk>
*/


/* SINGLE HEADER LIBRARY */

#ifndef __JMP_SINGLE_HEADER_H__
#define __JMP_SINGLE_HEADER_H__ 1

#include "types.h"
#include <string>

namespace JMP_SINGLE_HEADER {

/*******************************************************************************
***                                 TYPES                                    ***
*******************************************************************************/

  /// char
  typedef char char8;
  typedef char16_t char16;
  typedef char32_t char32;

  /// unsigned char
  typedef unsigned char uchar8;

  /// integer
  typedef int8_t int8;
  typedef int16_t int16;
  typedef int32_t int32;
  typedef int64_t int64;

  /// unsigned integer
  typedef uint8_t uint8;
  typedef uint16_t uint16;
  typedef uint32_t uint32;
  typedef uint64_t uint64;

  /// precision
  typedef float float32;
  typedef double float64;


}; /* JMP_SINGLE_FILE */

#endif /* __JMP_SINGLE_HEADER_H__ */