#if !defined(ARCADIA_VISUALS_WINDOWS__CHARCONV_H_INCLUDED)
#define ARCADIA_VISUALS_WINDOWS__CHARCONV_H_INCLUDED

#include "Arcadia/Engine/Include.h"
#if !defined(WIN32_LEAN_AND_MEAN)
  #define WIN32_LEAN_AND_MEAN
#endif
#if !defined(NOCOMM)
  #define NOCOMM
#endif
#include <Windows.h>

Arcadia_String*
_fromMultiByte
  (
    Arcadia_Thread* thread,
    CONST CHAR* p,
    size_t n
  );

Arcadia_String*
_fromWideChar
  (
    Arcadia_Thread* thread,
    CONST WCHAR* p,
    size_t n
  );

BOOL
_multiByteToWideChar
  (
    const char* p,
    size_t n,
    wchar_t** q,
    size_t* m
  );

#endif // ARCADIA_VISUALS_WINDOWS__CHARCONV_H_INCLUDED
