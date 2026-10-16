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

#include "Arcadia/Engine/Tests/Nodes/Cases.h"
#include "Arcadia/Engine/Include.h"

void
Arcadia_Engine_Tests_Nodes_factoryFamilies
  (
    Arcadia_Thread* thread
  );

typedef struct FactoryProbe FactoryProbe;
typedef struct FactoryProbeDispatch FactoryProbeDispatch;

struct FactoryProbe {
  Arcadia_Engine_NodeFactory _parent;
  Arcadia_String* label;
};

struct FactoryProbeDispatch {
  Arcadia_Engine_NodeFactoryDispatch _parent;
};

Arcadia_declareObjectType(u8"Tests.FactoryProbe", FactoryProbe,
                          u8"Arcadia.Engine.NodeFactory");

static Arcadia_SizeValue destroyed;

static void
constructImpl
  (
    Arcadia_Thread* thread,
    FactoryProbe* self
  )
{
  Arcadia_EnterConstructor(FactoryProbe);
  if (_numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->label = Arcadia_String_createFromCxxString(thread, "factory probe");
  Arcadia_LeaveConstructor(FactoryProbe);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    FactoryProbe* self
  )
{
  if (self->label) Arcadia_Object_visit(thread, (Arcadia_Object*)self->label);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    FactoryProbe* self
  )
{ destroyed++; }

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectType(u8"Tests.FactoryProbe", FactoryProbe,
                         u8"Arcadia.Engine.NodeFactory", Arcadia_Engine_NodeFactory,
                         &typeOperations);

static Arcadia_Object*
create0
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  return _Arcadia_EndCreate0(thread, type, base);
}

static void
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  destroyed = 0;
  FactoryProbe* self = (FactoryProbe*)create0(thread, _FactoryProbe_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_NodeFactory_getType(thread)));
  Arcadia_SizeValue hash = Arcadia_Object_getHash(thread, (Arcadia_Object*)self);
  Arcadia_Engine_Audials_NodeFactory* audials = Arcadia_Engine_Audials_NodeFactory_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, audials);
  Arcadia_Engine_Visuals_NodeFactory* visuals = Arcadia_Engine_Visuals_NodeFactory_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, visuals);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed == 0 && Arcadia_Object_getHash(thread, (Arcadia_Object*)self) == hash);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, self->label, "factory probe", sizeof("factory probe") - 1));
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)audials, _Arcadia_Engine_NodeFactory_getType(thread)));
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)visuals, _Arcadia_Engine_NodeFactory_getType(thread)));
  Arcadia_Engine_Audials_NodeFactoryDispatch* a = (Arcadia_Engine_Audials_NodeFactoryDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)audials));
  Arcadia_Engine_Visuals_NodeFactoryDispatch* v = (Arcadia_Engine_Visuals_NodeFactoryDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)visuals));
  Arcadia_Tests_assertTrue(thread, a->createSoundSourceNode && a->createStreamingSoundSourceNodeFromDefinition);
  Arcadia_Tests_assertTrue(thread, v->createCameraNode && v->createFrameBufferNode && v->createEnterPassNode && v->createTextureNode);
  Arcadia_ValueStack_popValues(thread, 3);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_ValueStack_pushNatural64Value(thread, 1);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    _Arcadia_EndCreate0(thread, _Arcadia_Engine_NodeFactory_getType(thread), base);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
    Arcadia_Thread_setStatus(thread, 0);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed == 1 && Arcadia_ValueStack_getSize(thread) == base);
  Arcadia_Engine_Tests_Nodes_factoryFamilies(thread);
}

int
Arcadia_Engine_Tests_Nodes_nodeFactory
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
