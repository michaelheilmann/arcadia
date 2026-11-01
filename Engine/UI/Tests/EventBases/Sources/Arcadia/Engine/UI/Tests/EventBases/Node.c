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

#include "Arcadia/Engine/UI/Tests/EventBases/Node.h"
#include "Arcadia/Engine/UI/Include.h"
#include <stdio.h>

_Static_assert(sizeof(Arcadia_Engine_UI_Node) == sizeof(Arcadia_Engine_Node), "UI Node adds no storage");
_Static_assert(_Alignof(Arcadia_Engine_UI_Node) == _Alignof(Arcadia_Engine_Node), "UI Node alignment");
_Static_assert(sizeof(Arcadia_Engine_UI_NodeDispatch) == sizeof(Arcadia_Engine_NodeDispatch), "UI Node adds no slots");
_Static_assert(_Alignof(Arcadia_Engine_UI_NodeDispatch) == _Alignof(Arcadia_Engine_NodeDispatch), "UI Node dispatch alignment");
_Static_assert(_Generic(&Arcadia_Engine_UI_Node_findByName, Arcadia_Object* (*)(Arcadia_Thread*, Arcadia_Object*, Arcadia_String*): 1, default: 0), "search C signature");

typedef struct NodeSearchWidget NodeSearchWidget;
typedef struct NodeSearchWidgetDispatch NodeSearchWidgetDispatch;
typedef struct NodeSearchBranch NodeSearchBranch;
typedef struct NodeSearchBranchDispatch NodeSearchBranchDispatch;

struct NodeSearchWidget { Arcadia_Engine_UI_WidgetNode _parent; };

struct NodeSearchWidgetDispatch { Arcadia_Engine_UI_WidgetNodeDispatch _parent; };

// A Container need not be a UI.Node or WidgetNode.
struct NodeSearchBranch {
  Arcadia_Object _parent;
  Arcadia_Object* first;
  Arcadia_Object* second;
  Arcadia_Natural64Value count;
};

struct NodeSearchBranchDispatch { Arcadia_ObjectDispatch _parent; };

Arcadia_declareObjectType(u8"Tests.NodeSearchWidget", NodeSearchWidget,
                          u8"Arcadia.Engine.UI.WidgetNode");
Arcadia_declareObjectType(u8"Tests.NodeSearchBranch", NodeSearchBranch,
                          u8"Arcadia.Object");

static unsigned failPhase;
static Arcadia_SizeValue counts, indices, comparisons;
static Arcadia_BooleanValue trackLifetime;
static Arcadia_SizeValue searchDestroyed[3];

static void
onSearchDestroy
  (
    void* observer,
    Arcadia_Object* object
  )
{ (*(Arcadia_SizeValue*)observer)++; }

static void
constructWidget
  (
    Arcadia_Thread* thread,
    NodeSearchWidget* self
  )
{
  Arcadia_EnterConstructor(NodeSearchWidget);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  Arcadia_LeaveConstructor(NodeSearchWidget);
}

static void
alwaysEqual
  (
    Arcadia_Thread* thread
  )
{
  comparisons++;
  Arcadia_ValueStack_popValues(thread, 3);
  Arcadia_ValueStack_pushBooleanValue(thread, true);
}

static void
initializeWidget
  (
    Arcadia_Thread* thread,
    NodeSearchWidgetDispatch* self
  )
{
  ((Arcadia_ObjectDispatch*)self)->isEqualTo = &alwaysEqual;
  ((Arcadia_ObjectDispatch*)self)->isIdenticalTo = &alwaysEqual;
}

static void
constructBranch
  (
    Arcadia_Thread* thread,
    NodeSearchBranch* self
  )
{
  Arcadia_EnterConstructor(NodeSearchBranch);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->first = self->second = NULL;
  self->count = 0;
  Arcadia_LeaveConstructor(NodeSearchBranch);
}

static void
visitBranch
  (
    Arcadia_Thread* thread,
    NodeSearchBranch* self
  )
{
  if (self->first) Arcadia_Object_visit(thread, self->first);
  if (self->second) Arcadia_Object_visit(thread, self->second);
}

static void
collectOrFail
  (
    Arcadia_Thread* thread,
    unsigned phase
  )
{
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  if (trackLifetime) {
    for (unsigned i = 0; i < 3; ++i) Arcadia_Tests_assertTrue(thread, searchDestroyed[i] == 0);
  }
  if (failPhase == phase) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
}

static Arcadia_Natural64Value
childCount
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  counts++;
  collectOrFail(thread, 1);
  return ((NodeSearchBranch*)self)->count;
}

