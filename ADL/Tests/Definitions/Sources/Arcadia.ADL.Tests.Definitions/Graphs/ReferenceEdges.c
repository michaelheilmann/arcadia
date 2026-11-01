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

#include "Arcadia.ADL.Tests.Definitions/Graphs/ReferenceEdges.h"

#include "Arcadia/ADL/Include.h"

// Collect only a template: no instance, ordinary reader, or linker can reject
// the fixture in place of the collector's reference checks.
static Arcadia_ADL_GraphCollector*
collectTemplate
  (
    Arcadia_Thread* thread,
    const char* nodes
  )
{
  Arcadia_StringBuilder* text = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackCxxString(thread, text, "{ type: \"GraphDefinition\", name: \"Edges\", nodes: [");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, nodes);
  Arcadia_StringBuilder_insertBackCxxString(thread, text, "] }");
  Arcadia_DDL_DefaultReader* reader = Arcadia_DDL_DefaultReader_create(thread);
  Arcadia_UnicodeCodePointReader* input = (Arcadia_UnicodeCodePointReader*)
    Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)
      Arcadia_ByteArray_ByteReader_create(thread, Arcadia_ByteArray_createByteArray(thread,
        Arcadia_RuntimeByteArray_create(thread, Arcadia_StringBuilder_getBytes(thread, text), Arcadia_StringBuilder_getNumberOfBytes(thread, text)))));
  Arcadia_DDL_Node* node = Arcadia_DDL_DefaultReader_run(thread, reader, input);
  Arcadia_ADL_GraphCollector* collector = Arcadia_ADL_GraphCollector_create(thread);
  Arcadia_ADL_GraphCollector_addNode(thread, collector, node);
  return collector;
}

static void
expectInvalidEdges
  (
    Arcadia_Thread* thread,
    const char* nodes
  )
{
  Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
  Arcadia_ADL_GraphCollector* collector = collectTemplate(thread, nodes);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)collector);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_ADL_GraphCollector_validate(thread, collector);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - entry);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - entry);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_SemanticalError, status);
  }
  Arcadia_Tests_assertEqualSize(thread, entry, Arcadia_ValueStack_getSize(thread));
}

static void
unresolvedOutput
  (
    Arcadia_Thread* thread
  )
{
  expectInvalidEdges(thread,
    "{ type: \"Graph.Output\", name: \"out\", referenceType: \"PixelBuffer\", source: \"missing\" }");
}

static void
unresolvedFrequency
  (
    Arcadia_Thread* thread
  )
{
  expectInvalidEdges(thread,
    "{ type: \"DSP.SineWave\", name: \"tone\", frequency: \"missing\" },"
    "{ type: \"Graph.Output\", name: \"out\", referenceType: \"DSP.SineWave\", source: \"tone\" }");
}

static void
unresolvedColor
  (
    Arcadia_Thread* thread
  )
{
  expectInvalidEdges(thread,
    "{ type: \"PixelBufferOperations.CheckerboardFillOperation\", name: \"fill\", firstCheckerColor: \"missing\", secondCheckerColor: \"missing\" },"
    "{ type: \"Graph.Output\", name: \"out\", referenceType: \"PixelBuffer\", source: \"fill\" }");
}

static void
unresolvedOperation
  (
    Arcadia_Thread* thread
  )
{
  // The missing reference is not the first element; duplicate edges are legal.
  expectInvalidEdges(thread,
    "{ type: \"Color\", name: \"color\" },"
    "{ type: \"PixelBufferOperations.CheckerboardFillOperation\", name: \"fill\", firstCheckerColor: \"color\", secondCheckerColor: \"color\" },"
    "{ type: \"PixelBuffer\", name: \"buffer\", operations: [\"fill\", \"fill\", \"missing\"] },"
    "{ type: \"Graph.Output\", name: \"out\", referenceType: \"PixelBuffer\", source: \"buffer\" }");
}

static void
wrongColorFamily
  (
    Arcadia_Thread* thread
  )
{
  expectInvalidEdges(thread,
    "{ type: \"PixelBuffer\", name: \"buffer\", operations: [] },"
    "{ type: \"PixelBufferOperations.CheckerboardFillOperation\", name: \"fill\", firstCheckerColor: \"buffer\", secondCheckerColor: \"buffer\" },"
    "{ type: \"Graph.Output\", name: \"out\", referenceType: \"PixelBuffer\", source: \"buffer\" }");
  expectInvalidEdges(thread,
    "{ type: \"Graph.Parameter\", name: \"frequency\", valueType: \"Integer32\" },"
    "{ type: \"PixelBufferOperations.CheckerboardFillOperation\", name: \"fill\", firstCheckerColor: \"frequency\" },"
    "{ type: \"Graph.Output\", name: \"out\", referenceType: \"PixelBuffer\", source: \"fill\" }");
  expectInvalidEdges(thread,
    "{ type: \"Graph.Parameter\", name: \"color\", referenceType: \"Color\" },"
    "{ type: \"DSP.SineWave\", name: \"tone\", frequency: \"color\" },"
    "{ type: \"Graph.Output\", name: \"out\", referenceType: \"DSP.SineWave\", source: \"tone\" }");
}

