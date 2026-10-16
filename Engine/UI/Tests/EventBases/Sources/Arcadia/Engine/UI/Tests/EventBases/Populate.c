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

#include "Arcadia/Engine/UI/Include.h"
#include "Arcadia/Engine/UI/Tests/EventBases/Populate.h"
#include <stdio.h>

_Static_assert(_Generic(&Arcadia_Engine_UI_populateContainerFromDefinition,
  void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_BackendContext*, Arcadia_Object*, Arcadia_Object*): 1, default: 0), "populate ABI");

typedef struct PopulateProbe PopulateProbe;
typedef struct PopulateProbeDispatch PopulateProbeDispatch;

struct PopulateProbe {
  Arcadia_Object _parent;
  Arcadia_List* references;
  Arcadia_List* added;
  unsigned id;
};

struct PopulateProbeDispatch { Arcadia_ObjectDispatch _parent; };

Arcadia_declareObjectType(u8"Tests.PopulateProbe", PopulateProbe,
                          u8"Arcadia.Object");

static unsigned destroyed[2], getterCalls, addCalls, failAt, mutation;
static Arcadia_BooleanValue detachReferences;
static PopulateProbe* activeSource;

static void
constructProbe
  (
    Arcadia_Thread* thread,
    PopulateProbe* self
  )
{
  Arcadia_EnterConstructor(PopulateProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->references = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  self->added = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  self->id = 0;
  Arcadia_LeaveConstructor(PopulateProbe);
}

static void
visitProbe
  (
    Arcadia_Thread* thread,
    PopulateProbe* self
  )
{
  if (self->references) Arcadia_Object_visit(thread, (Arcadia_Object*)self->references);
  if (self->added) Arcadia_Object_visit(thread, (Arcadia_Object*)self->added);
}

static void
destructProbe
  (
    Arcadia_Thread* thread,
    PopulateProbe* self
  )
{ destroyed[self->id]++; }

static void
collect
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, !destroyed[0] && !destroyed[1]);
}

static Arcadia_List*
getReferences
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object
  )
{
  getterCalls++;
  collect(thread);
  Arcadia_List* result = ((PopulateProbe*)object)->references;
  // A callback may return a list no longer retained by the source. The
  // generated traversal must keep that captured list alive independently.
  if (detachReferences) ((PopulateProbe*)object)->references = NULL;
  return result;
}

static void
addChild
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_Object* child
  )
{
  PopulateProbe* self = (PopulateProbe*)object;
  addCalls++;
  collect(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_StackPanelNode_getType(thread)));
  if (failAt == addCalls) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_List_insertBackObjectReferenceValue(thread, self->added, child);
  if (addCalls == 1 && mutation) {
    Arcadia_Object* reference = Arcadia_List_getObjectReferenceValueAt(thread, activeSource->references, 0);
    if (mutation == 2) Arcadia_List_removeAt(thread, activeSource->references, 1, 1);
    Arcadia_List_insertObjectReferenceValueAt(thread, activeSource->references, mutation == 1 ? 3 : 1, reference);
  }
}

static void
initializeSource
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ContainerDefinitionDispatch* self
  )
{ self->getChildReferences = &getReferences; }

static void
initializeTarget
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ContainerDispatch* self
  )
{ self->addChild = &addChild; }

static const Arcadia_ObjectType_Operations probeObjects = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructProbe,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitProbe,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructProbe,
};
static const Arcadia_Type_Operations probeOperations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &probeObjects };

Arcadia_defineObjectTypeWithInterfaces(u8"Tests.PopulateProbe", PopulateProbe,
                                      u8"Arcadia.Object", Arcadia_Object, &probeOperations,
  { .interfaceType = _Arcadia_ADL_UI_ContainerDefinition_getType(thread), .dispatchSize = sizeof(Arcadia_ADL_UI_ContainerDefinitionDispatch),
    .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeSource },
  { .interfaceType = _Arcadia_Engine_UI_Container_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_ContainerDispatch),
    .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeTarget })

