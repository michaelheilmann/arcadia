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

#if !defined(ARCADIA_VPL_GLSL_BACKENDINTERNAL_H_INCLUDED)
#define ARCADIA_VPL_GLSL_BACKENDINTERNAL_H_INCLUDED

#include "Arcadia/VPL/GLSL/ProgramArtifact.h"
#include "Arcadia/VPL/TextInternal.h"

typedef struct Arcadia_VPL_GLSL_State {
  Arcadia_Thread* thread;
  Arcadia_VPL_Text_Context* memory;
  Arcadia_VPL_IR_Module* module;
  Arcadia_VPL_IR_Stage* stage;
  Arcadia_VPL_IR_Function* function;
  Arcadia_VPL_Backends_GLSL_ProgramArtifact* artifact;
  Arcadia_List* records;
  Arcadia_Map* recordIds;
  Arcadia_Map* samplerAliases;
  Arcadia_Map* samplerDefinitions;
  Arcadia_Map* samplerActive;
  Arcadia_StringBuilder* output;
} Arcadia_VPL_GLSL_State;

Arcadia_Object*
_Arcadia_VPL_GLSL_own
  (
    Arcadia_VPL_GLSL_State* state,
    Arcadia_Object* object
  );

void
_Arcadia_VPL_GLSL_error
  (
    Arcadia_VPL_GLSL_State* state,
    Arcadia_Object* subject,
    const char* message
  );

Arcadia_VPL_Backends_GLSL_Binding*
_Arcadia_VPL_GLSL_newBinding
  (
    Arcadia_Thread* thread
  );

Arcadia_VPL_Backends_GLSL_ProgramArtifact*
_Arcadia_VPL_GLSL_newArtifact
  (
    Arcadia_Thread* thread
  );

void
_Arcadia_VPL_GLSL_resolve
  (
    Arcadia_VPL_GLSL_State* state
  );

Arcadia_String*
_Arcadia_VPL_GLSL_print
  (
    Arcadia_VPL_GLSL_State* state,
    Arcadia_VPL_IR_Stage* stage
  );

#endif // ARCADIA_VPL_GLSL_BACKENDINTERNAL_H_INCLUDED
