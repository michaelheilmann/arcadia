#if !defined(ARCADIA_VISUALS_WINDOWS__WINDOWTEXT_H_INCLUDED)
#define ARCADIA_VISUALS_WINDOWS__WINDOWTEXT_H_INCLUDED

#include "Arcadia/Engine/Include.h"

#if !defined(WIN32_LEAN_AND_MEAN)
  #define WIN32_LEAN_AND_MEAN
#endif
#if !defined(NOCOMM)
  #define NOCOMM
#endif
#include <Windows.h>

Arcadia_String*
_getWindowText
  (
    Arcadia_Thread* thread,
    HWND hWnd
  );

void
_setWindowText
  (
    Arcadia_Thread* thread,
    HWND hWnd,
    Arcadia_String* text
  );

static BOOL
_multiByteToWideChar
  (
    const char* p,
    size_t n,
    wchar_t** q,
    size_t* m
  );

#endif // ARCADIA_VISUALS_WINDOWS__WINDOWTEXT_H_INCLUDED
