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

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/Visuals/Resources/EnterPassResource.h` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Engine/Visuals/Resource.h"

typedef struct Arcadia_Engine_Visuals_ConstantBufferResource Arcadia_Engine_Visuals_ConstantBufferResource;
typedef struct Arcadia_Engine_Visuals_EnterPassResource Arcadia_Engine_Visuals_EnterPassResource;
typedef struct Arcadia_Engine_Visuals_FrameBufferResource Arcadia_Engine_Visuals_FrameBufferResource;
typedef struct Arcadia_Math_Color4Real32 Arcadia_Math_Color4Real32;
typedef struct Arcadia_Math_Matrix4Real32 Arcadia_Math_Matrix4Real32;

Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.EnterPassResource", Arcadia_Engine_Visuals_EnterPassResource,
                          u8"Arcadia.Engine.Visuals.Resource");

struct Arcadia_Engine_Visuals_EnterPassResourceDispatch {
  Arcadia_Engine_Visuals_ResourceDispatch _parent;

  void (*setTargetFrameBuffer)(Arcadia_Thread*, Arcadia_Engine_Visuals_EnterPassResource*, Arcadia_Engine_Visuals_FrameBufferResource*);

  void (*setViewToProjectionMatrix)(Arcadia_Thread*, Arcadia_Engine_Visuals_EnterPassResource*, Arcadia_Math_Matrix4Real32*);

  void (*setWorldToViewMatrix)(Arcadia_Thread*, Arcadia_Engine_Visuals_EnterPassResource*, Arcadia_Math_Matrix4Real32*);

  void (*setClearColorBuffer)(Arcadia_Thread*, Arcadia_Engine_Visuals_EnterPassResource*, Arcadia_BooleanValue);

  void (*setClearColor)(Arcadia_Thread*, Arcadia_Engine_Visuals_EnterPassResource*, Arcadia_Math_Color4Real32*);

  void (*setClearDepthBuffer)(Arcadia_Thread*, Arcadia_Engine_Visuals_EnterPassResource*, Arcadia_BooleanValue);

  void (*setClearDepth)(Arcadia_Thread*, Arcadia_Engine_Visuals_EnterPassResource*, Arcadia_Real32Value);

  void (*setRelativeViewportRectangle)(Arcadia_Thread*, Arcadia_Engine_Visuals_EnterPassResource*, Arcadia_Real32Value, Arcadia_Real32Value, Arcadia_Real32Value, Arcadia_Real32Value);

  void (*setCanvasSize)(Arcadia_Thread*, Arcadia_Engine_Visuals_EnterPassResource*, Arcadia_Real32Value, Arcadia_Real32Value);
};

struct Arcadia_Engine_Visuals_EnterPassResource {
  Arcadia_Engine_Visuals_Resource _parent;
  Arcadia_Natural32Value dirty;
  Arcadia_Math_Matrix4Real32* worldToViewMatrix;
  Arcadia_Math_Matrix4Real32* viewToProjectionMatrix;
  struct { Arcadia_Real32Value width; Arcadia_Real32Value height; } canvasSize;
  struct { Arcadia_Real32Value left; Arcadia_Real32Value bottom; Arcadia_Real32Value right; Arcadia_Real32Value top; } relativeViewportRectangle;
  Arcadia_BooleanValue clearColorBuffer;
  Arcadia_Math_Color4Real32* clearColor;
  Arcadia_BooleanValue clearDepthBuffer;
  Arcadia_Real32Value clearDepth;
  Arcadia_Engine_Visuals_ConstantBufferResource* viewerConstantBuffer;
};

#define Arcadia_Engine_Visuals_EnterPassResource_ViewToProjectionMatrixDirty INT32_C(1)

#define Arcadia_Engine_Visuals_EnterPassResource_WorldToViewMatrixDirty INT32_C(2)

#define Arcadia_Engine_Visuals_EnterPassResource_CanvasSizeDirty INT32_C(4)

#define Arcadia_Engine_Visuals_EnterPassResource_ClearColorDirty INT32_C(8)

#define Arcadia_Engine_Visuals_EnterPassResource_ClearDepthDirty INT32_C(16)

#define Arcadia_Engine_Visuals_EnterPassResource_RelativeViewportRectangleDirty INT32_C(32)

#define Arcadia_Engine_Visuals_EnterPassResource_AllDirty INT32_C(63)

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

void
Arcadia_Engine_Visuals_EnterPassResource_setClearColorBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassResource* self,
    Arcadia_BooleanValue clearColorBuffer
  );

void
Arcadia_Engine_Visuals_EnterPassResource_setClearColor
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassResource* self,
    Arcadia_Math_Color4Real32* clearColor
  );

void
Arcadia_Engine_Visuals_EnterPassResource_setClearDepthBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassResource* self,
    Arcadia_BooleanValue clearDepthBuffer
  );

void
Arcadia_Engine_Visuals_EnterPassResource_setClearDepth
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassResource* self,
    Arcadia_Real32Value clearDepth
  );

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

void
Arcadia_Engine_Visuals_EnterPassResource_setCanvasSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassResource* self,
    Arcadia_Real32Value width,
    Arcadia_Real32Value height
  );

#endif // ARCADIA_ENGINE_VISUALS_RESOURCES_ENTERPASSRESOURCE_H_INCLUDED