static void
wrongOperationFamily
  (
    Arcadia_Thread* thread
  )
{
  expectInvalidEdges(thread,
    "{ type: \"Color\", name: \"color\" },"
    "{ type: \"PixelBuffer\", name: \"buffer\", operations: [\"color\"] },"
    "{ type: \"Graph.Output\", name: \"out\", referenceType: \"PixelBuffer\", source: \"buffer\" }");
  expectInvalidEdges(thread,
    "{ type: \"Graph.Parameter\", name: \"color\", referenceType: \"Color\" },"
    "{ type: \"PixelBuffer\", name: \"buffer\", operations: [\"color\"] },"
    "{ type: \"Graph.Output\", name: \"out\", referenceType: \"PixelBuffer\", source: \"buffer\" }");
}

static void
outputReference
  (
    Arcadia_Thread* thread
  )
{
  expectInvalidEdges(thread,
    "{ type: \"PixelBuffer\", name: \"buffer\", operations: [\"out\"] },"
    "{ type: \"Graph.Output\", name: \"out\", referenceType: \"PixelBuffer\", source: \"buffer\" }");
}

static void
selfCycle
  (
    Arcadia_Thread* thread
  )
{
  // A frequency is syntactically a local-name string. No instance is needed
  // to exercise the collector's DFS rather than later value substitution.
  expectInvalidEdges(thread,
    "{ type: \"DSP.SineWave\", name: \"tone\", frequency: \"tone\" },"
    "{ type: \"Graph.Output\", name: \"out\", referenceType: \"DSP.SineWave\", source: \"tone\" }");
}

static void
crossCycle
  (
    Arcadia_Thread* thread
  )
{
  expectInvalidEdges(thread,
    "{ type: \"DSP.SineWave\", name: \"first\", frequency: \"second\" },"
    "{ type: \"DSP.SineWave\", name: \"second\", frequency: \"first\" },"
    "{ type: \"Graph.Output\", name: \"out\", referenceType: \"DSP.SineWave\", source: \"first\" }");
}

static void
retainedEdges
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
  Arcadia_ADL_GraphCollector* collector = collectTemplate(thread,
    "{ type: \"Graph.Output\", name: \"out\", referenceType: \"PixelBuffer\", source: \"buffer\" },"
    "{ type: \"PixelBuffer\", name: \"buffer\", operations: [\"fill\", \"fill\"] },"
    "{ type: \"PixelBufferOperations.CheckerboardFillOperation\", name: \"fill\", firstCheckerColor: \"color\", secondCheckerColor: \"color\" },"
    "{ type: \"Color\", name: \"color\" }");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)collector);
  Arcadia_ADL_GraphCollector_validate(thread, collector);
  Arcadia_ADL_GraphTemplate* graph = (Arcadia_ADL_GraphTemplate*)Arcadia_List_getObjectReferenceValueCheckedAt(
    thread, collector->templateList, 0, _Arcadia_ADL_GraphTemplate_getType(thread));
  Arcadia_WeakReference* observer = Arcadia_WeakReference_create(thread,
    Arcadia_List_getAt(thread, graph->nodes, 3));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)observer);
  for (unsigned i = 0; i < 3; ++i) {
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    Arcadia_Value retained = Arcadia_WeakReference_getValue(thread, observer);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isObjectReferenceValue(&retained));
    Arcadia_ADL_GraphCollector_validate(thread, collector);
  }
  Arcadia_ValueStack_popValues(thread, 2);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)observer);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Value released = Arcadia_WeakReference_getValue(thread, observer);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isVoidValue(&released));
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertEqualSize(thread, entry, Arcadia_ValueStack_getSize(thread));
}

int
Arcadia_ADL_Tests_Graphs_unresolvedOutput
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&unresolvedOutput) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
Arcadia_ADL_Tests_Graphs_unresolvedFrequency
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&unresolvedFrequency) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
Arcadia_ADL_Tests_Graphs_unresolvedColor
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&unresolvedColor) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
Arcadia_ADL_Tests_Graphs_unresolvedOperation
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&unresolvedOperation) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
Arcadia_ADL_Tests_Graphs_wrongColorFamily
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&wrongColorFamily) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
Arcadia_ADL_Tests_Graphs_wrongOperationFamily
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&wrongOperationFamily) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
Arcadia_ADL_Tests_Graphs_outputReference
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&outputReference) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
Arcadia_ADL_Tests_Graphs_selfCycle
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&selfCycle) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
Arcadia_ADL_Tests_Graphs_crossCycle
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&crossCycle) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
Arcadia_ADL_Tests_Graphs_retainedEdges
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&retainedEdges) ? EXIT_SUCCESS : EXIT_FAILURE;
}
