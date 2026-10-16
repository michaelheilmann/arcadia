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

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/Application.h` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

typedef struct Arcadia_DDL_Node Arcadia_DDL_Node;
typedef struct Arcadia_Engine Arcadia_Engine;
typedef struct Arcadia_Engine_Application Arcadia_Engine_Application;
typedef struct Arcadia_Engine_Input_KeyboardKeyEvent Arcadia_Engine_Input_KeyboardKeyEvent;
typedef struct Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent;
typedef struct Arcadia_Engine_Visuals_WindowClosedEvent Arcadia_Engine_Visuals_WindowClosedEvent;
typedef struct Arcadia_List Arcadia_List;

Arcadia_declareObjectType(u8"Arcadia.Engine.Application", Arcadia_Engine_Application,
                          u8"Arcadia.Object");

struct Arcadia_Engine_ApplicationDispatch {
  Arcadia_ObjectDispatch _parent;

  void (*startup)(Arcadia_Thread*, Arcadia_Engine_Application*);

  void (*shutdown)(Arcadia_Thread*, Arcadia_Engine_Application*);

  void (*onApplicationQuitRequestedEvent)(Arcadia_Thread*, Arcadia_Engine_Application*, Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent*);

  void (*onKeyboardKeyEvent)(Arcadia_Thread*, Arcadia_Engine_Application*, Arcadia_Engine_Input_KeyboardKeyEvent*);

  void (*onWindowClosedEvent)(Arcadia_Thread*, Arcadia_Engine_Application*, Arcadia_Engine_Visuals_WindowClosedEvent*);
};

struct Arcadia_Engine_Application {
  Arcadia_Object _parent;
  Arcadia_BooleanValue quitRequested;
  Arcadia_Engine* engine;
  Arcadia_List* windows;
  Arcadia_DDL_Node* configuration;
  Arcadia_String* configurationDirectoryName;
};

void
Arcadia_Engine_Application_startup
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Application* self
  );

void
Arcadia_Engine_Application_shutdown
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Application* self
  );

void
Arcadia_Engine_Application_onApplicationQuitRequestedEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Application* self,
    Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent* event
  );

void
Arcadia_Engine_Application_onKeyboardKeyEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Application* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  );

void
Arcadia_Engine_Application_onWindowClosedEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Application* self,
    Arcadia_Engine_Visuals_WindowClosedEvent* event
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

void
Arcadia_Engine_Application_setConfigurationDirectoryName
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Application* self,
    Arcadia_String* configurationDirectoryName
  );

#endif // ARCADIA_ENGINE_APPLICATION_H_INCLUDED
