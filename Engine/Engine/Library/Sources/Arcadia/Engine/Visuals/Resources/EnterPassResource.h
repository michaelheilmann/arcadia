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

#if !defined(ARCADIA_ENGINE_VISUALS_RESOURCES_ENTERPASSRESOURCE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_RESOURCES_ENTERPASSRESOURCE_H_INCLUDED

#include "Arcadia/Engine/Visuals/Resource.h"
typedef struct Arcadia_Engine_Visuals_BackendContext Arcadia_Engine_Visuals_BackendContext;

typedef struct Arcadia_Engine_Visuals_ConstantBufferResource Arcadia_Engine_Visuals_ConstantBufferResource;
typedef struct Arcadia_Engine_Visuals_FrameBufferResource Arcadia_Engine_Visuals_FrameBufferResource;

#define Arcadia_Engine_Visuals_EnterPassResource_ViewToProjectionMatrixDirty (1 << 0)
#define Arcadia_Engine_Visuals_EnterPassResource_WorldToViewMatrixDirty (1 << 1)
#define Arcadia_Engine_Visuals_EnterPassResource_CanvasSizeDirty (1 << 2)
#define Arcadia_Engine_Visuals_EnterPassResource_ClearColorDirty (1 << 3)
#define Arcadia_Engine_Visuals_EnterPassResource_ClearDepthDirty (1 << 4)
#define Arcadia_Engine_Visuals_EnterPassResource_RelativeViewportRectangleDirty (1 << 5)

#define Arcadia_Engine_Visuals_EnterPassResource_AllDirty \
    ( \
      Arcadia_Engine_Visuals_EnterPassResource_ViewToProjectionMatrixDirty | \
      Arcadia_Engine_Visuals_EnterPassResource_WorldToViewMatrixDirty | \
      Arcadia_Engine_Visuals_EnterPassResource_CanvasSizeDirty | \
      Arcadia_Engine_Visuals_EnterPassResource_ClearColorDirty | \
      Arcadia_Engine_Visuals_EnterPassResource_ClearDepthDirty | \
      Arcadia_Engine_Visuals_EnterPassResource_RelativeViewportRectangleDirty \
    )

// The representation of a enter pass node in the backend.
Arcadia_declareObjectType(u8"Arcadia.Visuals.EnterPassResource", Arcadia_Engine_Visuals_EnterPassResource,
                          u8"Arcadia.Visuals.Resource");

struct Arcadia_Engine_Visuals_EnterPassResourceDispatch {
  Arcadia_Engine_Visuals_ResourceDispatch _parent;

  void (*setTargetFrameBuffer)
    (
      Arcadia_Thread*,
      Arcadia_Engine_Visuals_EnterPassResource*,
      Arcadia_Engine_Visuals_FrameBufferResource*
    );

  void (*setViewToProjectionMatrix)
    (
      Arcadia_Thread*,
      Arcadia_Engine_Visuals_EnterPassResource*,
      Arcadia_Math_Matrix4Real32*
    );

  void (*setWorldToViewMatrix)
    (
      Arcadia_Thread*,
      Arcadia_Engine_Visuals_EnterPassResource*,
      Arcadia_Math_Matrix4Real32*
    );

  void (*setClearColorBuffer)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_EnterPassResource* self,
      Arcadia_BooleanValue clearColorBuffer
    );

  void (*setClearColor)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_EnterPassResource* self,
      Arcadia_Math_Color4Real32* clearColor
    );


  void (*setClearDepthBuffer)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_EnterPassResource* self,
      Arcadia_BooleanValue clearDepthBuffer
    );

  void (*setClearDepth)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_EnterPassResource* self,
      Arcadia_Real32Value clearDepth
    );

  void
  (*setRelativeViewportRectangle)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_EnterPassResource* self,
      Arcadia_Real32Value left,
      Arcadia_Real32Value bottom,
      Arcadia_Real32Value right,
      Arcadia_Real32Value top
    );

  void
  (*setCanvasSize)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_EnterPassResource* self,
      Arcadia_Real32Value width,
      Arcadia_Real32Value height
    );
};

