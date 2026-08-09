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

#if !defined(ARCADIA_ENGINE_VISUALS_RESOURCES_CONSTANTBUFFERRESOURCE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_RESOURCES_CONSTANTBUFFERRESOURCE_H_INCLUDED

#include "Arcadia/Engine/Visuals/Resource.h"

Arcadia_declareObjectType(u8"Arcadia.Visuals.ConstantBufferResource", Arcadia_Engine_Visuals_ConstantBufferResource,
                          u8"Arcadia.Visuals.Resource");

struct Arcadia_Engine_Visuals_ConstantBufferResourceDispatch {
  Arcadia_Engine_Visuals_ResourceDispatch _parent;

  void (*setData)(Arcadia_Thread*, Arcadia_Engine_Visuals_ConstantBufferResource*, const void*, Arcadia_SizeValue);
  void (*clear)(Arcadia_Thread*, Arcadia_Engine_Visuals_ConstantBufferResource*);
  void (*writeColor4Real32)(Arcadia_Thread*, Arcadia_Engine_Visuals_ConstantBufferResource*, Arcadia_Math_Color4Real32 const* source);
  void (*writeMatrix4x4Real32)(Arcadia_Thread*, Arcadia_Engine_Visuals_ConstantBufferResource*, Arcadia_BooleanValue transpose, Arcadia_Math_Matrix4Real32 const* source);
};

struct Arcadia_Engine_Visuals_ConstantBufferResource {
  Arcadia_Engine_Visuals_Resource _parent;
};

void
Arcadia_Engine_Visuals_ConstantBufferResource_setData
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResource* self,
    const void* bytes,
    Arcadia_SizeValue numberOfBytes
  );

void
Arcadia_Engine_Visuals_ConstantBufferResource_clear
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResource* self
  );

void
Arcadia_Engine_Visuals_ConstantBufferResource_writeColor4Real32
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResource* self,
    Arcadia_Math_Color4Real32 const* source
  );

void
Arcadia_Engine_Visuals_ConstantBufferResource_writeMatrix4x4Real32
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResource* self,
    Arcadia_BooleanValue transpose,
    Arcadia_Math_Matrix4Real32 const* source
  );

#endif // ARCADIA_ENGINE_VISUALS_RESOURCES_CONSTANTBUFFERRESOURCE_H_INCLUDED
