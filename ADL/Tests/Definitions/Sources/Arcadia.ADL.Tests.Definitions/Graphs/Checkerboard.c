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

#include "Arcadia.ADL.Tests.Definitions/Graphs/Checkerboard.h"

#include "Arcadia/ADL/Include.h"
#include "Arcadia/Media/Include.h"

// Parse a top-level DDL list document and return the list of its elements.
static Arcadia_List*
parseNodeList
  (
    Arcadia_Thread* thread,
    const char* ddl
  )
{
  Arcadia_DDL_DefaultReader* reader = (Arcadia_DDL_DefaultReader*)Arcadia_DDL_DefaultReader_create(thread);
  Arcadia_UnicodeCodePointReader* input =
    (Arcadia_UnicodeCodePointReader*)
    Arcadia_ByteReader_UnicodeCodePointReader_create
      (
        thread,
        (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, Arcadia_String_toByteArray(thread, Arcadia_String_createFromCxxString(thread, ddl)))
      );
  Arcadia_DDL_Node* node = Arcadia_DDL_DefaultReader_run(thread, reader, input);
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node, _Arcadia_DDL_ListNode_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  return ((Arcadia_DDL_ListNode*)node)->elements;
}

// Expand the specified DDL batch into a fresh definitions container.
static Arcadia_ADL_Definitions*
expandBatch
  (
    Arcadia_Thread* thread,
    const char* ddl
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_List* nodes = parseNodeList(thread, ddl);
  Arcadia_ADL_Graphs_readFromNodes(thread, context, definitions, nodes);
  Arcadia_ADL_Definitions_link(thread, definitions);
  return definitions;
}

// Assert that a definition of the specified name exists and return it.
static Arcadia_ADL_Definition*
requireDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    const char* name
  )
{
  Arcadia_ADL_Definition* definition = Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, name));
  if (NULL == definition) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  return definition;
}

static void
requireTrue
  (
    Arcadia_Thread* thread,
    Arcadia_BooleanValue condition
  )
{
  if (Arcadia_BooleanValue_True != condition) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
}

static const char* CHECKERBOARD_BATCH =
  "[\n"
  "  {\n"
  "    type : \"Color\",\n"
  "    name : \"Colors.Black\",\n"
  "    red : 0, green : 0, blue : 0,\n"
  "  },\n"
  "  {\n"
  "    type : \"Color\",\n"
  "    name : \"Colors.White\",\n"
  "    red : 255, green : 255, blue : 255,\n"
  "  },\n"
  "  {\n"
  "    type : \"GraphDefinition\",\n"
  "    name : \"Patterns.Checkerboard\",\n"
  "    nodes : [\n"
  "      {\n"
  "        type : \"Graph.Parameter\",\n"
  "        name : \"first\",\n"
  "        referenceType : \"Color\",\n"
  "      },\n"
  "      {\n"
  "        type : \"Graph.Parameter\",\n"
  "        name : \"second\",\n"
  "        referenceType : \"Color\",\n"
  "      },\n"
  "      {\n"
  "        type : \"PixelBufferOperations.CheckerboardFillOperation\",\n"
  "        name : \"fill\",\n"
  "        checkerWidth : 4,\n"
  "        checkerHeight : 4,\n"
  "        firstCheckerColor : \"first\",\n"
  "        secondCheckerColor : \"second\",\n"
  "      },\n"
  "      {\n"
  "        type : \"PixelBuffer\",\n"
  "        name : \"buffer\",\n"
  "        width : 8,\n"
  "        height : 8,\n"
  "        operations : [ \"fill\" ],\n"
  "      },\n"
  "      {\n"
  "        type : \"Graph.Output\",\n"
  "        name : \"output\",\n"
  "        referenceType : \"PixelBuffer\",\n"
  "        source : \"buffer\",\n"
  "      },\n"
  "    ],\n"
  "  },\n"
  "  {\n"
  "    type : \"GraphInstance\",\n"
  "    name : \"MyGame.Board\",\n"
  "    graph : \"Patterns.Checkerboard\",\n"
  "    bindings : [\n"
  "      { parameter : \"first\", source : \"Colors.White\" },\n"
  "      { parameter : \"second\", source : \"Colors.Black\" },\n"
  "    ],\n"
  "  },\n"
  "]\n"
  ;

