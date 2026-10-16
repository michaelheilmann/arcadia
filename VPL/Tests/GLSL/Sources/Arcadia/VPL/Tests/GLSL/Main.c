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

#include "Arcadia/VPL/Include.h"
#include "Arcadia/FileSystem/Include.h"
#include <string.h>

static Arcadia_Object*
root
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object
  )
{
  Arcadia_ValueStack_pushObjectReferenceValue(thread, object);
  return object;
}

static void
assertAsset
  (
    Arcadia_Thread* thread,
    Arcadia_String* actual,
    const char* name
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_FilePath* path = (Arcadia_FilePath*)root(thread, (Arcadia_Object*)Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, name)));
  Arcadia_ByteArrayBuilder* expected = (Arcadia_ByteArrayBuilder*)root(thread, (Arcadia_Object*)Arcadia_FileSystem_getFileContents(thread, Arcadia_FileSystem_getOrCreate(thread), path));
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, actual, Arcadia_ByteArrayBuilder_getBytes(thread, expected), Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, expected)));
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
}

static void
testPrograms
  (
    Arcadia_Thread* thread
  )
{
  const char* vertexAssets[] = { "Assets/canonical-vertex-mesh.txt", "Assets/canonical-vertex-vertex.txt", "Assets/canonical-vertex-mesh.txt", "Assets/canonical-vertex-vertex.txt" };
  const char* fragmentAssets[] = { "Assets/canonical-fragment-color.txt", "Assets/canonical-fragment-color.txt", "Assets/canonical-fragment-texture.txt", "Assets/canonical-fragment-texture-vertex.txt" };
  const char* logicalNames[] = { "viewer", "model", "mesh", "ambientColorTexture" };
  for (Arcadia_Natural8Value flags = 0; flags < 4; ++flags) {
    Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
    Arcadia_VPL_Context* context = (Arcadia_VPL_Context*)root(thread, (Arcadia_Object*)Arcadia_VPL_Context_create(thread));
    Arcadia_VPL_VerifiedModule* program = (Arcadia_VPL_VerifiedModule*)root(thread, (Arcadia_Object*)Arcadia_VPL_Programs_create(thread, flags));
    Arcadia_VPL_Backends_GLSL_ProgramArtifact* artifact = (Arcadia_VPL_Backends_GLSL_ProgramArtifact*)root(thread,
      (Arcadia_Object*)Arcadia_VPL_Backends_GLSL_compile(thread, program, Arcadia_VPL_Context_getDiagnostics(thread, context), 420));
    assertAsset(thread, artifact->vertexCode, vertexAssets[flags]);
    assertAsset(thread, artifact->fragmentCode, fragmentAssets[flags]);
    Arcadia_Tests_assertTrue(thread, artifact->version == 420 && artifact->fragmentColorLocation == 0);
    Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, artifact->fragmentColorOutput, "vpl_output_0\0", sizeof("vpl_output_0")));
    Arcadia_Tests_assertTrue(thread, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)artifact->uniforms) == 4);
    for (Arcadia_SizeValue i = 0; i < 4; ++i) {
      Arcadia_VPL_Backends_GLSL_Binding* binding = (Arcadia_VPL_Backends_GLSL_Binding*)Arcadia_List_getObjectReferenceValueAt(thread, artifact->uniforms, i);
      Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, binding->name, logicalNames[i], strlen(logicalNames[i])));
      Arcadia_Tests_assertTrue(thread, binding->slot == i && binding->type != NULL);
      Arcadia_Tests_assertTrue(thread, binding->blockBinding == (i < 3 ? (Arcadia_Integer32Value)i : -1));
      Arcadia_Tests_assertTrue(thread, Arcadia_String_getByteAt(thread, binding->glName, Arcadia_String_getNumberOfBytes(thread, binding->glName) - 1) == 0);
    }
    Arcadia_Tests_assertTrue(thread, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)artifact->vertexInterface) == 6);
    for (Arcadia_SizeValue i = 0; i < 3; ++i) {
      Arcadia_VPL_Backends_GLSL_Binding* attribute = (Arcadia_VPL_Backends_GLSL_Binding*)Arcadia_List_getObjectReferenceValueAt(thread, artifact->vertexInterface, i);
      Arcadia_Tests_assertTrue(thread, attribute->location == i && attribute->direction == Arcadia_VPL_IR_StageIO_Direction_In);
    }
    Arcadia_VPL_Backends_GLSL_ProgramArtifact* repeat = (Arcadia_VPL_Backends_GLSL_ProgramArtifact*)root(thread,
      (Arcadia_Object*)Arcadia_VPL_Backends_GLSL_compile(thread, program, Arcadia_VPL_Context_getDiagnostics(thread, context), 420));
    Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, artifact->vertexCode, Arcadia_String_getBytes(thread, repeat->vertexCode), Arcadia_String_getNumberOfBytes(thread, repeat->vertexCode)));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
  }
}

