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

#include "Arcadia.ADL.Tests.Definitions/Visuals/Color.h"

#include "Arcadia/ADL/Include.h"
#include "Arcadia/FileSystem/Include.h"
#include "Arcadia.ADL.Tests.Definitions/Configure.h"

void
Arcadia_ADL_Tests_Visuals_ColorDefinitionTest1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  static const char *DDL =
    "{\n"
    "  type : \"Color\",\n"
    "  name : \"Colors.Red\",\n"
    "  red : 255,\n"
    "  green : 0,\n"
    "  blue : 0,\n"
    "}\n"
    ;
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
}

static Arcadia_ADL_ColorDefinition*
loadColor
  (
    Arcadia_Thread* thread,
    Arcadia_String* text
  )
{
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ADL_Definition* definition = Arcadia_ADL_Context_readFromString(thread,
    Arcadia_ADL_Context_getOrCreate(thread), definitions, text, false);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_ColorDefinition_getType(thread)));
  Arcadia_ValueStack_popValues(thread, 1);
  return (Arcadia_ADL_ColorDefinition*)definition;
}

static void
checkColor
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ColorDefinition* color,
    Arcadia_Natural8Value alpha
  )
{
  Arcadia_Tests_assertEqualNatural64(thread, 13, color->red);
  Arcadia_Tests_assertEqualNatural64(thread, 79, color->green);
  Arcadia_Tests_assertEqualNatural64(thread, 227, color->blue);
  Arcadia_Tests_assertEqualNatural64(thread, alpha, color->alpha);
}

static void
testColorAlpha
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)Arcadia_ADL_Context_getOrCreate(thread));
  for (Arcadia_SizeValue i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural64Value(thread, i);
  static const char* const texts[] = {
    "{ type: \"Color\", name: \"Alpha\", red: 13, green: 79, blue: 227 }",
    "{ type: \"Color\", name: \"Alpha\", red: 13, green: 79, blue: 227, alpha: 0 }",
    "{ type: \"Color\", name: \"Alpha\", alpha: 128, red: 13, green: 79, blue: 227 }",
    "{ alpha: 255, type: \"Color\", name: \"Alpha\", red: 13, green: 79, blue: 227 }",
  };
  static const Arcadia_Natural8Value expected[] = { 255, 0, 128, 255 };
  Arcadia_WeakReference* finalName = NULL;
  for (unsigned i = 0; i < 4; ++i) {
    Arcadia_ADL_ColorDefinition* color = loadColor(thread, Arcadia_String_createFromCxxString(thread, texts[i]));
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)color);
    checkColor(thread, color, expected[i]);
    Arcadia_ADL_Definition_link(thread, (Arcadia_ADL_Definition*)color);
    // Only color retains definitions. The rooted context also retains the
    // parser's interned name through its DDLS validation string table.
    Arcadia_WeakReference* owner = Arcadia_WeakReference_create(thread,
      Arcadia_Value_makeObjectReferenceValue(((Arcadia_ADL_Definition*)color)->definitions));
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)owner);
    Arcadia_WeakReference* name = Arcadia_WeakReference_create(thread,
      Arcadia_Value_makeObjectReferenceValue(((Arcadia_ADL_Definition*)color)->name));
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    checkColor(thread, color, expected[i]);
    Arcadia_Value retainedOwner = Arcadia_WeakReference_getValue(thread, owner);
    Arcadia_Value retainedName = Arcadia_WeakReference_getValue(thread, name);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isObjectReferenceValue(&retainedOwner) && Arcadia_Value_isObjectReferenceValue(&retainedName));
    Arcadia_ValueStack_popValues(thread, 3);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)owner);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    Arcadia_Value releasedOwner = Arcadia_WeakReference_getValue(thread, owner);
    Arcadia_Value releasedName = Arcadia_WeakReference_getValue(thread, name);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isVoidValue(&releasedOwner));
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isObjectReferenceValue(&releasedName));
    finalName = name;
    Arcadia_ValueStack_popValues(thread, 2);
  }
  for (Arcadia_SizeValue i = 0; i < 300; ++i) Arcadia_Tests_assertEqualNatural64(thread, i, Arcadia_ValueStack_getNatural64Value(thread, 299 - i));
  Arcadia_ValueStack_popValues(thread, 300);
  Arcadia_ValueStack_popValues(thread, 1);
  // Once the context/string table is no longer rooted, the interned name is
  // collectible too. Keep just the final weak observer for that check.
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)finalName);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Value releasedName = Arcadia_WeakReference_getValue(thread, finalName);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isVoidValue(&releasedName));
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertEqualSize(thread, entry, Arcadia_ValueStack_getSize(thread));
}

static void
testColorAlphaInvalid
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)Arcadia_ADL_Context_getOrCreate(thread));
  for (Arcadia_SizeValue i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural64Value(thread, i);
  static const char* const values[] = { "-1", "256", "0.5", "\"0\"", "false", "void" };
  for (unsigned i = 0; i < 6; ++i) {
    Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
    Arcadia_StringBuilder* text = Arcadia_StringBuilder_create(thread);
    Arcadia_StringBuilder_insertBackCxxString(thread, text, "{ type: \"Color\", name: \"Invalid\", red: 13, green: 79, blue: 227, alpha: ");
    Arcadia_StringBuilder_insertBackCxxString(thread, text, values[i]);
    Arcadia_StringBuilder_insertBackCxxString(thread, text, " }");
    Arcadia_String* input = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(
      Arcadia_RuntimeUTF8String_create(thread, Arcadia_StringBuilder_getBytes(thread, text), Arcadia_StringBuilder_getNumberOfBytes(thread, text))));
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      loadColor(thread, input);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Status status = Arcadia_Thread_getStatus(thread);
      Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
      Arcadia_Tests_assertEqualStatus(thread, i < 3 ? Arcadia_Status_ConversionFailed : Arcadia_Status_SemanticalError, status);
    }
    Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  }
  for (Arcadia_SizeValue i = 0; i < 300; ++i) Arcadia_Tests_assertEqualNatural64(thread, i, Arcadia_ValueStack_getNatural64Value(thread, 299 - i));
  Arcadia_ValueStack_popValues(thread, 300);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertEqualSize(thread, entry, Arcadia_ValueStack_getSize(thread));
}