void
  Arcadia_ADL_Tests_Definitions_Graphs_CheckerboardTest1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Definitions* definitions = expandBatch(thread, CHECKERBOARD_BATCH);

  Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition* fill =
    (Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition*)
    requireDefinition(thread, definitions, "MyGame.Board.__graph.fill");
  // The local pixel buffer referenced by the output is published directly under the instance name.
  Arcadia_ADL_PixelBufferDefinition* output =
    (Arcadia_ADL_PixelBufferDefinition*)
    requireDefinition(thread, definitions, "MyGame.Board.output");

  requireTrue(thread, 4 == fill->checkerWidth ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False);
  requireTrue(thread, 4 == fill->checkerHeight ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False);
  requireTrue(thread, 8 == output->width ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False);
  requireTrue(thread, 8 == output->height ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False);

  // The parameter bindings must be resolved to the external colors.
  requireTrue(thread, fill->firstCheckerColor->definition == Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, "Colors.White")) ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False);
  requireTrue(thread, fill->secondCheckerColor->definition == Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, "Colors.Black")) ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False);

  // The internal buffer name and the internal output name must not be visible.
  requireTrue(thread, NULL == Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, "MyGame.Board.__graph.buffer")) ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False);
  requireTrue(thread, NULL == Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, "MyGame.Board.__graph.output")) ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False);
}

// Read the RGBA components of the pixel at the specified position of the specified pixel buffer
// obtained by applying the checkerboard fill operation of the specified definition.
static void
getPixel
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition* definition,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height,
    Arcadia_Integer32Value column,
    Arcadia_Integer32Value row,
    Arcadia_Natural8Value* r,
    Arcadia_Natural8Value* g,
    Arcadia_Natural8Value* b
  )
{
  Arcadia_Media_PixelBufferOperations_CheckerboardFill* operation =
    (Arcadia_Media_PixelBufferOperations_CheckerboardFill*)
    Arcadia_Media_PixelBufferOperations_CheckerboardFill_create(thread, definition);
  Arcadia_Media_PixelBuffer* pixelBuffer = Arcadia_Media_PixelBuffer_create(thread, 0, width, height, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_Media_PixelBufferOperation_apply(thread, (Arcadia_Media_PixelBufferOperation*)operation, pixelBuffer);
  Arcadia_Natural8Value a = 0;
  Arcadia_Media_PixelBuffer_getPixelRGBA(thread, pixelBuffer, column, row, r, g, b, &a);
}

void
Arcadia_ADL_Tests_Definitions_Graphs_CheckerboardTest2
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Definitions* expanded = expandBatch(thread, CHECKERBOARD_BATCH);
  Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition* expandedFill =
    (Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition*)
    requireDefinition(thread, expanded, "MyGame.Board.__graph.fill");

  // An equivalent hand-written definition.
  static const char* HANDWRITTEN_BATCH =
    "[\n"
    "  {\n"
    "    type : \"Color\",\n"
    "    name : \"Colors.Black\",\n"
    "    red : 0, green : 0, blue : 0,\n"
    "  },\n"
    "  {\n"
    "    type : \"Color\",\n"
    "    name : \"Colors.White\",\n"
    "    red : 255, green : 255, blue : 255,\n"
    "  },\n"
    "  {\n"
    "    type : \"PixelBufferOperations.CheckerboardFillOperation\",\n"
    "    name : \"Handwritten.Fill\",\n"
    "    checkerWidth : 4,\n"
    "    checkerHeight : 4,\n"
    "    firstCheckerColor : \"Colors.White\",\n"
    "    secondCheckerColor : \"Colors.Black\",\n"
    "  },\n"
    "]\n"
    ;
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* handwritten = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ADL_Context* dummy = Arcadia_ADL_Context_getOrCreate(thread);
  (void)dummy;
  // Read the handwritten definitions directly with the ordinary reader.
  {
    Arcadia_List* nodes = parseNodeList(thread, HANDWRITTEN_BATCH);
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)nodes); i < n; ++i) {
      Arcadia_Value element = Arcadia_List_getAt(thread, nodes, i);
      Arcadia_ADL_Context_readFromNode(thread, context, handwritten, (Arcadia_DDL_Node*)Arcadia_Value_getObjectReferenceValue(&element), Arcadia_BooleanValue_False);
    }
  }
  Arcadia_ADL_Definitions_link(thread, handwritten);
  Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition* handwrittenFill =
    (Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition*)
    requireDefinition(thread, handwritten, "Handwritten.Fill");

  for (Arcadia_Integer32Value row = 0; row < 8; ++row) {
    for (Arcadia_Integer32Value column = 0; column < 8; ++column) {
      Arcadia_Natural8Value er, eg, eb, hr, hg, hb;
      getPixel(thread, expandedFill, 8, 8, column, row, &er, &eg, &eb);
      getPixel(thread, handwrittenFill, 8, 8, column, row, &hr, &hg, &hb);
      requireTrue(thread, (er == hr && eg == hg && eb == hb) ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False);
    }
  }
}

