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

#if !defined(ARCADIA_STARSHIP_VIEWER3D_H_INCLUDED)
#define ARCADIA_STARSHIP_VIEWER3D_H_INCLUDED

#include "Arcadia/Math/Include.h"
#include "Arcadia/Starship/OrientationChangeInputEvent.h"
#include "Arcadia/Starship/PositionChangeInputEvent.h"

/// A "free fly" viewer.
/// 'w' moves forward.
/// 's' moves backward.
/// 'a' moves left.
/// 'd' moves right.
/// 'q' rotates left around its axis.
/// 'e' rotates right around its axis.
/// 
///  We use a right-handed coordinate system.
/// - negative z-axis forward, positive z-axis backward
/// - negative y-axis down, positive y-axis up
/// - negative x-axis left, positive x-axis right
/// 
/// Initial values:
/// - position = (0, 0, +1)
/// - forward = (0, 0, -1)
/// - right = (+1, 0, 0)
/// - yaw = 0, pitch = 0, roll = 0
Arcadia_declareObjectType(u8"Arcadia.Starship.Viewer3D", Arcadia_Starship_Viewer3D,
                          u8"Arcadia.Object");

struct Arcadia_Starship_Viewer3DDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Starship_Viewer3D {
  Arcadia_Object _parent;
  
  /// The rotation of the viewer.
  Arcadia_Math_QuaternionReal32* rotation;
  /// The position of the viewer.
  Arcadia_Math_Vector3Real32* position;

  /// The forward vector of the viewer.
  Arcadia_Math_Vector3Real32* forward;

  /// The up vector of the viewer.
  Arcadia_Math_Vector3Real32* up;

  /// The right vector of the viewer.
  Arcadia_Math_Vector3Real32* right;

  
  /// "pitch" is the angle, in degrees, of a counter-clockwise rotation of the camera around its local x-axis
  /// The initial value is @a 0.
  Arcadia_Real32Value pitch;

  /// "yaw" is the angle, in degrees, of a counter-clockwise rotation of the camera around its local y-axis.
  /// The initial value is @a 0.
  Arcadia_Real32Value yaw;

  /// "roll" is the angle, in degrees, of a counter-clockwise rotation of the camera around its local z-axis
  /// The initial value is @a 0.
  Arcadia_Real32Value roll;
};

/// @brief Create a viewer with default values.
/// @return A pointer to the viewer.
Arcadia_Starship_Viewer3D*
Arcadia_Starship_Viewer3D_create
  (
    Arcadia_Thread* thread
  );

/// @brief Move the viewer to the specified position.
/// @param thread A pointer to this thread.
/// @param self A pointer to this viewer.
/// @param position A pointer to the position vector.
/// @warning This ignores the simulation.
void
Arcadia_Starship_Viewer3D_setPosition
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self,
    Arcadia_Math_Vector3Real32* position
  );

/// @brief Move the viewer into the specified direction.
/// @param thread A pointer to this thread.
/// @param self A pointer to this viewer.
/// @param direction A pointer to the direction vector.
/// @warning This ignores the simulation.
void
Arcadia_Starship_Viewer3D_move
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self,
    Arcadia_Math_Vector3Real32* direction
  );

/// @brief Get the vector pointing backward.
/// @param thread A pointer to this thread.
/// @param self A pointer to this viewer.
Arcadia_Math_Vector3Real32*
Arcadia_Starship_Viewer3D_getBackward
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  );

/// @brief Get the vector pointing downward.
/// @param thread A pointer to this thread.
/// @param self A pointer to this viewer.
Arcadia_Math_Vector3Real32*
Arcadia_Starship_Viewer3D_getDownward
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  );

/// @brief Get the vector pointing forward.
/// @param thread A pointer to this thread.
/// @param self A pointer to this viewer.
Arcadia_Math_Vector3Real32*
Arcadia_Starship_Viewer3D_getForward
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  );

/// @brief Get the vector pointing leftward.
/// @param thread A pointer to this thread.
/// @param self A pointer to this viewer.
Arcadia_Math_Vector3Real32*
Arcadia_Starship_Viewer3D_getLeftward
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  );

/// @brief Get the vector pointing rightward.
/// @param thread A pointer to this thread.
/// @param self A pointer to this viewer.
Arcadia_Math_Vector3Real32*
Arcadia_Starship_Viewer3D_getRightward
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  );

/// @brief Get the vector pointing upward.
/// @param thread A pointer to this thread.
/// @param self A pointer to this viewer.
Arcadia_Math_Vector3Real32*
Arcadia_Starship_Viewer3D_getUpward
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  );

/// Get the local to world space matrix.
/// @param thread A pointer to this thread.
/// @param self A pointer to this viewer.
/// @return A pointer to the local space to world space matrix.
/// @remarks A copy of the original object is returned.
/// @remarks
/// The transformation matrix M of the viewer is given by
/// \[
/// M = ((T*Ry)*Rx)*Rz
/// \]
/// where
/// - \(Rx\) is the matrix to rotate around the x-axis
/// - \(Ry\) is the matrix to rotate around the y-axis
/// - \(Rz\) is the matrix to rotate around the z-axis
/// - \(T\) is the matrix to translate
Arcadia_Math_Matrix4Real32*
Arcadia_Starship_Viewer3D_getLoclaToWorldTransform
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  );

/// @brief Get the world space to view space matrix.
/// @param thread A pointer to this thread.
/// @param self A pointer to this viewer.
/// @return A pointer to the world space to view space matrix.
/// @remarks A copy of the original object is returned.
/// @remarks
/// The transformation matrix M of the viewer is given by
/// \[
/// M = ((T*Ry)*Rx)*Rz
/// \]
/// where
/// - \(Rx\) is the matrix to rotate around the x-axis
/// - \(Ry\) is the matrix to rotate around the y-axis
/// - \(Rz\) is the matrix to rotate around the z-axis
/// - \(T\) is the matrix to translate
/// The inverse \(inv(M)\) is given by
/// \[
///  &inv(M)\\
/// =&inv(((T*Ry)*Rx)*Rz)\\
/// =&inv(Rx*Rz)*inv(T*Ry)\\
/// =&inv(Rz)*(inv(Rx)*inv(T*Ry))\\
/// =&inv(Rz)*(inv(Rx)*(inv(Ry)*inv(T)))
/// \]
Arcadia_Math_Matrix4Real32*
Arcadia_Starship_Viewer3D_getWorldToViewMatrix
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  );

/// @brief Process an orientation change input event.
/// @param thread A pointer to this thread.
/// @param self A pointer to this viewer.
/// @param event A pointer to the event.
void
Arcadia_Starship_Viewer3D_onOrientationChangeInputEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self,
    Arcadia_Starship_OrientationChangeInputEvent* event
  );

/// @brief Process a position change input event.
/// @param thread A pointer to this thread.
/// @param self A pointer to this viewer.
/// @param event A pointer to the event.
void
Arcadia_Starship_Viewer3D_onPositionChangeInputEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self,
    Arcadia_Starship_PositionChangeInputEvent* event
  );

#endif // ARCADIA_STARSHIP_VIEWER3D_H_INCLUDED
