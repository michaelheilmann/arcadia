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
#include "Arcadia/VPL/TextInternal.h"
#include <string.h>

static const char prologue[] =
  "(module 1 (types (type Z void) (type V3 vector float 3) (type V4 vector float 4)"
  " (type V2 vector float 2) (type M matrix 4 4) (type S sampler2D)"
  " (type Viewer record \"Viewer\" (field \"viewToProjection\" M) (field \"worldToView\" M))"
  " (type Model record \"Model\" (field \"localToWorld\" M))"
  " (type Mesh record \"Mesh\" (field \"ambientColor\" V4)))"
  " (uniforms (record viewer \"viewer\" Viewer) (record model \"model\" Model)"
  " (record mesh \"mesh\" Mesh) (scalar texture \"ambientColorTexture\" S))"
  " (vertex main (interface"
  " (in pos \"vertexProgram_inputs_vertex_position\" V3 0 none)"
  " (in color \"vertexProgram_inputs_vertex_ambientColor\" V4 1 none)"
  " (in uv \"vertexProgram_inputs_vertex_ambientColorTextureCoordinate\" V2 2 none)"
  " (out position \"position\" V4 -1 position)"
  " (out outUV \"uv\" V2 0 texcoord) (out outColor \"ambientColor\" V4 1 color))"
  " (functions (function main \"vertex_main\" Z (parameters)"
  " (registers (register r0 M) (register r1 M) (register r2 M) (register r3 M) (register r4 M)"
  " (register r5 V3) (register r6 V4) (register r7 V4) (register r8 V2) (register r9 V4))"
  " (block entry"
  " (load r0 (member_ref (uniform viewer) \"viewToProjection\"))"
  " (load r1 (member_ref (uniform viewer) \"worldToView\"))"
  " (load r2 (member_ref (uniform model) \"localToWorld\"))"
  " (multiply r3 r1 r2) (multiply r4 r0 r3)"
  " (load r5 (stage pos)) (construct r6 r5 (float 1.0)) (multiply r7 r4 r6)"
  " (store (stage position) r7) (load r8 (stage uv)) (store (stage outUV) r8)";

static const char fragmentStart[] =
  " (store (stage outColor) r9) (return)))))"
  " (fragment main (interface (in uv \"uv\" V2 0 texcoord)"
  " (in color \"ambientColor\" V4 1 color) (out pixel \"fragmentColor\" V4 0 fragment_color))"
  " (functions (function main \"fragment_main\" Z (parameters)"
  " (registers (register r0 V4) (register r1 V2) (register r2 S) (register r3 V4))"
  " (block entry";

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
    Arcadia_StringBuilder* source = Arcadia_StringBuilder_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)source);
    Arcadia_VPL_Text_append(thread, source, prologue, sizeof(prologue) - 1);
    const char* vertexColor = flags == Arcadia_VPL_ProgramFlags_MeshAmbientColor || flags == Arcadia_VPL_ProgramFlags_TextureAmbientColor
      ? " (load r9 (member_ref (uniform mesh) \"ambientColor\"))" : " (load r9 (stage color))";
    Arcadia_VPL_Text_append(thread, source, vertexColor, strlen(vertexColor));
    Arcadia_VPL_Text_append(thread, source, fragmentStart, sizeof(fragmentStart) - 1);
    const char* fragment = flags == Arcadia_VPL_ProgramFlags_MeshAmbientColor || flags == Arcadia_VPL_ProgramFlags_VertexAmbientColor
      ? " (load r0 (stage color))"
      : flags == Arcadia_VPL_ProgramFlags_TextureAmbientColor
      ? " (load r1 (stage uv)) (load r2 (uniform texture)) (texture r0 r2 r1)"
      : " (load r1 (stage uv)) (load r2 (uniform texture)) (texture r3 r2 r1) (load r0 (stage color)) (multiply r0 r3 r0)";
    Arcadia_VPL_Text_append(thread, source, fragment, strlen(fragment));
    const char* ending = " (store (stage pixel) r0) (return))))))";
    Arcadia_VPL_Text_append(thread, source, ending, strlen(ending));
    Arcadia_String* name = Arcadia_String_createFromCxxString(thread, "builtin-material.vpl");
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
    Arcadia_VPL_IR_Module* module = Arcadia_VPL_Text_read(thread, Arcadia_StringBuilder_getBytes(thread, source), Arcadia_StringBuilder_getNumberOfBytes(thread, source), Arcadia_VPL_Context_getDiagnostics(thread, context), name);
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
