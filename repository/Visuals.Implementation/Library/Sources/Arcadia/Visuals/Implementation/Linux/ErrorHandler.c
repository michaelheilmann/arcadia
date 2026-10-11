#include "Arcadia/Visuals/Implementation/Linux/ErrorHandler.h"

#include <X11/Xlib.h>
#include <stdio.h> // @todo Remove references to `stdio.h`.

static int (*g_oldErrorHandler)(Display*, XErrorEvent*) = NULL;

static Arcadia_BooleanValue g_error = Arcadia_BooleanValue_False;

static int
errorHandler
  (
    Display *display,
    XErrorEvent *event
  )
{
  char buf[1024+1];
  XGetErrorText(display, event->error_code, buf, 1024+1);
  printf("%s:%d: %s\n", __FILE__, __LINE__, buf);
  g_error = Arcadia_BooleanValue_True;
  return 0;
}

void
Arcadia_Visuals_Implementation_Linux_startupX11ErrorHandler
  (
    Arcadia_Thread* thread
  )
{
  // Replace the original error handler with our error handler.
  g_oldErrorHandler = XSetErrorHandler(&errorHandler);
}

void
Arcadia_Visuals_Implementation_Linux_shutdownX11ErrorHandler
  (
    Arcadia_Thread* thread
  )
{
  // Restore the original error handler.
  XSetErrorHandler(g_oldErrorHandler);
  g_oldErrorHandler = NULL;
}

Arcadia_BooleanValue
Arcadia_Visuals_Implementation_Linux_hasX11Error
  (
    Arcadia_Thread* thread
  )
{ return g_error; }

void
Arcadia_Visuals_Implementation_Linux_clearX11Error
  (
      Arcadia_Thread* thread
  )
{ g_error = Arcadia_BooleanValue_False; }
