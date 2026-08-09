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

#if !defined(ARCADIA_ENGINE_UI_WIDGETSIZECHANGEDEVENT_H_INCLUDED)
#define ARCADIA_ENGINE_UI_WIDGETSIZECHANGEDEVENT_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif
#include "Arcadia/Engine/UI/WidgetEvent.h"

// https://michaelheilmann.com/Arcadia/Engine/UI/#Arcadia_Engine_UI_WidgetSizeChangedEvent
Arcadia_declareObjectType(u8"Arcadia.Engine.UI.WidgetSizeChangedEvent", Arcadia_Engine_UI_WidgetSizeChangedEvent,
                          u8"Arcadia.Engine.UI.WidgetEvent");

struct Arcadia_Engine_UI_WidgetSizeChangedEventDispatch {
  Arcadia_Engine_UI_WidgetEventDispatch _parent;
};

struct Arcadia_Engine_UI_WidgetSizeChangedEvent {
  Arcadia_Engine_UI_WidgetEvent _parent;
  Arcadia_Integer32Value horizontalSize;
  Arcadia_Integer32Value verticalSize;
};

Arcadia_Engine_UI_WidgetSizeChangedEvent*
Arcadia_Engine_UI_WidgetSizeChangedEvent_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value timestamp,
    Arcadia_Engine_UI_WidgetNode* source,
    Arcadia_Integer32Value horizontalSize,
    Arcadia_Integer32Value verticalSize
  );

#endif // ARCADIA_ENGINE_UI_WIDGETSIZECHANGEDEVENT_H_INCLUDED
