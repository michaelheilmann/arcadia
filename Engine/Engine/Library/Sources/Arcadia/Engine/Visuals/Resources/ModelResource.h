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

#if !defined(ARCADIA_ENGINE_VISUALS_RESOURCES_MODELRESOURCE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_RESOURCES_MODELRESOURCE_H_INCLUDED

#include "Arcadia/Engine/Visuals/Resource.h"
typedef struct Arcadia_Engine_Visuals_ConstantBufferResource Arcadia_Engine_Visuals_ConstantBufferResource;
typedef struct Arcadia_Engine_Visuals_MaterialResource Arcadia_Engine_Visuals_MaterialResource;
typedef struct Arcadia_Engine_Visuals_VertexBufferResource Arcadia_Engine_Visuals_VertexBufferResource;

#define Arcadia_Engine_Visuals_ModelResource_LocalToWorldMatrixDirty (1)

Arcadia_declareObjectType(u8"Arcadia.Visuals.Resources.ModelResource", Arcadia_Engine_Visuals_ModelResource,
                          u8"Arcadia.Visuals.Resource");

struct Arcadia_Engine_Visuals_ModelResourceDispatch {
  Arcadia_Engine_Visuals_ResourceDispatch _parent;
  void (*setLocalToWorldMatrix)(Arcadia_Thread*, Arcadia_Engine_Visuals_ModelResource*, Arcadia_Math_Matrix4Real32*);
};

struct Arcadia_Engine_Visuals_ModelResource {
  Arcadia_Engine_Visuals_Resource _parent;
  // The dirty flags.
  Arcadia_Natural8Value dirty;
  // The local to world matrix of the model.
  // The default value is the identity matrix.
  Arcadia_Math_Matrix4Real32* localToWorldMatrix;

  /// Constant buffer with model-specific information.
  /// This is currently only the model to world matrix.
  Arcadia_Engine_Visuals_ConstantBufferResource* constantBuffer;

  /// Constant buffer with mesh-specific information.
  /// This is currently only the mesh color.
  Arcadia_Engine_Visuals_ConstantBufferResource* meshConstantBuffer;

  /// The vertices of the mesh.
  Arcadia_Engine_Visuals_VertexBufferResource* meshVertexBuffer;

  /// The material of the model.
  Arcadia_Engine_Visuals_MaterialResource* material;

};

void
Arcadia_Engine_Visuals_ModelResource_setLocalToWorldMatrix
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ModelResource* self,
    Arcadia_Math_Matrix4Real32* localToWorldMatrix
  );

#endif // ARCADIA_ENGINE_VISUALS_RESOURCES_MODELRESOURCE_H_INCLUDED
