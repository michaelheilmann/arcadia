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

#if !defined(ARCADIA_ENGINE_AUDIALS_NODE_H_INCLUDED)
#define ARCADIA_ENGINE_AUDIALS_NODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif
#include "Arcadia/Engine/Node.h"
#include "Arcadia/Math/Include.h"
typedef struct Arcadia_Engine_Audials_BackendContext Arcadia_Engine_Audials_BackendContext;

// The base of all audials nodes.
// A "resource" is owned by a "backend context".
// That is, the "backend context" holds a STRONG reference to its "resources".
// In addition, the "backend context" retains a GC lock unless its "resources" such that they are only gc'ed if the "backend context" drops this lock.
Arcadia_declareObjectType(u8"Arcadia.Engine.Audials.Node", Arcadia_Engine_Audials_Node,
                          u8"Arcadia.Engine.Node");

struct Arcadia_Engine_Audials_NodeDispatch {
  Arcadia_Engine_NodeDispatch _parent;
  void (*render)(Arcadia_Thread*, Arcadia_Engine_Audials_Node*);
};

struct Arcadia_Engine_Audials_Node {
  Arcadia_Engine_Node _parent;
};

// Render this node.
void
Arcadia_Engine_Audials_Node_render
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Node* self
  );

#endif // ARCADIA_ENGINE_AUDIALS_NODE_H_INCLUDED
