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
#include "Arcadia/Engine/Visuals/Nodes/ViewportNode.h"

#include "Arcadia/Engine//Include.h"

#define ClearColorDirty (1)
#define ClearColorBufferDirty (2)

#define ClearDepthDirty (4)
#define ClearDepthBufferDirty (8)

#define RelativeRectangleDirty (16)
#define CanvasSizeDirty (32)

#define DirtyBitsAll (ClearColorDirty | ClearColorBufferDirty | ClearDepthDirty | ClearDepthBufferDirty | RelativeRectangleDirty | CanvasSizeDirty)

static void
Arcadia_Engine_Visuals_ViewportNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self
  );

static void
Arcadia_Engine_Visuals_ViewportNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self
  );

static void
Arcadia_Engine_Visuals_ViewportNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNodeDispatch* self
  );

static void
Arcadia_Engine_Visuals_ViewportNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self
  );

static void
Arcadia_Engine_Visuals_ViewportNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

static void
Arcadia_Engine_Visuals_ViewportNode_setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

static Arcadia_BooleanValue
Arcadia_Engine_Visuals_ViewportNode_getClearColorBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self
  );

static void
Arcadia_Engine_Visuals_ViewportNode_setClearColorBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_BooleanValue clearColorBuffer
  );

static Arcadia_Math_Color4Real32*
Arcadia_Engine_Visuals_ViewportNode_getClearColorImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Math_Color4Real32* clearColor
  );

static void
Arcadia_Engine_Visuals_ViewportNode_setClearColorImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Math_Color4Real32* clearColor
  );

static void
Arcadia_Engine_Visuals_ViewportNode_setClearDepthBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_BooleanValue clearDepthBuffer
  );

static Arcadia_BooleanValue
Arcadia_Engine_Visuals_ViewportNode_getClearDepthBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self
  );

static Arcadia_Real32Value
Arcadia_Engine_Visuals_ViewportNode_getClearDepthImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self
  );

static void
Arcadia_Engine_Visuals_ViewportNode_setClearDepthImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Real32Value depth
  );

static void
Arcadia_Engine_Visuals_ViewportNode_getRelativeViewportRectangleImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Real32Value* left,
    Arcadia_Real32Value* bottom,
    Arcadia_Real32Value* right,
    Arcadia_Real32Value* top
  );

static void
Arcadia_Engine_Visuals_ViewportNode_setRelativeViewportRectangleImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Real32Value left,
    Arcadia_Real32Value bottom,
    Arcadia_Real32Value right,
    Arcadia_Real32Value top
  );

static void
Arcadia_Engine_Visuals_ViewportNode_getCanvasSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Real32Value* width,
    Arcadia_Real32Value* height
  );

static void
Arcadia_Engine_Visuals_ViewportNode_setCanvasSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Real32Value width,
    Arcadia_Real32Value height
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_ViewportNode_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_ViewportNode_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_ViewportNode_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_ViewportNode_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Visuals.ViewportNode", Arcadia_Engine_Visuals_ViewportNode,
                         u8"Arcadia.Engine.Visuals.Node", Arcadia_Engine_Visuals_Node,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_ViewportNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_ViewportNode);
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (Arcadia_ValueStack_isVoidValue(thread, 1)) {
    self->backendContext = NULL;
  } else {
    self->backendContext = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_Visuals_BackendContext_getType(thread));
    Arcadia_Object_lock(thread, (Arcadia_Object*)self->backendContext);
  }

  self->dirtyBits = DirtyBitsAll;

  self->clearColorBuffer = Arcadia_BooleanValue_True;
  self->clearColor = Arcadia_Math_Color4Real32_create4(thread, 193.f / 255.f, 216.f / 255.f, 195.f / 255.f, 1.f);

  self->clearDepthBuffer = Arcadia_BooleanValue_True;
  self->clearDepth = 1.f;

  self->relativeViewportRectangle.left = 0.f;
  self->relativeViewportRectangle.bottom = 0.f;
  self->relativeViewportRectangle.right = 1.f;
  self->relativeViewportRectangle.top = 1.f;

  self->canvasSize.width = 320.f;
  self->canvasSize.height = 240.f;
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_ViewportNode);
}

static void
Arcadia_Engine_Visuals_ViewportNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self
  )
{
  if (self->backendContext) {
    Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
    self->backendContext = NULL;
    self->dirtyBits = ClearColorDirty | ClearDepthDirty | RelativeRectangleDirty | CanvasSizeDirty;
  }
}

