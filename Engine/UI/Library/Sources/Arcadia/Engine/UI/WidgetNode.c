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

#define ARCADIA_ENGINE_UI_PRIVATE (1)
#include "Arcadia/Engine/UI/WidgetNode.h"

#include "Arcadia/Engine/UI/Events/WidgetPositionChangedEvent.h"
#include "Arcadia/Engine/UI/Events/WidgetSizeChangedEvent.h"

static void
Arcadia_Engine_UI_WidgetNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self
  );

static void
Arcadia_Engine_UI_WidgetNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNodeDispatch* self
  );

static void
Arcadia_Engine_UI_WidgetNode_getPositionImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value* left,
    Arcadia_Integer32Value* top
  );

static void
Arcadia_Engine_UI_WidgetNode_getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  );

static void
Arcadia_Engine_UI_WidgetNode_setPositionImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value top
  );

static void
Arcadia_Engine_UI_WidgetNode_setSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
Arcadia_Engine_UI_WidgetNode_raiseSignalImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Engine_UI_WidgetEvent* event
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_UI_WidgetNode_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_WidgetNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.UI.WidgetNode", Arcadia_Engine_UI_WidgetNode,
                         u8"Arcadia.Engine.UI.Node", Arcadia_Engine_UI_Node,
                         &_typeOperations);

static void
Arcadia_Engine_UI_WidgetNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_WidgetNode);
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_WidgetNode);
}

static void
Arcadia_Engine_UI_WidgetNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNodeDispatch* self
  )
{
  self->getPosition = &Arcadia_Engine_UI_WidgetNode_getPositionImpl;
  self->getSize = &Arcadia_Engine_UI_WidgetNode_getSizeImpl;
  self->setPosition = &Arcadia_Engine_UI_WidgetNode_setPositionImpl;
  self->setSize = &Arcadia_Engine_UI_WidgetNode_setSizeImpl;
  self->raiseSignal = &Arcadia_Engine_UI_WidgetNode_raiseSignalImpl;
}

static void
Arcadia_Engine_UI_WidgetNode_getPositionImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value* left,
    Arcadia_Integer32Value* top
  )
{
  *left = self->left;
  *top = self->top;
}

static void
Arcadia_Engine_UI_WidgetNode_getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{
  *width = self->width;
  *height = self->height;
}

static void
Arcadia_Engine_UI_WidgetNode_setPositionImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value top
  )
{
  if (left < Arcadia_Integer32Value_Minimum / 2 || left > Arcadia_Integer32Value_Maximum / 2) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (top < Arcadia_Integer32Value_Minimum / 2 || top > Arcadia_Integer32Value_Maximum / 2) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_BooleanValue changed = Arcadia_BooleanValue_False;
  changed |= self->left != left;
  changed |= self->top != top;
  self->left = left;
  self->top = top;
  if (changed) {
    Arcadia_Engine_UI_WidgetEvent* e = (Arcadia_Engine_UI_WidgetEvent*)Arcadia_Engine_UI_WidgetPositionChangedEvent_create(thread, Arcadia_getTickCount(thread), self, left, top);
    Arcadia_Engine_UI_WidgetNode_raiseSignal(thread, self, e);
  }
}

static void
Arcadia_Engine_UI_WidgetNode_setSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  if (width < Arcadia_Integer32Value_Minimum / 2 || width > Arcadia_Integer32Value_Maximum / 2) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (height < Arcadia_Integer32Value_Minimum / 2 || height > Arcadia_Integer32Value_Maximum / 2) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_BooleanValue changed = Arcadia_BooleanValue_False;
  changed |= self->width != width;
  changed |= self->height != height;
  self->width = width;
  self->height = height;
  if (changed) {
    Arcadia_Engine_UI_WidgetEvent* e = (Arcadia_Engine_UI_WidgetEvent*)Arcadia_Engine_UI_WidgetSizeChangedEvent_create(thread, Arcadia_getTickCount(thread), self, width, height);
    Arcadia_Engine_UI_WidgetNode_raiseSignal(thread, self, e);
  }
}

static void
Arcadia_Engine_UI_WidgetNode_raiseSignalImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Engine_UI_WidgetEvent* event
  )
{/*@todo Add implementation.*/}

Arcadia_Engine_UI_WidgetNode*
Arcadia_Engine_UI_WidgetNode_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_WidgetNode);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_UI_WidgetNode);
}

void
Arcadia_Engine_UI_WidgetNode_render
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  ) 
{ Arcadia_VirtualCall(Arcadia_Engine_UI_WidgetNode, render, self, enterPassNode); }

void
Arcadia_Engine_UI_WidgetNode_getPosition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value *left,
    Arcadia_Integer32Value *top
  )
{ Arcadia_VirtualCall(Arcadia_Engine_UI_WidgetNode, getPosition, self, left, top); }

void
Arcadia_Engine_UI_WidgetNode_getSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{ Arcadia_VirtualCall(Arcadia_Engine_UI_WidgetNode, getSize, self, width, height); }

void
Arcadia_Engine_UI_WidgetNode_setPosition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value top
  )
{ Arcadia_VirtualCall(Arcadia_Engine_UI_WidgetNode, setPosition, self, left, top); }

void
Arcadia_Engine_UI_WidgetNode_setSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{ Arcadia_VirtualCall(Arcadia_Engine_UI_WidgetNode, setSize, self, width, height); }

void
Arcadia_Engine_UI_WidgetNode_raiseSignal
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Engine_UI_WidgetEvent* event
  )
{ Arcadia_VirtualCall(Arcadia_Engine_UI_WidgetNode, raiseSignal, self, event); }
