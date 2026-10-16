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
#include "Arcadia/VPL/GLSL/BackendInternal.h"
#include <string.h>

Arcadia_VPL_Backends_GLSL_ProgramArtifact*
Arcadia_VPL_Backends_GLSL_compile
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_VerifiedModule* program,
    Arcadia_Languages_Diagnostics* diagnostics,
    Arcadia_Natural32Value version
  )
{
  if (!program || !diagnostics) { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)program);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)diagnostics);
    Arcadia_VPL_GLSL_State state;
    memset(&state, 0, sizeof(state));
    state.thread = thread;
    state.memory = Arcadia_VPL_Text_createContext(thread, diagnostics);
    if (version != 420) _Arcadia_VPL_GLSL_error(&state, (Arcadia_Object*)program, "GLSL backend supports only version 420 core\n");
    state.module = (Arcadia_VPL_IR_Module*)_Arcadia_VPL_GLSL_own(&state, (Arcadia_Object*)Arcadia_VPL_VerifiedModule_materialize(thread, program, diagnostics));
    state.artifact = (Arcadia_VPL_Backends_GLSL_ProgramArtifact*)_Arcadia_VPL_GLSL_own(&state, (Arcadia_Object*)_Arcadia_VPL_GLSL_newArtifact(thread));
    state.artifact->uniforms = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    state.artifact->vertexInterface = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    state.artifact->fragmentInterface = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    state.records = (Arcadia_List*)_Arcadia_VPL_GLSL_own(&state, (Arcadia_Object*)Arcadia_ArrayList_create(thread));
    state.recordIds = (Arcadia_Map*)_Arcadia_VPL_GLSL_own(&state, (Arcadia_Object*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void)));
    _Arcadia_VPL_GLSL_resolve(&state);
    state.artifact->vertexCode = _Arcadia_VPL_GLSL_print(&state, state.module->vertex);
    state.artifact->fragmentCode = _Arcadia_VPL_GLSL_print(&state, state.module->fragment);
    Arcadia_VPL_Backends_GLSL_ProgramArtifact* result = state.artifact;
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
