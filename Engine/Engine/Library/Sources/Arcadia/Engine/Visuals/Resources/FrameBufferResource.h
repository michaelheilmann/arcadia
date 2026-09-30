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

#include "Arcadia/PixelBufferIO/Include.h"
#include "Arcadia/Engine/Visuals/Resource.h"

// A "frame buffer resource" must be created and activated before rendering of a scene may occur.
// The "default" render target writes to the "default" frame buffer.
// A "texture" render target creates a framebuffer and writes to that frame buffer.
Arcadia_declareObjectType(u8"Arcadia.Visuals.FrameBufferResource", Arcadia_Engine_Visuals_FrameBufferResource,
                          u8"Arcadia.Visuals.Resource");

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

/// @brief Capture the pixels currently rendered to this frame buffer resource.
/// @param thread A pointer to this thread.
/// @param self A pointer to this frame buffer resource.
/// @return A pointer to a new pixel buffer of #Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8,
/// sized by FrameBufferResource_getSize, holding one row per frame buffer row,
/// first row being the topmost frame buffer row.
/// @pre
/// The backend-side representation of this frame buffer resource must have been created,
/// that is, the resource must have been loaded and rendered into.
/// @warning
/// The returned pixel buffer is a fresh object which is not owned by this frame buffer resource.
/// The caller is responsible for keeping it reachable for as long as it is used.
/// @remarks
/// This frame buffer resource does not need to be the activated one. The previously activated
/// frame buffer resource, if any, remains activated.
Arcadia_Media_PixelBuffer*
Arcadia_Engine_Visuals_FrameBufferResource_capturePixels
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self
  );

#endif // ARCADIA_ENGINE_VISUALS_RESOURCES_FRAMEBUFFERRESOURCE_H_INCLUDED
