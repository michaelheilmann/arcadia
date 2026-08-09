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

#if !defined(ARCADIA_ENGINE_VISUALS_NODES_TEXTURENODE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_NODES_TEXTURENODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Engine/Visuals/Node.h"
#include "Arcadia/ADL/Include.h"
#include "Arcadia/Engine/Visuals/Nodes/PixelBufferNode.h"
#include "Arcadia/Engine/Visuals/TextureAddressMode.h"
#include "Arcadia/Engine/Visuals/TextureFilter.h"

Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.TextureNode", Arcadia_Engine_Visuals_TextureNode,
                          u8"Arcadia.Engine.Visuals.Node")

struct Arcadia_Engine_Visuals_TextureNodeDispatch {
  Arcadia_Engine_Visuals_NodeDispatch parent;

  Arcadia_Engine_Visuals_TextureAddressMode (*getAddressModeU)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureNode*);
  Arcadia_Engine_Visuals_TextureAddressMode (*getAddressModeV)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureNode*);
  Arcadia_Integer32Value (*getHeight)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureNode*);
  Arcadia_Engine_Visuals_TextureFilter (*getMagnificationFilter)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureNode*);
  Arcadia_Engine_Visuals_TextureFilter (*getMinificationFilter)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureNode*);
  Arcadia_Integer32Value (*getWidth)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureNode*);
};

struct Arcadia_Engine_Visuals_TextureNode {
  Arcadia_Engine_Visuals_Node parent;

  Arcadia_ADL_TextureDefinition* source;

  // The pixel buffer node.
  Arcadia_Engine_Visuals_PixelBufferNode* pixelBuffer;

  // The address mode of this texture along the u-axis.
  // @default Arcadia_Visuals_TextureAddressMode_Repeat.
  Arcadia_Engine_Visuals_TextureAddressMode addressModeU;

  // The address mode of this texture along the v-axis.
  // @default Arcadia_Engine_Visuals_TextureAddressMode_Repeat.
  Arcadia_Engine_Visuals_TextureAddressMode addressModeV;

  // @brief The height, in pixels of the frame buffer.
  // @default 240.
  Arcadia_Integer32Value height;

  // The texture magnification filter.
  // @default Arcadia_Engine_Visuals_TextureFilter_Linear
  Arcadia_Engine_Visuals_TextureFilter magnificationFilter;

  // The texture minification filter.
  // @default Arcadia_Engine_Visuals_TextureFilter_Linear
  Arcadia_Engine_Visuals_TextureFilter minificationFilter;

  // @brief The width, in pixels, of the frame buffer.
  // @default 320.
  Arcadia_Integer32Value width;

  // Bitmask of the dirty properties, that is, properties which were not uploaded.
  Arcadia_Natural8Value dirtyBits;
  // The backend context.
  Arcadia_Engine_Visuals_BackendContext* backendContext;
  // The texture resource.
  Arcadia_Engine_Visuals_TextureResource* textureResource;
};

/// @brief Create a texture node.
/// @param thread A pointer to this thread.
/// @param backendContext A pointer to the backend context or a null pointer.
/// @return A pointer to the texture node.
Arcadia_Engine_Visuals_TextureNode*
Arcadia_Engine_Visuals_TextureNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_TextureDefinition* source
  );

/* Get the address mode of this texture for the u axis. */
Arcadia_Engine_Visuals_TextureAddressMode
Arcadia_Engine_Visuals_TextureNode_getAddressModeU
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  );

/* Get the address mode of this texture for the v axis. */
Arcadia_Engine_Visuals_TextureAddressMode
Arcadia_Engine_Visuals_TextureNode_getAddressModeV
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  );

/* Get the height, in pixels, of this texture. */
Arcadia_Integer32Value
Arcadia_Engine_Visuals_TextureNode_getHeight
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  );

/* Get the magnification filter of this texture. */
Arcadia_Engine_Visuals_TextureFilter
Arcadia_Engine_Visuals_TextureNode_getMagnificationFilter
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  );

/* Get the minification filter of this texture. */
Arcadia_Engine_Visuals_TextureFilter
Arcadia_Engine_Visuals_TextureNode_getMinificationFilter
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  );

/* Get the width, in pixels, of this texture. */
Arcadia_Integer32Value
Arcadia_Engine_Visuals_TextureNode_getWidth
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  );

#endif // ARCADIA_ENGINE_VISUALS_NODES_TEXTURENODE_H_INCLUDED
