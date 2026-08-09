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
#include "Arcadia/Engine/Visuals/WindowBackend.h"

static void
Arcadia_Engine_Visuals_WindowBackend_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self
  );

static void
Arcadia_Engine_Visuals_WindowBackend_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackendDispatch* self
  );

static void
Arcadia_Engine_Visuals_WindowBackend_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_WindowBackend_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_WindowBackend_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_WindowBackend_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.WindowBackend", Arcadia_Engine_Visuals_WindowBackend,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_WindowBackend_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_WindowBackend);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  self->title = Arcadia_String_createFromCxxString(thread, u8"Arcadia Engine Window");

  self->bounds.left = 0;
  self->bounds.top = 0;
  self->bounds.width = 1;
  self->bounds.height = 1;

  self->mouse.inWindow = Arcadia_BooleanValue_False;
  self->mouse.oldx = 0;
  self->mouse.oldy = 0;

  self->fullscreen = Arcadia_BooleanValue_False;

  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_WindowBackend);
}

static void
Arcadia_Engine_Visuals_WindowBackend_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackendDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_WindowBackend_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self
  )
{
  if (self->title) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->title);
  }
  if (self->window) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->window);
  }
}

void
Arcadia_Engine_Visuals_WindowBackend_open
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_WindowBackend, open, self); }

void
Arcadia_Engine_Visuals_WindowBackend_close
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_WindowBackend, close, self); }

void
Arcadia_Engine_Visuals_WindowBackend_update
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_WindowBackend, update, self); }

void
Arcadia_Engine_Visuals_WindowBackend_getRequiredBigIconSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_WindowBackend, getRequiredBigIconSize, self, width, height); }

void
Arcadia_Engine_Visuals_WindowBackend_getRequiredSmallIconSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_WindowBackend, getRequiredSmallIconSize, self, width, height); }

Arcadia_Engine_Visuals_Icon*
Arcadia_Engine_Visuals_WindowBackend_getBigIcon
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_WindowBackend, getBigIcon, self); }

void
Arcadia_Engine_Visuals_WindowBackend_setBigIcon
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self,
    Arcadia_Engine_Visuals_Icon* icon
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_WindowBackend, setBigIcon, self, icon); }

Arcadia_Engine_Visuals_Icon*
Arcadia_Engine_Visuals_WindowBackend_getSmallIcon
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_WindowBackend, getSmallIcon, self); }

void
Arcadia_Engine_Visuals_WindowBackend_setSmallIcon
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self,
    Arcadia_Engine_Visuals_Icon* icon
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_WindowBackend, setSmallIcon, self, icon); }

Arcadia_String*
Arcadia_Engine_Visuals_WindowBackend_getTitle
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_WindowBackend, getTitle, self); }

void
Arcadia_Engine_Visuals_WindowBackend_setTitle
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self,
    Arcadia_String* title
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_WindowBackend, setTitle, self, title); }

void
Arcadia_Engine_Visuals_WindowBackend_getCanvasSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_WindowBackend, getCanvasSize, self, width, height); }

void
Arcadia_Engine_Visuals_WindowBackend_beginRender
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_WindowBackend, beginRender, self); }

void
Arcadia_Engine_Visuals_WindowBackend_endRender
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_WindowBackend, endRender, self); }

void
Arcadia_Engine_Visuals_WindowBackend_setPosition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value top
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_WindowBackend, setPosition, self, left, top); }

void
Arcadia_Engine_Visuals_WindowBackend_getPosition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self,
    Arcadia_Integer32Value* left,
    Arcadia_Integer32Value* top
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_WindowBackend, getPosition, self, left, top); }

void
Arcadia_Engine_Visuals_WindowBackend_setSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_WindowBackend, setSize, self, width, height); }

void
Arcadia_Engine_Visuals_WindowBackend_getSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_WindowBackend, getSize, self, width, height); }

Arcadia_BooleanValue
Arcadia_Engine_Visuals_WindowBackend_getFullscreen
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_WindowBackend, getFullscreen, self); }

void
Arcadia_Engine_Visuals_WindowBackend_setFullscreen
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowBackend* self,
    Arcadia_BooleanValue fullscreen
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_WindowBackend, setFullscreen, self, fullscreen); }
