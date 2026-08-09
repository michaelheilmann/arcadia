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

#if !defined(ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_WGL_WINDOWBACKEND_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_WGL_WINDOWBACKEND_H_INCLUDED

#include "Arcadia/Visuals/Implementation/OpenGL4/WGL/BackendContext.h"
#include "Arcadia/Visuals/Implementation/Windows/Icon.h"
typedef struct Arcadia_Engine_Visuals_Windows_DisplayDevice Arcadia_Engine_Visuals_Windows_DisplayDevice;

#if !defined(WIN32_LEAN_AND_MEAN)
  #define WIN32_LEAN_AND_MEAN
#endif
#if !defined(NOCOMM)
  #define NOCOMM
#endif
#include <Windows.h>

Arcadia_declareObjectType(u8"Arcadia.Visuals.Implementation.OpennGL4.WGL.WindowBackend", Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend,
                          u8"Arcadia.Visuals.WindowBackend");

struct Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackendDispatch {
  Arcadia_Engine_Visuals_WindowBackendDispatch _parent;
};

struct Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend {
  Arcadia_Engine_Visuals_WindowBackend _parent;
  Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* backendContext;

  HWND windowHandle;
  HDC deviceContextHandle;

  Arcadia_Engine_Visuals_Implementation_Windows_Icon* smallIcon;
  Arcadia_Engine_Visuals_Implementation_Windows_Icon* bigIcon;
};

Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend*
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_WindowBackend_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext* backendContext,
    Arcadia_Engine_Visuals_Windows_DisplayDevice* displayDevice
  );

#endif // ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_WGL_WINDOWBACKEND_H_INCLUDED
