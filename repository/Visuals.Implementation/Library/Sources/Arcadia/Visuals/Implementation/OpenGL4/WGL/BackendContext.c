// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
// details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include "Arcadia/Visuals/Implementation/OpenGL4/WGL/BackendContext.h"

#include "Arcadia/Visuals/Implementation/Windows/_GetDisplayDevices.h"
#include "Arcadia/Visuals/Implementation/Windows/Icon.h"

#include "Arcadia/Visuals/Implementation/OpenGL4/WGL/WindowBackend.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/WGL/FactoryContext.h"

#include "Arcadia/Logging/Include.h"
#include "Arcadia/Engine/Include.h"

#include <assert.h>

static Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* g_instance = NULL;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct _Linker _Linker;

static void*
_Linker_link
  (
    Arcadia_Thread* thread,
    _Linker* linker,
    const char* name
  );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined(WIN32_LEAN_AND_MEAN)
  #define WIN32_LEAN_AND_MEAN
#endif
#if !defined(NOCOMM)
  #define NOCOMM
#endif
#include <Windows.h>
#include <GL/glcorearb.h>

#include <windowsx.h>

struct _Linker {
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Linker _parent;
  HANDLE hLibrary;
};

static void*
_Linker_link
  (
    Arcadia_Thread* thread,
    _Linker* linker,
    const char *name
  )
{
  void* p = wglGetProcAddress(name);
  if (!p && linker->hLibrary) {
    p = GetProcAddress(linker->hLibrary, name);
  }
  return p;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions*
getFunctionsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* self
  )
{ return &self->_functions; }

static Arcadia_BooleanValue
isConfigurationSupported
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
    return Arcadia_BooleanValue_True;
  }
  return Arcadia_BooleanValue_False;
}