static PopulateProbe*
createProbe
  (
    Arcadia_Thread* thread,
    unsigned id
  )
{
  _Arcadia_BeginCreate(PopulateProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  PopulateProbe* result = (PopulateProbe*)_Arcadia_EndCreate0(thread, _PopulateProbe_getType(thread), oldValueStackSize);
  result->id = id;
  return result;
}

static Arcadia_ADL_Reference*
createReference
  (
    Arcadia_Thread* thread,
    PopulateProbe* source,
    Arcadia_ADL_Definitions* definitions,
    char const* text,
    Arcadia_Integer32Value left
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_String* name = Arcadia_String_createFromCxxString(thread, text);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, name);
  Arcadia_String* alignment = Arcadia_String_createFromCxxString(thread, "Start");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, alignment);
  Arcadia_ADL_UI_StackPanelDefinition* definition = Arcadia_ADL_UI_StackPanelDefinition_create(thread, definitions, name,
    left, 23, 100, 40, false, 3, 1, 2, 3, 4, alignment, alignment, NULL, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, definition);
  Arcadia_Map_set(thread, definitions->definitions, Arcadia_Value_makeObjectReferenceValue(name), Arcadia_Value_makeObjectReferenceValue(definition), NULL, NULL);
  Arcadia_ADL_Reference* reference = Arcadia_ADL_Reference_create(thread, definitions, name);
  Arcadia_List_insertBackObjectReferenceValue(thread, source->references, (Arcadia_Object*)reference);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return reference;
}

static void
fixture
  (
    Arcadia_Thread* thread,
    PopulateProbe** source,
    PopulateProbe** target
  )
{
  destroyed[0] = destroyed[1] = getterCalls = addCalls = failAt = mutation = 0;
  detachReferences = false;
  *source = createProbe(thread, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, *source);
  *target = createProbe(thread, 1);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, *target);
  activeSource = *source;
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, definitions);
  Arcadia_ADL_Reference* first = createReference(thread, *source, definitions, "Tests.Populate.First", 11);
  createReference(thread, *source, definitions, "Tests.Populate.Second", 37);
  Arcadia_List_insertBackObjectReferenceValue(thread, (*source)->references, (Arcadia_Object*)first);
  Arcadia_ValueStack_popValues(thread, 1);
}

static void
checkChildren
  (
    Arcadia_Thread* thread,
    PopulateProbe* target,
    Arcadia_SizeValue count
  )
{
  Arcadia_Tests_assertTrue(thread, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)target->added) == count);
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    Arcadia_Engine_UI_WidgetNode* child = (void*)Arcadia_List_getObjectReferenceValueAt(thread, target->added, i);
    Arcadia_Tests_assertTrue(thread, child->left == (i == 1 && mutation != 2 ? 37 : 11));
    Arcadia_Tests_assertTrue(thread, child->bottom == 23 && child->width == 100 && child->height == 40);
  }
  if (count == 3) Arcadia_Tests_assertTrue(thread, Arcadia_List_getObjectReferenceValueAt(thread, target->added, 0) != Arcadia_List_getObjectReferenceValueAt(thread, target->added, 2));
}

static void
testOrderAndMutation
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  PopulateProbe *source, *target;
  fixture(thread, &source, &target);
  mutation = 1;
  Arcadia_Engine_UI_populateContainerFromDefinition(thread, NULL, (void*)target, (void*)source);
  Arcadia_Tests_assertTrue(thread, getterCalls == 1 && addCalls == 3 && Arcadia_Collection_getSize(thread, (Arcadia_Collection*)source->references) == 4);
  checkChildren(thread, target, 3);
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)target->added);
  Arcadia_List_removeAt(thread, source->references, 3, 1);
  mutation = 2; addCalls = getterCalls = 0;
  Arcadia_Engine_UI_populateContainerFromDefinition(thread, NULL, (void*)target, (void*)source);
  checkChildren(thread, target, 3);
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)source->references);
  addCalls = getterCalls = 0;
  // An empty source never consults the target's Container interface.
  Arcadia_Engine_UI_populateContainerFromDefinition(thread, NULL, (void*)source->references, (void*)source);
  Arcadia_Tests_assertTrue(thread, getterCalls == 1 && addCalls == 0);
  Arcadia_Engine_UI_populateContainerFromDefinition(thread, NULL, NULL, (void*)source);
  Arcadia_Tests_assertTrue(thread, getterCalls == 2 && addCalls == 0);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static void
