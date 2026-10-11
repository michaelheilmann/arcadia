#include "Arcadia/Visuals/Implementation/OpenGL4/GLX/_Extensions.h"

#include <X11/Xlib.h>
#include <GL/glx.h>

Arcadia_Set*
Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_getExtensions
  (
    Arcadia_Thread* thread,
    Display* display
  )
{
  const char *start = glXQueryExtensionsString(display, DefaultScreen(display));
  const char *current = start;
  Arcadia_Set* set = (Arcadia_Set*)Arcadia_HashSet_create(thread);

  // as long as we did not hit the end of the string
  while (*current != '\0') {
    // ... there are two actions:
    if (*current == ' ') {
      // 1) skip whitespace
      do {
        current++;
      } while (*current == ' ');
      start = current;
    } else {
      // 2) read extension
      do {
        current++;
      } while (*current != ' ' && *current != '\0');
      Arcadia_Value extension =
        Arcadia_Value_makeObjectReferenceValue
          (
            Arcadia_String_create
              (
                thread,
                Arcadia_Value_makeRuntimeUTF8StringValue
                  (
                    Arcadia_RuntimeUTF8String_create(thread, start, current - start)
                  )
              )
          );
      Arcadia_Set_add(thread, set, extension, NULL);
      start = current;
    }
  }
  return set;
}

Arcadia_BooleanValue
Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_hasExtension
  (
    Arcadia_Thread* thread,
    Display* display,
    Arcadia_String* extension
  )
{
  Arcadia_Set* extensions = Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_getExtensions(thread, display);
  return Arcadia_Set_contains(thread, extensions, Arcadia_Value_makeObjectReferenceValue(extension));
}
