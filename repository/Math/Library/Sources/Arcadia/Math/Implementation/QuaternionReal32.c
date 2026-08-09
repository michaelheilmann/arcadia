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

#include "Arcadia/Math/Implementation/QuaternionReal32.h"

#include "Arcadia/Math/Implementation/trigonometry.h"
#include "Arcadia/Math/Implementation/Vector3Real32.h"
#include "Arcadia/Math/Implementation/Matrix4Real32.h"

#include "Arcadia/Math/Implementation/crossProduct.h"

static void
Arcadia_Math_QuaternionReal32_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self
  );

static void
Arcadia_Math_QuaternionReal32_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32Dispatch* self
  );

static void
Arcadia_Math_QuaternionReal32_clone
  (
    Arcadia_Thread* thread
  );

static void
Arcadia_Math_QuaternionReal32_isEqualTo
  (
    Arcadia_Thread* thread
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Math_QuaternionReal32_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Math_QuaternionReal32_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Math.QuaternionReal32", Arcadia_Math_QuaternionReal32,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Math_QuaternionReal32_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Math_QuaternionReal32);
  if (4 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  self->elements[0] = Arcadia_ValueStack_getReal32Value(thread, 4);
  self->elements[1] = Arcadia_ValueStack_getReal32Value(thread, 3);
  self->elements[2] = Arcadia_ValueStack_getReal32Value(thread, 2);
  self->elements[3] = Arcadia_ValueStack_getReal32Value(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_Math_QuaternionReal32);
}

static void
Arcadia_Math_QuaternionReal32_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32Dispatch* self
  )
{
  ((Arcadia_ObjectDispatch*)self)->clone = &Arcadia_Math_QuaternionReal32_clone;
  ((Arcadia_ObjectDispatch*)self)->isEqualTo = &Arcadia_Math_QuaternionReal32_isEqualTo;
}

static void
Arcadia_Math_QuaternionReal32_clone
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Natural8Value numberOfArguments = Arcadia_ValueStack_getNatural8Value(thread, 0);
  if (1 != numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Math_QuaternionReal32 *self = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Math_QuaternionReal32_getType(thread));
  Arcadia_Math_QuaternionReal32 *clone = Arcadia_Math_QuaternionReal32_create(thread, self->elements[0], self->elements[1], self->elements[2], self->elements[3]);
  Arcadia_ValueStack_popValues(thread, 2);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)clone);
}

static void
Arcadia_Math_QuaternionReal32_isEqualTo
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Natural8Value numberOfArguments = Arcadia_ValueStack_getNatural8Value(thread, 0);
  if (2 != numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Math_QuaternionReal32* self = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Math_QuaternionReal32_getType(thread));
  if (!Arcadia_ValueStack_isObjectReferenceValue(thread, 1)) {
    Arcadia_ValueStack_popValues(thread, 3);
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
  }
  Arcadia_Object* otherObject = Arcadia_ValueStack_getObjectReferenceValue(thread, 1);
  if (!Arcadia_Object_isInstanceOf(thread, otherObject, _Arcadia_Math_QuaternionReal32_getType(thread))) {
    Arcadia_ValueStack_popValues(thread, 3);
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
  }
  Arcadia_Math_QuaternionReal32* other = (Arcadia_Math_QuaternionReal32*)otherObject;
  Arcadia_ValueStack_popValues(thread, 3);
  Arcadia_ValueStack_pushBooleanValue(thread, self->elements[0] == other->elements[0]
                                           && self->elements[1] == other->elements[1]
                                           && self->elements[2] == other->elements[2]
                                           && self->elements[3] == other->elements[3]);
}