testNested
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  PopulateProbe *source, *target;
  fixture(thread, &source, &target);
  Arcadia_ADL_Reference* first = (void*)Arcadia_List_getObjectReferenceValueAt(thread, source->references, 0);
  Arcadia_ADL_Reference_resolve(thread, first);
  Arcadia_ADL_UI_StackPanelDefinition* definition = (void*)first->definition;
  Arcadia_List_insertBackObjectReferenceValue(thread, definition->childReferences,
    Arcadia_List_getObjectReferenceValueAt(thread, source->references, 1));
  Arcadia_Engine_UI_populateContainerFromDefinition(thread, NULL, (void*)target, (void*)source);
  checkChildren(thread, target, 3);
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    Arcadia_Object* child = Arcadia_List_getObjectReferenceValueAt(thread, target->added, i);
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildCount(thread, child) == (i == 1 ? 0 : 1));
    if (i != 1) {
      Arcadia_Object* grandchild = Arcadia_Engine_UI_Container_getChildAt(thread, child, 0);
      Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, grandchild, _Arcadia_Engine_UI_StackPanelNode_getType(thread)));
      Arcadia_String* name = Arcadia_Engine_UI_WidgetNode_getName(thread, (void*)grandchild);
      Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, name, "Tests.Populate.Second", sizeof("Tests.Populate.Second") - 1));
    }
  }
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static void
testErrors
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  PopulateProbe *source, *target;
  fixture(thread, &source, &target);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  for (unsigned mode = 0; mode < 4; ++mode) {
    Arcadia_Collection_clear(thread, (Arcadia_Collection*)target->added);
    addCalls = 0;
    failAt = mode == 0 ? 2 : 0;
    Arcadia_ADL_Reference* second = (void*)Arcadia_List_getObjectReferenceValueAt(thread, source->references, 1);
    Arcadia_ADL_Definition* saved = second->definition;
    if (mode == 1) {
      second->definition = NULL;
      second->definitionName = Arcadia_String_createFromCxxString(thread, "Tests.Populate.Missing");
    } else if (mode == 2) {
      second->definition = (void*)Arcadia_ADL_ColorDefinition_create(thread, second->definitions,
        Arcadia_String_createFromCxxString(thread, "Tests.Populate.Unsupported"), 1, 2, 3);
    } else if (mode == 3) {
      Arcadia_List_removeAt(thread, source->references, 1, 1);
      Arcadia_List_insertObjectReferenceValueAt(thread, source->references, 1, (Arcadia_Object*)source);
    }
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_Engine_UI_populateContainerFromDefinition(thread, NULL, (void*)target, (void*)source);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == (mode == 0 ? Arcadia_Status_OperationFailed : mode == 1 ? Arcadia_Status_NotFound : Arcadia_Status_ArgumentTypeInvalid));
      Arcadia_Thread_setStatus(thread, 0);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
      checkChildren(thread, target, 1);
    }
    if (mode == 1 || mode == 2) second->definition = saved;
  }
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static void
testRootless
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  PopulateProbe *source, *target;
  fixture(thread, &source, &target);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  detachReferences = true;
  Arcadia_Engine_UI_populateContainerFromDefinition(thread, NULL, (void*)target, (void*)source);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
  checkChildren(thread, target, 3);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed[0] == 1 && destroyed[1] == 1);
}

static void
testRootlessFailure
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  PopulateProbe *source, *target;
  fixture(thread, &source, &target);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  detachReferences = true;
  failAt = 2;
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Engine_UI_populateContainerFromDefinition(thread, NULL, (void*)target, (void*)source);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationFailed);
    Arcadia_Thread_setStatus(thread, 0);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 300);
    checkChildren(thread, target, 1);
    for (unsigned i = 0; i < 300; ++i) Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getNatural8Value(thread, i) == 77);
    Arcadia_ValueStack_popValues(thread, 300);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed[0] == 1 && destroyed[1] == 1);
}

int
Arcadia_Engine_UI_Tests_EventBases_populate
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(testOrderAndMutation)) { fprintf(stderr, "populate order/mutation failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testNested)) { fprintf(stderr, "populate nested construction failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testErrors)) { fprintf(stderr, "populate errors failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testRootless)) { fprintf(stderr, "populate roots failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testRootlessFailure)) { fprintf(stderr, "populate failure roots failed\n"); return EXIT_FAILURE; }
  return EXIT_SUCCESS;
}
