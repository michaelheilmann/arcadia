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

#if !defined(ARCADIA_MATH_QUATERNIONREAL32_H_INCLUDED)
#define ARCADIA_MATH_QUATERNIONREAL32_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
typedef struct Arcadia_Math_Matrix4Real32 Arcadia_Math_Matrix4Real32;
typedef struct Arcadia_Math_Vector3Real32 Arcadia_Math_Vector3Real32;

Arcadia_declareObjectType(u8"Arcadia.Math.QuaternionReal32", Arcadia_Math_QuaternionReal32,
                          u8"Arcadia.Object");

struct Arcadia_Math_QuaternionReal32Dispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Math_QuaternionReal32 {
  Arcadia_Object _parent;
  Arcadia_Real32Value elements[4];
};

/// @brief Create the quaternion (x,y,z,w).
/// @param thread A pointer to this thread.
/// @param x, y, z, w The values for the x, y, z, and w elements of the quaternion.
/// @return A pointer to the quaternion.
Arcadia_Math_QuaternionReal32*
Arcadia_Math_QuaternionReal32_create
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value x,
    Arcadia_Real32Value y,
    Arcadia_Real32Value z,
    Arcadia_Real32Value w
  );

/// @brief Compute the product of this quaternion and another quaternion and store the result in this quaternion.
/// @param thread A pointer to this thread.
/// @param self A pointer to this quaternion.
/// @param other A pointer to the other quaternion.
/// @remarks
/// The product of two quaternions \(p\) and \(q\) is defined asctime
/// \[
/// \quat{p}\quat{q}=(&p_w\cdot q_w -p_x\cdot q_x  -p_y\cdot q_y -p_z\cdot q_z,\\
///                   &p_w\cdot q_x +p_x\cdot q_w  +p_y\cdot q_z -p_z\cdot q_y,\\
///                   &p_w\cdot q_y -p_x\cdot q_z  +p_y\cdot q_w +p_z\cdot q_x,\\
///                   &p_w\cdot q_z +p_x\cdot q_y  -p_y\cdot q_x +p_z\cdot q_w)
/// \]
void
Arcadia_Math_QuaternionReal32_multiply
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self,
    Arcadia_Math_QuaternionReal32* other
  );

/// @brief Create a quaternion representing the specified axis-angle rotation.
/// @param axis A pointer to the axis vector.
/// @param angle The angle of rotation in degrees.
/// @error Arcadia_Status_ArgumentValueInvalid axis is a null pointer or is a zero vector
void
Arcadia_Math_QuaternionReal32_setFromAxisAngle
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self,
    Arcadia_Math_Vector3Real32* axis,
    Arcadia_Real32Value angle
  );

/// @error Arcadia_Status_ArgumentValueInvalid self is a null pointer or is a zero quaternion
void
Arcadia_Math_QuaternionReal32_toAxisAngle
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self,
    Arcadia_Math_Vector3Real32* axis,
    Arcadia_Real32Value* angle
  );

/// @brief Normalize this quaterion.
/// @param thread A pointer to this thread.
/// @param self A pointer to this quaternion.
/// @error #Arcadia_Status_DivisionByZero The quaternion is the zero quaterion.
void
Arcadia_Math_QuaternionReal32_normalize
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self
  );

/// @brief Get the length of this quaternion.
/// @param thread A pointer to this thread.
/// @param self A pointer to this quaternion.
/// @return The length of this quaternion.
Arcadia_Real32Value
Arcadia_Math_QuaternionReal32_getLength
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self
  );

/// @brief Get the squared length of this quaternion.
/// @param thread A pointer to this thread.
/// @param self A pointer to this quaternion.
/// @return The squared length of this quaternion.
Arcadia_Real32Value
Arcadia_Math_QuaternionReal32_getSquaredLength
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self
  );

/// @brief Assign the values of another quaternion to this quaternion.
/// @param thread A pointer to this thread.
/// @param self A pointer to this quaternion.
/// @param other A pointer to the other quaternion.
void
Arcadia_Math_QuaternionReal32_assign
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self,
    Arcadia_Math_QuaternionReal32* other
  );

/// @brief Convert this quaternion to a rotation matrix.
/// @param thread A pointer to this thread.
/// @param self A pointer to this quaternion.
/// @param target A pointer to the matrix to assign the component values to.
void
Arcadia_Math_QuaternionReal32_toRotationMatrix
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self,
    Arcadia_Math_Matrix4Real32* target
  );

/// @brief Transform a direction vector.
/// @param thread A pointer to this thread.
/// @param self A pointer to this quaternion.
/// @param v A pointer to the vector.
void
Arcadia_Math_QuaternionReal32_transformVector
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self,
    Arcadia_Math_Vector3Real32* v
  );
  
/// @brief Compute the conjugate quaternion of this quaternion. Store the result in this quaternion.
/// @param thread A pointer to this thread.
/// @param self A pointer to this quaternion.
void
Arcadia_Math_QuaternionReal32_conjugate
  (
    Arcadia_Thread* thread,
    Arcadia_Math_QuaternionReal32* self
  );

#endif // ARCADIA_MATH_QUATERNIONREAL32_H_INCLUDED
