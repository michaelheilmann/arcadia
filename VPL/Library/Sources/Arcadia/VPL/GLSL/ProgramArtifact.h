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

#if !defined(ARCADIA_VPL_GLSL_PROGRAMARTIFACT_H_INCLUDED)
#define ARCADIA_VPL_GLSL_PROGRAMARTIFACT_H_INCLUDED

#include "Arcadia/VPL/VerifiedModule.h"

Arcadia_declareObjectType(u8"Arcadia.VPL.Backends.GLSL.Binding", Arcadia_VPL_Backends_GLSL_Binding, u8"Arcadia.Object");

struct Arcadia_VPL_Backends_GLSL_BindingDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_Backends_GLSL_Binding {
  Arcadia_Object _parent;
  Arcadia_String* name;
  /// The exact GL query/declaration name, including one trailing zero byte.
  Arcadia_String* glName;
  Arcadia_VPL_IR_Type* type;
  Arcadia_Natural8Value kind;
  Arcadia_SizeValue slot;
  Arcadia_Integer32Value location;
  /// Record-uniform binding point; -1 for non-block bindings.
  Arcadia_Integer32Value blockBinding;
  Arcadia_Natural8Value direction;
  Arcadia_Natural8Value semantics;
};

Arcadia_declareObjectType(u8"Arcadia.VPL.Backends.GLSL.ProgramArtifact", Arcadia_VPL_Backends_GLSL_ProgramArtifact, u8"Arcadia.Object");

struct Arcadia_VPL_Backends_GLSL_ProgramArtifactDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_Backends_GLSL_ProgramArtifact {
  Arcadia_Object _parent;
  /// Shader source without an extra zero byte; pass an explicit byte length to GL.
  Arcadia_String* vertexCode;
  Arcadia_String* fragmentCode;
  Arcadia_List* uniforms;
  Arcadia_List* vertexInterface;
  Arcadia_List* fragmentInterface;
  Arcadia_String* fragmentColorOutput;
  Arcadia_Integer32Value fragmentColorLocation;
  Arcadia_Natural32Value version;
};

/// Compile an independent verified snapshot. The initial supported profile is 420 core.
Arcadia_VPL_Backends_GLSL_ProgramArtifact*
Arcadia_VPL_Backends_GLSL_compile
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_VerifiedModule* program,
    Arcadia_Languages_Diagnostics* diagnostics,
    Arcadia_Natural32Value version
  );

#endif // ARCADIA_VPL_GLSL_PROGRAMARTIFACT_H_INCLUDED
