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

#if !defined(ARCADIA_VISUALS_IMPLEMENTATION_OPENGL4_GLX_BACKENDCONTEXT_H_INCLUDED)
#define ARCADIA_VISUALS_IMPLEMENTATION_OPENGL4_GLX_BACKENDCONTEXT_H_INCLUDED

#include "Arcadia/Engine/Include.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/BackendContext.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/Functions.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/GLX/Functions.h"
#include <X11/Xlib.h> // For Atom.
#include <GL/glx.h> // For XVisualInfo and GLXFBConfig.

/// @code
/// class Arcadia.Visuals.OpenGL4.GLX.BackendContext extends Arcadia.Visuals.BackendContext {
///   construct()
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.Visuals.Implementation.OpenGL4.GLX.BackendContext", Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_BackendContext,
                          u8"Arcadia.Visuals.Implementation.OpenGL4.BackendContext")

struct Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_BackendContextDispatch {
  Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContextDispatch _parent;
};

struct Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_BackendContext {
  Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext _parent;

  // The display used by the OpenGL/GLX context and the X11 windows.
  Display* display;

  Atom _NET_WM_ICON;
  Atom WM_DELETE_WINDOW;
  Atom _NET_FRAME_EXTENTS;
  Atom _MOTIF_WM_HINTS;

  // See https://linux.die.net/man/3/xrandr for more information.
  struct {
    /// The base of the event IDs of the XRANDR extension.
    int eventBase;
    /// The base of the error IDs of the XRANDR extension.
    int errorBase;
  } xrandr;

  // The visuals used by the OpenGL/GLX context and the X11 windows.
  // Default value is NULL.
  XVisualInfo *visualInfo;
  // The frame buffer configuration used by the OpenGL/GLX context and the X11 windows.
  // Default value is NULL.
  GLXFBConfig *frameBufferConfiguration;
  // The color map used by the OpenGL/GLX context and the X11 windows.
  // Default value is None.
  Colormap colormap;
  // The OpenGL/GLX context.
  // Default value is NULL.
  GLXContext context;

  /// The GLX functions available to us.
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_Functions functionsGLX;
  /// The OpenGL functions available to us.
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions _functions;
};

Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_BackendContext*
Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_BackendContext_create
  (
    Arcadia_Thread* thread
  );

Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_BackendContext*
Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_BackendContext_getOrCreate
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_VISUALS_IMPLEMENTATION_OPENGL4_GLX_BACKENDCONTEXT_H_INCLUDED
