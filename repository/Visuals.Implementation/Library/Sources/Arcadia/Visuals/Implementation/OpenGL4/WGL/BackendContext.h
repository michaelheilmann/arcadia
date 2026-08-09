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

#if !defined(ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_WGL_BACKENDCONTEXT_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_WGL_BACKENDCONTEXT_H_INCLUDED

#include "Arcadia/Visuals/Implementation/OpenGL4/BackendContext.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/WGL/SystemWindow.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/Functions.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/WGL/Functions.h"
typedef struct Arcadia_Engine_Visuals_Implementation_NodeFactory Arcadia_Engine_Visuals_Implementation_NodeFactory;

#if !defined(WIN32_LEAN_AND_MEAN)
  #define WIN32_LEAN_AND_MEAN
#endif
#if !defined(NOCOMM)
  #define NOCOMM
#endif
#include <Windows.h>

/// The system maintains
/// - a window class for all window
/// - a pixel format for all window
/// - a system window (with the default window classs)
/// - an HGLRC window (compatible with the system window)
///   This context is attached to actual windows on demand
/// - an OpenGL4 context
///
/// @code
/// class Arcadia.Visuals.Implementation.OpenGL4.WGL.BackendContext extends Arcadia.Visuals.BackendContext {
///   constructor()
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.Implementation.OpenGL4.WGL.BackendContext", Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext,
                          u8"Arcadia.Engine.Visuals.Implementation.OpenGL4.BackendContext");

struct Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContextDispatch {
  Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContextDispatch _parent;
};

struct Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext {
  Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext _parent;
  // The instance handle of the process.
  HINSTANCE instanceHandle;
  // The atom of the window class used for window backends.
  ATOM classAtom;
  // The name of the window class used for window backends.
  char* className;
  // The window callback procedure of the window class used for window backends.
  WNDPROC defaultWindowCallbackProcedure;
  // The WGL/OpenGL system-wide pixel format indexused for window backends.
  int pixelFormatIndex;
  // The WGL/OpenGL system-wide window.
  Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_SystemWindow* systemWindow;
  // The WGL/OpenGL system-wide resource context handle.
  HGLRC glResourceContextHandle;
  // The WGL/OpenGL driver library handle.That is, the handle to OPENGL32.DLL.
  HANDLE glDriverLibrary;

  /// The WGL functions available to us.
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_Functions _functionsWGL;
  /// The OpenGL functions available to us.
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions _functions;
};

Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext*
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_create
  (
    Arcadia_Thread* thread
  );

Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext*
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_BackendContext_getOrCreate
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_WGL_BACKENDCONTEXT_H_INCLUDED