static void
testColorAlphaGraph
  (
    Arcadia_Thread* thread
  )
{
  static const char* const text =
    "[{ type: \"GraphDefinition\", name: \"AlphaGraph\", nodes: ["
    "{ type: \"Color\", name: \"clear\", red: 13, green: 79, blue: 227, alpha: 0 },"
    "{ type: \"Color\", name: \"half\", red: 13, green: 79, blue: 227, alpha: 128 },"
    "{ type: \"PixelBufferOperations.CheckerboardFillOperation\", name: \"fill\", checkerWidth: 1, checkerHeight: 1, firstCheckerColor: \"clear\", secondCheckerColor: \"half\" },"
    "{ type: \"PixelBuffer\", name: \"buffer\", width: 2, height: 2, operations: [\"fill\"] },"
    "{ type: \"Graph.Output\", name: \"output\", referenceType: \"PixelBuffer\", source: \"buffer\" }] },"
    "{ type: \"GraphInstance\", name: \"First\", graph: \"AlphaGraph\", bindings: [] },"
    "{ type: \"GraphInstance\", name: \"Second\", graph: \"AlphaGraph\", bindings: [] }]";
  Arcadia_DDL_DefaultReader* reader = Arcadia_DDL_DefaultReader_create(thread);
  Arcadia_ByteReader* bytes = (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread,
    Arcadia_String_toByteArray(thread, Arcadia_String_createFromCxxString(thread, text)));
  Arcadia_DDL_Node* root = Arcadia_DDL_DefaultReader_run(thread, reader,
    (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, bytes));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)root);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)root, _Arcadia_DDL_ListNode_getType(thread)));
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ADL_Graphs_readFromNodes(thread, Arcadia_ADL_Context_getOrCreate(thread), definitions, ((Arcadia_DDL_ListNode*)root)->elements);
  Arcadia_ADL_Definitions_link(thread, definitions);
  static const char* const names[] = { "First.__graph.clear", "First.__graph.half", "Second.__graph.clear", "Second.__graph.half" };
  Arcadia_ADL_ColorDefinition* colors[4];
  for (unsigned i = 0; i < 4; ++i) {
    Arcadia_ADL_Definition* definition = Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, names[i]));
    Arcadia_Tests_assertTrue(thread, definition != NULL && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_ColorDefinition_getType(thread)));
    colors[i] = (Arcadia_ADL_ColorDefinition*)definition;
    checkColor(thread, colors[i], i % 2 ? 128 : 0);
  }
  Arcadia_Tests_assertTrue(thread, colors[0] != colors[2] && colors[1] != colors[3]);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  for (unsigned i = 0; i < 4; ++i) checkColor(thread, colors[i], i % 2 ? 128 : 0);
  Arcadia_ValueStack_popValues(thread, 2);
}

static void
testColorAlphaAssets
  (
    Arcadia_Thread* thread
  )
{
  for (unsigned repeat = 0; repeat < 2; ++repeat) {
    Arcadia_FilePath* path = Arcadia_FilePath_parseGeneric(thread,
      Arcadia_String_createFromCxxString(thread, ARCADIA_ADL_TESTS_CSS_DIRECTORY "/Red.adl"));
    Arcadia_ByteArrayBuilder* contents = Arcadia_FileSystem_getFileContents(thread, Arcadia_FileSystem_getOrCreate(thread), path);
    Arcadia_String* input = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(
      Arcadia_RuntimeUTF8String_create(thread, Arcadia_ByteArrayBuilder_getBytes(thread, contents), Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, contents))));
    Arcadia_ADL_ColorDefinition* color = loadColor(thread, input);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)color);
    Arcadia_Tests_assertEqualNatural64(thread, 255, color->red);
    Arcadia_Tests_assertEqualNatural64(thread, 0, color->green);
    Arcadia_Tests_assertEqualNatural64(thread, 0, color->blue);
    Arcadia_Tests_assertEqualNatural64(thread, 255, color->alpha);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    Arcadia_Tests_assertEqualNatural64(thread, 255, color->alpha);
    Arcadia_ValueStack_popValues(thread, 1);
  }
}

int
Arcadia_ADL_Tests_Visuals_colorAlpha
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testColorAlpha) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
Arcadia_ADL_Tests_Visuals_colorAlphaInvalid
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testColorAlphaInvalid) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
Arcadia_ADL_Tests_Visuals_colorAlphaGraph
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testColorAlphaGraph) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
Arcadia_ADL_Tests_Visuals_colorAlphaAssets
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testColorAlphaAssets) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void
Arcadia_ADL_Tests_Visuals_ColorDefinitionTest2
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  static const char* DDL =
    "{\n"
    "  type : \"Color\",\n"
    "  name : \"Colors.Green\",\n"
    "  red : 0,\n"
    "  green : 255,\n"
    "  blue : 0,\n"
    "}\n"
    ;
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
}
