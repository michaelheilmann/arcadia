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

#if !defined(ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_WGL_FUNCTIONS_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_WGL_FUNCTIONS_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

typedef struct _Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_Functions _Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_Functions;

struct _Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_Functions {
  int dummy;
#define Define(Type, Name) \
  Type Name;
#include "Arcadia/Visuals/Implementation/OpenGL4/WGL/Functions.i"
#undef Define
};

/// @error functions is a null pointer
void
_Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_Functions_initialize
  (
    Arcadia_Thread* thread,
    _Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_Functions* functions
  );

/// @error functions is a null pointer
void
_Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_Functions_uninitialize
  (
    Arcadia_Thread* thread,
    _Arcadia_Engine_Visuals_Implementation_OpenGL4_WGL_Functions* functions
  );

#endif // ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_WGL_FUNCTIONS_H_INCLUDED
