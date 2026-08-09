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

#if !defined(ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_RESOURCES_PROGRAMRESOURCE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_RESOURCES_PROGRAMRESOURCE_H_INCLUDED

#include "Arcadia/Engine/Include.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/BackendIncludes.h"
typedef struct Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext;

Arcadia_declareObjectType(u8"Arcadia.Visuals.Implementation.OpenGL4.ProgramResource", Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource,
                          u8"Arcadia.Visuals.ProgramResource");

struct Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResourceDispatch {
  Arcadia_Engine_Visuals_ProgramResourceDispatch _parent;
};

struct Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource {
  Arcadia_Engine_Visuals_ProgramResource _parent;
  Arcadia_Natural8Value dirty;
  /// The underlaying VPL program.
  Arcadia_VPL_Symbols_Program* program;
  // The OpenGL ID of the fragment shader.
  GLuint fragmentShaderID;
  // The OpenGL ID of the vertex shader.
  GLuint vertexShaderID;
  // The OpenGL ID of the program.
  GLuint programID;
  // A map from VPL constant record names/VPL constant scalar names to OpenGL uniform block names / OpenGL uniform names as used by glGetUniformBlockIndex / glGetUniformLocation.
  Arcadia_Map* constantMapping;
  /// A map from "VPL constant names" to
  /// - for "VPL constant record"s: OpenGL uniform block binding IDs as specified by glUniformBlockBinding(programID, uniformBlockID, uniformmBlockBindingID).
  ///   Specify the name of a VPL constant record and obtain the uniform block binding ID to be used with glBindBufferBase(GL_UNIFORM_BUFFER, uniformBlockBindingID, uniformBufferID).
  /// - for "VPL constant scalar"s: OpenGL uniform locations as specified by gl(Get/Set)Uniform(programID, uniformLocation, ...).
  Arcadia_Map* constantBindings;
  /// The OpenGL/GLSL name of the fragment color output variable.
  /// Must be assigned a color buffer index using `glBindFragDataLocation(programID, targetFrameBufferIndex, fragmentColorOutput)`.
  Arcadia_String* fragmentColorOutput;
};

Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource*
Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* backendContext,
    Arcadia_VPL_Symbols_Program* program
  );

#endif // ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_RESOURCES_PROGRAMRESOURCE_H_INCLUDED
