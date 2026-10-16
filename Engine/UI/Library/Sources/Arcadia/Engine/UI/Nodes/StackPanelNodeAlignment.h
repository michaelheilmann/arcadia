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

#if !defined(ARCADIA_ENGINE_UI_NODES_STACKPANELNODEALIGNMENT_H_INCLUDED)
#define ARCADIA_ENGINE_UI_NODES_STACKPANELNODEALIGNMENT_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

Arcadia_declareEnumerationType(u8"Arcadia.Engine.UI.StackPanelNodeAlignment", Arcadia_Engine_UI_StackPanelNodeAlignment);

typedef enum Arcadia_Engine_UI_StackPanelNodeAlignment {
  Arcadia_Engine_UI_StackPanelNodeAlignment_Start,
  Arcadia_Engine_UI_StackPanelNodeAlignment_Center,
  Arcadia_Engine_UI_StackPanelNodeAlignment_End,
  Arcadia_Engine_UI_StackPanelNodeAlignment_Stretch,
} Arcadia_Engine_UI_StackPanelNodeAlignment;

#endif // ARCADIA_ENGINE_UI_NODES_STACKPANELNODEALIGNMENT_H_INCLUDED
