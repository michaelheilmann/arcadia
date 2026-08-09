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

#if !defined(ARCADIA_VISUALS_CONFIGURATION_H_INCLUDED)
#define ARCADIA_VISUALS_CONFIGURATION_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring2/Include.h"

/// @code
/// class Arcadia.Engine.Visuals.Configuration extends Arcadia.Object {
///   constructor()
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.Configuration", Arcadia_Visuals_Configuration,
                          u8"Arcadia.Object")

struct Arcadia_Visuals_ConfigurationDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Visuals_Configuration {
  Arcadia_Object _parent;
  struct {

    struct {

      /// Must be non-negative integer.
      /// Default: 4.
      Arcadia_String* major;

      /// Must be non-negative integer.
      /// Default: 3.
      Arcadia_String* minor;

    } version;

  } opengl;

  struct {

    /// Must be non-negative integer.
    /// Default: 24.
    Arcadia_String* depthBits;

  } depthBuffer;

  struct {

    /// Must be non-negative.
    /// Default: 8.
    Arcadia_String* redBits;

    /// Must be non-negative.
    /// Default: 8.
    Arcadia_String* greenBits;

    /// Must be non-negative.
    /// Default: 8.
    Arcadia_String* blueBits;

    /// Must be non-negative.
    /// Default: 8.
    Arcadia_String*  alphaBits;

  } colorBuffer;

};

Arcadia_Visuals_Configuration*
Arcadia_Visuals_Configuration_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_VISUALS_CONFIGURATION_H_INCLUDED
