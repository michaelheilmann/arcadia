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

#include "Arcadia/Visuals/Implementation/OpenGL4/WGL/SystemWindow.h"

#include <stdio.h> // @todo Remove references to `stdio.h`.

#define WindowClassName "Arcadia.Visuals.OpenGL4.WGL.FactoryWindow"
#define WindowTitle "Arcadia OpenGL 4 WGL Factory Window"

static LRESULT CALLBACK
WindowProc
  (
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindowDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow_destruct,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.Implementation.OpenGL4.WGL.SystemWindow", Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static LRESULT CALLBACK
WindowProc
  (
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
  )
{ return DefWindowProc(hWnd, uMsg, wParam, lParam); }

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow* self
  )
{
  if (self->deviceContextHandle) {
    ReleaseDC(self->windowHandle, self->deviceContextHandle);
    self->deviceContextHandle = NULL;
  }
  if (self->windowHandle) {
    DestroyWindow(self->windowHandle);
    self->windowHandle = NULL;
  }
  if (self->instanceHandle) {
    UnregisterClass(WindowClassName, self->instanceHandle);
    self->instanceHandle = NULL;
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->instanceHandle = NULL;
  self->classAtom = 0;
  self->windowHandle = NULL;
  self->deviceContextHandle = NULL;
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindowDispatch* self
  )
{/*Intentionally empty.*/}

Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow*
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow);
}

void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow_open
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow* self
  )
{
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);


  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    self->instanceHandle = GetModuleHandleA(NULL);
    if (!self->instanceHandle) {
      fprintf(stderr, "%s:%d: failed to acquire module handle\n", __FILE__, __LINE__);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }

    WNDCLASSEXA wcex;
    if (!GetClassInfoEx(self->instanceHandle, WindowClassName, &wcex)) {
      memset(&wcex, 0, sizeof(wcex));
      wcex.cbSize = sizeof(WNDCLASSEX);
      wcex.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
      wcex.lpfnWndProc = (WNDPROC)WindowProc;
      wcex.cbClsExtra = 0;
      wcex.cbWndExtra = 0;
      wcex.hInstance = GetModuleHandle(NULL);
      wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO);
      wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
      wcex.hbrBackground = NULL;
      wcex.lpszMenuName = NULL;
      wcex.lpszClassName = WindowClassName;
      wcex.hIconSm = NULL;
      self->classAtom = RegisterClassExA(&wcex);
      if (!self->classAtom) {
        fprintf(stderr, "%s:%d: failed to register window class\n", __FILE__, __LINE__);
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
    }

    self->windowHandle = CreateWindowA(WindowClassName,
                                       WindowTitle,
                                       WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                                       CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                       NULL,
                                       NULL,
                                       self->instanceHandle,
                                       NULL);
    if (!self->windowHandle) {
      fprintf(stderr, "%s:%d: failed to create window\n", __FILE__, __LINE__);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    self->deviceContextHandle = GetDC(self->windowHandle);
    if (!self->deviceContextHandle) {
      fprintf(stderr, "%s:%d: failed to create device context\n", __FILE__, __LINE__);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (self->deviceContextHandle) {
      ReleaseDC(self->windowHandle, self->deviceContextHandle);
      self->deviceContextHandle = NULL;
    }
    if (self->windowHandle) {
      DestroyWindow(self->windowHandle);
      self->windowHandle = NULL;
    }
    if (self->classAtom) {
      UnregisterClass(WindowClassName, self->instanceHandle);
      self->classAtom = 0;
    }
    if (self->instanceHandle) {
      self->instanceHandle = NULL;
    }
    Arcadia_Thread_jump(thread);
  }
}

void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow_close
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow* self
  )
{
  if (self->deviceContextHandle) {
    ReleaseDC(self->windowHandle, self->deviceContextHandle);
    self->deviceContextHandle = NULL;
  }
  if (self->windowHandle) {
    DestroyWindow(self->windowHandle);
    self->windowHandle = NULL;
  }
  if (self->classAtom) {
    UnregisterClass(WindowClassName, self->instanceHandle);
    self->classAtom = 0;
  }
  if (self->instanceHandle) {
    self->instanceHandle = NULL;
  }
}
