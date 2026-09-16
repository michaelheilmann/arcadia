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
#include "Arcadia/Engine/Visuals/Nodes/MaterialNode.h"

#include "Arcadia/Engine/Include.h"

static void
Arcadia_Engine_Visuals_MaterialNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MaterialNode* self
  );
  
static void
Arcadia_Engine_Visuals_MaterialNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MaterialNode* self
  );

static void
Arcadia_Engine_Visuals_MaterialNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MaterialNodeDispatch* self
  );

static void
Arcadia_Engine_Visuals_MaterialNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MaterialNode* self
  );

static void
Arcadia_Engine_Visuals_MaterialNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MaterialNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

static void
Arcadia_Engine_Visuals_MaterialNode_setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MaterialNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_MaterialNode_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_MaterialNode_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_MaterialNode_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_MaterialNode_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Visuals.MaterialNode", Arcadia_Engine_Visuals_MaterialNode,
                         u8"Arcadia.Engine.Visuals.Node", Arcadia_Engine_Visuals_Node,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_MaterialNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MaterialNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_MaterialNode);
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  self->source = (Arcadia_ADL_MaterialDefinition*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_ADL_MaterialDefinition_getType(thread));
  Arcadia_ADL_Definition_link(thread, (Arcadia_ADL_Definition*)self->source);
  switch (self->source->ambientColorSource) {
    case Arcadia_ADL_AmbientColorSource_Mesh: {
      self->program = Arcadia_VPL_Symbols_Program_createProgram(thread, Arcadia_VPL_Symbols_ProgramFlags_MeshAmbientColor);
    } break;
    case Arcadia_ADL_AmbientColorSource_Vertex: {
      self->program = Arcadia_VPL_Symbols_Program_createProgram(thread, Arcadia_VPL_Symbols_ProgramFlags_VertexAmbientColor);
    } break;
    case Arcadia_ADL_AmbientColorSource_Texture: {
      self->program = Arcadia_VPL_Symbols_Program_createProgram(thread, Arcadia_VPL_Symbols_ProgramFlags_TextureAmbientColor);
    } break;
    default: {
      Arcadia_logf(Arcadia_LogFlags_Error, "unknown/unsupported ADL value for Mesh.ambientSource\n");
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    } break;
  };
  Arcadia_Engine* engine = Arcadia_Engine_getOrCreate(thread);
  self->ambientColorTexture = Arcadia_Engine_Visuals_NodeFactory_createTextureNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, NULL,
                                                                                   (Arcadia_ADL_TextureDefinition*)self->source->ambientColorTexture->definition);
  //
  if (Arcadia_ValueStack_isVoidValue(thread, 2)) {
    self->backendContext = NULL;
  } else {
    self->backendContext = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_Visuals_BackendContext_getType(thread));
    Arcadia_Object_lock(thread, (Arcadia_Object*)self->backendContext);
  }
  self->materialResource = NULL;
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_MaterialNode);
}

static void
Arcadia_Engine_Visuals_MaterialNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MaterialNode* self
  )
{
  if (self->backendContext) {
    if (self->materialResource) {
      Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->materialResource);
      self->materialResource = NULL;
    }
    Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
    self->backendContext = NULL;
  }
}

static void
Arcadia_Engine_Visuals_MaterialNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MaterialNodeDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_NodeDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Node*, Arcadia_Engine_Visuals_EnterPassNode*)) & Arcadia_Engine_Visuals_MaterialNode_renderImpl;
  ((Arcadia_Engine_NodeDispatch*)self)->setVisualsBackendContext = (void (*)(Arcadia_Thread*, Arcadia_Engine_Node*, Arcadia_Engine_Visuals_BackendContext*)) & Arcadia_Engine_Visuals_MaterialNode_setVisualsBackendContextImpl;  
}

static void
Arcadia_Engine_Visuals_MaterialNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MaterialNode* self
  )
{
  if (self->source) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->source);
  }
  if (self->ambientColorTexture) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->ambientColorTexture);
  }
  if (self->program) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->program);
  }
}