// Run the specified batch expansion expecting a failure with the specified status.
static void
expectFailure
  (
    Arcadia_Thread* thread,
    const char* ddl,
    Arcadia_Status expected
  )
{
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
    Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
    Arcadia_ADL_Graphs_readFromNodes(thread, context, definitions, parseNodeList(thread, ddl));
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (expected != Arcadia_Thread_getStatus(thread)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
      Arcadia_Thread_jump(thread);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    }
  }
}

void
Arcadia_ADL_Tests_Definitions_Graphs_CheckerboardTest3
  (
    Arcadia_Thread* thread
  )
{
  static const char* DDL =
    "[\n"
    "  {\n"
    "    type : \"Color\",\n"
    "    name : \"Colors.White\",\n"
    "    red : 255, green : 255, blue : 255,\n"
    "  },\n"
    "  {\n"
    "    type : \"GraphDefinition\",\n"
    "    name : \"Patterns.Fill\",\n"
    "    nodes : [\n"
    "      { type : \"Graph.Parameter\", name : \"color\", referenceType : \"Color\" },\n"
    "      { type : \"Graph.Output\", name : \"output\", referenceType : \"PixelBuffer\", source : \"color\" },\n"
    "    ],\n"
    "  },\n"
    "  {\n"
    "    type : \"GraphInstance\",\n"
    "    name : \"MyGame.Instance\",\n"
    "    graph : \"Patterns.Fill\",\n"
    "    bindings : [],\n"
    "  },\n"
    "]\n"
    ;
  expectFailure(thread, DDL, Arcadia_Status_SemanticalError);
}

void
Arcadia_ADL_Tests_Definitions_Graphs_CheckerboardTest4
  (
    Arcadia_Thread* thread
  )
{
  static const char* DDL =
    "[\n"
    "  {\n"
    "    type : \"Color\",\n"
    "    name : \"Colors.White\",\n"
    "    red : 255, green : 255, blue : 255,\n"
    "  },\n"
    "  {\n"
    "    type : \"Color\",\n"
    "    name : \"Colors.Black\",\n"
    "    red : 0, green : 0, blue : 0,\n"
    "  },\n"
    "  {\n"
    "    type : \"GraphDefinition\",\n"
    "    name : \"Patterns.Checkerboard\",\n"
    "    nodes : [\n"
    "      { type : \"Graph.Parameter\", name : \"first\", referenceType : \"Color\" },\n"
    "      { type : \"Graph.Parameter\", name : \"second\", referenceType : \"Color\" },\n"
    "      {\n"
    "        type : \"PixelBufferOperations.CheckerboardFillOperation\",\n"
    "        name : \"fill\",\n"
    "        checkerWidth : 4, checkerHeight : 4,\n"
    "        firstCheckerColor : \"first\", secondCheckerColor : \"second\",\n"
    "      },\n"
    "      { type : \"PixelBuffer\", name : \"buffer\", width : 8, height : 8, operations : [ \"fill\" ] },\n"
    "      { type : \"Graph.Output\", name : \"output\", referenceType : \"PixelBuffer\", source : \"buffer\" },\n"
    "    ],\n"
    "  },\n"
    "  {\n"
    "    type : \"GraphInstance\",\n"
    "    name : \"MyGame.Instance\",\n"
    "    graph : \"Patterns.Checkerboard\",\n"
    "    bindings : [\n"
    "      { parameter : \"first\", source : \"Colors.White\" },\n"
    "      { parameter : \"first\", source : \"Colors.Black\" },\n"
    "      { parameter : \"second\", source : \"Colors.Black\" },\n"
    "    ],\n"
    "  },\n"
    "]\n"
    ;
  expectFailure(thread, DDL, Arcadia_Status_SemanticalError);
}

void
Arcadia_ADL_Tests_Definitions_Graphs_CheckerboardTest5
  (
    Arcadia_Thread* thread
  )
{
  static const char* DDL =
    "[\n"
    "  {\n"
    "    type : \"GraphDefinition\",\n"
    "    name : \"Patterns.NoOutput\",\n"
    "    nodes : [\n"
    "      { type : \"Color\", name : \"color\", red : 0, green : 0, blue : 0 },\n"
    "    ],\n"
    "  },\n"
    "]\n"
    ;
  expectFailure(thread, DDL, Arcadia_Status_SemanticalError);
}
