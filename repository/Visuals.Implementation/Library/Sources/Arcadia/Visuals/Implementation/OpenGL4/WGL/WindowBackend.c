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

#include "Arcadia/Visuals/Implementation/OpenGL4/WGL/WindowBackend.h"

#include "Arcadia/Visuals/Implementation/Windows/_WindowText.h"
#include <limits.h>

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackendDispatch* self
  );

static void
openImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  );

static void
closeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  );

static Arcadia_BooleanValue
getFullscreenImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  );

static void
setFullscreenImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_BooleanValue fullscreen
  );

static void
getRequiredBigIconSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  );

static void
getRequiredSmallIconSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  );

static Arcadia_Engine_Visuals_Implementation_Windows_Icon*
getBigIconImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  );

static void
setBigIconImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_Engine_Visuals_Implementation_Windows_Icon* icon
  );

static Arcadia_Engine_Visuals_Implementation_Windows_Icon*
getSmallIconImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  );

static void
setSmallIconImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_Engine_Visuals_Implementation_Windows_Icon* icon
  );

static Arcadia_String*
getTitleImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  );

static void
setTitleImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_String* title
  );

static void
getCanvasSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  );

static void
setPixelFormat
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* backendContext
  );

static void
beginRenderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  );

static void
endRenderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  );

static void
getPositionImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_Integer32Value* left,
    Arcadia_Integer32Value* top
  );

static void
setPositionImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value top
  );

static void
getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  );

static void
setSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.Implementation.OpenGL4.WGL.WindowBackend", Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend,
                         u8"Arcadia.Visuals.WindowBackend", Arcadia_Engine_Visuals_WindowBackend,
                         &_typeOperations);

static Arcadia_BooleanValue
filter
  (
    Arcadia_Thread* thread,
    Arcadia_Value context,
    Arcadia_Value value
  )
{
  Arcadia_Engine_Visuals_WindowBackend* windowBackend = (Arcadia_Engine_Visuals_WindowBackend*)Arcadia_Value_getObjectReferenceValue(&context);
  Arcadia_WeakReference* weakReferenceElement = (Arcadia_WeakReference*)Arcadia_Value_getObjectReferenceValue(&value);
  Arcadia_Value valueElement = Arcadia_WeakReference_getValue(thread, weakReferenceElement);
  if (Arcadia_Value_isObjectReferenceValue(&valueElement)) {
    Arcadia_Engine_Visuals_WindowBackend* windowBackendElement = (Arcadia_Engine_Visuals_WindowBackend*)Arcadia_Value_getObjectReferenceValue(&valueElement);
    if (windowBackend == windowBackendElement) {
      return Arcadia_BooleanValue_False;
    }
  } else {
    return Arcadia_BooleanValue_False;
  }
  return Arcadia_BooleanValue_True;
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  )
{
  if (NULL != self->deviceContextHandle) {
    ReleaseDC(self->windowHandle, self->deviceContextHandle);
    self->deviceContextHandle = NULL;
  }
  if (NULL != self->windowHandle) {
    SetWindowLongPtr(self->windowHandle, GWLP_USERDATA, (LONG_PTR)NULL);
    DeleteObject(self->windowHandle);
    self->windowHandle = NULL;
  }
  Arcadia_List_filter(thread, ((Arcadia_Engine_Visuals_BackendContext*)self->backendContext)->windows, Arcadia_Value_makeObjectReferenceValue(self), &filter);
  if (NULL != self->backendContext) {
    Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
    self->backendContext = NULL;
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  )
{
  if (self->backendContext) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->backendContext);
  }
  if (self->smallIcon) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->smallIcon);
  }
  if (self->bigIcon) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->bigIcon);
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->backendContext = NULL;
  self->windowHandle = NULL;
  self->deviceContextHandle = NULL;
  self->bigIcon = NULL;
  self->smallIcon = NULL;

  self->backendContext = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_getType(thread));
  Arcadia_Object_lock(thread, (Arcadia_Object*)self->backendContext);
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackendDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->open = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*)) & openImpl;
  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->close = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*)) & closeImpl;

  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->getRequiredBigIconSize = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_Integer32Value*, Arcadia_Integer32Value*)) & getRequiredBigIconSizeImpl;
  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->getRequiredSmallIconSize = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_Integer32Value*, Arcadia_Integer32Value*)) & getRequiredSmallIconSizeImpl;

  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->getBigIcon = (Arcadia_Engine_Visuals_Icon *(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*)) & getBigIconImpl;
  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->setBigIcon = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_Engine_Visuals_Icon*)) & setBigIconImpl;

  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->getSmallIcon = (Arcadia_Engine_Visuals_Icon *(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*)) & getSmallIconImpl;
  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->setSmallIcon = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_Engine_Visuals_Icon*)) & setSmallIconImpl;

  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->getTitle = (Arcadia_String*(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*)) & getTitleImpl;
  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->setTitle = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_String*)) & setTitleImpl;

  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->getCanvasSize = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_Integer32Value*, Arcadia_Integer32Value*)) & getCanvasSizeImpl;

  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->beginRender = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*))&beginRenderImpl;
  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->endRender = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*))&endRenderImpl;

  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->getPosition = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_Integer32Value*, Arcadia_Integer32Value*)) & getPositionImpl;
  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->setPosition = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_Integer32Value, Arcadia_Integer32Value)) & setPositionImpl;

  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->getSize = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_Integer32Value*, Arcadia_Integer32Value*)) & getSizeImpl;
  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->setSize = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_Integer32Value, Arcadia_Integer32Value)) & setSizeImpl;

  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->getFullscreen = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*)) & getFullscreenImpl;
  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->setFullscreen = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_BooleanValue)) & setFullscreenImpl;
}

