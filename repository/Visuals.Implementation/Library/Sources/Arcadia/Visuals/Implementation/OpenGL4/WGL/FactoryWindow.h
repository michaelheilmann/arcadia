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

#if !defined(ARCADIA_VISUALS_OPENGL_WGL_FACTORYWINDOW_H_INCLUDED)
#define ARCADIA_VISUALS_OPENGL_WGL_FACTORYWINDOW_H_INCLUDED

#include "Arcadia/Engine/Include.h"
#if !defined(WIN32_LEAN_AND_MEAN)
  #define WIN32_LEAN_AND_MEAN
#endif
#if !defined(NOCOMM)
  #define NOCOMM
#endif
#include <Windows.h>

// A temporary window used to create old-style contexts.
Arcadia_declareObjectType(u8"Arcadia.Visuals.Implementation.OpenGL4.WGL.FactoryWindow", Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryWindow,
                          u8"Arcadia.Object");

struct Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryWindowDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryWindow {
  Arcadia_Object parent;
  HINSTANCE instanceHandle;
  ATOM classAtom;
  HWND windowHandle;
  HDC deviceContextHandle;
};

Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryWindow*
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryWindow_create
  (
    Arcadia_Thread* thread
  );

void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryWindow_open
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryWindow* self
  );

void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryWindow_close
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryWindow* self
  );

#endif // ARCADIA_VISUALS_OPENGL_WGL_FACTORYWINDOW_H_INCLUDED
