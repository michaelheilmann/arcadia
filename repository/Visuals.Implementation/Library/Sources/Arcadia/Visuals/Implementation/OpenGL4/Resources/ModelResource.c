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

#include "Arcadia/Visuals/Implementation/OpenGL4/Resources/ModelResource.h"

#include "Arcadia/Visuals/Implementation/OpenGL4/BackendContext.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/Resources/MaterialResource.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/Resources/ConstantBufferResource.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/Resources/ProgramResource.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/Resources/TextureResource.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/Resources/VertexBufferResource.h"

#include <assert.h>

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResourceDispatch* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource* self,
    Arcadia_Engine_Visuals_EnterPassResource* enterPassResource
  );

static GLuint
getUniformBlockBindingIndex
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* self,
    Arcadia_String* nameVPL
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.Implementation.OpenGL4.ModelResource", Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource,
                         u8"Arcadia.Visuals.ModelResource", Arcadia_Engine_Visuals_ModelResource,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource);
  if (4 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_Value t[4];
    t[0] = Arcadia_ValueStack_getValue(thread, 4);
    t[1] = Arcadia_ValueStack_getValue(thread, 3);
    t[2] = Arcadia_ValueStack_getValue(thread, 2);
    t[3] = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_ValueStack_pushValue(thread, &t[0]);
    Arcadia_ValueStack_pushValue(thread, &t[1]);
    Arcadia_ValueStack_pushValue(thread, &t[2]);
    Arcadia_ValueStack_pushValue(thread, &t[3]);
    Arcadia_ValueStack_pushNatural8Value(thread, 4);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource);
}


static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResourceDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*, Arcadia_Engine_Visuals_EnterPassResource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource_renderImpl;
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource* self
  )
{/*Intentionally empty.*/}

static GLuint
getUniformBlockBindingIndex
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* self,
    Arcadia_String* nameVPL
  )
{
  Arcadia_Value temporary = Arcadia_Map_get(thread, self->constantBindings, Arcadia_Value_makeObjectReferenceValue(nameVPL));
  if (!Arcadia_Value_isSizeValue(&temporary)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  return (GLuint)Arcadia_Value_getSizeValue(&temporary);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource* self,
    Arcadia_Engine_Visuals_EnterPassResource* enterPassResource
  )
{
  Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource* material = (Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource*)((Arcadia_Engine_Visuals_ModelResource*)self)->material;
  Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource* meshVertexBuffer = (Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource*)(((Arcadia_Engine_Visuals_ModelResource*)self)->meshVertexBuffer);
  Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource *meshConstantBuffer = (Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource*)((Arcadia_Engine_Visuals_ModelResource*)self)->meshConstantBuffer;
  Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* constantBuffer = (Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource*)((Arcadia_Engine_Visuals_ModelResource*)self)->constantBuffer;
  Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* viewerConstantBuffer = (Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource*)enterPassResource->viewerConstantBuffer;


  // Invoke our load function.
  Arcadia_Engine_Visuals_Resource_load(thread, (Arcadia_Engine_Visuals_Resource*)self);

  if (((Arcadia_Engine_Visuals_ModelResource*)self)->dirty & (Arcadia_Engine_Visuals_ModelResource_LocalToWorldMatrixDirty)) {
    Arcadia_Engine_Visuals_ConstantBufferResource_clear(thread, (Arcadia_Engine_Visuals_ConstantBufferResource*)constantBuffer);
    // @todo We need to ability to do sub-range updates:
    // There is no need to rewrite every value if only one value is dirty.
    Arcadia_Engine_Visuals_ConstantBufferResource_writeMatrix4x4Real32(thread, (Arcadia_Engine_Visuals_ConstantBufferResource*)constantBuffer, Arcadia_BooleanValue_True, ((Arcadia_Engine_Visuals_ModelResource*)self)->localToWorldMatrix);
    ((Arcadia_Engine_Visuals_ModelResource*)self)->dirty &= ~Arcadia_Engine_Visuals_ModelResource_LocalToWorldMatrixDirty;
    Arcadia_Engine_Visuals_Resource_load(thread, (Arcadia_Engine_Visuals_Resource*)constantBuffer);
  }


  Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);

  Arcadia_Engine_Visuals_Resource_render(thread, (Arcadia_Engine_Visuals_Resource*)material, enterPassResource);
  Arcadia_Engine_Visuals_Resource_render(thread, (Arcadia_Engine_Visuals_Resource*)meshVertexBuffer, enterPassResource);

  //
  gl->glUseProgram(material->program->programID);
  //
  gl->glBindFragDataLocation(material->program->programID, 0, Arcadia_String_getBytes(thread, material->program->fragmentColorOutput));
  gl->glBindVertexArray(meshVertexBuffer->vertexArrayID);

  //
  if (material->ambientColorSource == Arcadia_Engine_Visuals_MaterialResource_AmbientColorSource_Texture) {
    while (gl->glGetError()) { }
    assert(0 != material->ambientColorTexture->id);
    gl->glActiveTexture(GL_TEXTURE0);
    gl->glBindTexture(GL_TEXTURE_2D, material->ambientColorTexture->id);
    if (gl->glGetError()) {
      return;
    }
  }
  GLuint uniformBlockBindingIndex;
  // Bind the "viewer" constant block.
  uniformBlockBindingIndex = getUniformBlockBindingIndex(thread, material->program, Arcadia_String_createFromCxxString(thread, u8"viewer"));
  gl->glBindBufferBase(GL_UNIFORM_BUFFER, uniformBlockBindingIndex, viewerConstantBuffer->bufferID);
  if (gl->glGetError()) {
    return;
  }
  // Bind the "mesh" constant block.
  uniformBlockBindingIndex = getUniformBlockBindingIndex(thread, material->program, Arcadia_String_createFromCxxString(thread, u8"mesh"));
  gl->glBindBufferBase(GL_UNIFORM_BUFFER, uniformBlockBindingIndex, meshConstantBuffer->bufferID);
  if (gl->glGetError()) {
    return;
  }
  // Bind the "model" constant block.
  uniformBlockBindingIndex = getUniformBlockBindingIndex(thread, material->program, Arcadia_String_createFromCxxString(thread, u8"model"));
  gl->glBindBufferBase(GL_UNIFORM_BUFFER, uniformBlockBindingIndex, constantBuffer->bufferID);
  if (gl->glGetError()) {
    return;
  }
  //
  gl->glDrawArrays(GL_TRIANGLES, 0, Arcadia_Engine_Visuals_VertexBufferResource_getNumberOfVertices(thread, (Arcadia_Engine_Visuals_VertexBufferResource*)((Arcadia_Engine_Visuals_ModelResource*)self)->meshVertexBuffer));
}

Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource*
Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* backendContext,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* meshConstantBuffer,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource* meshVertexBuffer,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource* material
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (meshConstantBuffer) Arcadia_ValueStack_pushObjectReferenceValue(thread, meshConstantBuffer); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (meshVertexBuffer) Arcadia_ValueStack_pushObjectReferenceValue(thread, meshVertexBuffer); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (material) Arcadia_ValueStack_pushObjectReferenceValue(thread, material); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_ModelResource);
}