Arcadia_Math_QuaternionReal32*
Arcadia_Math_QuaternionReal32_create
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value x,
    Arcadia_Real32Value y,
    Arcadia_Real32Value z,
    Arcadia_Real32Value w
  )
{
  _Arcadia_BeginCreate(Arcadia_Math_QuaternionReal32);
  Arcadia_ValueStack_pushReal32Value(thread, x);
  Arcadia_ValueStack_pushReal32Value(thread, y);
  Arcadia_ValueStack_pushReal32Value(thread, z);
  Arcadia_ValueStack_pushReal32Value(thread, w);
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  _Arcadia_EndCreate(Arcadia_Math_QuaternionReal32);
}

void
Arcadia_Math_QuaternionReal32_multiply
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self,
    Arcadia_Math_QuaternionReal32* other
  )
{ 
  Arcadia_Real32Value w = self->elements[3] * other->elements[3]
                        - self->elements[0] * other->elements[0]
                        - self->elements[1] * other->elements[1]
                        - self->elements[2] * other->elements[2];

  Arcadia_Real32Value x = self->elements[3] * other->elements[0]
                        + self->elements[0] * other->elements[3]
                        + self->elements[1] * other->elements[2]
                        - self->elements[2] * other->elements[1];

  Arcadia_Real32Value y = self->elements[3] * other->elements[1]
                        - self->elements[0] * other->elements[2]
                        + self->elements[1] * other->elements[3]
                        + self->elements[2] * other->elements[0];

  Arcadia_Real32Value z = self->elements[3] * other->elements[2]
                        + self->elements[0] * other->elements[1]
                        - self->elements[1] * other->elements[0]
                        + self->elements[2] * other->elements[3];
  self->elements[3] = w;
  self->elements[0] = x;
  self->elements[1] = y;
  self->elements[2] = z;
}

void
Arcadia_Math_QuaternionReal32_setFromAxisAngle
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self,
    Arcadia_Math_Vector3Real32* axis,
    Arcadia_Real32Value angle
  )
{
  angle = Arcadia_Math_degreesToRadiansReal32Value(thread, angle) / 2.f;
  Arcadia_Real32Value s = Arcadia_sinReal32Value(thread, angle);
  Arcadia_Real32Value c = Arcadia_cosReal32Value(thread, angle);
  Arcadia_Real32Value l = Arcadia_Math_Vector3Real32_getLength(thread, axis);
  if (0.f == l) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->elements[0] = (axis->elements[0] / l) * s;
  self->elements[1] = (axis->elements[1] / l) * s;
  self->elements[2] = (axis->elements[2] / l) * s;
  self->elements[3] = c;
}

void
Arcadia_Math_QuaternionReal32_toAxisAngle
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self,
    Arcadia_Math_Vector3Real32* axis,
    Arcadia_Real32Value* angle
  )
{
  
}

void
Arcadia_Math_QuaternionReal32_normalize
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self
  )
{
  Arcadia_Real32Value l = Arcadia_Math_QuaternionReal32_getLength(thread, self);
  if (0.f == l) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_DivisionByZero);
    Arcadia_Thread_jump(thread);
  }
  self->elements[0] /= l;
  self->elements[1] /= l;
  self->elements[2] /= l;
  self->elements[3] /= l;
}

Arcadia_Real32Value
Arcadia_Math_QuaternionReal32_getLength
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self
  )
{
  return Arcadia_sqrtReal32Value(thread, Arcadia_Math_QuaternionReal32_getSquaredLength(thread, self));
}

Arcadia_Real32Value
Arcadia_Math_QuaternionReal32_getSquaredLength
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self
  )
{
  return self->elements[0] * self->elements[0]
       + self->elements[1] * self->elements[1]
       + self->elements[2] * self->elements[2]
       + self->elements[3] * self->elements[3];
}

void
Arcadia_Math_QuaternionReal32_assign
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self,
    Arcadia_Math_QuaternionReal32* other
  )
{
  if (self != other) {
    self->elements[0] = other->elements[0];
    self->elements[1] = other->elements[1];
    self->elements[2] = other->elements[2];
    self->elements[3] = other->elements[3];
  }
}