static Arcadia_Object*
childAt
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Natural64Value index
  )
{
  indices++;
  collectOrFail(thread, 2);
  Arcadia_Tests_assertTrue(thread, index < ((NodeSearchBranch*)self)->count && index < 2);
  return index == 0 ? ((NodeSearchBranch*)self)->first : ((NodeSearchBranch*)self)->second;
}

static void
initializeContainer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ContainerDispatch* self
  )
{
  self->getChildCount = &childCount;
  self->getChildAt = &childAt;
}

static const Arcadia_ObjectType_Operations widgetObjects = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructWidget,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeWidget,
};
static const Arcadia_Type_Operations widgetOperations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &widgetObjects };
static const Arcadia_ObjectType_Operations branchObjects = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructBranch,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitBranch,
};
static const Arcadia_Type_Operations branchOperations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &branchObjects };

Arcadia_defineObjectType(u8"Tests.NodeSearchWidget", NodeSearchWidget,
                         u8"Arcadia.Engine.UI.WidgetNode", Arcadia_Engine_UI_WidgetNode,
                         &widgetOperations);
Arcadia_defineObjectTypeWithInterfaces(u8"Tests.NodeSearchBranch", NodeSearchBranch,
                                      u8"Arcadia.Object", Arcadia_Object, &branchOperations,
  { .interfaceType = _Arcadia_Engine_UI_Container_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_ContainerDispatch),
    .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeContainer })

static NodeSearchBranch*
createBranch
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  return (void*)_Arcadia_EndCreate0(thread, _NodeSearchBranch_getType(thread), base);
}

static NodeSearchWidget*
createWidget
  (
    Arcadia_Thread* thread,
    const char* name
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  NodeSearchWidget* self = (void*)_Arcadia_EndCreate0(thread, _NodeSearchWidget_getType(thread), base);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  if (name) ((Arcadia_Engine_UI_WidgetNode*)self)->name = Arcadia_String_createFromCxxString(thread, name);
  Arcadia_ValueStack_popValues(thread, 1);
  return self;
}

static void
testSearch
  (
    Arcadia_Thread* thread
  )
{
  failPhase = 0; comparisons = counts = indices = 0; trackLifetime = false;
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  NodeSearchBranch* root = createBranch(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, root);
  Arcadia_Engine_UI_StackPanelNode* branch = Arcadia_Engine_UI_StackPanelNode_create(thread, 0, 0, 100, 100, true, 0);
  root->first = (void*)branch; root->count = 1;
  NodeSearchWidget* deep = createWidget(thread, "needle");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, deep);
  Arcadia_Engine_UI_Container_addChild(thread, (void*)branch, (void*)deep);
  Arcadia_ValueStack_popValues(thread, 1);
  NodeSearchWidget* shallow = createWidget(thread, "needle");
  root->second = (void*)shallow; root->count = 2;
  Arcadia_String* query = Arcadia_String_createFromCxxString(thread, "needle");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, query);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Node_findByName(thread, (void*)root, query) == (Arcadia_Object*)deep);
  Arcadia_Tests_assertTrue(thread, counts == 1 && indices == 1 && comparisons == 0);
  // Root-first matching must avoid traversing children with the same name.
  Arcadia_Engine_UI_WidgetNode_setName(thread, (void*)branch, query);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Node_findByName(thread, (void*)branch, query) == (Arcadia_Object*)branch);
  Arcadia_Engine_UI_WidgetNode_setName(thread, (void*)branch, NULL);
  const char* misses[] = { "", "needl", "needleX", "Needle", "\xC3\xA4" };
  for (unsigned i = 0; i < sizeof(misses) / sizeof(misses[0]); ++i) {
    Arcadia_String* miss = Arcadia_String_createFromCxxString(thread, misses[i]);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, miss);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_UI_Node_findByName(thread, (void*)root, miss));
    Arcadia_ValueStack_popValues(thread, 1);
  }
  // Empty and unnamed widgets differ; non-widget leaves are not matches.
  Arcadia_Engine_UI_WidgetNode_setName(thread, (void*)deep, NULL);
  Arcadia_Engine_UI_WidgetNode_setName(thread, (void*)shallow, Arcadia_String_createFromCxxString(thread, ""));
  Arcadia_String* empty = ((Arcadia_Engine_UI_WidgetNode*)shallow)->name;
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Node_findByName(thread, (void*)root, empty) == (Arcadia_Object*)shallow);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_UI_Node_findByName(thread, (void*)query, query));
  // Full-width Natural64 child counts must not narrow to a stack/count byte.
  root->first = (void*)shallow; root->count = UINT64_MAX;
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Node_findByName(thread, (void*)root, empty) == (Arcadia_Object*)shallow);
  root->count = 1;
  for (unsigned phase = 1; phase <= 2; ++phase) {
    failPhase = phase;
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_Engine_UI_Node_findByName(thread, (void*)root, query);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationFailed);
      Arcadia_Thread_setStatus(thread, 0);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 302);
    }
  }
  failPhase = 0;
  Arcadia_Tests_assertTrue(thread, comparisons == 0);
  Arcadia_ValueStack_popValues(thread, 302);
}

