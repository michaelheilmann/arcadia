#include "Arcadia/Visuals/Implementation/OpenGL4/GLX/_Configurations.h"

#include <X11/Xlib.h>
#include <GL/glx.h>
#include "Arcadia/Engine/Include.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/GLX/_Extensions.h"
#include "Arcadia/Visuals/Implementation/Linux/ErrorHandler.h"
#include <stdio.h> // @todo Remove references to `stdio.h`.

static Arcadia_String*
integerToString
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value integer32Value
  )
{
  Arcadia_RuntimeUTF8String* runtimeUTF8StringValue = Arcadia_RuntimeUTF8String_createFromInteger32(thread, integer32Value);
  Arcadia_Value value = { .tag = Arcadia_ValueTag_RuntimeUTF8String, .runtimeUTF8StringValue = runtimeUTF8StringValue };
  return Arcadia_String_create(thread, value);
}

static void
addIfNotExisting
  (
    Arcadia_Thread* thread,
    Arcadia_List* configurations,
    Arcadia_Visuals_Configuration* configuration
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)configurations); i < n; ++i) {
    Arcadia_Visuals_Configuration* element = Arcadia_List_getObjectReferenceValueAt(thread, configurations, i);
    Arcadia_Value a, b;
    a = Arcadia_Value_makeObjectReferenceValue(element->opengl.version.major);
    b = Arcadia_Value_makeObjectReferenceValue(configuration->opengl.version.major);
    if (!Arcadia_Value_isEqualTo(thread, &a, &b)) {
      continue;
    }
    a = Arcadia_Value_makeObjectReferenceValue(element->opengl.version.minor);
    b = Arcadia_Value_makeObjectReferenceValue(configuration->opengl.version.minor);
    if (!Arcadia_Value_isEqualTo(thread, &a, &b)) {
      continue;
    }
    a = Arcadia_Value_makeObjectReferenceValue(element->colorBuffer.redBits);
    b = Arcadia_Value_makeObjectReferenceValue(configuration->colorBuffer.redBits);
    if (!Arcadia_Value_isEqualTo(thread, &a, &b)) {
      continue;
    }
    a = Arcadia_Value_makeObjectReferenceValue(element->colorBuffer.greenBits);
    b = Arcadia_Value_makeObjectReferenceValue(configuration->colorBuffer.greenBits);
    if (!Arcadia_Value_isEqualTo(thread, &a, &b)) {
      continue;
    }
    a = Arcadia_Value_makeObjectReferenceValue(element->colorBuffer.blueBits);
    b = Arcadia_Value_makeObjectReferenceValue(configuration->colorBuffer.blueBits);
    if (!Arcadia_Value_isEqualTo(thread, &a, &b)) {
      continue;
    }
    a = Arcadia_Value_makeObjectReferenceValue(element->colorBuffer.alphaBits);
    b = Arcadia_Value_makeObjectReferenceValue(configuration->colorBuffer.alphaBits);
    if (!Arcadia_Value_isEqualTo(thread, &a, &b)) {
      continue;
    }
    a = Arcadia_Value_makeObjectReferenceValue(element->depthBuffer.depthBits);
    b = Arcadia_Value_makeObjectReferenceValue(configuration->depthBuffer.depthBits);
    if (!Arcadia_Value_isEqualTo(thread, &a, &b)) {
      continue;
    }
    // Already in the list.
    return;
  }
  // Not in the list yet. Add.
  Arcadia_List_insertBackObjectReferenceValue(thread, configurations, (Arcadia_ObjectReferenceValue)configuration);
}

