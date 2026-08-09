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

#if !defined(ARCADIA_ENGINE_VISUALS_NODES_MATERIALNODE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_NODES_MATERIALNODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif
#include "Arcadia/Engine/Visuals/Node.h"
#include "Arcadia/ADL/Include.h"
typedef struct Arcadia_Engine_Visuals_MaterialResource Arcadia_Engine_Visuals_MaterialResource;
typedef struct Arcadia_Engine_Visuals_NodeFactory Arcadia_Engine_Visuals_NodeFactory;
typedef struct Arcadia_Engine_Visuals_TextureNode Arcadia_Engine_Visuals_TextureNode;
typedef struct Arcadia_VPL_Symbols_Program Arcadia_VPL_Symbols_Program;

Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.MaterialNode", Arcadia_Engine_Visuals_MaterialNode,
                          u8"Arcadia.Engine.Visuals.Node");

struct Arcadia_Engine_Visuals_MaterialNodeDispatch {
  Arcadia_Engine_Visuals_NodeDispatch parent;
};

struct Arcadia_Engine_Visuals_MaterialNode {
  Arcadia_Engine_Visuals_Node parent;
  Arcadia_ADL_MaterialDefinition* source;
  Arcadia_Engine_Visuals_TextureNode* ambientColorTexture;
  Arcadia_VPL_Symbols_Program* program;
  /// The material resource of this material node.
  Arcadia_Engine_Visuals_MaterialResource* materialResource;
  /// The backend context of this material node.
  Arcadia_Engine_Visuals_BackendContext* backendContext;
};

// @brief Create a material node.
// @param backendContext A pointer to the backend context or the null pointer.
// @return A pointer to the material node.
Arcadia_Engine_Visuals_MaterialNode*
Arcadia_Engine_Visuals_MaterialNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_MaterialDefinition* source
  );

#endif  // ARCADIA_ENGINE_VISUALS_NODES_MATERIALNODE_H_INCLUDED
