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

#if !defined(ARCADIA_ENGINE_UI_WIDGETEVENT_H_INCLUDED)
#define ARCADIA_ENGINE_UI_WIDGETEVENT_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif
#include "Arcadia/Engine/UI/Event.h"
typedef struct Arcadia_Engine_UI_WidgetNode Arcadia_Engine_UI_WidgetNode;

// @code
// constructor(timestamp : Arcadia.Natural64, source : Arcadia.Engine.UI.Widget)
// @endcode
// https://michaelheilmann.com/Arcadia/Engine/UI/#Arcadia_Engine_UI_WidgetEvent
Arcadia_declareObjectType(u8"Arcadia.Engine.UI.WidgetEvent", Arcadia_Engine_UI_WidgetEvent,
                          u8"Arcadia.Engine.UI.Event");

struct Arcadia_Engine_UI_WidgetEventDispatch {
  Arcadia_Engine_UI_EventDispatch _parent;
};

struct Arcadia_Engine_UI_WidgetEvent {
  Arcadia_Engine_UI_Event _parent;
  /// The source of the event.
  Arcadia_Engine_UI_WidgetNode* source;
};

#endif // ARCADIA_ENGINE_UI_WIDGETEVENT_H_INCLUDED
