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

#define ARCADIA_VPL_PRIVATE (1)
#include "Arcadia/VPL/Programs.h"
#include "Arcadia/VPL/Context.h"
#include "Arcadia/VPL/BuiltinPrograms.h"

typedef struct BuiltinProgramSource {
  const unsigned char* bytes;
  Arcadia_SizeValue count;
  const char* name;
} BuiltinProgramSource;

static const BuiltinProgramSource sources[] = {
  { _Arcadia_VPL_BuiltinPrograms_MeshAmbientColor, sizeof(_Arcadia_VPL_BuiltinPrograms_MeshAmbientColor), "mesh-ambient-color.vpl" },
  { _Arcadia_VPL_BuiltinPrograms_VertexAmbientColor, sizeof(_Arcadia_VPL_BuiltinPrograms_VertexAmbientColor), "vertex-ambient-color.vpl" },
  { _Arcadia_VPL_BuiltinPrograms_TextureAmbientColor, sizeof(_Arcadia_VPL_BuiltinPrograms_TextureAmbientColor), "texture-ambient-color.vpl" },
  { _Arcadia_VPL_BuiltinPrograms_TextureVertexAmbientColor, sizeof(_Arcadia_VPL_BuiltinPrograms_TextureVertexAmbientColor), "texture-vertex-ambient-color.vpl" },
};

Arcadia_VPL_VerifiedModule*
Arcadia_VPL_Programs_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value flags
  )
{
  if (flags > Arcadia_VPL_ProgramFlags_TextureVertexAmbientColor) { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) {
    Arcadia_VPL_Context* context = Arcadia_VPL_Context_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
    const BuiltinProgramSource* source = &sources[flags];
    Arcadia_String* name = Arcadia_String_createFromCxxString(thread, source->name);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
    Arcadia_VPL_IR_Module* module = Arcadia_VPL_Text_read(thread, source->bytes, source->count, Arcadia_VPL_Context_getDiagnostics(thread, context), name);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)module);
    Arcadia_VPL_VerifiedModule* result = Arcadia_VPL_VerifiedModule_create(thread, module, Arcadia_VPL_Context_getDiagnostics(thread, context));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
    Arcadia_Thread_popJumpTarget(thread);
    return result;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
    Arcadia_Thread_jump(thread);
    return NULL;
  }
}
