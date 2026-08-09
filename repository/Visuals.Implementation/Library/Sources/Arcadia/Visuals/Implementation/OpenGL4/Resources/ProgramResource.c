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

#include "Arcadia/Visuals/Implementation/OpenGL4/Resources/ProgramResource.h"

#include "Arcadia/Visuals/Implementation/OpenGL4/BackendContext.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/BackendIncludes.h"
#include "Arcadia/VPL/Include.h"
#include <assert.h>

#define VertexShaderDirty (1)
#define FragmentShaderDirty (2)
#define ProgramDirty (4)

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResourceDispatch* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* self
  );

typedef enum ShaderType {
  VertexShader,
  FragmentShader,
} ShaderType;

static GLuint
createShader
  (
    Arcadia_Thread* thread,
    _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl,
    ShaderType shaderType,
    Arcadia_Map* constantMapping,
    Arcadia_Map* variableScalarMapping,
    Arcadia_String** fragmentColorOutput,
    Arcadia_VPL_Symbols_Program* program
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_unloadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_unlinkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* self,
    Arcadia_Engine_Visuals_EnterPassResource* enterPassResource
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.Implementation.OpenGL4.ProgramResource", Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource,
                         u8"Arcadia.Visuals.ProgramResource", Arcadia_Engine_Visuals_ProgramResource,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource);
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_Value t;
    t = Arcadia_ValueStack_getValue(thread, 2);
    Arcadia_ValueStack_pushValue(thread, &t);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }

  self->dirty = VertexShaderDirty | FragmentShaderDirty | ProgramDirty;
  self->vertexShaderID = 0;
  self->fragmentShaderID = 0;
  self->programID = 0;
  self->program = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_VPL_Symbols_Program_getType(thread));
  self->constantMapping = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  self->constantBindings = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  self->fragmentColorOutput = NULL;

  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResourceDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->load = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_loadImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->unload = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_unloadImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->unlink = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_unlinkImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*, Arcadia_Engine_Visuals_EnterPassResource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_renderImpl;
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* self
  )
{
  assert(0 == self->vertexShaderID);
  assert(0 == self->fragmentShaderID);
  assert(0 == self->programID);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* self
  )
{
  if (self->constantBindings) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->constantBindings);
  }
  if (self->constantMapping) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->constantMapping);
  }
  if (self->fragmentColorOutput) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->fragmentColorOutput);
  }
  if (self->program) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->program);
  }
}

static GLuint
createShader
  (
    Arcadia_Thread* thread,
    _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl,
    ShaderType shaderType,
    Arcadia_Map* constantMapping,
    Arcadia_Map* variableScalarMapping,
    Arcadia_String** fragmentColorOutput,
    Arcadia_VPL_Symbols_Program* program
  )
{
  GLuint id = 0;
  switch (shaderType) {
    case VertexShader: {
      id = gl->glCreateShader(GL_VERTEX_SHADER);
    } break;
    case FragmentShader: {
      id = gl->glCreateShader(GL_FRAGMENT_SHADER);
    } break;
    default: {
      return Arcadia_BooleanValue_False;
    } break;
  };
  if (0 == id) {
    return id;
  }
  Arcadia_ByteArrayBuilder* code = Arcadia_ByteArrayBuilder_create(thread);
  switch (shaderType) {
    case VertexShader: {
      Arcadia_VPL_Backends_GLSL_Transpiler* transpiler = Arcadia_VPL_Backends_GLSL_Transpiler_create(thread);
      Arcadia_VPL_Backends_GLSL_Transpiler_writeDefaultVertexShader(thread, transpiler, program, constantMapping, variableScalarMapping, code);
      *fragmentColorOutput = NULL;
    } break;
    case FragmentShader: {
      Arcadia_VPL_Backends_GLSL_Transpiler* transpiler = Arcadia_VPL_Backends_GLSL_Transpiler_create(thread);
      Arcadia_VPL_Backends_GLSL_Transpiler_writeDefaultFragmentShader(thread, transpiler, program, constantMapping, variableScalarMapping, fragmentColorOutput, code);
    } break;
    default: {
      return Arcadia_BooleanValue_False;
    } break;
  };
  // (1) set shader source
  while (gl->glGetError()) { }
  const GLchar* temporary = Arcadia_ByteArrayBuilder_getBytes(thread, code);
  gl->glShaderSource(id, 1, &temporary, NULL);
  if (gl->glGetError()) {
    gl->glDeleteShader(id);
    id = 0;
    return 0;
  }
  // (2) compile shader
  while (gl->glGetError()) { }
  gl->glCompileShader(id);
  if (gl->glGetError()) {
    gl->glDeleteShader(id);
    id = 0;
    return 0;
  }
  // (3) check if compilation was successful
  GLint success = GL_FALSE;
  gl->glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLuint infoLogLength;
    gl->glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar* infoLog = malloc(sizeof(GLchar) + (infoLogLength + 1));
    if (!infoLog) {
      gl->glDeleteShader(id);
      id = 0;
      return 0;
    }
    gl->glGetShaderInfoLog(id, infoLogLength, NULL, infoLog);
    free(infoLog);
    infoLog = NULL;
    gl->glDeleteShader(id);
    id = 0;
    return 0;
  }
  return id;
}