static void
testTargetRejection
  (
    Arcadia_Thread* thread
  )
{
  for (int test = 0; test < 4; ++test) {
    Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
    Arcadia_VPL_Context* context = (Arcadia_VPL_Context*)root(thread, (Arcadia_Object*)Arcadia_VPL_Context_create(thread));
    Arcadia_Languages_Diagnostics* diagnostics = Arcadia_VPL_Context_getDiagnostics(thread, context);
    Arcadia_VPL_VerifiedModule* program = (Arcadia_VPL_VerifiedModule*)root(thread, (Arcadia_Object*)Arcadia_VPL_Programs_create(thread, 0));
    if (test) {
      Arcadia_VPL_IR_Module* module = (Arcadia_VPL_IR_Module*)root(thread, (Arcadia_Object*)Arcadia_VPL_VerifiedModule_materialize(thread, program, diagnostics));
      Arcadia_String* name = (Arcadia_String*)root(thread, (Arcadia_Object*)Arcadia_String_createFromCxxString(thread, "extra"));
      if (test == 1) {
        Arcadia_VPL_IR_Type* type = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ScalarType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Boolean));
        Arcadia_List_insertBackObjectReferenceValue(thread, module->vertex->interface, (Arcadia_Object*)Arcadia_VPL_IR_StageIO_create(thread, Arcadia_VPL_IR_StageIO_Direction_In, name, type, 3, Arcadia_VPL_IR_StageIO_Semantics_None));
      } else if (test == 2) {
        Arcadia_VPL_IR_Type* integer = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ScalarType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Integer));
        Arcadia_VPL_IR_Type* inner = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ArrayType_create(thread, integer, 2));
        Arcadia_List_insertBackObjectReferenceValue(thread, module->uniforms, (Arcadia_Object*)Arcadia_VPL_IR_ArrayUniform_create(thread, name, inner, 2));
      } else {
        Arcadia_VPL_IR_Type* matrix = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_MatrixType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating, 4, 4));
        Arcadia_List_insertBackObjectReferenceValue(thread, module->vertex->interface, (Arcadia_Object*)Arcadia_VPL_IR_StageIO_create(thread, Arcadia_VPL_IR_StageIO_Direction_In, name, matrix, 3, Arcadia_VPL_IR_StageIO_Semantics_None));
        Arcadia_String* other = (Arcadia_String*)root(thread, (Arcadia_Object*)Arcadia_String_createFromCxxString(thread, "overlap"));
        Arcadia_List_insertBackObjectReferenceValue(thread, module->vertex->interface, (Arcadia_Object*)Arcadia_VPL_IR_StageIO_create(thread, Arcadia_VPL_IR_StageIO_Direction_In, other, matrix, 4, Arcadia_VPL_IR_StageIO_Semantics_None));
      }
      program = (Arcadia_VPL_VerifiedModule*)root(thread, (Arcadia_Object*)Arcadia_VPL_VerifiedModule_create(thread, module, diagnostics));
    }
    Arcadia_SizeValue before = Arcadia_ValueStack_getSize(thread);
    Arcadia_BooleanValue raised = Arcadia_BooleanValue_False;
    Arcadia_JumpTarget target;
    Arcadia_Thread_pushJumpTarget(thread, &target);
    if (Arcadia_JumpTarget_save(&target)) Arcadia_VPL_Backends_GLSL_compile(thread, program, diagnostics, test ? 420 : 330);
    else raised = Arcadia_BooleanValue_True;
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_setStatus(thread, 0);
    Arcadia_Tests_assertTrue(thread, raised && status == Arcadia_Status_SemanticalError && before == Arcadia_ValueStack_getSize(thread));
    Arcadia_Tests_assertTrue(thread, Arcadia_Languages_Diagnostics_hasErrors(thread, diagnostics));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
  }
}

int
main
  (
    int argc,
    char** argv
  )
{
  if (!Arcadia_Tests_safeExecute(&testPrograms)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(&testTargetRejection)) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