static void
testNodeConstructor
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_TypeValue type = _Arcadia_Engine_UI_Node_getType(thread);
  Arcadia_Engine_NodeDispatch* dispatch = (void*)Arcadia_ObjectType_getDispatch(type);
  Arcadia_Tests_assertTrue(thread, !dispatch->setAudialsBackendContext && !dispatch->setVisualsBackendContext);
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  for (unsigned count = 1; count <= 255; ++count) {
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      for (unsigned i = 0; i < count; ++i) Arcadia_ValueStack_pushBooleanValue(thread, false);
      Arcadia_ValueStack_pushNatural8Value(thread, (Arcadia_Natural8Value)count);
      _Arcadia_EndCreate0(thread, type, depth);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_NumberOfArgumentsInvalid);
      Arcadia_Thread_setStatus(thread, 0);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
    }
  }
  Arcadia_ValueStack_popValues(thread, 300);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testSearchLifetime
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  for (unsigned phase = 0; phase < 3; ++phase) {
    trackLifetime = false;
    NodeSearchBranch* root = createBranch(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, root);
    NodeSearchWidget* leaf = createWidget(thread, "needle");
    root->first = (void*)leaf; root->count = 1;
    Arcadia_String* query = Arcadia_String_createFromCxxString(thread, "needle");
    Arcadia_ValueStack_pushObjectReferenceValue(thread, query);
    Arcadia_Tests_assertTrue(thread, query != ((Arcadia_Engine_UI_WidgetNode*)leaf)->name);
    Arcadia_Object* objects[] = { (void*)root, (void*)query, (void*)leaf };
    for (unsigned i = 0; i < 3; ++i) {
      searchDestroyed[i] = 0;
      Arcadia_Object_addNotifyDestroyCallback(thread, objects[i], &searchDestroyed[i], &onSearchDestroy);
    }
    failPhase = phase; trackLifetime = true;
    Arcadia_ValueStack_popValues(thread, 2);
    // Only generated frames can retain root/query. A successful result will
    // retain just the leaf after those frames have returned.
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_Object* result = Arcadia_Engine_UI_Node_findByName(thread, (void*)root, query);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, phase == 0 && result == (Arcadia_Object*)leaf);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, result);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, phase != 0 && Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationFailed);
      Arcadia_Thread_setStatus(thread, 0);
    }
    trackLifetime = false;
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 300 + (phase == 0 ? 1 : 0));
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    Arcadia_Tests_assertTrue(thread, searchDestroyed[0] == 1 && searchDestroyed[1] == 1 && searchDestroyed[2] == (phase == 0 ? 0 : 1));
    if (phase == 0) Arcadia_ValueStack_popValues(thread, 1);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    Arcadia_Tests_assertTrue(thread, searchDestroyed[2] == 1);
  }
  failPhase = 0;
  Arcadia_ValueStack_popValues(thread, 300);
}

static void
testSearchErrors
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  NodeSearchWidget* root = createWidget(thread, "needle");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, root);
  Arcadia_String* query = ((Arcadia_Engine_UI_WidgetNode*)root)->name;
  for (unsigned error = 0; error < 3; ++error) {
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_Engine_UI_Node_findByName(thread, error == 0 ? NULL : (void*)root,
        error == 1 ? NULL : error == 2 ? (void*)root : query);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_setStatus(thread, 0);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 301);
    }
  }
  Arcadia_ValueStack_popValues(thread, 301);
}

int
Arcadia_Engine_UI_Tests_node
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(testSearch)) { fprintf(stderr, "UI Node search failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testNodeConstructor)) { fprintf(stderr, "UI Node constructor failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testSearchLifetime)) { fprintf(stderr, "UI Node search lifetime failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testSearchErrors)) { fprintf(stderr, "UI Node search errors failed\n"); return EXIT_FAILURE; }
  return EXIT_SUCCESS;
}
