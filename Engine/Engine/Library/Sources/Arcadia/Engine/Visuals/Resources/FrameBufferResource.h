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

#if !defined(ARCADIA_ENGINE_VISUALS_RESOURCES_FRAMEBUFFERRESOURCE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_RESOURCES_FRAMEBUFFERRESOURCE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/Visuals/Resources/FrameBufferResource.h` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Engine/Visuals/Resource.h"

typedef struct Arcadia_Engine_Visuals_FrameBufferResource Arcadia_Engine_Visuals_FrameBufferResource;
typedef struct Arcadia_Media_PixelBuffer Arcadia_Media_PixelBuffer;

Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.FrameBufferResource", Arcadia_Engine_Visuals_FrameBufferResource,
                          u8"Arcadia.Engine.Visuals.Resource");

struct Arcadia_Engine_Visuals_FrameBufferResourceDispatch {
  Arcadia_Engine_Visuals_ResourceDispatch _parent;

  void (*activate)(Arcadia_Thread*, Arcadia_Engine_Visuals_FrameBufferResource*);

  void (*deactivate)(Arcadia_Thread*, Arcadia_Engine_Visuals_FrameBufferResource*);

  void (*setSize)(Arcadia_Thread*, Arcadia_Engine_Visuals_FrameBufferResource*, Arcadia_Integer32Value, Arcadia_Integer32Value);

  void (*getSize)(Arcadia_Thread*, Arcadia_Engine_Visuals_FrameBufferResource*, Arcadia_Integer32Value*, Arcadia_Integer32Value*);

  Arcadia_Media_PixelBuffer* (*capturePixels)(Arcadia_Thread*, Arcadia_Engine_Visuals_FrameBufferResource*);
};

struct Arcadia_Engine_Visuals_FrameBufferResource {
  Arcadia_Engine_Visuals_Resource _parent;
};

void
Arcadia_Engine_Visuals_FrameBufferResource_activate
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self
  );

void
Arcadia_Engine_Visuals_FrameBufferResource_deactivate
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self
  );

void
Arcadia_Engine_Visuals_FrameBufferResource_setSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

void
Arcadia_Engine_Visuals_FrameBufferResource_getSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  );

Arcadia_Media_PixelBuffer*
Arcadia_Engine_Visuals_FrameBufferResource_capturePixels
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self
  );

#endif // ARCADIA_ENGINE_VISUALS_RESOURCES_FRAMEBUFFERRESOURCE_H_INCLUDED
