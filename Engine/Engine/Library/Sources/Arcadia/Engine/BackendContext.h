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

#if !defined(ARCADIA_ENGINE_BACKENDCONTEXT_H_INCLUDED)
#define ARCADIA_ENGINE_BACKENDCONTEXT_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"

/// @brief
/// The base of all backend context.
///
/// @details
/// A backend context is either an "audials" backend context (Arcadia.Engine.Audials.BackendContext) or a "visuals" backend context (Arcadia.Engine.Visuals.BackendContext).
/// The engine maintains pointers to the active backend context of the respective type.
///
/// The attempt to create a backend context of a type with an active of that type is an error.
/// Systems check that by inspecting the pointers to active backend context maintained by the engine.
///
/// One can first create a system of a type, shut it down, and create a system of another type.
/// For example, one can create a OpenGL backend context, shut it down, and create an Vulkan visuals system.
///
/// @details
/// @code
/// class Arcadia.Engine.BackendContext {
///
///   construct
///     (
///     )
///
///   method
///   update
///     (
///     )
///     : Arcadia.Void
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.Engine.BackendContext", Arcadia_Engine_BackendContext,
                          u8"Arcadia.Object")

struct Arcadia_Engine_BackendContextDispatch {
  Arcadia_ObjectDispatch _parent;
  void (*update)(Arcadia_Thread* thread, Arcadia_Engine_BackendContext* self);
};

struct Arcadia_Engine_BackendContext {
  Arcadia_Object _parent;
};

/// @brief Update this backend context.
/// @param thread A pointer to this thread.
/// @param self A pointer to this backend context.
void
Arcadia_Engine_BackendContext_update
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_BackendContext* self
  );

#endif // ARCADIA_ENGINE_BACKENDCONTEXT_H_INCLUDED