void
Arcadia_Math_QuaternionReal32_toRotationMatrix
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self,
    Arcadia_Math_Matrix4Real32* target
  )
{
  // If the quaternion was not normalized, then we would need to scale the matrix by 1 / (x^2 + y^2 + z^2 + w^2).
  Arcadia_Real32Value x = self->elements[0],
                      y = self->elements[1],
                      z = self->elements[2],
                      w = self->elements[3];
  Arcadia_Real32Value const values[] = {
    1.f - 2.f*(y*y + z*z),  2.f * (x * y - z * w),       2.f * (x * z + y * w),        0.f,
    2.f * (x * y + z * w),  1.f - 2.f * (x * x + z * z), 2.f * (y * z - x * w),        0.f,
    2.f * (x * z - y * w),  2.f * (y * z + x * w),       1.f - 2.f * (x * x + y * y),  0.f,
    0.f,                    0.f,                         0.f,                          1.f,
  };
  Arcadia_Math_Matrix4Real32_setWithValues(thread, target, &values[0]);
}

static Arcadia_Math_Vector3Real32*
makeVector
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value x,
    Arcadia_Real32Value y,
    Arcadia_Real32Value z
  )
{
  Arcadia_Math_Vector3Real32* v = Arcadia_Math_Vector3Real32_create(thread);
  v->elements[0] = x;
  v->elements[1] = y;
  v->elements[2] = z;
  return v;
}

void
Arcadia_Math_QuaternionReal32_transformVector
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self,
    Arcadia_Math_Vector3Real32* v
  )
{
  Arcadia_Math_Vector3Real32* uv, *uuv;
  Arcadia_Math_Vector3Real32* q = makeVector(thread, self->elements[0], self->elements[1], self->elements[2]);
  uv = Arcadia_Math_Vector3Real32_crossProduct(thread, q, v);
  uuv = Arcadia_Math_Vector3Real32_crossProduct(thread, q, uv);
  Arcadia_Math_Vector3Real32_multiplyScalar(thread, uv, 2.f * self->elements[3]);
  Arcadia_Math_Vector3Real32_multiplyScalar(thread, uuv, 2.f);
  Arcadia_Math_Vector3Real32_add(thread, v, uv);
  Arcadia_Math_Vector3Real32_add(thread, v, uuv);
}

/// Any unit quaternion represents a rotation.
/// @remark
/// A unit quaternion \(q\) represents a rotation. The conjugate quaternion \(\overline{q}=(q_w,-q_x,-q_y,-q_z)\) represents the inverse rotation.
/// We show this by multiplying the two quaternions
/// \[
/// \quat{p}\overline{\quat{p}}=(&p_w \cdot  p_w - p_x \cdot -p_x  - p_y \cdot -p_y - p_z\cdot -p_z,\\
///                              &p_w \cdot -p_x + p_x \cdot  p_w  + p_y \cdot -p_z - p_z\cdot -p_y,\\
///                              &p_w \cdot -p_y - p_x \cdot -p_z  + p_y \cdot  p_w  + p_z\cdot -p_x,\\
///                              &p_w \cdot -p_z + p_x \cdot -p_y  - p_y \cdot -p_x + p_z\cdot  p_w)\\
/// \quat{p}\overline{\quat{p}}=(&p_w \cdot  p_w + p_x \cdot  p_x  + p_y \cdot  p_y  + p_z\cdot p_z,\\
///                              &0,\\
///                              &0,\\
///                              &0)
/// \]
/// As \(p\) is a unit quaternion
/// \quat{p}\overline{\quat{p}}=(&1,\\
///                              &0,\\
///                              &0,\\
///                              &0)
void
Arcadia_Math_QuaternionReal32_conjugate
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self
  )
{ 
  self->elements[3] = self->elements[3];
  self->elements[0] = -self->elements[0];
  self->elements[1] = -self->elements[1];
  self->elements[2] = -self->elements[2];
}
