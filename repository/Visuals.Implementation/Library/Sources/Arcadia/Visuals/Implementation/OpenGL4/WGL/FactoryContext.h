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

#if !defined(ARCADIA_VISUALS_OPENGL_WGL_FACTORYCONTEXT_H_INCLUDED)
#define ARCADIA_VISUALS_OPENGL_WGL_FACTORYCONTEXT_H_INCLUDED

#include "Arcadia/Visuals/Implementation/OpenGL4/WGL/FactoryWindow.h"
#include "Arcadia/Engine/Include.h"

#if !defined(WIN32_LEAN_AND_MEAN)
  #define WIN32_LEAN_AND_MEAN
#endif
#if !defined(NOCOMM)
  #define NOCOMM
#endif
#include <GL/glcorearb.h>
#include <GL/wglext.h>

// An old style context. Provides means to enumerate new style context configurations and create new style contexts.
Arcadia_declareObjectType(u8"Arcadia.Visuals.Implementation.OpenGL4.WGL.FactoryContext", Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryContext,
                          u8"Arcadia.Object");

struct Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryContextDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryContext {
  Arcadia_Object parent;
  Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryWindow* window;
  HGLRC glResourceContextHandle;

  PFNWGLCHOOSEPIXELFORMATARBPROC _wglChoosePixelFormat; // wglChoosePixelFormat(ARB|EXT)
  PFNWGLCREATECONTEXTATTRIBSARBPROC _wglCreateContextAttribs; // wglCreateContextAttribs(ARB|EXT)
  PFNWGLGETEXTENSIONSSTRINGARBPROC _wglGetExtensionsString; // wglGetExtensionString(ARB|EXT)
  PFNWGLGETPIXELFORMATATTRIBIVARBPROC _wglGetPixelFormatAttribiv; // wglGetPixelFormatAttribiv(ARB|EXT)
  PFNWGLGETPIXELFORMATATTRIBFVARBPROC _wglGetPixelFormatAttribfv; // wglGetPixelFormatAttribiv(ARB|EXT)
};

Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryContext*
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryContext_create
  (
    Arcadia_Thread* thread
  );

void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryContext_open
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryContext* self
  );

void
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryContext_close
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryContext* self
  );

/// @brief Get all available configurations.
/// @param thread A pointer to this thread.
/// @param self This OpenGL/WGL device information.
/// @return A list of Arcadia_Visuals_Configuration objects.
Arcadia_List*
Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryContext_getConfigurations
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_FactoryContext* self
  );

#endif // ARCADIA_VISUALS_OPENGL_WGL_FACTORYCONTEXT_H_INCLUDED