static void
Arcadia_Engine_Visuals_MaterialNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MaterialNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{
  Arcadia_Engine_Node_setVisualsBackendContext(thread, (Arcadia_Engine_Node*)self, (Arcadia_Engine_Visuals_BackendContext*)enterPassNode->backendContext);
  Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)((Arcadia_Engine_Visuals_MaterialNode*)self)->ambientColorTexture, (Arcadia_Engine_Visuals_EnterPassNode*)enterPassNode);
  if (self->backendContext) {
    if (!self->materialResource) {
      Arcadia_Engine_Visuals_BackendContext* backendContext = self->backendContext;
      //
      Arcadia_Engine_Visuals_ProgramResource* programResource =
        Arcadia_Engine_Visuals_BackendContext_createProgramResource(thread, backendContext,
                                                                            ((Arcadia_Engine_Visuals_MaterialNode*)self)->program);
      //
      Arcadia_Engine_Visuals_TextureResource* textureResource =
        ((Arcadia_Engine_Visuals_TextureNode*)((Arcadia_Engine_Visuals_MaterialNode*)self)->ambientColorTexture)->textureResource;

      Arcadia_Engine_Visuals_MaterialResource_AmbientColorSource ambientColorSource = Arcadia_Engine_Visuals_MaterialResource_AmbientColorSource_Mesh;
      switch (((Arcadia_Engine_Visuals_MaterialNode*)self)->source->ambientColorSource) {
        case Arcadia_ADL_AmbientColorSource_Mesh: {
          ambientColorSource = Arcadia_Engine_Visuals_MaterialResource_AmbientColorSource_Mesh;
        } break;
        case Arcadia_ADL_AmbientColorSource_Vertex: {
          ambientColorSource = Arcadia_Engine_Visuals_MaterialResource_AmbientColorSource_Vertex;
        } break;
        case Arcadia_ADL_AmbientColorSource_Texture: {
          ambientColorSource = Arcadia_Engine_Visuals_MaterialResource_AmbientColorSource_Texture;
        } break;
        default: {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
          Arcadia_Thread_jump(thread);
        } break;
      };

      Arcadia_BooleanValue blendEnabled = Arcadia_BooleanValue_False;
      Arcadia_Engine_Visuals_BlendFunction blendSourceFunction = Arcadia_Engine_Visuals_BlendFunction_Zero;
      Arcadia_Engine_Visuals_BlendFunction blendDestinationFunction = Arcadia_Engine_Visuals_BlendFunction_Zero;
      if (Arcadia_ADL_BlendFunction_None != ((Arcadia_Engine_Visuals_MaterialNode*)self)->source->blendSourceFunction &&
          Arcadia_ADL_BlendFunction_None != ((Arcadia_Engine_Visuals_MaterialNode*)self)->source->blendDestinationFunction) {
        blendEnabled = Arcadia_BooleanValue_True;
        switch (((Arcadia_Engine_Visuals_MaterialNode*)self)->source->blendSourceFunction) {
          case Arcadia_ADL_BlendFunction_Zero: blendSourceFunction = Arcadia_Engine_Visuals_BlendFunction_Zero; break;
          case Arcadia_ADL_BlendFunction_One: blendSourceFunction = Arcadia_Engine_Visuals_BlendFunction_One; break;
          case Arcadia_ADL_BlendFunction_SourceColor: blendSourceFunction = Arcadia_Engine_Visuals_BlendFunction_SourceColor; break;
          case Arcadia_ADL_BlendFunction_OneMinusSourceColor: blendSourceFunction = Arcadia_Engine_Visuals_BlendFunction_OneMinusSourceColor; break;
          case Arcadia_ADL_BlendFunction_DestinationColor: blendSourceFunction = Arcadia_Engine_Visuals_BlendFunction_DestinationColor; break;
          case Arcadia_ADL_BlendFunction_OneMinusDestinationColor: blendSourceFunction = Arcadia_Engine_Visuals_BlendFunction_OneMinusDestinationColor; break;
          case Arcadia_ADL_BlendFunction_SourceAlpha: blendSourceFunction = Arcadia_Engine_Visuals_BlendFunction_SourceAlpha; break;
          case Arcadia_ADL_BlendFunction_OneMinusSourceAlpha: blendSourceFunction = Arcadia_Engine_Visuals_BlendFunction_OneMinusSourceAlpha; break;
          case Arcadia_ADL_BlendFunction_DestinationAlpha: blendSourceFunction = Arcadia_Engine_Visuals_BlendFunction_DestinationAlpha; break;
          case Arcadia_ADL_BlendFunction_OneMinusDestinationAlpha: blendSourceFunction = Arcadia_Engine_Visuals_BlendFunction_OneMinusDestinationAlpha; break;
          default: {
            Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
            Arcadia_Thread_jump(thread);
          } break;
        };
        switch (((Arcadia_Engine_Visuals_MaterialNode*)self)->source->blendDestinationFunction) {
          case Arcadia_ADL_BlendFunction_Zero: blendDestinationFunction = Arcadia_Engine_Visuals_BlendFunction_Zero; break;
          case Arcadia_ADL_BlendFunction_One: blendDestinationFunction = Arcadia_Engine_Visuals_BlendFunction_One; break;
          case Arcadia_ADL_BlendFunction_SourceColor: blendDestinationFunction = Arcadia_Engine_Visuals_BlendFunction_SourceColor; break;
          case Arcadia_ADL_BlendFunction_OneMinusSourceColor: blendDestinationFunction = Arcadia_Engine_Visuals_BlendFunction_OneMinusSourceColor; break;
          case Arcadia_ADL_BlendFunction_DestinationColor: blendDestinationFunction = Arcadia_Engine_Visuals_BlendFunction_DestinationColor; break;
          case Arcadia_ADL_BlendFunction_OneMinusDestinationColor: blendDestinationFunction = Arcadia_Engine_Visuals_BlendFunction_OneMinusDestinationColor; break;
          case Arcadia_ADL_BlendFunction_SourceAlpha: blendDestinationFunction = Arcadia_Engine_Visuals_BlendFunction_SourceAlpha; break;
          case Arcadia_ADL_BlendFunction_OneMinusSourceAlpha: blendDestinationFunction = Arcadia_Engine_Visuals_BlendFunction_OneMinusSourceAlpha; break;
          case Arcadia_ADL_BlendFunction_DestinationAlpha: blendDestinationFunction = Arcadia_Engine_Visuals_BlendFunction_DestinationAlpha; break;
          case Arcadia_ADL_BlendFunction_OneMinusDestinationAlpha: blendDestinationFunction = Arcadia_Engine_Visuals_BlendFunction_OneMinusDestinationAlpha; break;
          default: {
            Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
            Arcadia_Thread_jump(thread);
          } break;
        };
      }

      self->materialResource = Arcadia_Engine_Visuals_BackendContext_createMaterialResource(thread, (Arcadia_Engine_Visuals_BackendContext*)backendContext, blendEnabled, blendSourceFunction, blendDestinationFunction, ambientColorSource, textureResource, programResource);
      Arcadia_Engine_Visuals_Resource_ref(thread, (Arcadia_Engine_Visuals_Resource*)self->materialResource);
    }
  }
}

static void
Arcadia_Engine_Visuals_MaterialNode_setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MaterialNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{
  if (((Arcadia_Engine_Visuals_MaterialNode*)self)->ambientColorTexture) {
    Arcadia_Engine_Node_setVisualsBackendContext(thread, (Arcadia_Engine_Node*)((Arcadia_Engine_Visuals_MaterialNode*)self)->ambientColorTexture, (Arcadia_Engine_Visuals_BackendContext*)backendContext);
  }
  if (backendContext == self->backendContext) {
    // Only change something if the backend context changes.
    return;
  }
  if (backendContext) {
    Arcadia_Object_lock(thread, (Arcadia_Object*)backendContext);
  }
  if (self->backendContext) {
    Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
  }
  if (self->materialResource) {
    Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->materialResource);
    self->materialResource = NULL;
  }
  self->backendContext = backendContext;
}

Arcadia_Engine_Visuals_MaterialNode*
Arcadia_Engine_Visuals_MaterialNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_MaterialDefinition* source
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_MaterialNode);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (source) Arcadia_ValueStack_pushObjectReferenceValue(thread, source); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_MaterialNode);
}
