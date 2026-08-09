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

#define ARCADIA_ENGINE_PRIVATE (1)
#include "Arcadia/Engine/Visuals/Nodes/RenderScene.h"

void
Arcadia_Engine_Visuals_renderScene
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode,
    Arcadia_Engine_Visuals_ModelNode* modelNode,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{
  Arcadia_Engine_Node_setVisualsBackendContext(thread, (Arcadia_Engine_Node*)enterPassNode, (Arcadia_Engine_Visuals_BackendContext*)backendContext);
  if (enterPassNode->frameBufferNode) {
    // "rendering" the scene buffer activates it.
    Arcadia_Engine_Visuals_Node_render
      (
        thread,
        (Arcadia_Engine_Visuals_Node*)enterPassNode->frameBufferNode,
        enterPassNode
      );
  }
  // Render the enter pass node.
  // Pass mesh nodes to the enter pass node.
  Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)enterPassNode, (Arcadia_Engine_Visuals_EnterPassNode*)enterPassNode);
  // Render the mesh node.
  // TODO: Render multiple mesh nodes.
  Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)modelNode, enterPassNode);
  // TODO: Render the leave pass node.
}