static void
Arcadia_Engine_Visuals_ViewportNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNodeDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_ViewportNodeDispatch*)self)->getCanvasSize = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_ViewportNode*, Arcadia_Real32Value*, Arcadia_Real32Value*)) & Arcadia_Engine_Visuals_ViewportNode_getCanvasSizeImpl;
  ((Arcadia_Engine_Visuals_ViewportNodeDispatch*)self)->setCanvasSize = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_ViewportNode*, Arcadia_Real32Value, Arcadia_Real32Value)) & Arcadia_Engine_Visuals_ViewportNode_setCanvasSizeImpl;

  ((Arcadia_Engine_Visuals_ViewportNodeDispatch*)self)->getClearColorBuffer = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_ViewportNode*)) & Arcadia_Engine_Visuals_ViewportNode_getClearColorBufferImpl;
  ((Arcadia_Engine_Visuals_ViewportNodeDispatch*)self)->setClearColorBuffer = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_ViewportNode*, Arcadia_BooleanValue)) & Arcadia_Engine_Visuals_ViewportNode_setClearColorBufferImpl;

  ((Arcadia_Engine_Visuals_ViewportNodeDispatch*)self)->getClearColor = (Arcadia_Math_Color4Real32 * (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_ViewportNode*)) & Arcadia_Engine_Visuals_ViewportNode_getClearColorImpl;
  ((Arcadia_Engine_Visuals_ViewportNodeDispatch*)self)->setClearColor = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_ViewportNode*, Arcadia_Math_Color4Real32*)) & Arcadia_Engine_Visuals_ViewportNode_setClearColorImpl;

  ((Arcadia_Engine_Visuals_ViewportNodeDispatch*)self)->getClearDepthBuffer = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_ViewportNode*)) & Arcadia_Engine_Visuals_ViewportNode_getClearDepthBufferImpl;
  ((Arcadia_Engine_Visuals_ViewportNodeDispatch*)self)->setClearDepthBuffer = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_ViewportNode*, Arcadia_BooleanValue)) & Arcadia_Engine_Visuals_ViewportNode_setClearDepthBufferImpl;

  ((Arcadia_Engine_Visuals_ViewportNodeDispatch*)self)->getClearDepth = (Arcadia_Real32Value(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_ViewportNode*)) & Arcadia_Engine_Visuals_ViewportNode_getClearDepthImpl;
  ((Arcadia_Engine_Visuals_ViewportNodeDispatch*)self)->setClearDepth = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_ViewportNode*, Arcadia_Real32Value)) & Arcadia_Engine_Visuals_ViewportNode_setClearDepthImpl;

  ((Arcadia_Engine_Visuals_ViewportNodeDispatch*)self)->getRelativeViewportRectangle = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_ViewportNode*, Arcadia_Real32Value*, Arcadia_Real32Value*, Arcadia_Real32Value*, Arcadia_Real32Value*)) & Arcadia_Engine_Visuals_ViewportNode_getRelativeViewportRectangleImpl;
  ((Arcadia_Engine_Visuals_ViewportNodeDispatch*)self)->setRelativeViewportRectangle = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_ViewportNode*, Arcadia_Real32Value, Arcadia_Real32Value, Arcadia_Real32Value, Arcadia_Real32Value)) & Arcadia_Engine_Visuals_ViewportNode_setRelativeViewportRectangleImpl;

  ((Arcadia_Engine_Visuals_NodeDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Node*, Arcadia_Engine_Visuals_EnterPassNode*)) & Arcadia_Engine_Visuals_ViewportNode_renderImpl;
  ((Arcadia_Engine_NodeDispatch*)self)->setVisualsBackendContext = (void (*)(Arcadia_Thread*, Arcadia_Engine_Node*, Arcadia_Engine_Visuals_BackendContext*)) & Arcadia_Engine_Visuals_ViewportNode_setVisualsBackendContextImpl;
}

static void
Arcadia_Engine_Visuals_ViewportNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self
  )
{
  if (self->clearColor) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->clearColor);
  }
}

static void
Arcadia_Engine_Visuals_ViewportNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{
  Arcadia_Engine_Node_setVisualsBackendContext(thread, (Arcadia_Engine_Node*)self, (Arcadia_Engine_Visuals_BackendContext*)enterPassNode->backendContext);
  if (self->backendContext) {
    /*Intentionally empty.*/
  }
}

static void
Arcadia_Engine_Visuals_ViewportNode_setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{
  if (backendContext == self->backendContext) {
    // Only change something if the backend context changes.
    return;
  }
  if (backendContext) {
    Arcadia_Object_lock(thread, (Arcadia_Object*)backendContext);
  }
  if (self->backendContext) {
    Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
  }
  if (self->backendContext) {
#if 0
    if (self->viewportResource) {
      Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->viewportResource);
      self->viewportResource = NULL;
    }
#endif
  }
  self->backendContext = backendContext;
}

static Arcadia_BooleanValue
Arcadia_Engine_Visuals_ViewportNode_getClearColorBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self
  )
{ return self->clearColorBuffer; }

static void
Arcadia_Engine_Visuals_ViewportNode_setClearColorBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_BooleanValue clearColorBuffer
  )
{
  self->clearColorBuffer = clearColorBuffer;
  self->dirtyBits |= ClearColorBufferDirty;
}

static Arcadia_Math_Color4Real32*
Arcadia_Engine_Visuals_ViewportNode_getClearColorImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Math_Color4Real32* clearColor
  )
{
  Arcadia_Math_Color4Real32* clone = Arcadia_Math_Color4Real32_create(thread);
  Arcadia_Math_Color4Real32_assign(thread, clone, self->clearColor);
  return clone;
}

