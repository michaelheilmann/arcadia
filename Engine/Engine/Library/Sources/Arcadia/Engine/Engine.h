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

#if !defined(ARCADIA_ENGINE_ENGINE_H_INCLUDED)
#define ARCADIA_ENGINE_ENGINE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Collections/Include.h"
typedef struct Arcadia_Engine_Event Arcadia_Engine_Event;
typedef struct Arcadia_Engine_NodeFactory Arcadia_Engine_NodeFactory;
typedef struct Arcadia_Engine_BackendContext Arcadia_Engine_BackendContext;

/// @code
/// class Arcadia.Engine {
///   construct
///     (
///     )
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.Engine", Arcadia_Engine,
                          u8"Arcadia.Object")

struct Arcadia_EngineDispatch {
  Arcadia_ObjectDispatch _parent;

  void (*update)(Arcadia_Thread* thread, Arcadia_Engine* self);
};

struct Arcadia_Engine {
  Arcadia_Object _parent;

  /// Set of Arcadia.Engine.Visuals.Backend derived type objects.
  Arcadia_Set* visualsBackendTypes;
  /// Set of Arcadia.Engine.Visuals.NodeFactory derived type objects.
  Arcadia_Set* visualsNodeFactoryTypes;

  /// Set of Arcadia.Engine.Audials.Backend derived type objects.
  Arcadia_Set* audialsBackendTypes;
  /// Set of Arcadia.Engine.Audials.NodeFactory derived type objects.
  Arcadia_Set* audialsNodeFactoryTypes;

  /// Pointer to the active visuals backend context if any, null otherwise.
  Arcadia_Engine_BackendContext* visualsBackendContext;
  /// The visuals node factory.
  Arcadia_Engine_NodeFactory* visualsNodeFactory;

  /// Pointer to the active audials backend context if any, null otherwise.
  Arcadia_Engine_BackendContext* audialsBackendContext;
  /// The audials node factory.
  Arcadia_Engine_NodeFactory* audialsNodeFactory;

  /// @brief The event queue.
  Arcadia_Deque* events;
};

/// @brief Enque an event to this application.
/// @param thread A pointer to this thread.
/// @param self A pointer to this application.
/// @param event A pointer to the event.
void
Arcadia_Engine_enqueEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self,
    Arcadia_Engine_Event* event
  );

/// @brief Deque an event from this engine.
/// @param thread A pointer to this thread.
/// @param self A pointer to this engine.
/// @return A pointer to the event (if there is an event), a null pointer otherwise.
Arcadia_Engine_Event*
Arcadia_Engine_dequeEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self
  );

/// @brief Update this engine.
/// @param thread A pointer to this thread.
/// @param self A pointer to this engine.
void
Arcadia_Engine_update
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self
  );

/// @brief Get or create the engine singleton.
/// @param thread A pointer to this thread.
/// @return A pointer to the engine singleton.
Arcadia_Engine*
Arcadia_Engine_getOrCreate
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_ENGINE_ENGINE_H_INCLUDED
