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

#include "Arcadia/Visuals/Implementation/Include.h"

#if Arcadia_Engine_Visuals_Implementation_Configuration_Direct3D12_Backend_Enabled
  #include "Arcadia/Visuals/Implementation/Direct3D12/Backend.h"
#endif

#if Arcadia_Engine_Visuals_Implementation_Configuration_OpenGL4_Backend_Enabled
  #include "Arcadia/Visuals/Implementation/OpenGL4/Backend.h"
#endif

#if Arcadia_Engine_Visuals_Implementation_Configuration_Vulkan_Backend_Enabled
  #include "Arcadia/Visuals/Implementation/Vulkan/Backend.h"
#endif

void
Arcadia_Engine_Visuals_Implementation_registerBackends
  (
    Arcadia_Thread* thread,
    Arcadia_Set* types
  )
{
#if Arcadia_Engine_Visuals_Implementation_Configuration_Direct3D12_Backend_Enabled
  Arcadia_Set_add(thread, types, Arcadia_Value_makeTypeValue(_Arcadia_Engine_Visuals_Implementation_Direct3D12_Backend_getType(thread)), NULL);
#endif
#if Arcadia_Engine_Visuals_Implementation_Configuration_OpenGL4_Backend_Enabled
  Arcadia_Set_add(thread, types, Arcadia_Value_makeTypeValue(_Arcadia_Engine_Visuals_Implementation_OpenGL4_Backend_getType(thread)), NULL);
#endif
#if Arcadia_Engine_Visuals_Implementation_Configuration_Vulkan_Backend_Enabled
  Arcadia_Set_add(thread, types, Arcadia_Value_makeTypeValue(_Arcadia_Engine_Visuals_Implementation_Vulkan_Backend_getType(thread)), NULL);
#endif
}
