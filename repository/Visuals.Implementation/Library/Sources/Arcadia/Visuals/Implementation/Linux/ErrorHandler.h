#if !defined(ARCADIA_VISUALS_IMPLEMENTATION_LINUX_ERROR_H_INCLUDED)
#define ARCADIA_VISUALS_IMPLEMENTATION_LINUX_ERROR_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

void
Arcadia_Visuals_Implementation_Linux_startupX11ErrorHandler
  (
    Arcadia_Thread* thread
  );

void
Arcadia_Visuals_Implementation_Linux_shutdownX11ErrorHandler
  (
    Arcadia_Thread* thread
  );

Arcadia_BooleanValue
Arcadia_Visuals_Implementation_Linux_hasX11Error
  (
    Arcadia_Thread* thread
  );

void
Arcadia_Visuals_Implementation_Linux_clearX11Error
  (
      Arcadia_Thread* thread
  );

#endif // ARCADIA_VISUALS_IMPLEMENTATION_LINUX_ERROR_H_INCLUDED
