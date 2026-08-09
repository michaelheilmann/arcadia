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

#if !defined(ARCADIA_ENGINE_VISUALS_NODES_MODELNODE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_NODES_MODELNODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif
#include "Arcadia/Engine/Visuals/Nodes/MeshNode.h"
#include "Arcadia/Engine/Visuals/Nodes/MaterialNode.h"

// A "model node" consists of a
// - a mesh
// - a material
Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.ModelNode", Arcadia_Engine_Visuals_ModelNode,
                          u8"Arcadia.Engine.Visuals.Node")

struct Arcadia_Engine_Visuals_ModelNodeDispatch {
  Arcadia_Engine_Visuals_NodeDispatch parent;
};

struct Arcadia_Engine_Visuals_ModelNode {
  Arcadia_Engine_Visuals_Node _parent;

  /// The model definition.
  Arcadia_ADL_ModelDefinition* source;
  /// The mesh of this model.
  Arcadia_Engine_Visuals_MeshNode* mesh;
  /// The material of this model.
  Arcadia_Engine_Visuals_MaterialNode* material;
  /// The backend context.
  Arcadia_Engine_Visuals_BackendContext* backendContext;
  /// The model resource.
  Arcadia_Engine_Visuals_ModelResource* modelResource;
};

Arcadia_Engine_Visuals_ModelNode*
Arcadia_Engine_Visuals_ModelNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_ModelDefinition* source
  );

#endif // ARCADIA_ENGINE_VISUALS_NODES_MODELNODE_H_INCLUDED
