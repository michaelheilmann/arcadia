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
#include "Arcadia/Engine/Visuals/Window.h"

static void
Arcadia_Engine_Visuals_Window_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self
  );

static void
Arcadia_Engine_Visuals_Window_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowDispatch* self
  );

static void
Arcadia_Engine_Visuals_Window_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self
  );

static void
Arcadia_Engine_Visuals_Window_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_Window_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_Window_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_Window_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_Window_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Visuals.Window", Arcadia_Engine_Visuals_Window,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_Window_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_Window);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  self->backend = NULL;
  self->bigIcon = NULL;
  self->smallIcon = NULL;
  self->title = Arcadia_String_createFromCxxString(thread, u8"Arcadia Engine Window");
  self->fullscreen = Arcadia_BooleanValue_False;
  self->bounds.left = 0;
  self->bounds.top = 0;
  self->bounds.width = 1;
  self->bounds.height = 1;

  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_Window);
}

static void
Arcadia_Engine_Visuals_Window_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_Window_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_Window_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self
  )
{
  if (self->title) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->title);
  }
  if (self->backend) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->backend);
  }
  if (self->smallIcon) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->smallIcon);
  }
  if (self->bigIcon) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->bigIcon);
  }
}

void
Arcadia_Engine_Visuals_Window_open
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self
  )
{
  if (self->backend) {
    Arcadia_Engine_Visuals_WindowBackend_open(thread, self->backend);
    Arcadia_Engine_Visuals_WindowBackend_setFullscreen(thread, self->backend, self->fullscreen);
    Arcadia_Engine_Visuals_WindowBackend_setPosition(thread, self->backend, self->bounds.left, self->bounds.top);
    Arcadia_Engine_Visuals_WindowBackend_setSize(thread, self->backend, self->bounds.width, self->bounds.height);
    Arcadia_Engine_Visuals_WindowBackend_setTitle(thread, self->backend, self->title);
  }
}

void
Arcadia_Engine_Visuals_Window_close
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self
  )
{
  if (self->backend) {
    Arcadia_Engine_Visuals_WindowBackend_close(thread, self->backend);
  }
}

void
Arcadia_Engine_Visuals_Window_getRequiredBigIconSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{ Arcadia_Engine_Visuals_WindowBackend_getRequiredBigIconSize(thread, self->backend, width, height); }

void
Arcadia_Engine_Visuals_Window_getRequiredSmallIconSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{ Arcadia_Engine_Visuals_WindowBackend_getRequiredSmallIconSize(thread, self->backend, width, height); }

Arcadia_Engine_Visuals_Icon*
Arcadia_Engine_Visuals_Window_getBigIcon
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self
  )
{ return Arcadia_Engine_Visuals_WindowBackend_getBigIcon(thread, self->backend); }

void
Arcadia_Engine_Visuals_Window_setBigIcon
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self,
    Arcadia_Engine_Visuals_Icon* icon
  )
{ Arcadia_Engine_Visuals_WindowBackend_setBigIcon(thread, self->backend, icon); }

Arcadia_Engine_Visuals_Icon*
Arcadia_Engine_Visuals_Window_getSmallIcon
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self
  )
{ return Arcadia_Engine_Visuals_WindowBackend_getSmallIcon(thread, self->backend); }

void
Arcadia_Engine_Visuals_Window_setSmallIcon
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self,
    Arcadia_Engine_Visuals_Icon* icon
  )
{ Arcadia_Engine_Visuals_WindowBackend_setSmallIcon(thread, self->backend, icon); }

Arcadia_String*
Arcadia_Engine_Visuals_Window_getTitle
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self
  )
{ return self->title; }

void
Arcadia_Engine_Visuals_Window_setTitle
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self,
    Arcadia_String* title
  )
{
  self->title = title;
  if (self->backend) {
    Arcadia_Engine_Visuals_WindowBackend_setTitle(thread, self->backend, title);
  }
}

void
Arcadia_Engine_Visuals_Window_getCanvasSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{
  if (!self->backend) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NotImplemented);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_Visuals_WindowBackend_getCanvasSize(thread, self->backend, width, height);
}

void
Arcadia_Engine_Visuals_Window_beginRender
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self
  )
{
  if (!self->backend) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NotImplemented);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_Visuals_WindowBackend_beginRender(thread, self->backend);
}

void
Arcadia_Engine_Visuals_Window_endRender
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self
  )
{
  if (!self->backend) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NotImplemented);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_Visuals_WindowBackend_endRender(thread, self->backend);
}

void
Arcadia_Engine_Visuals_Window_setPosition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value top
  )
{
  self->bounds.left = left;
  self->bounds.top = top;
  if (self->backend) {
    Arcadia_Engine_Visuals_WindowBackend_setPosition(thread, self->backend, left, top);
  }
}

void
Arcadia_Engine_Visuals_Window_getPosition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self,
    Arcadia_Integer32Value* left,
    Arcadia_Integer32Value* top
  )
{
  *left = self->bounds.left;
  *top = self->bounds.top;
}

void
Arcadia_Engine_Visuals_Window_setSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  self->bounds.width = width;
  self->bounds.height = height;
  if (self->backend) {
    Arcadia_Engine_Visuals_WindowBackend_setSize(thread, self->backend, width, height);
  }
}

void
Arcadia_Engine_Visuals_Window_getSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{
  *width = self->bounds.width;
  *height = self->bounds.height;
}

Arcadia_BooleanValue
Arcadia_Engine_Visuals_Window_getFullscreen
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self
  )
{
  return self->fullscreen;
}

void
Arcadia_Engine_Visuals_Window_setFullscreen
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* self,
    Arcadia_BooleanValue fullscreen
  )
{
  self->fullscreen = fullscreen;
  if (self->backend) {
    Arcadia_Engine_Visuals_WindowBackend_setFullscreen(thread, self->backend, fullscreen);
  }
}

Arcadia_Engine_Visuals_Window*
Arcadia_Engine_Visuals_Window_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_Window);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_Window);
}
