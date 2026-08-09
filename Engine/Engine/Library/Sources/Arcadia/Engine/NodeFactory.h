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

#if !defined(ARCADIA_ENGINE_NODEFACTORY_H_INCLUDED)
#define ARCADIA_ENGINE_NODEFACTORY_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"

/// The base of all node factories.
/// The bases of implementations specific to audials and visuals can be found in Arcadia.Audials and Arcadia.Visuals, respectively.
Arcadia_declareObjectType(u8"Arcadia.Engine.NodeFactory", Arcadia_Engine_NodeFactory,
                          Arcadia_Object);

struct Arcadia_Engine_NodeFactoryDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Engine_NodeFactory {
  Arcadia_Object _parent;
};

#endif // ARCADIA_ENGINE_NODEFACTORY_H_INCLUDED
