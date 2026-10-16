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

#if !defined(ARCADIA_GAMES_ANIMA_APPLICATION_H_INCLUDED)
#define ARCADIA_GAMES_ANIMA_APPLICATION_H_INCLUDED

#include "Arcadia/Engine/Include.h"

Arcadia_declareObjectType
  (
    u8"Arcadia.Games.Anima.Application",
    Arcadia_Games_Anima_Application,
    u8"Arcadia.Engine.Application"
  );

struct Arcadia_Games_Anima_ApplicationDispatch {
  Arcadia_Engine_ApplicationDispatch parent;
};

struct Arcadia_Games_Anima_Application {
  Arcadia_Engine_Application parent;
  Arcadia_Engine_SceneManager* sceneManager;
  Arcadia_Slot* sceneChangedSlot;
  Arcadia_Slot* sceneOnQuitRequestedSlot;
};

Arcadia_Games_Anima_Application*
Arcadia_Games_Anima_Application_create
  (
    Arcadia_Thread* thread
  );

void
Arcadia_Games_Anima_Application_onWindowClosedEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_Application* self,
    Arcadia_Engine_Visuals_WindowClosedEvent* event
  );

#endif // ARCADIA_GAMES_ANIMA_APPLICATION_H_INCLUDED
