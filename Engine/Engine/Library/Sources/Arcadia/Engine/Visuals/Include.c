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
#include "Arcadia/Engine/Visuals/Include.h"

#include "Arcadia/Engine/Visuals/NodeFactory.h"

void
Arcadia_Engine_Visuals_registerNodeFactories
  (
    Arcadia_Thread* thread,
    Arcadia_Set* types
  )
{ Arcadia_Set_add(thread, types, Arcadia_Value_makeTypeValue(_Arcadia_Engine_Visuals_NodeFactory_getType(thread)), NULL); }
