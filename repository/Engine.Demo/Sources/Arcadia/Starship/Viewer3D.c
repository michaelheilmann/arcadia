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

#include "Arcadia/Starship/Viewer3D.h"

static void
Arcadia_Math_Matrix4x4Real32_transformDirection
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Matrix4Real32* self,
    Arcadia_Math_Vector3Real32* direction
  );

static void
Arcadia_Math_Matrix4x4Real32_transformPosition
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Matrix4Real32* self,
    Arcadia_Math_Vector3Real32* position
  );

static void
Arcadia_Starship_Viewer3D_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  );

static void
Arcadia_Starship_Viewer3D_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3DDispatch* self
  );

static void
Arcadia_Starship_Viewer3D_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_Starship_Viewer3D_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Starship_Viewer3D_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Starship_Viewer3D_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Starship_Viewer3D_visit,
};

static const Arcadia_Type_Operations _Arcadia_Starship_Viewer3D_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_Starship_Viewer3D_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Starship.Viewer3D", Arcadia_Starship_Viewer3D,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_Arcadia_Starship_Viewer3D_typeOperations);

static void
Arcadia_Math_Matrix4x4Real32_transformDirection
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Matrix4Real32* self,
    Arcadia_Math_Vector3Real32* direction
  )
{
  direction->elements[0] = self->elements[0][0] * direction->elements[0]
                         + self->elements[0][1] * direction->elements[1]
                         + self->elements[0][2] * direction->elements[2]
                         + self->elements[0][3] * 0.f;
  direction->elements[1] = self->elements[1][0] * direction->elements[0]
                         + self->elements[1][1] * direction->elements[1]
                         + self->elements[1][2] * direction->elements[2]
                         + self->elements[1][3] * 0.f;
  direction->elements[2] = self->elements[2][0] * direction->elements[0]
                         + self->elements[2][1] * direction->elements[1]
                         + self->elements[2][2] * direction->elements[2]
                         + self->elements[2][3] * 0.f;
}

static void
Arcadia_Math_Matrix4x4Real32_transformPosition
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Matrix4Real32* self,
    Arcadia_Math_Vector3Real32* position
  )
{
  position->elements[0] = self->elements[0][0] * position->elements[0]
                        + self->elements[0][1] * position->elements[1]
                        + self->elements[0][2] * position->elements[2]
                        + self->elements[0][3] * 1.f;
  position->elements[1] = self->elements[1][0] * position->elements[0]
                        + self->elements[1][1] * position->elements[1]
                        + self->elements[1][2] * position->elements[2]
                        + self->elements[1][3] * 1.f;
  position->elements[2] = self->elements[2][0] * position->elements[0]
                        + self->elements[2][1] * position->elements[1]
                        + self->elements[2][2] * position->elements[2]
                        + self->elements[2][3] * 1.f;
}

static void
Arcadia_Starship_Viewer3D_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Starship_Viewer3D);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  {
    Arcadia_Real32Value values[] = { 0.f, 0.f, +1.f };
    self->position = Arcadia_Math_Vector3Real32_createWithValues(thread, &values[0]);
  }
  {
    Arcadia_Real32Value values[] = { 0.f, 0.f, -1.f };
    self->forward = Arcadia_Math_Vector3Real32_createWithValues(thread, &values[0]);
  }
  {
    Arcadia_Real32Value values[] = { 0.f, +1.f, 0.f };
    self->up = Arcadia_Math_Vector3Real32_createWithValues(thread, &values[0]);
  }
  {
    Arcadia_Real32Value values[] = { +1.f, 0.f, 0.f };
    self->right = Arcadia_Math_Vector3Real32_createWithValues(thread, &values[0]);
  }
  //
  self->yaw = 0.f;
  self->pitch = 0.f;
  self->roll = 0.f;
  //
  self->rotation = Arcadia_Math_QuaternionReal32_create(thread, 0, 0, 0, 1);
  Arcadia_Real32Value values[] = { 0.f, 1.f, 0.f };
  Arcadia_Math_QuaternionReal32_setFromAxisAngle(thread, self->rotation, Arcadia_Math_Vector3Real32_createWithValues(thread, values), 0.f);
  //
  Arcadia_LeaveConstructor(Arcadia_Starship_Viewer3D);
}

static void
Arcadia_Starship_Viewer3D_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3DDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Starship_Viewer3D_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  )
{
  if (self->rotation) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->rotation);
  }
  if (self->forward) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->forward);
  }
  if (self->up) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->up);
  }
  if (self->right) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->right);
  }
  if (self->position) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->position);
  }
}

Arcadia_Starship_Viewer3D*
Arcadia_Starship_Viewer3D_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Starship_Viewer3D);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Starship_Viewer3D);
}

void
Arcadia_Starship_Viewer3D_setPosition
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self,
    Arcadia_Math_Vector3Real32* position
  )
{
  Arcadia_Math_Vector3Real32_assign(thread, self->position, position);  
}

void
Arcadia_Starship_Viewer3D_move
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self,
    Arcadia_Math_Vector3Real32* direction
  )
{
  Arcadia_Math_Vector3Real32_add(thread, self->position, direction);  
}

Arcadia_Math_Vector3Real32*
Arcadia_Starship_Viewer3D_getBackward
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  )
{ 
  // Create the initial backward vector.
  Arcadia_Real32Value values[] = { 0.f, 0.f, +1.f };
  Arcadia_Math_Vector3Real32* v = Arcadia_Math_Vector3Real32_createWithValues(thread, &values[0]);
  // Transform.
  Arcadia_Math_QuaternionReal32_transformVector(thread, self->rotation, v);
  // Return.
  return v;
}