static void
createContext
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* self,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryContext* wglFactoryContext
  )
{
  // (0) Open the system window.
  self->systemWindow = Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow_create(thread);
  Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow_open(thread, self->systemWindow);
  // (1) Get supported configuration.
  Arcadia_List* supportedConfigurations = Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryContext_getConfigurations(thread, wglFactoryContext);
  Arcadia_StringBuilder* logMessage = Arcadia_StringBuilder_create(thread);
  Arcadia_Log* log = (Arcadia_Log*)Arcadia_ConsoleLog_create(thread);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)supportedConfigurations); i < n; ++i) {
    Arcadia_Visuals_Configuration* supportedConfiguration = Arcadia_List_getObjectReferenceValueAt(thread, supportedConfigurations, i);
    Arcadia_StringBuilder_clear(thread, logMessage);

    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8"index: ");
    Arcadia_StringBuilder_insertBackString(thread, logMessage, Arcadia_String_createFromSize(thread, i));
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8"\n");

    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8"red bits: ");
    Arcadia_StringBuilder_insertBack(thread, logMessage, Arcadia_Value_makeObjectReferenceValue(supportedConfiguration->colorBuffer.redBits));
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8"\n");

    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8"green bits: ");
    Arcadia_StringBuilder_insertBack(thread, logMessage, Arcadia_Value_makeObjectReferenceValue(supportedConfiguration->colorBuffer.greenBits));
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8"\n");

    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8"blue bits: ");
    Arcadia_StringBuilder_insertBack(thread, logMessage, Arcadia_Value_makeObjectReferenceValue(supportedConfiguration->colorBuffer.blueBits));
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8"\n");

    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8"alpha bits: ");
    Arcadia_StringBuilder_insertBack(thread, logMessage, Arcadia_Value_makeObjectReferenceValue(supportedConfiguration->colorBuffer.alphaBits));
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8"\n");

    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8"depth bits: ");
    Arcadia_StringBuilder_insertBack(thread, logMessage, Arcadia_Value_makeObjectReferenceValue(supportedConfiguration->depthBuffer.depthBits));
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8"\n");

    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8"OpenGL major version: ");
    Arcadia_StringBuilder_insertBack(thread, logMessage, Arcadia_Value_makeObjectReferenceValue(supportedConfiguration->opengl.version.major));
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8"\n");

    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8"OpenGL minor version: ");
    Arcadia_StringBuilder_insertBack(thread, logMessage, Arcadia_Value_makeObjectReferenceValue(supportedConfiguration->opengl.version.minor));
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8"\n");

    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8"\n");

    Arcadia_Log_information(thread, log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(logMessage)));
  }
  // (2) Get desired configuration.
  Arcadia_Visuals_Configuration* desiredConfiguration = Arcadia_Visuals_Configuration_create(thread);
  // (3) Check if desired configuration is in supported configurations. If yes, continue. If no, fail.
  if (!isConfigurationSupported(thread, supportedConfigurations, desiredConfiguration)) {
    Arcadia_StringBuilder_clear(thread, logMessage);
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, __FILE__);
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8":");
    Arcadia_StringBuilder_insertBackString(thread, logMessage, Arcadia_String_createFromCxxInt(thread, __LINE__));
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8": visuals configuration not supported\n");

    Arcadia_Log_error(thread, log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(logMessage)));

    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  const int pixelFormatAttribs[] = {
    WGL_ACCELERATION_ARB, // WGL_ACCELERATION_ARB
    WGL_FULL_ACCELERATION_ARB, // WGL_FULL_ACCELERATION_ARB
    WGL_DEPTH_BITS_ARB, Arcadia_String_toCxxInt(thread, desiredConfiguration->depthBuffer.depthBits), // WGL_DEPTH_BITS_ARB
    WGL_DRAW_TO_WINDOW_ARB, 1, // WGL_DRAW_TO_WINDOW_ARB
    WGL_RED_BITS_ARB, Arcadia_String_toCxxInt(thread, desiredConfiguration->colorBuffer.redBits), // WGL_RED_BITS_ARB
    WGL_GREEN_BITS_ARB, Arcadia_String_toCxxInt(thread, desiredConfiguration->colorBuffer.greenBits), // WGL_GREEN_BITS_ARB
    WGL_BLUE_BITS_ARB, Arcadia_String_toCxxInt(thread, desiredConfiguration->colorBuffer.blueBits), // WGL_BLUE_BITS_ARB
    WGL_ALPHA_BITS_ARB, Arcadia_String_toCxxInt(thread, desiredConfiguration->colorBuffer.alphaBits), // WGL_ALPHA_BITS_ARB
    WGL_PIXEL_TYPE_ARB,  WGL_TYPE_RGBA_ARB, // WGL_PIXEL_TYPE_ARB // WGL_TYPE_RGBA_ARB
    WGL_SUPPORT_OPENGL_ARB, 1, // WGL_SUPPORT_OPENGL_ARB
    WGL_COLOR_BITS_ARB,	32, // WGL_COLOR_BITS_ARB
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
  int numberOfPixelFormats;
  wglFactoryContext->_wglChoosePixelFormat(self->systemWindow->deviceContextHandle, &pixelFormatAttribs[0], NULL, 1, &self->pixelFormatIndex, &numberOfPixelFormats);
  if (!numberOfPixelFormats) {
    Arcadia_StringBuilder_clear(thread, logMessage);
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, __FILE__);
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8":");
    Arcadia_StringBuilder_insertBackString(thread, logMessage, Arcadia_String_createFromCxxInt(thread, __LINE__));
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8": failed to select pixel format\n");

    Arcadia_Log_error(thread, log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(logMessage)));

    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  PIXELFORMATDESCRIPTOR pixelFormatDescriptors;
  if (!DescribePixelFormat(self->systemWindow->deviceContextHandle, self->pixelFormatIndex, sizeof(pixelFormatDescriptors), &pixelFormatDescriptors)) {
    Arcadia_StringBuilder_clear(thread, logMessage);
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, __FILE__);
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8":");
    Arcadia_StringBuilder_insertBackString(thread, logMessage, Arcadia_String_createFromCxxInt(thread, __LINE__));
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8": failed to describe pixel format\n");

    Arcadia_Log_error(thread, log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(logMessage)));

    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  if (!SetPixelFormat(self->systemWindow->deviceContextHandle, self->pixelFormatIndex, &pixelFormatDescriptors)) {
    Arcadia_StringBuilder_clear(thread, logMessage);
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, __FILE__);
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8":");
    Arcadia_StringBuilder_insertBackString(thread, logMessage, Arcadia_String_createFromCxxInt(thread, __LINE__));
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8": failed to set pixel format\n");

    Arcadia_Log_error(thread, log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(logMessage)));

    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  int contextAttribs[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, Arcadia_String_toCxxInt(thread, desiredConfiguration->opengl.version.major),
    WGL_CONTEXT_MINOR_VERSION_ARB, Arcadia_String_toCxxInt(thread, desiredConfiguration->opengl.version.minor),
    WGL_CONTEXT_FLAGS_ARB, 0,
    0
  };
  self->glResourceContextHandle = wglFactoryContext->_wglCreateContextAttribs(self->systemWindow->deviceContextHandle, NULL, &contextAttribs[0]);
  if (!self->glResourceContextHandle) {
    Arcadia_StringBuilder_clear(thread, logMessage);
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, __FILE__);
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8":");
    Arcadia_StringBuilder_insertBackString(thread, logMessage, Arcadia_String_createFromCxxInt(thread, __LINE__));
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8": failed to create OpenGL/WGL context for OpenGL version ");
    Arcadia_StringBuilder_insertBackString(thread, logMessage, desiredConfiguration->opengl.version.major);
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8".");
    Arcadia_StringBuilder_insertBackString(thread, logMessage, desiredConfiguration->opengl.version.minor);
    Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8"\n");

    Arcadia_Log_error(thread, log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(logMessage)));

    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_StringBuilder_clear(thread, logMessage);
  Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, __FILE__);
  Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8":");
  Arcadia_StringBuilder_insertBackString(thread, logMessage, Arcadia_String_createFromCxxInt(thread, __LINE__));
  Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8": created OpenGL/WGL context for OpenGL version \n");
  Arcadia_StringBuilder_insertBackString(thread, logMessage, desiredConfiguration->opengl.version.major);
  Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8".");
  Arcadia_StringBuilder_insertBackString(thread, logMessage, desiredConfiguration->opengl.version.minor);
  Arcadia_StringBuilder_insertBackCxxString(thread, logMessage, u8"\n");

  Arcadia_Log_information(thread, log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(logMessage)));
}

static LRESULT CALLBACK
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_windowCallbackProcedure
  (
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_mapKeyboardKey
  (
    Arcadia_Thread* thread,
    WORD source,
    Arcadia_Engine_Input_KeyboardKey* target
  );

// @brief Update resources.
// @remmarks
// If a resource has a reference count of 0
// - unload the resource
// - unlink the resource
// - remove the resource from the list
// @return The number of removed resources
static Arcadia_SizeValue
updateResources
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* self,
    Arcadia_BooleanValue force
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_updateImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* self
  );

static Arcadia_Engine_Visuals_Implementation_Windows_Icon*
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_createIconImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* self,
    Arcadia_Media_PixelBuffer* pixelBuffer
  );

static Arcadia_Engine_Visuals_Window*
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_createWindowImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* self
  );

static Arcadia_List*
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_getDisplayDevicesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* self
  );

static void
destroyCallback
  (
    void* observer,
    void* observed
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContextDispatch* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.Implementation.OpenGL4.WGL.BackendContext", Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext,
                         u8"Arcadia.Visuals.Implementation.OpenGL4.BackendContext", Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext,
                         &_typeOperations);

