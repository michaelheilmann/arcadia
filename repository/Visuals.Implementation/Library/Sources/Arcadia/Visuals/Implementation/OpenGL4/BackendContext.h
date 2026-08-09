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

#if !defined(ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_BACKENDCONTEXT_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_BACKENDCONTEXT_H_INCLUDED

#include "Arcadia/Engine/Include.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/Functions.h"

/// Base of all OpenGL4 backend contexts.
Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.Implementation.OpenGL4.BackendContext", Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext,
                          u8"Arcadia.Engine.Visuals.BackendContext");

struct Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContextDispatch {
  Arcadia_Engine_Visuals_BackendContextDispatch _parent;

  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions*
  (*getFunctions)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* self
    );
};

struct Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext {
  Arcadia_Engine_Visuals_BackendContext _parent;

  // The list of resources.
  Arcadia_List* resources;
};

_Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions*
Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* self
  );

#endif // ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_BACKENDCONTEXT_H_INCLUDED