static void
openImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  )
{
  if (self->windowHandle) {
    return;
  }

  if (!self->windowHandle) {
    DWORD dwStyle = WS_OVERLAPPEDWINDOW;
    DWORD dwExStyle = 0;
    self->windowHandle =
      CreateWindowEx
        (
          dwExStyle,                            // extended window style
          self->backendContext->className,      // window class
          u8"Arcadia Engine Window",            // window text
          dwStyle,                              // window style
          CW_USEDEFAULT,                        // left
          CW_USEDEFAULT,                        // top
          CW_USEDEFAULT,                        // width
          CW_USEDEFAULT,                        // height
          NULL,                                 // parent window
          NULL,                                 // menu
          self->backendContext->instanceHandle, // instance handle
          NULL                                  // additional application data
        );

    if (!self->windowHandle) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    SetWindowLongPtr(self->windowHandle, GWLP_USERDATA, (LONG_PTR)self);
    MoveWindow(self->windowHandle, ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.left,
                                   ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.top,
                                   ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.width,
                                   ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.height,
                                   TRUE);
  }

  self->deviceContextHandle = GetDC(self->windowHandle);
  if (!self->deviceContextHandle) {
    DestroyWindow(self->windowHandle);
    self->windowHandle = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    _setWindowText(thread, self->windowHandle, ((Arcadia_Engine_Visuals_WindowBackend*)self)->title);
    setPixelFormat(thread, self, self->backendContext);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    ReleaseDC(self->windowHandle, self->deviceContextHandle);
    self->deviceContextHandle = NULL;
    DestroyWindow(self->windowHandle);
    self->windowHandle = NULL;
    Arcadia_Thread_jump(thread);
  }

  ShowWindow(self->windowHandle, SW_SHOW);
}

static void
closeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  )
{
  if (!self->windowHandle) {
    return;
  }
  if (self->deviceContextHandle) {
    ReleaseDC(self->windowHandle, self->deviceContextHandle);
    self->deviceContextHandle = NULL;
  }
  if (self->windowHandle) {
    SetWindowLongPtr(self->windowHandle, GWLP_USERDATA, (LONG_PTR)NULL);
    DestroyWindow(self->windowHandle);
    self->windowHandle = NULL;
  }
}

static Arcadia_BooleanValue
getFullscreenImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  )
{ return ((Arcadia_Engine_Visuals_WindowBackend*)self)->fullscreen; }

