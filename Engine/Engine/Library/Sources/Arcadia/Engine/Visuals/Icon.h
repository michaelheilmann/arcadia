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

#if !defined(ARCADIA_ENGINE_VISUALS_ICON_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_ICON_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"

/// @brief Base of native icons. Used with native windows.
/// @code
/// class Arcadia.Engine.Visuals.Icon {
///   constructor()
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.Icon", Arcadia_Engine_Visuals_Icon,
                          u8"Arcadia.Object")

struct Arcadia_Engine_Visuals_IconDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Engine_Visuals_Icon {
  Arcadia_Object _parent;
};

#endif // ARCADIA_ENGINE_VISUALS_ICON_H_INCLUDED