// @return The OpenGL/GLSL uniform location for a VPL constant scalar.
static GLuint
getUniformLocation
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* self,
    Arcadia_VPL_Symbols_ConstantScalar* constantScalar
  )
{
  Arcadia_String* nameGL = (Arcadia_String*)Arcadia_Map_getObjectReferenceValueChecked(thread, self->constantMapping, Arcadia_Value_makeObjectReferenceValue(constantScalar->name), _Arcadia_String_getType(thread));

  Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);
  while (gl->glGetError()) { }
  return gl->glGetUniformLocation(self->programID, Arcadia_String_getBytes(thread, nameGL));
}

// @return The OpenGL/GLSL uniform block index for a VPL constant record.
static GLuint
getUniformBlockIndex
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* self,
    Arcadia_VPL_Symbols_ConstantRecord* constantRecord
  )
{
  Arcadia_String* nameGL = (Arcadia_String*)Arcadia_Map_getObjectReferenceValueChecked(thread, self->constantMapping, Arcadia_Value_makeObjectReferenceValue(constantRecord->name), _Arcadia_String_getType(thread));

  Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);
  while (gl->glGetError()) { }
  return gl->glGetUniformBlockIndex(self->programID, Arcadia_String_getBytes(thread, nameGL));
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* self
  )
{
  Arcadia_Map* vertexShaderVariableScalarMapping = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_Map* fragmentShaderVariableScalarMapping = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));

  Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);

  if (self->dirty) {
    // If something is dirty, everything is dirty.
    self->dirty |= VertexShaderDirty | FragmentShaderDirty | ProgramDirty;
  }
  if (self->dirty) {
    // If something is dirty, destroy the vertex shader, the fragment shader, and the program.
    Arcadia_Collection_clear(thread, (Arcadia_Collection*)self->constantBindings);
    Arcadia_Collection_clear(thread, (Arcadia_Collection*)self->constantMapping);

    if (self->programID) {
      gl->glDeleteProgram(self->programID);
      self->programID = 0;
    }
    if (self->fragmentShaderID) {
      gl->glDeleteShader(self->fragmentShaderID);
      self->fragmentShaderID = 0;
    }
    if (self->vertexShaderID) {
      gl->glDeleteShader(self->vertexShaderID);
      self->vertexShaderID = 0;
    }
  }
  if (0 == self->vertexShaderID) {
    Arcadia_String* dummy = NULL;
    self->vertexShaderID = createShader(thread, gl, VertexShader, self->constantMapping, vertexShaderVariableScalarMapping , &dummy, self->program);
    if (!self->vertexShaderID) {
      return;
    }
  }
  if (0 == self->fragmentShaderID) {
    self->fragmentShaderID = createShader(thread, gl, FragmentShader, self->constantMapping, fragmentShaderVariableScalarMapping, &self->fragmentColorOutput, self->program);
    if (!self->fragmentShaderID) {
      return;
    }
  }
  if (0 == self->programID) {
    self->programID = gl->glCreateProgram();
    if (0 == self->programID) {
      return;
    }
    // (1) attach vertex program
    while (gl->glGetError()) { }
    gl->glAttachShader(self->programID, self->vertexShaderID);
    if (gl->glGetError()) {
      return;
    }
    // (2) attach fragment program
    while (gl->glGetError()) { }
    gl->glAttachShader(self->programID, self->fragmentShaderID);
    if (gl->glGetError()) {
      return;
    }
    // (3) link program
    while (gl->glGetError()) { }
    gl->glLinkProgram(self->programID);
    GLint success = GL_FALSE;
    gl->glGetProgramiv(self->programID, GL_LINK_STATUS, &success);
    if (!success) {
      return;
    }
    // Rebuilt the bindings.
    Arcadia_List* constants = Arcadia_VPL_Symbols_Program_getConstants(thread, self->program);
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)constants), uniformBlockBindingIndex = 0; i < n; ++i) {
      Arcadia_VPL_Symbols_Constant* constant = (Arcadia_VPL_Symbols_Constant*)Arcadia_List_getObjectReferenceValueAt(thread, constants, i);
      if (Arcadia_VPL_Symbols_Constant_getKind(thread, constant) == Arcadia_VPL_ConstantKind_Record) {
        // (1) Get the OpenGL/GLSL name of the constant record.
        // (2) Get the uniform block index given that name.
        // (3) If the uniform block was not found, advance to next constant record.
        // (4) Otherwise add the entry <constant record name> -> <uniform block index> to self.constantBindings.
        Arcadia_VPL_Symbols_ConstantRecord* constantRecord = (Arcadia_VPL_Symbols_ConstantRecord*)constant;
        GLuint uniformBlockIndex = getUniformBlockIndex(thread, self, constantRecord);
        if (GL_INVALID_INDEX == uniformBlockIndex) {
          continue;
        }
        gl->glUniformBlockBinding(self->programID, uniformBlockIndex, uniformBlockBindingIndex);
        if (gl->glGetError()) {
          return;
        }
        Arcadia_Map_set(thread, self->constantBindings, Arcadia_Value_makeObjectReferenceValue(constantRecord->name), Arcadia_Value_makeSizeValue(uniformBlockBindingIndex), NULL, NULL);
        uniformBlockBindingIndex++;
      } else if (Arcadia_VPL_Symbols_Constant_getKind(thread, constant) == Arcadia_VPL_ConstantKind_Scalar) {
        // (1) Get the OpenGL/GLSL name of the constant scalar.
        // (2) Get the uniform location given that name.
        // (3) If the uniform was not found, advance to next constant record.
        // (4) Otherwise add the entry <constant scalar name> -> <uniform location> to self.constantBindings.
        Arcadia_VPL_Symbols_ConstantScalar* constantScalar = (Arcadia_VPL_Symbols_ConstantScalar*)constant;
        GLuint uniformLocation = getUniformLocation(thread, self, constantScalar);
        if (GL_INVALID_INDEX == uniformLocation) {
          continue;
        }
        Arcadia_Map_set(thread, self->constantBindings, Arcadia_Value_makeObjectReferenceValue(constantScalar->name), Arcadia_Value_makeSizeValue(uniformLocation), NULL, NULL);
      }
    }
  }
  self->dirty = 0;
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_unloadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* self
  )
{
  if (self->programID) {
    Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
    _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);
    gl->glDeleteProgram(self->programID);
    self->programID = 0;
  }
  if (self->fragmentShaderID) {
    Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
    _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);
    gl->glDeleteShader(self->fragmentShaderID);
    self->fragmentShaderID = 0;
  }
  if (self->vertexShaderID) {
    Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
    _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);
    gl->glDeleteShader(self->vertexShaderID);
    self->vertexShaderID = 0;
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_unlinkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* self
  )
{
  assert(0 == self->vertexShaderID);
  assert(0 == self->fragmentShaderID);
  assert(0 == self->programID);
  ((Arcadia_Engine_Visuals_Resource*)self)->context = NULL;
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* self,
    Arcadia_Engine_Visuals_EnterPassResource* enterPassResource
  )
{
  Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_loadImpl(thread, self);
}

Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource*
Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* backendContext,
    Arcadia_VPL_Symbols_Program* program
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (program) Arcadia_ValueStack_pushObjectReferenceValue(thread, program); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource);
}