Arcadia_List*
Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_getConfigurations
  (
    Arcadia_Thread* thread,
    Display* display
  )
{
  Arcadia_List* configurations = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_Set* extensions = Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_getExtensions(thread, display);

  if (!Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_hasExtension(thread, display, Arcadia_String_createFromCxxString(thread, u8"GLX_ARB_create_context"))) {
    return configurations;
  }

  int glXMajorVersion, glXMinorVersion;

  // FBConfigs were added in GLX version 1.3.
  if (!glXQueryVersion(display, &glXMajorVersion, &glXMinorVersion)) {
    fprintf(stderr, "%s:%d: unable to determine GLX version\n", __FILE__, __LINE__);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  if ((glXMajorVersion == 1 && glXMinorVersion < 3) || glXMajorVersion < 1) {
    fprintf(stderr, "%s:%d: unsupported GLX version\n", __FILE__, __LINE__);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  int glXNumberOfConfigurations;
  GLXFBConfig* glXConfigurations = glXChooseFBConfig(display, DefaultScreen(display),
                                                     NULL, &glXNumberOfConfigurations);
  if (!glXConfigurations) {
    fprintf(stderr, "%s:%d: unable to obtain GLX frame buffer configuration\n", __FILE__, __LINE__);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    for (int majorVersion = 0; majorVersion < 5; majorVersion++) {
      for (int minorVersion = 0; minorVersion < 9; minorVersion++) {
        for (int i = 0, n = glXNumberOfConfigurations; i < n; ++i) {
          GLXFBConfig* glXConfiguration = &(glXConfigurations[i]);
          int key, value;

          key = GLX_DRAWABLE_TYPE;
          if (glXGetFBConfigAttrib(display, *glXConfiguration, key, &value)) {
            continue;
          }
          if (GLX_WINDOW_BIT != (value & GLX_WINDOW_BIT)) {
            continue;
          }

          key = GLX_RENDER_TYPE;
          if (glXGetFBConfigAttrib(display, *glXConfiguration, key, &value)) {
            continue;
          }
          if (GLX_RGBA_BIT != (value & GLX_RGBA_BIT)) {
            continue;
          }

          key = GLX_X_VISUAL_TYPE;
          if (glXGetFBConfigAttrib(display, *glXConfiguration, key, &value)) {
            continue;
          }
          if (value != GLX_TRUE_COLOR) {
            continue;
          }

          key = GLX_RED_SIZE;
          int redBits;
          if (glXGetFBConfigAttrib(display, *glXConfiguration, key, &redBits)) {
            continue;
          }
          if (redBits < 8) {
            continue;
          }
          key = GLX_GREEN_SIZE;
          int greenBits;
          if (glXGetFBConfigAttrib(display, *glXConfiguration, key, &greenBits)) {
            continue;
          }
          if (greenBits < 8) {
            continue;
          }
          key = GLX_BLUE_SIZE;
          int blueBits;
          if (glXGetFBConfigAttrib(display, *glXConfiguration, key, &blueBits)) {
            continue;
          }
          if (blueBits < 8) {
            continue;
          }
          key = GLX_ALPHA_SIZE;
          int alphaBits;
          if (glXGetFBConfigAttrib(display, *glXConfiguration, key, &alphaBits)) {
            continue;
          }
          if (alphaBits < 8) {
            continue;
          }

          key = GLX_DEPTH_SIZE;
          int depthBits;
          if (glXGetFBConfigAttrib(display, *glXConfiguration, key, &depthBits)) {
            continue;
          }
          if (depthBits < 24) {
            continue;
          }
#if 0
          // Get the default screen's GLX extension list.
          const char *glXExtensions = glXQueryExtensionsString(display, DefaultScreen(display));
          // Check for the GLX_ARB_create_context extension string.
          if (!isExtensionSupported(glXExtensions, "GLX_ARB_create_context")) {
            continue;
          }
#endif
          // Check for glCreateContextAttribsARB function.
          // NOTE: It is not necessary to create or make current to a context before calling glXGetProcAddressARB.
          PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB =
            (PFNGLXCREATECONTEXTATTRIBSARBPROC)
            glXGetProcAddressARB
              (
                (const GLubyte *)"glXCreateContextAttribsARB"
              );

          int contextAttributes[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, majorVersion,
            GLX_CONTEXT_MINOR_VERSION_ARB, minorVersion,
            //GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            None
          };
#if 0
          // Replace the original error handler with our error handler.
          g_oldErrorHandler = XSetErrorHandler(&ignoreErrorHandler);
#endif
          GLXContext glXContext = glXCreateContextAttribsARB(display, *glXConfiguration, 0, True,
                                                             contextAttributes);
          if (NULL == glXContext || Arcadia_Visuals_Implementation_Linux_hasX11Error(thread)) {
            Arcadia_Visuals_Implementation_Linux_clearX11Error(thread);
            continue;
          }

#if 0
          // Restore the original error handler.
          XSetErrorHandler(g_oldErrorHandler);
          g_oldErrorHandler = NULL;
#endif

          glXDestroyContext(display, glXContext);
          glXContext = NULL;

          Arcadia_Visuals_Configuration* configuration = Arcadia_Visuals_Configuration_create(thread);
          configuration->opengl.version.major = integerToString(thread, majorVersion);
          configuration->opengl.version.minor = integerToString(thread, minorVersion);
          configuration->colorBuffer.redBits = integerToString(thread, redBits);
          configuration->colorBuffer.greenBits = integerToString(thread, greenBits);
          configuration->colorBuffer.blueBits = integerToString(thread, blueBits);
          configuration->colorBuffer.alphaBits = integerToString(thread, alphaBits);
          configuration->depthBuffer.depthBits = integerToString(thread, depthBits);
          addIfNotExisting(thread, configurations, configuration);
        }
      }
    }

    Arcadia_Thread_popJumpTarget(thread);

    XFree(glXConfigurations);
    glXConfigurations = NULL;

    return configurations;
  } else {
    Arcadia_Thread_popJumpTarget(thread);

    if (glXConfigurations) {
      XFree(glXConfigurations);
      glXConfigurations = NULL;
    }

    Arcadia_Thread_jump(thread);
  }
}
