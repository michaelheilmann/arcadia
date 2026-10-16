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
#include <limits.h>

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
    Arcadia_String* code
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
  self->program = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_VPL_VerifiedModule_getType(thread));
  self->artifact = NULL;
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
  if (self->artifact) Arcadia_Object_visit(thread, (Arcadia_Object*)self->artifact);
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
    Arcadia_String* code
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
  Arcadia_SizeValue size = Arcadia_String_getNumberOfBytes(thread, code);
  if (size > INT_MAX) { gl->glDeleteShader(id); return 0; }
  // (1) set shader source
  while (gl->glGetError()) { }
  const GLchar* temporary = (const GLchar*)Arcadia_String_getBytes(thread, code);
  GLint length = (GLint)size;
  gl->glShaderSource(id, 1, &temporary, &length);
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
    GLint infoLogLength;
    gl->glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar* infoLog = malloc(sizeof(GLchar) + (infoLogLength + 1));
    if (!infoLog) {
      gl->glDeleteShader(id);
      id = 0;
      return 0;
    }
    gl->glGetShaderInfoLog(id, infoLogLength, NULL, infoLog);
    Arcadia_logf(Arcadia_LogFlags_Error, "shader compilation failed: %s\n", infoLog);
    free(infoLog);
    infoLog = NULL;
    gl->glDeleteShader(id);
    id = 0;
    return 0;
  }
  return id;
}

static void
compileArtifact
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* self
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) {
    Arcadia_VPL_Context* context = Arcadia_VPL_Context_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
    self->artifact = Arcadia_VPL_Backends_GLSL_compile(thread, self->program, Arcadia_VPL_Context_getDiagnostics(thread, context), 420);
    self->fragmentColorOutput = self->artifact->fragmentColorOutput;
    Arcadia_ValueStack_popValues(thread, 1);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
    Arcadia_Thread_jump(thread);
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* self
  )
{
  if (!self->artifact) compileArtifact(thread, self);

  Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);

  if (self->dirty) {
    // If something is dirty, everything is dirty.
    self->dirty |= VertexShaderDirty | FragmentShaderDirty | ProgramDirty;
  }
  if (self->dirty) {
    // If something is dirty, destroy the vertex shader, the fragment shader, and the program.
    Arcadia_Collection_clear(thread, (Arcadia_Collection*)self->constantBindings);

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
    self->vertexShaderID = createShader(thread, gl, VertexShader, self->artifact->vertexCode);
    if (!self->vertexShaderID) {
      return;
    }
  }
  if (0 == self->fragmentShaderID) {
    self->fragmentShaderID = createShader(thread, gl, FragmentShader, self->artifact->fragmentCode);
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
    // Output bindings affect linking and must be installed before glLinkProgram.
    gl->glBindFragDataLocation(self->programID, (GLuint)self->artifact->fragmentColorLocation, (const GLchar*)Arcadia_String_getBytes(thread, self->artifact->fragmentColorOutput));
    // (3) link program
    while (gl->glGetError()) { }
    gl->glLinkProgram(self->programID);
    GLint success = GL_FALSE;
    gl->glGetProgramiv(self->programID, GL_LINK_STATUS, &success);
    if (!success) {
      GLint length = 0;
      gl->glGetProgramiv(self->programID, GL_INFO_LOG_LENGTH, &length);
      GLchar* log = (GLchar*)Arcadia_Memory_allocateUnmanaged(thread, (Arcadia_SizeValue)length + 1);
      log[0] = 0;
      gl->glGetProgramInfoLog(self->programID, length, NULL, log);
      Arcadia_logf(Arcadia_LogFlags_Error, "program linking failed: %s\n", log);
      Arcadia_Memory_deallocateUnmanaged(thread, log);
      return;
    }
    // Rebuilt the bindings.
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->artifact->uniforms); i < n; ++i) {
      Arcadia_VPL_Backends_GLSL_Binding* binding = (Arcadia_VPL_Backends_GLSL_Binding*)Arcadia_List_getObjectReferenceValueAt(thread, self->artifact->uniforms, i);
      const GLchar* name = (const GLchar*)Arcadia_String_getBytes(thread, binding->glName);
      if (binding->blockBinding >= 0) {
        GLuint index = gl->glGetUniformBlockIndex(self->programID, name);
        if (index != GL_INVALID_INDEX) gl->glUniformBlockBinding(self->programID, index, (GLuint)binding->blockBinding);
        // Keep deterministic binding points even when a driver optimizes a block away.
        Arcadia_Map_set(thread, self->constantBindings, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)binding->name), Arcadia_Value_makeSizeValue((Arcadia_SizeValue)binding->blockBinding), NULL, NULL);
      } else {
        GLint location = gl->glGetUniformLocation(self->programID, name);
        if (location >= 0) Arcadia_Map_set(thread, self->constantBindings, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)binding->name), Arcadia_Value_makeSizeValue((Arcadia_SizeValue)location), NULL, NULL);
      }
      if (gl->glGetError()) return;
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
    Arcadia_VPL_VerifiedModule* program
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (program) Arcadia_ValueStack_pushObjectReferenceValue(thread, program); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource);
}
