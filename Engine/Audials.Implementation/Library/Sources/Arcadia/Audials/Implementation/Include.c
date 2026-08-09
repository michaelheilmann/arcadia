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

#define ARCADIA_ENGINE_AUDIALS_IMPLEMENTATION_PRIVATE (1)
#include "Arcadia/Audials/Implementation/Include.h"

#if Arcadia_Engine_Audials_Implementation_Configuration_OpenAL_Backend_Enabled
  #include "Arcadia/Audials/Implementation/OpenAL/Backend.h"
#endif

void
Arcadia_Engine_Audials_Implementation_registerBackends
  (
    Arcadia_Thread* thread,
    Arcadia_Set* types
  )
{
#if Arcadia_Engine_Audials_Implementation_Configuration_OpenAL_Backend_Enabled
  Arcadia_Set_add(thread, types, Arcadia_Value_makeTypeValue(_Arcadia_Engine_Audials_Implementation_OpenAL_Backend_getType(thread)), NULL);
#endif
}