struct Arcadia_Engine_Visuals_EnterPassResource {
  Arcadia_Engine_Visuals_Resource _parent;

  Arcadia_Natural32Value dirty;

  // The world space to view space matrix.
  Arcadia_Math_Matrix4Real32* worldToViewMatrix;

  // The view space to projection space matrix.
  Arcadia_Math_Matrix4Real32* viewToProjectionMatrix;

  // The canvas size.
  struct {

    // The width of the canvas.
    // Default is 320.
    Arcadia_Real32Value width;

    // The height of the canvas.
    // Default is 240.
    Arcadia_Real32Value height;

  } canvasSize;

  struct {

    // Default is 0.
    Arcadia_Real32Value left;

    // Default is 0.
    Arcadia_Real32Value bottom;

    // Default is 1.
    Arcadia_Real32Value right;

    // Default is 1.
    Arcadia_Real32Value top;

  } relativeViewportRectangle;

  // If the color buffer should be cleared.
  Arcadia_BooleanValue clearColorBuffer;
  // The color to clear the color buffer with.
  // Default is 193, 216, 195, 255.
  Arcadia_Math_Color4Real32* clearColor;

  // If the depth buffer should be cleared.
  Arcadia_BooleanValue clearDepthBuffer;
  // The depth to clear the depth buffer with.
  // Default is 1.
  Arcadia_Real32Value clearDepth;

  Arcadia_Engine_Visuals_ConstantBufferResource* viewerConstantBuffer;
};

void
Arcadia_Engine_Visuals_EnterPassResource_setTargetFrameBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassResource* self,
    Arcadia_Engine_Visuals_FrameBufferResource* frameBuffer
  );

void
Arcadia_Engine_Visuals_EnterPassResource_setViewToProjectionMatrix
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassResource* self,
    Arcadia_Math_Matrix4Real32* viewToProjectionMatrix
  );

void
Arcadia_Engine_Visuals_EnterPassResource_setWorldToViewMatrix
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassResource* self,
    Arcadia_Math_Matrix4Real32* worldToViewMatrix
  );

/// Enable or disable clearing the color buffer.
/// @param clearDepthBuffer #Arcadia_BooleanValue_True to enable clearing the color buffer. #Arcadia_BooleanValue_False to disable.
void
Arcadia_Engine_Visuals_EnterPassResource_setClearColorBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassResource* self,
    Arcadia_BooleanValue clearColorBuffer
  );

/// Set the clear color.
/// Default values are red = 193, green = 216, blue = 195, and alpha = 255.
void
Arcadia_Engine_Visuals_EnterPassResource_setClearColor
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassResource* self,
    Arcadia_Math_Color4Real32* clearColor
  );

/// Enable or disable clearing the depth buffer.
/// @param clearDepthBuffer #Arcadia_BooleanValue_True to enable clearing the depth buffer. #Arcadia_BooleanValue_False to disable.
void
Arcadia_Engine_Visuals_EnterPassResource_setClearDepthBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassResource* self,
    Arcadia_BooleanValue clearDepthBuffer
  );

/// Set the clear depth.
/// Default values are
void
Arcadia_Engine_Visuals_EnterPassResource_setClearDepth
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassResource* self,
    Arcadia_Real32Value clearDepth
  );

/// Set the viewport relative rectangle.
/// x axis going from left to right, y axis going from bottom to top
/// Default values are left = 0, bottom = 0, right = 1, and top  = 1.
void
Arcadia_Engine_Visuals_EnterPassResource_setRelativeViewportRectangle
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassResource* self,
    Arcadia_Real32Value left,
    Arcadia_Real32Value bottom,
    Arcadia_Real32Value right,
    Arcadia_Real32Value top
  );

/// Set the canvas size.
/// Default values are width = 320 and height = 240.
void
Arcadia_Engine_Visuals_EnterPassResource_setCanvasSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassResource* self,
    Arcadia_Real32Value width,
    Arcadia_Real32Value height
  );

#endif // ARCADIA_ENGINE_VISUALS_RESOURCES_ENTERPASSRESOURCE_H_INCLUDED