static void
Arcadia_Engine_Visuals_ViewportNode_setClearColorImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Math_Color4Real32* clearColor
  )
{
  Arcadia_Math_Color4Real32_assign(thread, self->clearColor, clearColor);
  self->dirtyBits |= ClearColorDirty;
}

static Arcadia_BooleanValue
Arcadia_Engine_Visuals_ViewportNode_getClearDepthBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self
  )
{ return self->clearDepthBuffer; }

static void
Arcadia_Engine_Visuals_ViewportNode_setClearDepthBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_BooleanValue clearDepthBuffer
  )
{
  self->clearDepthBuffer = clearDepthBuffer;
  self->dirtyBits |= ClearDepthBufferDirty;
}

static Arcadia_Real32Value
Arcadia_Engine_Visuals_ViewportNode_getClearDepthImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self
  )
{ return self->clearDepth; }

static void
Arcadia_Engine_Visuals_ViewportNode_setClearDepthImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Real32Value depth
  )
{
  self->clearDepth = depth;
  self->dirtyBits |= ClearDepthDirty;
}

static void
Arcadia_Engine_Visuals_ViewportNode_getRelativeViewportRectangleImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Real32Value* left,
    Arcadia_Real32Value* bottom,
    Arcadia_Real32Value* right,
    Arcadia_Real32Value* top
  )
{
  *left = self->relativeViewportRectangle.left;
  *bottom = self->relativeViewportRectangle.bottom;
  *right = self->relativeViewportRectangle.right;
  *top = self->relativeViewportRectangle.top;
}


static void
Arcadia_Engine_Visuals_ViewportNode_setRelativeViewportRectangleImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Real32Value left,
    Arcadia_Real32Value bottom,
    Arcadia_Real32Value right,
    Arcadia_Real32Value top
  )
{
  self->relativeViewportRectangle.left = left;
  self->relativeViewportRectangle.bottom = bottom;
  self->relativeViewportRectangle.right = right;
  self->relativeViewportRectangle.top = top;
  self->dirtyBits |= RelativeRectangleDirty;
}

static void
Arcadia_Engine_Visuals_ViewportNode_getCanvasSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Real32Value* width,
    Arcadia_Real32Value* height
  )
{
  *width = self->canvasSize.width;
  *height = self->canvasSize.height;
}

static void
Arcadia_Engine_Visuals_ViewportNode_setCanvasSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Real32Value width,
    Arcadia_Real32Value height
  )
{
  self->canvasSize.width = width;
  self->canvasSize.height = height;
  self->dirtyBits |= CanvasSizeDirty;
}


Arcadia_Engine_Visuals_ViewportNode*
Arcadia_Engine_Visuals_ViewportNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_ViewportNode);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_ViewportNode);
}

Arcadia_BooleanValue
Arcadia_Engine_Visuals_ViewportNode_getClearColorBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_ViewportNode, getClearColorBuffer, self); }

void
Arcadia_Engine_Visuals_ViewportNode_setClearColorBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_BooleanValue clearColorBuffer
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_ViewportNode, setClearColorBuffer, self, clearColorBuffer); }

Arcadia_Math_Color4Real32*
Arcadia_Engine_Visuals_ViewportNode_getClearColor
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_ViewportNode, getClearColor, self); }

void
Arcadia_Engine_Visuals_ViewportNode_setClearColor
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Math_Color4Real32* clearColor
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_ViewportNode, setClearColor, self, clearColor); }

Arcadia_BooleanValue
Arcadia_Engine_Visuals_ViewportNode_getClearDepthBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_ViewportNode, getClearDepthBuffer, self); }

void
Arcadia_Engine_Visuals_ViewportNode_setClearDepthBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_BooleanValue clearDepthBuffer
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_ViewportNode, setClearDepthBuffer, self, clearDepthBuffer); }

Arcadia_Real32Value
Arcadia_Engine_Visuals_ViewportNode_getClearDepth
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_ViewportNode, getClearDepth, self); }

void
Arcadia_Engine_Visuals_ViewportNode_setClearDepth
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Real32Value depth
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_ViewportNode, setClearDepth, self, depth); }

void
Arcadia_Engine_Visuals_ViewportNode_getRelativeViewportRectangle
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Real32Value* left,
    Arcadia_Real32Value* bottom,
    Arcadia_Real32Value* right,
    Arcadia_Real32Value* top
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_ViewportNode, getRelativeViewportRectangle, self, left, bottom, right, top); }

void
Arcadia_Engine_Visuals_ViewportNode_setRelativeViewportRectangle
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Real32Value left,
    Arcadia_Real32Value bottom,
    Arcadia_Real32Value right,
    Arcadia_Real32Value top
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_ViewportNode, setRelativeViewportRectangle, self, left, bottom, right, top); }

void
Arcadia_Engine_Visuals_ViewportNode_getCanvasSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Real32Value* width,
    Arcadia_Real32Value* height
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_ViewportNode, getCanvasSize, self, width, height); }

void
Arcadia_Engine_Visuals_ViewportNode_setCanvasSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Real32Value width,
    Arcadia_Real32Value height
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_ViewportNode, setCanvasSize, self, width, height); }
