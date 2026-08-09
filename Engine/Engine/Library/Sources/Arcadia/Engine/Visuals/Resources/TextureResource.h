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

#if !defined(ARCADIA_ENGINE_VISUALS_RESOURCES_TEXTURERESOURCE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_RESOURCES_TEXTURERESOURCE_H_INCLUDED

#include "Arcadia/Engine/Visuals/Resource.h"
#include "Arcadia/Engine/Visuals/TextureAddressMode.h"
#include "Arcadia/Engine/Visuals/TextureFilter.h"
#include "Arcadia/Media/Include.h"

// the texture resource maintains an insernal pixel buffer buffer (Arcadia_Media_PixelBuffer).
// When the texture resource is created, its size 256 x 256, its format is RGBA, its default color is opaque black.
// setPixelBuffer copies the data, that is, the source pixel buffer is not required anymore.
// setWidth and setHeight adjust the width and the height of the internal pixel buffer.
Arcadia_declareObjectType(u8"Arcadia.Visuals.TextureResource", Arcadia_Engine_Visuals_TextureResource,
                          u8"Arcadia.Visuals.Resource");

struct Arcadia_Engine_Visuals_TextureResourceDispatch {
  Arcadia_Engine_Visuals_ResourceDispatch _parent;

  Arcadia_Engine_Visuals_TextureAddressMode(*getAddressModeU)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*);
  Arcadia_Engine_Visuals_TextureAddressMode(*getAddressModeV)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*);

  Arcadia_Integer32Value(*getHeight)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*);

  Arcadia_Engine_Visuals_TextureFilter(*getMagnificationFilter)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*);
  Arcadia_Engine_Visuals_TextureFilter(*getMinificationFilter)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*);

  Arcadia_Integer32Value(*getWidth)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*);

  void (*setAddressModeU)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*, Arcadia_Engine_Visuals_TextureAddressMode);
  void (*setAddressModeV)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*, Arcadia_Engine_Visuals_TextureAddressMode);

  void (*setHeight)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*, Arcadia_Integer32Value);

  void (*setMagnificationFilter)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*, Arcadia_Engine_Visuals_TextureFilter);
  void (*setMinificationFilter)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*, Arcadia_Engine_Visuals_TextureFilter);

  void (*setWidth)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*, Arcadia_Integer32Value);

  void (*setPixelBuffer)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*, Arcadia_Media_PixelBuffer*);
};

struct Arcadia_Engine_Visuals_TextureResource {
  Arcadia_Engine_Visuals_Resource _parent;
};

Arcadia_Engine_Visuals_TextureAddressMode
Arcadia_Engine_Visuals_TextureResource_getAddressModeU
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
  );

Arcadia_Engine_Visuals_TextureAddressMode
Arcadia_Engine_Visuals_TextureResource_getAddressModeV
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
  );

Arcadia_Integer32Value
Arcadia_Engine_Visuals_TextureResource_getHeight
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
  );

Arcadia_Engine_Visuals_TextureFilter
Arcadia_Engine_Visuals_TextureResource_getMagnificationFilter
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
  );

Arcadia_Engine_Visuals_TextureFilter
Arcadia_Engine_Visuals_TextureResource_getMinificationFilter
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
  );

Arcadia_Integer32Value
Arcadia_Engine_Visuals_TextureResource_getWidth
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
  );

void
Arcadia_Engine_Visuals_TextureResource_setAddressModeU
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Engine_Visuals_TextureAddressMode addressMode
  );

void
Arcadia_Engine_Visuals_TextureResource_setAddressModeV
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Engine_Visuals_TextureAddressMode addressMode
  );

void
Arcadia_Engine_Visuals_TextureResource_setHeight
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Integer32Value height
  );

void
Arcadia_Engine_Visuals_TextureResource_setMagnificationFilter
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Engine_Visuals_TextureFilter magnificationFilter
  );

void
Arcadia_Engine_Visuals_TextureResource_setMinificationFilter
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Engine_Visuals_TextureFilter minificationFilter
  );

void
Arcadia_Engine_Visuals_TextureResource_setWidth
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Integer32Value width
  );

void
Arcadia_Engine_Visuals_TextureResource_setPixelBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Media_PixelBuffer* pixelBuffer
  );

#endif // ARCADIA_ENGINE_VISUALS_RESOURCES_TEXTURERESOURCE_H_INCLUDED
