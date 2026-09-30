#if !defined(ARCADIA_RING1_DIAGNOSTICS_H_INCLUDED)
#define ARCADIA_RING1_DIAGNOSTICS_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include <stdarg.h>
#include <stdint.h>

#define Arcadia_LogFlags_Debug (1)
#define Arcadia_LogFlags_Info (2)
#define Arcadia_LogFlags_Trace (4)
#define Arcadia_LogFlags_Error (8)

typedef uint8_t Arcadia_LogFlags;

void
Arcadia_logf
  (
    Arcadia_LogFlags flags,
    const char* format,
    ...
  );

void
Arcadia_logfv
  (
    Arcadia_LogFlags flags,
    const char* format,
    va_list arguments
  );

#endif // ARCADIA_RING1_DIAGNOSTICS_H_INCLUDED

