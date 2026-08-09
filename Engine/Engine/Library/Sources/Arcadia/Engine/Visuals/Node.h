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

#if !defined(ARCADIA_ENGINE_VISUALS_NODE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_NODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif
#include "Arcadia/Engine/Node.h"
#include "Arcadia/Math/Include.h"
typedef struct Arcadia_Engine_Visuals_EnterPassNode Arcadia_Engine_Visuals_EnterPassNode;
typedef struct Arcadia_Engine_Visuals_NodeFactory Arcadia_Engine_Visuals_NodeFactory;
typedef struct Arcadia_Engine_Visuals_BackendContext Arcadia_Engine_Visuals_BackendContext;

// The base of all visuals nodes.
Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.Node", Arcadia_Engine_Visuals_Node,
                          u8"Arcadia.Engine.Node")

struct Arcadia_Engine_Visuals_NodeDispatch {
  Arcadia_Engine_NodeDispatch parent;
  void (*render)(Arcadia_Thread*, Arcadia_Engine_Visuals_Node*, Arcadia_Engine_Visuals_EnterPassNode*);
};

struct Arcadia_Engine_Visuals_Node {
  Arcadia_Engine_Node _parent;
};

// Render this node.
void
Arcadia_Engine_Visuals_Node_render
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Node* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

#endif // ARCADIA_ENGINE_VISUALS_NODE_H_INCLUDED
