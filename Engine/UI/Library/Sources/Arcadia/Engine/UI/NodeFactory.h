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

#if !defined(ARCADIA_ENGINE_UI_NODES_CANVASNODE_H_INCLUDED)
#define ARCADIA_ENGINE_UI_NODES_CANVASNODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif
#include "Arcadia/Audials/Include.h"
#include "Arcadia/Visuals/Include.h"

// A "UI canvas".
// Defining elements:
// Postion (left, top) and size (width, height) within canvas coordinates.
// NOTE: canvas coordinates != UI canvas coordinates
Arcadia_declareObjectType(u8"Arcadia.Engine.UI.CanvasNode", Arcadia_Engine_UI_CanvasNode,
                          u8"Arcadia.Engine.Node")

struct Arcadia_Engine_UI_CanvasNodeDispatch {
  Arcadia_Engine_NodeDispatch _parent;
};

struct Arcadia_Engine_UI_CanvasNode {
  Arcadia_Engine_Node _parent;
};

Arcadia_Engine_UI_CanvasNode*
Arcadia_Engine_UI_CanvasNode_create
  (
  );

#endif // ARCADIA_ENGINE_UI_NODES_CANVASNODE_H_INCLUDED