Arcadia_Math_Vector3Real32*
Arcadia_Starship_Viewer3D_getDownward
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  )
{ 
  // Create the initial downward vector.
  Arcadia_Real32Value values[] = { 0.f, -1.f, 0.f };
  Arcadia_Math_Vector3Real32* v = Arcadia_Math_Vector3Real32_createWithValues(thread, &values[0]);
  // Transform.
  Arcadia_Math_QuaternionReal32_transformVector(thread, self->rotation, v);
  // Return.
  return v;
}

Arcadia_Math_Vector3Real32*
Arcadia_Starship_Viewer3D_getForward
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  )
{ 
  // Create the initial forward vector.
  Arcadia_Real32Value values[] = { 0.f, 0.f, -1.f };
  Arcadia_Math_Vector3Real32* v = Arcadia_Math_Vector3Real32_createWithValues(thread, &values[0]);
  // Transform.
  Arcadia_Math_QuaternionReal32_transformVector(thread, self->rotation, v);
  // Return.
  return v;
}

Arcadia_Math_Vector3Real32*
Arcadia_Starship_Viewer3D_getRightward
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  )
{ 
  // Create the initial rightward vector.
  Arcadia_Real32Value values[] = { +1.f, 0.f, 0.f };
  Arcadia_Math_Vector3Real32* v = Arcadia_Math_Vector3Real32_createWithValues(thread, &values[0]);
  // Transform.
  Arcadia_Math_QuaternionReal32_transformVector(thread, self->rotation, v);
  // Return.
  return v;
}

Arcadia_Math_Vector3Real32*
Arcadia_Starship_Viewer3D_getLeftward
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  )
{ 
  // Create the initial rightward vector.
  Arcadia_Real32Value values[] = { -1.f, 0.f, 0.f };
  Arcadia_Math_Vector3Real32* v = Arcadia_Math_Vector3Real32_createWithValues(thread, &values[0]);
  // Transform.
  Arcadia_Math_QuaternionReal32_transformVector(thread, self->rotation, v);
  // Return.
  return v;
}

Arcadia_Math_Vector3Real32*
Arcadia_Starship_Viewer3D_getUpward
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  )
{
  // Create the initial upward vector.
  Arcadia_Real32Value values[] = { 0.f, +1.f, 0.f };
  Arcadia_Math_Vector3Real32* v = Arcadia_Math_Vector3Real32_createWithValues(thread, &values[0]);
  // Transform.
  Arcadia_Math_QuaternionReal32_transformVector(thread, self->rotation, v);
  // Return.
  return v;
}

Arcadia_Math_Matrix4Real32*
Arcadia_Starship_Viewer3D_getLoclaToWorldTransform
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  )
{ 
  Arcadia_Math_Matrix4Real32* r2 = Arcadia_Math_Matrix4Real32_create(thread);
  Arcadia_Math_QuaternionReal32_toRotationMatrix(thread, self->rotation, r2);

  Arcadia_Math_Matrix4Real32* t2 = Arcadia_Math_Matrix4Real32_create(thread);
  Arcadia_Math_Matrix4x4Real32_setTranslation(thread, t2, -self->position->elements[0], -self->position->elements[1], -self->position->elements[2]);

  Arcadia_Math_Matrix4Real32* tmp2 = Arcadia_Math_Matrix4Real32_multiply(thread, t2, r2);

  return tmp2;
}

Arcadia_Math_Matrix4Real32*
Arcadia_Starship_Viewer3D_getWorldToViewMatrix
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  )
{
  Arcadia_Math_QuaternionReal32* inverseRotation = Arcadia_Math_QuaternionReal32_create(thread, 0, 0, 0, 0);
  Arcadia_Math_QuaternionReal32_assign(thread, inverseRotation, self->rotation);
  Arcadia_Math_QuaternionReal32_conjugate(thread, inverseRotation);
  Arcadia_Math_Matrix4Real32* r2 = Arcadia_Math_Matrix4Real32_create(thread);
  Arcadia_Math_QuaternionReal32_toRotationMatrix(thread, inverseRotation, r2);

  Arcadia_Math_Matrix4Real32* t2 = Arcadia_Math_Matrix4Real32_create(thread);
  Arcadia_Math_Matrix4x4Real32_setTranslation(thread, t2, -self->position->elements[0], -self->position->elements[1], -self->position->elements[2]);
  
  Arcadia_Math_Matrix4Real32* tmp2 = Arcadia_Math_Matrix4Real32_multiply(thread, r2, t2);

  return tmp2;
}

void
Arcadia_Starship_Viewer3D_onOrientationChangeInputEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self,
    Arcadia_Starship_OrientationChangeInputEvent* event
  )
{ 
  Arcadia_Math_QuaternionReal32* temporary = Arcadia_Math_QuaternionReal32_create(thread, 0, 0, 0, 0);
  Arcadia_Math_QuaternionReal32_assign(thread, temporary, event->rotation);
  Arcadia_Math_QuaternionReal32_multiply(thread, temporary, self->rotation);
  Arcadia_Math_QuaternionReal32_normalize(thread, temporary);
  Arcadia_Math_QuaternionReal32_assign(thread, self->rotation, temporary);
}

void
Arcadia_Starship_Viewer3D_onPositionChangeInputEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self,
    Arcadia_Starship_PositionChangeInputEvent* event
  )
{ 
  Arcadia_Math_Vector3Real32_add(thread, self->position, event->translation);
}
