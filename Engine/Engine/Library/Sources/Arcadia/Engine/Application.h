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

#if !defined(ARCADIA_ENGINE_APPLICATION_H_INCLUDED)
#define ARCADIA_ENGINE_APPLICATION_H_INCLUDED

#include "Arcadia/DDL/Include.h"

#include "Arcadia/Engine/Visuals/ApplicationQuitRequestedEvent.h"
#include "Arcadia/Engine/Input/KeyboardKeyEvent.h"
#include "Arcadia/Engine/Visuals/WindowClosedEvent.h"
typedef struct Arcadia_Engine Arcadia_Engine;

Arcadia_declareObjectType(u8"Arcadia.Engine.Application", Arcadia_Engine_Application,
                          u8"Arcadia.Object");

struct Arcadia_Engine_ApplicationDispatch {
  Arcadia_ObjectDispatch parent;
  void (*startup)(Arcadia_Thread*, Arcadia_Engine_Application*);
  void (*shutdown)(Arcadia_Thread*, Arcadia_Engine_Application*);

  void (*onApplicationQuitRequestedEvent)(Arcadia_Thread*, Arcadia_Engine_Application*, Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent*);
  void (*onKeyboardKeyEvent)(Arcadia_Thread*, Arcadia_Engine_Application*, Arcadia_Engine_Input_KeyboardKeyEvent*);
  void (*onWindowClosedEvent)(Arcadia_Thread*, Arcadia_Engine_Application*, Arcadia_Engine_Visuals_WindowClosedEvent*);
};

struct Arcadia_Engine_Application {
  Arcadia_Object parent;
  /// @a Arcadia_BooleanValue_False if quit was not requested.
  /// @a Arcadia_BooleanValue_True if quit was requested.
  /// @default Arcadia_BooleanValue_False
  Arcadia_BooleanValue quitRequested;
  /// @brief A pointer to the engine.
  Arcadia_Engine* engine;
  /// @brief A list of windows. When a window is closed, it is removed from that list.
  /// If all windows were removed, an application quit request message is sent.
  Arcadia_List* windows;
  /// @brief A pointer to the configuration
  Arcadia_DDL_Node* configuration;
};

// Invoked when the application starts up.
// @remarks If you override this, make sure to call the superclass implementation at the start of the override.
void
Arcadia_Engine_Application_startup
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Application* self
  );

// Invoked when the application shuts down.
// @remarks If you override this, make sure th call the superclass implementation at the end of the override.
void
Arcadia_Engine_Application_shutdown
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Application* self
  );

Arcadia_BooleanValue
Arcadia_Engine_Application_getQuitRequested
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Application* self
  );

void
Arcadia_Engine_Application_setQuitRequested
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Application* self,
    Arcadia_BooleanValue quitRequested
  );

#endif // ARCADIA_ENGINE_APPLICATION_H_INCLUDED
