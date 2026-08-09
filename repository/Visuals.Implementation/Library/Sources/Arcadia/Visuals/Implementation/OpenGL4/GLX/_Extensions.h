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

#if !defined(ARCADIA_VISUALS_IMPLEMENTATION_OPENGL4_GLX__EXTENSIONS_H_INCLUDED)
#define ARCADIA_VISUALS_IMPLEMENTATION_OPENGL4_GLX__EXTENSIONS_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Collections/Include.h"
#include <X11/Xlib.h>

/// Obtain the set of GLX extension strings.
/// @remarks The GLX extension strings are obtained via a call to
/// glXQueryExtensionsString. That call returns the GLX extension
/// strings in a single string separated by whitespaces. A GLX
/// extension string itself does not contain whitespaces.
Arcadia_Set*
Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_getExtensions
  (
    Arcadia_Thread* thread,
    Display* display
  );

/// Obtain the set of GLX extension strings.
/// Check if the specified extension string is contained in that set.
Arcadia_BooleanValue
Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_hasExtension
  (
    Arcadia_Thread* thread,
    Display* display,
    Arcadia_String* extension
  );

#endif // ARCADIA_VISUALS_IMPLEMENTATION_OPENGL4_GLX__EXTENSIONS_H_INCLUDED
