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

#if !defined(ARCADIA_ENGINE_BACKEND_H_INCLUDED)
#define ARCADIA_ENGINE_BACKEND_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif
#include "Arcadia/Engine/BackendContext.h"

// Arcadia.Engine.Backend objects represent backends like "OpenGL" or "OpenAL".
// Arcadia.Engine provides a list of the available backends.
// Arcadia.Engine.Backend allow for the creation of contexts to backends via Arcadia.Engine.Backend.createBackendContext(...) : Arcadia.Engine.BackendContext.
// A Arcadia.Engine.BackendContext is a heavyweight object, potentially occupying contested system resources like audio devices or video devices.
Arcadia_declareObjectType(u8"Arcadia.Engine.Backend", Arcadia_Engine_Backend,
                          u8"Arcadia.Object");

struct Arcadia_Engine_BackendDispatch {
  Arcadia_ObjectDispatch _parent;

  Arcadia_String* (*getName)(Arcadia_Thread*, Arcadia_Engine_Backend*);
  Arcadia_Engine_BackendContext* (*createBackendContext)(Arcadia_Thread*, Arcadia_Engine_Backend*);
};

struct Arcadia_Engine_Backend {
  Arcadia_Object _parent;
};

/// @brief Get the name of this backend.
/// @param thread A pointer to this thread.
/// @param self A pointer to this window.
Arcadia_String*
Arcadia_Engine_Backend_getName
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Backend* self
  );

/// @brief Create a backend context of this backend.
/// @papram self A pointer to this backend.
/// @return A Arcadia_Engine_BackendContext to the system.
Arcadia_Engine_BackendContext*
Arcadia_Engine_Backend_createBackendContext
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Backend* self
  );

#endif // ARCADIA_ENGINE_BACKEND_H_INCLUDED
