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

#if !defined(ARCADIA_ENGINE_INCLUDE_H_INCLUDED)
#define ARCADIA_ENGINE_INCLUDE_H_INCLUDED

#pragma push_macro("ARCADIA_ENGINE_PRIVATE")
#undef ARCADIA_ENGINE_PRIVATE
#define ARCADIA_ENGINE_PRIVATE (1)

#include "Arcadia/Engine/Backend.h"
#include "Arcadia/Engine/BackendContext.h"

#include "Arcadia/Engine/Engine.h"
#include "Arcadia/Engine/Event.h"

#include "Arcadia/Engine/Node.h"
#include "Arcadia/Engine/NodeFactory.h"

// Visuals.
#include "Arcadia/Engine/Visuals/Include.h"

// Audials.
#include "Arcadia/Engine/Audials/Include.h"

#undef ARCADIA_ENGINE_PRIVATE
#pragma pop_macro("ARCADIA_ENGINE_PRIVATE")

#endif // ARCADIA_ENGINE_INCLUDE_H_INCLUDED