static void
setFullscreenImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_BooleanValue fullscreen
  )
{
  if (self->windowHandle) {
    if (((Arcadia_Engine_Visuals_WindowBackend*)self)->fullscreen != fullscreen) {
      DWORD dwStyle = 0;
      if (fullscreen) {
        dwStyle |= WS_POPUP;
      } else {
        dwStyle |= WS_OVERLAPPEDWINDOW;
      }
      SetWindowLongPtr(self->windowHandle, GWL_STYLE, dwStyle);
      if (!SetWindowPos(self->windowHandle,
                        HWND_TOPMOST,
                        ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.left,
                        ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.top,
                        ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.width,
                        ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.height,
                        SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_DRAWFRAME | SWP_SHOWWINDOW)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
      ((Arcadia_Engine_Visuals_WindowBackend*)self)->fullscreen = fullscreen;
    }
  } else {
    ((Arcadia_Engine_Visuals_WindowBackend*)self)->fullscreen = fullscreen;
  }
}

static void
getRequiredBigIconSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{
  *width = GetSystemMetrics(SM_CXICON);
  *height = GetSystemMetrics(SM_CYICON);
}

static void
getRequiredSmallIconSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{
  *width = GetSystemMetrics(SM_CXSMICON);
  *height = GetSystemMetrics(SM_CYSMICON);
}

static Arcadia_Engine_Visuals_Implementation_Windows_Icon*
getBigIconImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  )
{ return self->bigIcon; }

static void
setBigIconImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_Engine_Visuals_Implementation_Windows_Icon* icon
  )
{
  self->bigIcon = icon;
  if (self->windowHandle) {
    if (self->bigIcon) {
      SendMessage(self->windowHandle, WM_SETICON, ICON_BIG, (LPARAM)self->bigIcon->hIcon);
    } else {
      SendMessage(self->windowHandle, WM_SETICON, ICON_BIG, (LPARAM)NULL);
    }
  }
}

static Arcadia_Engine_Visuals_Implementation_Windows_Icon*
getSmallIconImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  )
{ return self->smallIcon; }

static void
setSmallIconImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_Engine_Visuals_Implementation_Windows_Icon* icon
  )
{
  self->smallIcon = icon;
  if (self->windowHandle) {
    if (self->smallIcon) {
      SendMessage(self->windowHandle, WM_SETICON, ICON_SMALL, (LPARAM)self->smallIcon->hIcon);
    } else {
      SendMessage(self->windowHandle, WM_SETICON, ICON_SMALL, (LPARAM)NULL);
    }
  }
}

static Arcadia_String*
getTitleImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  )
{ return ((Arcadia_Engine_Visuals_WindowBackend*)self)->title; }

static void
setTitleImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_String* title
  )
{
  if (!title) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  ((Arcadia_Engine_Visuals_WindowBackend*)self)->title = title;
  if (self->windowHandle) {
    _setWindowText(thread, self->windowHandle, title);
  }
}

static void
getCanvasSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{
  if (!self->windowHandle) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  RECT clientRectangle;
  if (!GetClientRect(self->windowHandle, &clientRectangle)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_StaticAssert(LONG_MAX <= INT32_MAX, "<internal error>");
  *width = clientRectangle.right - clientRectangle.left;
  *height = clientRectangle.bottom - clientRectangle.top;
}

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
setPixelFormat
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* backendContext
  )
{
  PIXELFORMATDESCRIPTOR pixelFormatDescriptors;
  if (!DescribePixelFormat(self->deviceContextHandle, backendContext->pixelFormatIndex, sizeof(pixelFormatDescriptors), &pixelFormatDescriptors)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  if (!SetPixelFormat(self->deviceContextHandle, backendContext->pixelFormatIndex, &pixelFormatDescriptors)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
}

static void
beginRenderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  )
{
  if (!wglMakeCurrent(self->deviceContextHandle, self->backendContext->glResourceContextHandle)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
}

static void
endRenderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self
  )
{
  if (self->backendContext->glResourceContextHandle == wglGetCurrentContext()) {
    if (!SwapBuffers(self->deviceContextHandle)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
  }
}

static void
getPositionImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_Integer32Value* left,
    Arcadia_Integer32Value* top
  )
{
  *left = ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.top;
  *top = ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.left;
}

static void
setPositionImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value top
  )
{
  ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.left = left;
  ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.top = top;
  if (self->windowHandle) {
    MoveWindow(self->windowHandle, ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.left,
                                   ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.top,
                                   ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.width,
                                   ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.height,
                                   FALSE);
  }
}

static void
getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{
  *width = ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.width;
  *height = ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.height;
}

static void
setSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.width = width;
  ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.height = height;
  if (self->windowHandle) {
    MoveWindow(self->windowHandle, ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.left,
                                    ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.top,
                                    ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.width,
                                    ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.height,
                                    FALSE);
  }
}

Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend*
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* backendContext,
    Arcadia_Engine_Visuals_Windows_DisplayDevice* displayDevice
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend);
  if (backendContext) { Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); } else { Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);  }
  if (displayDevice) { Arcadia_ValueStack_pushObjectReferenceValue(thread, displayDevice); } else { Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void); }
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend);
}
