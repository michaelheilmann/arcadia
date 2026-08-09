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

#if !defined(ARCADIA_ENGINE_VISUALS_BACKEND_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_BACKEND_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Visuals/Include.h` instead")
#endif

#include "Arcadia/Engine/Include.h"

// A visuals backend.
Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.Backend", Arcadia_Engine_Visuals_Backend,
                          u8"Arcadia.Engine.Backend");

struct Arcadia_Engine_Visuals_BackendDispatch {
  Arcadia_Engine_BackendDispatch _parent;
};

struct Arcadia_Engine_Visuals_Backend {
  Arcadia_Engine_Backend _parent;
};

#endif // ARCADIA_ENGINE_VISUALS_BACKEND_H_INCLUDED