static LRESULT CALLBACK
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_windowCallbackProcedure
  (
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
  )
{
  Arcadia_Process* process = NULL;
  if (Arcadia_Process_get(&process)) {
    return DefWindowProc(hWnd, uMsg, wParam, lParam); // There is nothing we can do actually.
  }
  Arcadia_Thread* thread = Arcadia_Process_getThread(process);
  Arcadia_Process_relinquish(process);
  process = NULL;
  switch (uMsg) {
    case WM_CLOSE: {
      Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* windowBackend = (Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
      if (!windowBackend) {
        return 0;
      }
      Arcadia_JumpTarget jumpTarget;
      Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
      if (Arcadia_JumpTarget_save(&jumpTarget)) {
        Arcadia_Engine_Event* event =
          (Arcadia_Engine_Event*)
            Arcadia_Engine_Visuals_WindowClosedEvent_create
              (
                thread,
                Arcadia_getTickCount(thread),
                (Arcadia_Engine_Visuals_Window*)((Arcadia_Engine_Visuals_WindowBackend*)windowBackend)->window
              );
        Arcadia_Engine_enqueEvent(thread, Arcadia_Engine_getOrCreate(thread), event);
        Arcadia_Thread_popJumpTarget(thread);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
      }
      return 0;
    } break;
    case WM_MOUSELEAVE: {
      Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* windowBackend = (Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
      if (!windowBackend) {
        return 0;
      }
      Arcadia_JumpTarget jumpTarget;
      Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
      if (Arcadia_JumpTarget_save(&jumpTarget)) {
        Arcadia_Engine_Event* event =
          (Arcadia_Engine_Event*)
          Arcadia_Engine_Input_MousePointerEvent_create
            (
              thread,
              Arcadia_getTickCount(thread),
              Arcadia_Engine_Input_MousePointerAction_Exited,
              ((Arcadia_Engine_Visuals_WindowBackend*)windowBackend)->mouse.oldx,
              ((Arcadia_Engine_Visuals_WindowBackend*)windowBackend)->mouse.oldy
            );
        Arcadia_Engine_enqueEvent(thread, Arcadia_Engine_getOrCreate(thread), event);
        Arcadia_Thread_popJumpTarget(thread);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
      }
      return 0;
    } break;
    case WM_MOUSEMOVE: {
      Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* windowBackend = (Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
      if (!windowBackend) {
        return 0;
      }
      Arcadia_JumpTarget jumpTarget;
      Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
      if (Arcadia_JumpTarget_save(&jumpTarget)) {
        LONG x = GET_X_LPARAM(lParam);
        LONG y = GET_Y_LPARAM(lParam);
        if (!((Arcadia_Engine_Visuals_WindowBackend*)windowBackend)->mouse.inWindow) {
          Arcadia_Engine_Event* event =
            (Arcadia_Engine_Event*)
            Arcadia_Engine_Input_MousePointerEvent_create
              (
                thread,
                Arcadia_getTickCount(thread),
                Arcadia_Engine_Input_MousePointerAction_Entered,
                x,
                y
              );
          Arcadia_Engine_enqueEvent(thread, Arcadia_Engine_getOrCreate(thread), event);
          ((Arcadia_Engine_Visuals_WindowBackend*)windowBackend)->mouse.oldx = x;
          ((Arcadia_Engine_Visuals_WindowBackend*)windowBackend)->mouse.oldy = y;
          ((Arcadia_Engine_Visuals_WindowBackend*)windowBackend)->mouse.inWindow = Arcadia_BooleanValue_True;
        }
        Arcadia_Engine_Event* event =
          (Arcadia_Engine_Event*)
          Arcadia_Engine_Input_MousePointerEvent_create
            (
              thread,
              Arcadia_getTickCount(thread),
              Arcadia_Engine_Input_MousePointerAction_Moved,
              x,
              y
            );
        Arcadia_Engine_enqueEvent(thread, Arcadia_Engine_getOrCreate(thread), event);
        Arcadia_Thread_popJumpTarget(thread);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
      }
      return 0;
    } break;

    case WM_LBUTTONUP: {
      Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* windowBackend = (Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
      if (!windowBackend) {
        return 0;
      }
      Arcadia_JumpTarget jumpTarget;
      Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
      if (Arcadia_JumpTarget_save(&jumpTarget)) {
        LONG x = GET_X_LPARAM(lParam);
        LONG y = GET_Y_LPARAM(lParam);
        Arcadia_Engine_Event* event =
          (Arcadia_Engine_Event*)
          Arcadia_Engine_Input_MouseButtonEvent_create
            (
              thread,
              Arcadia_getTickCount(thread),
              Arcadia_Engine_Input_MouseButtonAction_Released,
              0,
              x,
              y
            );
        Arcadia_Engine_enqueEvent(thread, Arcadia_Engine_getOrCreate(thread), event);
        Arcadia_Thread_popJumpTarget(thread);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
      }
      return 0;
    } break;
    case WM_LBUTTONDOWN: {
      Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* windowBackend = (Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
      if (!windowBackend) {
        return 0;
      }
      Arcadia_JumpTarget jumpTarget;
      Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
      if (Arcadia_JumpTarget_save(&jumpTarget)) {
        LONG x = GET_X_LPARAM(lParam);
        LONG y = GET_Y_LPARAM(lParam);
        Arcadia_Engine_Event* event =
          (Arcadia_Engine_Event*)
          Arcadia_Engine_Input_MouseButtonEvent_create
          (
            thread,
            Arcadia_getTickCount(thread),
            Arcadia_Engine_Input_MouseButtonAction_Released,
            1,
            x,
            y
          );
        Arcadia_Engine_enqueEvent(thread, Arcadia_Engine_getOrCreate(thread), event);
        Arcadia_Thread_popJumpTarget(thread);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
      }
      return 0;
    } break;

    case WM_MBUTTONUP: {
      Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* windowBackend = (Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
      if (!windowBackend) {
        return 0;
      }
      Arcadia_JumpTarget jumpTarget;
      Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
      if (Arcadia_JumpTarget_save(&jumpTarget)) {
        LONG x = GET_X_LPARAM(lParam);
        LONG y = GET_Y_LPARAM(lParam);
        Arcadia_Engine_Event* event =
          (Arcadia_Engine_Event*)
          Arcadia_Engine_Input_MouseButtonEvent_create
            (
              thread,
              Arcadia_getTickCount(thread),
              Arcadia_Engine_Input_MouseButtonAction_Released,
              1,
              x,
              y
            );
        Arcadia_Engine_enqueEvent(thread, Arcadia_Engine_getOrCreate(thread), event);
        Arcadia_Thread_popJumpTarget(thread);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
      }
      return 0;
    } break;
    case WM_MBUTTONDOWN: {
      Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* windowBackend = (Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
      if (!windowBackend) {
        return 0;
      }
      Arcadia_JumpTarget jumpTarget;
      Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
      if (Arcadia_JumpTarget_save(&jumpTarget)) {
        LONG x = GET_X_LPARAM(lParam);
        LONG y = GET_Y_LPARAM(lParam);
        Arcadia_Engine_Event* event =
          (Arcadia_Engine_Event*)
          Arcadia_Engine_Input_MouseButtonEvent_create
            (
              thread,
              Arcadia_getTickCount(thread),
              Arcadia_Engine_Input_MouseButtonAction_Pressed,
              1,
              x,
              y
            );
        Arcadia_Engine_enqueEvent(thread, Arcadia_Engine_getOrCreate(thread), event);
        Arcadia_Thread_popJumpTarget(thread);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
      }
      return 0;
    } break;

    case WM_RBUTTONUP: {
      Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* windowBackend = (Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
      if (!windowBackend) {
        return 0;
      }
      Arcadia_JumpTarget jumpTarget;
      Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
      if (Arcadia_JumpTarget_save(&jumpTarget)) {
        LONG x = GET_X_LPARAM(lParam);
        LONG y = GET_Y_LPARAM(lParam);
        Arcadia_Engine_Event* event =
          (Arcadia_Engine_Event*)
          Arcadia_Engine_Input_MouseButtonEvent_create
            (
              thread,
              Arcadia_getTickCount(thread),
              Arcadia_Engine_Input_MouseButtonAction_Released,
              2,
              x,
              y
            );
        Arcadia_Engine_enqueEvent(thread, Arcadia_Engine_getOrCreate(thread), event);
        Arcadia_Thread_popJumpTarget(thread);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
      }
      return 0;
    } break;
    case WM_RBUTTONDOWN: {
      Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* windowBackend = (Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
      if (!windowBackend) {
        return 0;
      }
      Arcadia_JumpTarget jumpTarget;
      Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
      if (Arcadia_JumpTarget_save(&jumpTarget)) {
        LONG x = GET_X_LPARAM(lParam);
        LONG y = GET_Y_LPARAM(lParam);
        Arcadia_Engine_Event* event =
          (Arcadia_Engine_Event*)
          Arcadia_Engine_Input_MouseButtonEvent_create
            (
              thread,
              Arcadia_getTickCount(thread),
              Arcadia_Engine_Input_MouseButtonAction_Pressed,
              0,
              x,
              y
            );
        Arcadia_Engine_enqueEvent(thread, Arcadia_Engine_getOrCreate(thread), event);
        Arcadia_Thread_popJumpTarget(thread);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
      }
      return 0;
    } break;

    case WM_KEYDOWN: {
      Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* windowBackend = (Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
      if (!windowBackend) {
        return 0;
      }
      Arcadia_JumpTarget jumpTarget;
      Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
      if (Arcadia_JumpTarget_save(&jumpTarget)) {
        Arcadia_Engine_Input_KeyboardKey keyboardKey;
        Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_mapKeyboardKey(thread, LOWORD(wParam), &keyboardKey);
        Arcadia_Engine_Event* event =
          (Arcadia_Engine_Event*)
            Arcadia_Engine_Input_KeyboardKeyEvent_create
              (
                thread,
                Arcadia_getTickCount(thread),
                Arcadia_Engine_Input_KeyboardKeyAction_Pressed,
                keyboardKey
              );
        Arcadia_Engine_enqueEvent(thread, Arcadia_Engine_getOrCreate(thread), event);
        Arcadia_Thread_popJumpTarget(thread);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
      }
      return 0;
    } break;
    case WM_KEYUP: {
      Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* windowBackend = (Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
      if (!windowBackend) {
        return 0;
      }
      Arcadia_JumpTarget jumpTarget;
      Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
      if (Arcadia_JumpTarget_save(&jumpTarget)) {
        Arcadia_Engine_Input_KeyboardKey keyboardKey;
        Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_mapKeyboardKey(thread, LOWORD(wParam), &keyboardKey);
        Arcadia_Engine_Event* event =
          (Arcadia_Engine_Event*)
          Arcadia_Engine_Input_KeyboardKeyEvent_create
              (
                thread,
                Arcadia_getTickCount(thread),
                Arcadia_Engine_Input_KeyboardKeyAction_Released,
                keyboardKey
              );
        Arcadia_Engine_enqueEvent(thread, Arcadia_Engine_getOrCreate(thread), event);
        Arcadia_Thread_popJumpTarget(thread);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
      }
      return 0;
    } break;
    case WM_SIZE: {
      Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* windowBackend = (Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
      if (!windowBackend) {
        return 0;
      }
      RECT rect;
      GetWindowRect(windowBackend->windowHandle, &rect);
      Arcadia_Integer32Value width = rect.right - rect.left;
      Arcadia_Integer32Value height = rect.bottom - rect.top;
      if (width != ((Arcadia_Engine_Visuals_WindowBackend*)windowBackend)->bounds.width ||
          height != ((Arcadia_Engine_Visuals_WindowBackend*)windowBackend)->bounds.height) {
        ((Arcadia_Engine_Visuals_WindowBackend*)windowBackend)->bounds.width = width;
        ((Arcadia_Engine_Visuals_WindowBackend*)windowBackend)->bounds.height = height;
        Arcadia_JumpTarget jumpTarget;
        Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
        if (Arcadia_JumpTarget_save(&jumpTarget)) {
          Arcadia_Engine_Event* event =
            (Arcadia_Engine_Event*)
              Arcadia_Engine_Visuals_WindowSizeChangedEvent_create
                (
                  thread,
                  Arcadia_getTickCount(thread),
                  (Arcadia_Engine_Visuals_Window*)((Arcadia_Engine_Visuals_WindowBackend*)windowBackend)->window,
                  width,
                  height
                );
          Arcadia_Engine_enqueEvent(thread, Arcadia_Engine_getOrCreate(thread), event);
          Arcadia_Thread_popJumpTarget(thread);
        } else {
          Arcadia_Thread_popJumpTarget(thread);
        }
      }
      return 0;
    } break;
    case WM_MOVE: {
      Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* windowBackend = (Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
      if (!windowBackend) {
        return 0; // There is nothing we can do actually.
      }
      RECT rect;
      GetWindowRect(windowBackend->windowHandle, &rect);
      Arcadia_Integer32Value left = rect.left;
      Arcadia_Integer32Value top = rect.top;
      if (left != ((Arcadia_Engine_Visuals_WindowBackend*)windowBackend)->bounds.left ||
          top != ((Arcadia_Engine_Visuals_WindowBackend*)windowBackend)->bounds.top) {
        ((Arcadia_Engine_Visuals_WindowBackend*)windowBackend)->bounds.left = left;
        ((Arcadia_Engine_Visuals_WindowBackend*)windowBackend)->bounds.top = top;
        Arcadia_JumpTarget jumpTarget;
        Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
        if (Arcadia_JumpTarget_save(&jumpTarget)) {
          Arcadia_Engine_Event* event =
            (Arcadia_Engine_Event*)
              Arcadia_Engine_Visuals_WindowPositionChangedEvent_create
                (
                  thread,
                  Arcadia_getTickCount(thread),
                  (Arcadia_Engine_Visuals_Window*)((Arcadia_Engine_Visuals_WindowBackend*)windowBackend)->window,
                  left,
                  top
                );
          Arcadia_Engine_enqueEvent(thread, Arcadia_Engine_getOrCreate(thread), event);
          Arcadia_Thread_popJumpTarget(thread);
        } else {
          Arcadia_Thread_popJumpTarget(thread);
        }
      }
      return 0;
    } break;
    default: {
      return DefWindowProc(hWnd, uMsg, wParam, lParam);
    } break;
  };
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_mapKeyboardKey
  (
    Arcadia_Thread* thread,
    WORD source,
    Arcadia_Engine_Input_KeyboardKey* target
  )
{
#define Define(Source, Target) \
  case Source: { \
    *target = Target; \
  } break;

  switch (source) {
      Define('A', Arcadia_Engine_Input_KeyboardKey_A)
      Define(VK_ACCEPT, Arcadia_Engine_Input_KeyboardKey_Accept)
      Define(VK_ADD, Arcadia_Engine_Input_KeyboardKey_Add)
      Define(VK_APPS, Arcadia_Engine_Input_KeyboardKey_Applications)

      Define('B', Arcadia_Engine_Input_KeyboardKey_B)
      Define(VK_BACK, Arcadia_Engine_Input_KeyboardKey_Backspace)

      Define(VK_BROWSER_BACK, Arcadia_Engine_Input_KeyboardKey_BrowserBack)
      Define(VK_BROWSER_FAVORITES, Arcadia_Engine_Input_KeyboardKey_BrowserFavorites)
      Define(VK_BROWSER_FORWARD, Arcadia_Engine_Input_KeyboardKey_BrowserForward)
      Define(VK_BROWSER_HOME, Arcadia_Engine_Input_KeyboardKey_BrowserHome)
      Define(VK_BROWSER_REFRESH, Arcadia_Engine_Input_KeyboardKey_BrowserRefresh)
      Define(VK_BROWSER_SEARCH, Arcadia_Engine_Input_KeyboardKey_BrowserSearch)
      Define(VK_BROWSER_STOP, Arcadia_Engine_Input_KeyboardKey_BrowserStop)

      Define('C', Arcadia_Engine_Input_KeyboardKey_C)
      Define(VK_CAPITAL, Arcadia_Engine_Input_KeyboardKey_Capital /*Arcadia_Engine_Input_KeyboardKey_CapsLock*/) // "Capital" and "CapsLock" are aliases of each other.

      Define(VK_CLEAR, Arcadia_Engine_Input_KeyboardKey_Clear)
      Define(VK_CONVERT, Arcadia_Engine_Input_KeyboardKey_Convert)

      Define('D', Arcadia_Engine_Input_KeyboardKey_D)
      Define('0', Arcadia_Engine_Input_KeyboardKey_Zero)
      Define('1', Arcadia_Engine_Input_KeyboardKey_One)
      Define('2', Arcadia_Engine_Input_KeyboardKey_Two)
      Define('3', Arcadia_Engine_Input_KeyboardKey_Three)
      Define('4', Arcadia_Engine_Input_KeyboardKey_Four)
      Define('5', Arcadia_Engine_Input_KeyboardKey_Five)
      Define('6', Arcadia_Engine_Input_KeyboardKey_Six)
      Define('7', Arcadia_Engine_Input_KeyboardKey_Seven)
      Define('8', Arcadia_Engine_Input_KeyboardKey_Eight)
      Define('9', Arcadia_Engine_Input_KeyboardKey_Nine)

      Define(VK_OEM_5, Arcadia_Engine_Input_KeyboardKey_DeadCircumflex) // "^" aka "VK_OEM_5" in WinAPI.
      Define(VK_OEM_6, Arcadia_Engine_Input_KeyboardKey_DeadGrave) // "`" aka "VK_OEM_6" in WinAPI.
      Define(VK_DECIMAL, Arcadia_Engine_Input_KeyboardKey_Decimal) // Decimal separator on NumPad
      Define(VK_DELETE, Arcadia_Engine_Input_KeyboardKey_Delete)
      Define(VK_DIVIDE, Arcadia_Engine_Input_KeyboardKey_Divide)
      Define(VK_DOWN, Arcadia_Engine_Input_KeyboardKey_DownArrow /*Arcadia_Engine_Input_KeyboardKey_Down*/) // "Down Arrow" and "Down" are aliases of each other.

      Define('E', Arcadia_Engine_Input_KeyboardKey_E)
      Define(VK_END, Arcadia_Engine_Input_KeyboardKey_End)
      Define(VK_ESCAPE, Arcadia_Engine_Input_KeyboardKey_Escape)

      Define('F', Arcadia_Engine_Input_KeyboardKey_F)
      Define(VK_F1, Arcadia_Engine_Input_KeyboardKey_F1)
      Define(VK_F10, Arcadia_Engine_Input_KeyboardKey_F10)
      Define(VK_F11, Arcadia_Engine_Input_KeyboardKey_F11)
      Define(VK_F12, Arcadia_Engine_Input_KeyboardKey_F12)
      Define(VK_F13, Arcadia_Engine_Input_KeyboardKey_F13)
      Define(VK_F14, Arcadia_Engine_Input_KeyboardKey_F14)
      Define(VK_F15, Arcadia_Engine_Input_KeyboardKey_F15)
      Define(VK_F16, Arcadia_Engine_Input_KeyboardKey_F16)
      Define(VK_F17, Arcadia_Engine_Input_KeyboardKey_F17)
      Define(VK_F18, Arcadia_Engine_Input_KeyboardKey_F18)
      Define(VK_F19, Arcadia_Engine_Input_KeyboardKey_F19)
      Define(VK_F2, Arcadia_Engine_Input_KeyboardKey_F2)
      Define(VK_F20, Arcadia_Engine_Input_KeyboardKey_F20)
      Define(VK_F21, Arcadia_Engine_Input_KeyboardKey_F21)
      Define(VK_F22, Arcadia_Engine_Input_KeyboardKey_F22)
      Define(VK_F23, Arcadia_Engine_Input_KeyboardKey_F23)
      Define(VK_F24, Arcadia_Engine_Input_KeyboardKey_F24)
      Define(VK_F3, Arcadia_Engine_Input_KeyboardKey_F3)
      Define(VK_F4, Arcadia_Engine_Input_KeyboardKey_F4)
      Define(VK_F5, Arcadia_Engine_Input_KeyboardKey_F5)
      Define(VK_F6, Arcadia_Engine_Input_KeyboardKey_F6)
      Define(VK_F7, Arcadia_Engine_Input_KeyboardKey_F7)
      Define(VK_F8, Arcadia_Engine_Input_KeyboardKey_F8)
      Define(VK_F9, Arcadia_Engine_Input_KeyboardKey_F9)
      Define(VK_FINAL, Arcadia_Engine_Input_KeyboardKey_FinalMode)

      Define('G', Arcadia_Engine_Input_KeyboardKey_G)

      Define('H', Arcadia_Engine_Input_KeyboardKey_H)

      Define(VK_HANGUL/*VK_KANA*/, Arcadia_Engine_Input_KeyboardKey_HangulMode) // "IME Hangul Mode" and "IME Hanguel Mode" are aliases of each other.
      Define(VK_HANJA/*VK_KANJI*/, Arcadia_Engine_Input_KeyboardKey_HanjaMode) // "IME Hanja Mode" and "IME Kanji Mode" are aliases of each other.
      Define(VK_HOME, Arcadia_Engine_Input_KeyboardKey_Home)

      Define('I', Arcadia_Engine_Input_KeyboardKey_I)
      Define(VK_INSERT, Arcadia_Engine_Input_KeyboardKey_Insert)


      Define('J', Arcadia_Engine_Input_KeyboardKey_J)
      Define(VK_JUNJA, Arcadia_Engine_Input_KeyboardKey_JunjaMode)	// IME Junja Mode key.

      Define('K', Arcadia_Engine_Input_KeyboardKey_K)

      Define('L', Arcadia_Engine_Input_KeyboardKey_L)
      Define(VK_LEFT, Arcadia_Engine_Input_KeyboardKey_LeftArrow /*Arcadia_Engine_Input_KeyboardKey_Left*/) // "Left Arrow" and "Left" are aliases of each other.
      Define(VK_LCONTROL, Arcadia_Engine_Input_KeyboardKey_LeftControl)
      Define(VK_LMENU, Arcadia_Engine_Input_KeyboardKey_LeftMenu)
      Define(VK_LSHIFT, Arcadia_Engine_Input_KeyboardKey_LeftShift)
      Define(VK_LWIN, Arcadia_Engine_Input_KeyboardKey_LeftWindows)

      Define('M', Arcadia_Engine_Input_KeyboardKey_M)

      Define(VK_MODECHANGE, Arcadia_Engine_Input_KeyboardKey_ModeChange)
      Define(VK_MULTIPLY, Arcadia_Engine_Input_KeyboardKey_Multiply)

      Define('N', Arcadia_Engine_Input_KeyboardKey_N)
    #if 0
      Define(NonConvert, 29, "IME NonConvert")
      Define(NumberSign, 227, "Number Sign") // "#" aka "VK_OEM_2" in WinAPI.
      Define(NumLock, "Num Lock")
    #endif
      Define(VK_NUMPAD0, Arcadia_Engine_Input_KeyboardKey_NumPadZero)
      Define(VK_NUMPAD1, Arcadia_Engine_Input_KeyboardKey_NumPadOne)
      Define(VK_NUMPAD2, Arcadia_Engine_Input_KeyboardKey_NumPadTwo)
      Define(VK_NUMPAD3, Arcadia_Engine_Input_KeyboardKey_NumPadThree)
      Define(VK_NUMPAD4, Arcadia_Engine_Input_KeyboardKey_NumPadFour)
      Define(VK_NUMPAD5, Arcadia_Engine_Input_KeyboardKey_NumPadFive)
      Define(VK_NUMPAD6, Arcadia_Engine_Input_KeyboardKey_NumPadSix)
      Define(VK_NUMPAD7, Arcadia_Engine_Input_KeyboardKey_NumPadSeven)
      Define(VK_NUMPAD8, Arcadia_Engine_Input_KeyboardKey_NumPadEight)
      Define(VK_NUMPAD9, Arcadia_Engine_Input_KeyboardKey_NumPadNine)
      Define(VK_SEPARATOR, Arcadia_Engine_Input_KeyboardKey_NumPadEnter) // NumPad Enter

      Define('O', Arcadia_Engine_Input_KeyboardKey_O)
      Define(VK_OEM_COMMA, Arcadia_Engine_Input_KeyboardKey_Comma /*Arcadia_Engine_Input_KeyboardKey_Less*/)
      Define(VK_OEM_MINUS, Arcadia_Engine_Input_KeyboardKey_Minus)

      Define('P', Arcadia_Engine_Input_KeyboardKey_P)
      Define(VK_NEXT, Arcadia_Engine_Input_KeyboardKey_PageDown)
      Define(VK_PRIOR, Arcadia_Engine_Input_KeyboardKey_PageUp)
      Define(VK_PAUSE, Arcadia_Engine_Input_KeyboardKey_Pause)
      Define(VK_OEM_PERIOD, Arcadia_Engine_Input_KeyboardKey_Period)
      Define(VK_PRINT, Arcadia_Engine_Input_KeyboardKey_Print)
      Define(VK_OEM_PLUS, Arcadia_Engine_Input_KeyboardKey_Plus)

      Define('Q', Arcadia_Engine_Input_KeyboardKey_Q)

      Define('R', Arcadia_Engine_Input_KeyboardKey_R)
      Define(VK_RCONTROL, Arcadia_Engine_Input_KeyboardKey_RightControl)
      Define(VK_RETURN, Arcadia_Engine_Input_KeyboardKey_Return)
      Define(VK_RIGHT, Arcadia_Engine_Input_KeyboardKey_RightArrow /*Arcadia_Engine_Input_KeyboardKey_Right*/) // "Right Arrow" and "Right" are aliases of each other.
      Define(VK_RMENU, Arcadia_Engine_Input_KeyboardKey_RightMenu)
      Define(VK_RSHIFT, Arcadia_Engine_Input_KeyboardKey_RightShift)
      Define(VK_RWIN, Arcadia_Engine_Input_KeyboardKey_RightWindows)

      Define('S', Arcadia_Engine_Input_KeyboardKey_S)
      Define(VK_SCROLL, Arcadia_Engine_Input_KeyboardKey_ScrollLock)
      Define(VK_SLEEP, Arcadia_Engine_Input_KeyboardKey_Sleep)
      Define(VK_SPACE, Arcadia_Engine_Input_KeyboardKey_Space)
      Define(VK_SUBTRACT, Arcadia_Engine_Input_KeyboardKey_Subtract)

      Define('T', Arcadia_Engine_Input_KeyboardKey_T)
      Define(VK_TAB, Arcadia_Engine_Input_KeyboardKey_Tabulator)

      Define('U', Arcadia_Engine_Input_KeyboardKey_U)
      Define(VK_UP, Arcadia_Engine_Input_KeyboardKey_UpArrow /*Arcadia_Engine_Input_KeyboardKey_Up*/) // "Up Arrow" and "Up" are aliases of each other.

      Define('V', Arcadia_Engine_Input_KeyboardKey_V)

      Define('W', Arcadia_Engine_Input_KeyboardKey_W)

      Define('X', Arcadia_Engine_Input_KeyboardKey_X)

      Define('Y', Arcadia_Engine_Input_KeyboardKey_Y)

      Define('Z', Arcadia_Engine_Input_KeyboardKey_Z)

    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_NotImplemented);
      Arcadia_Thread_jump(thread);
    } break;
  };

#undef Define
}

static Arcadia_SizeValue
updateResources
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* self,
    Arcadia_BooleanValue force
  )
{
  Arcadia_SizeValue removed = 0;
  Arcadia_SizeValue removedNow = 0;
  do {
    removedNow = 0;
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)((Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)self)->resources); i < n;) {
      Arcadia_Engine_Visuals_Resource* resource = (Arcadia_Engine_Visuals_Resource*)Arcadia_List_getObjectReferenceValueAt(thread, ((Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)self)->resources, i);
      if (!resource->referenceCount || force) {
        Arcadia_Engine_Visuals_Resource_unload(thread, resource);
        Arcadia_Engine_Visuals_Resource_unlink(thread, resource);
        Arcadia_List_removeAt(thread, ((Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)self)->resources, i, 1);
        n--;
        removedNow++;
      } else {
        i++;
      }
    }
    removed += removedNow;
  } while (removedNow);
  return removed;
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_updateImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* self
  )
{
  // (1) Process the messages.
  MSG msg = { 0 };
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    if (msg.message == WM_QUIT) {
      Arcadia_Engine_Event* event =
        (Arcadia_Engine_Event*)
        Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent_create
          (
            thread,
            Arcadia_getTickCount(thread)
          );
      Arcadia_Engine_enqueEvent(thread, Arcadia_Engine_getOrCreate(thread), event);
    }
  }
  // (2) Update the resources.
  updateResources(thread, self, Arcadia_BooleanValue_False);
}

static Arcadia_Engine_Visuals_Implementation_Windows_Icon*
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_createIconImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* self,
    Arcadia_Media_PixelBuffer* pixelBuffer
  )
{
  Arcadia_Engine_Visuals_Implementation_Windows_Icon* icon = Arcadia_Engine_Visuals_Implementation_Windows_Icon_create(thread, pixelBuffer);
  return icon;
}

static Arcadia_Engine_Visuals_Window*
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_createWindowImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* self
  )
{
  Arcadia_Engine_Visuals_WindowBackend* windowBackend = (Arcadia_Engine_Visuals_WindowBackend*)Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend_create(thread, self, NULL);
  Arcadia_Engine_Visuals_Window* window = Arcadia_Engine_Visuals_Window_create(thread);
  Arcadia_WeakReference* windowWeakReference =
    Arcadia_WeakReference_create
      (
        thread,
        Arcadia_Value_makeObjectReferenceValue(windowBackend)
      );
  Arcadia_List_insertBackObjectReferenceValue(thread, ((Arcadia_Engine_Visuals_BackendContext*)self)->windows, (Arcadia_Object*)windowWeakReference);
  window->backend = windowBackend;
  windowBackend->window = window;
  return window;
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->defaultWindowCallbackProcedure = &Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_windowCallbackProcedure;
  self->instanceHandle = GetModuleHandle(NULL);
  if (!self->instanceHandle) {
    self->defaultWindowCallbackProcedure = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }

  self->glDriverLibrary = LoadLibrary(u8"OPENGL32.DLL");

  if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) {
    self->instanceHandle = NULL;
    self->defaultWindowCallbackProcedure = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }

  self->className = _strdup(u8"Aracadia.Visuals.WGL.Window|WindowClass");
  if (!self->className) {
    self->instanceHandle = NULL;
    self->defaultWindowCallbackProcedure = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }

  WNDCLASS wc = { 0 };
  wc.lpfnWndProc = self->defaultWindowCallbackProcedure;
  wc.hInstance = self->instanceHandle;
  wc.lpszClassName = self->className;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Prevent the busy cursor from showing up.
  self->classAtom = RegisterClass(&wc);
  if (!self->classAtom) {
    free(self->className);
    self->className = NULL;
    self->instanceHandle = NULL;
    self->defaultWindowCallbackProcedure = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }

  self->pixelFormatIndex = -1;
  self->glResourceContextHandle = NULL;

  Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryContext* wglFactoryContext = Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryContext_create(thread);
  Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryContext_open(thread, wglFactoryContext);
  createContext(thread, self, wglFactoryContext);

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    _Linker linker = { 0 };
    linker.hLibrary = self->glDriverLibrary;
    ((_Arcadia_Engine_Visuals_Implementation_OpenGL4_Linker*)&linker)->link = (void* (*)(Arcadia_Thread*, _Arcadia_Engine_Visuals_Implementation_OpenGL4_Linker*, const char *)) & _Linker_link;
    _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions_initialize(thread, &self->_functions, (_Arcadia_Engine_Visuals_Implementation_OpenGL4_Linker*)&linker);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    // We must invalidate all the objects in the context.
    if (self->glResourceContextHandle) {
      if (self->glResourceContextHandle == wglGetCurrentContext()) {
        wglMakeCurrent(NULL, NULL);
      }
      wglDeleteContext(self->glResourceContextHandle);
      self->glResourceContextHandle = NULL;
    }
    if (self->pixelFormatIndex) {
      self->pixelFormatIndex = -1;
    }
    if (self->classAtom) {
      UnregisterClass(self->className, self->instanceHandle);
      self->classAtom = 0;
    }
    if (self->className) {
      free(self->className),
        self->className = NULL;
    }
    if (self->glDriverLibrary) {
      FreeLibrary(self->glDriverLibrary);
      self->glDriverLibrary = NULL;
    }
    if (self->instanceHandle) {
      self->instanceHandle = NULL;
    }
    if (self->defaultWindowCallbackProcedure) {
      self->defaultWindowCallbackProcedure = NULL;
    }
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContextDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContextDispatch*)self)->getFunctions = (_Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions * (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)) & getFunctionsImpl;
  ((Arcadia_Engine_Visuals_BackendContextDispatch*)self)->createIcon = (Arcadia_Engine_Visuals_Icon * (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_BackendContext*, Arcadia_Media_PixelBuffer*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_createIconImpl;
  ((Arcadia_Engine_Visuals_BackendContextDispatch*)self)->createWindow = (Arcadia_Engine_Visuals_Window * (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_BackendContext*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_createWindowImpl;
  ((Arcadia_Engine_Visuals_BackendContextDispatch*)self)->getDisplayDevices = (Arcadia_List * (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_BackendContext*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_getDisplayDevicesImpl;
  ((Arcadia_Engine_BackendContextDispatch*)self)->update = (void(*)(Arcadia_Thread*, Arcadia_Engine_BackendContext*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_updateImpl;
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* self
  )
{
  // (1) Update resources until no resources are left.
  while (updateResources(thread, self, Arcadia_BooleanValue_True))
  {/*Intentionally empty.*/}

  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions_uninitialize(thread, &self->_functions);

  if (self->glResourceContextHandle) {
    if (self->glResourceContextHandle == wglGetCurrentContext()) {
      wglMakeCurrent(NULL, NULL);
    }
    wglDeleteContext(self->glResourceContextHandle);
    self->glResourceContextHandle = NULL;
  }
  if (self->pixelFormatIndex) {
    self->pixelFormatIndex = -1;
  }
  if (self->classAtom) {
    UnregisterClass(self->className, self->instanceHandle);
    self->classAtom = 0;
  }
  if (self->className) {
    free(self->className),
    self->className = NULL;
  }
  if (self->glDriverLibrary) {
    FreeLibrary(self->glDriverLibrary);
    self->glDriverLibrary = NULL;
  }
  if (self->instanceHandle) {
    self->instanceHandle = NULL;
  }
  if (self->defaultWindowCallbackProcedure) {
    self->defaultWindowCallbackProcedure = NULL;
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* self
  )
{
  if (self->systemWindow) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->systemWindow);
  }
  // "visitation" of "resources" is not necessary as "backend context" holds a GC lock for this object.
}

static Arcadia_List*
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_getDisplayDevicesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* self
  )
{
  return _getDisplayDevices(thread);
}

static void
destroyCallback
  (
    void* argument1,
    void* argument2
  )
{
  g_instance = NULL;
}

Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext*
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext);
}

Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext*
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_getOrCreate
  (
    Arcadia_Thread* thread
  )
{
  if (!g_instance) {
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* instance = Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_create(thread);
    Arcadia_Object_addNotifyDestroyCallback(thread, (Arcadia_Object*)instance, NULL, (void (*)(void*, Arcadia_Object*)) &destroyCallback);
    g_instance = instance;
  }
  return g_instance;
}
