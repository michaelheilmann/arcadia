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

#include "Arcadia/Engine/Tests/Backends/Cases.h"
#include "Arcadia/Engine/Include.h"

typedef struct DisplayDeviceProbe DisplayDeviceProbe;
typedef struct DisplayDeviceProbeDispatch DisplayDeviceProbeDispatch;
struct DisplayDeviceProbe {
  Arcadia_Engine_Visuals_DisplayDevice _parent;
  Arcadia_String* name;
  Arcadia_String* id;
  Arcadia_List* modes;
  Arcadia_Engine_Visuals_DisplayMode* mode;
};
struct DisplayDeviceProbeDispatch { Arcadia_Engine_Visuals_DisplayDeviceDispatch _parent; };
Arcadia_declareObjectType(u8"Tests.DisplayDeviceProbe", DisplayDeviceProbe, u8"Arcadia.Engine.Visuals.DisplayDevice");
static Arcadia_SizeValue destroyed;
static Arcadia_BooleanValue fail;

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
constructImpl
  (
    Arcadia_Thread* thread,
    DisplayDeviceProbe* self
  )
{
  Arcadia_EnterConstructor(DisplayDeviceProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->name = NULL; self->id = NULL; self->modes = NULL; self->mode = NULL;
  Arcadia_Object_setType(thread, (Arcadia_Object*)self, _type);
  self->name = Arcadia_String_createFromCxxString(thread, "display name");
  self->id = Arcadia_String_createFromCxxString(thread, "display identifier");
  self->modes = (void*)Arcadia_ArrayList_create(thread);
  self->mode = (void*)create0(thread, _Arcadia_Engine_Visuals_DisplayMode_getType(thread));
  Arcadia_LeaveConstructor(DisplayDeviceProbe);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    DisplayDeviceProbe* self
  )
{
  if (self->name) Arcadia_Object_visit(thread, (void*)self->name);
  if (self->id) Arcadia_Object_visit(thread, (void*)self->id);
  if (self->modes) Arcadia_Object_visit(thread, (void*)self->modes);
  if (self->mode) Arcadia_Object_visit(thread, (void*)self->mode);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    DisplayDeviceProbe* self
  )
{ destroyed++; }

static void
collect
  (
    Arcadia_Thread* thread
  )
{
  if (fail) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed); Arcadia_Thread_jump(thread); }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
}

static Arcadia_Engine_Visuals_DisplayMode*
getMode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self
  )
{ collect(thread); return ((DisplayDeviceProbe*)self)->mode; }

static Arcadia_List*
getModes
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self
  )
{ collect(thread); return ((DisplayDeviceProbe*)self)->modes; }

static Arcadia_String*
getName
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self
  )
{ collect(thread); return ((DisplayDeviceProbe*)self)->name; }

static Arcadia_String*
getId
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self
  )
{ collect(thread); return ((DisplayDeviceProbe*)self)->id; }

static void
getBounds
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self,
    Arcadia_Integer32Value* left,
    Arcadia_Integer32Value* top,
    Arcadia_Integer32Value* right,
    Arcadia_Integer32Value* bottom
  )
{
  collect(thread);
  if (left) *left = INT32_MIN;
  if (top) *top = -17;
  if (right) *right = INT32_MAX;
  if (bottom) *bottom = 123;
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    DisplayDeviceProbeDispatch* self
  )
{
  Arcadia_Engine_Visuals_DisplayDeviceDispatch* base = (Arcadia_Engine_Visuals_DisplayDeviceDispatch*)self;
  base->getCurrentDisplayMode = &getMode; base->getAvailableDisplayModes = &getModes;
  base->getBounds = &getBounds; base->getName = &getName; base->getId = &getId;
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
};
static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer, .objectTypeOperations = &objectOperations,
};
Arcadia_defineObjectType(u8"Tests.DisplayDeviceProbe", DisplayDeviceProbe,
                         u8"Arcadia.Engine.Visuals.DisplayDevice", Arcadia_Engine_Visuals_DisplayDevice, &typeOperations);

static void
call
  (
    Arcadia_Thread* thread,
    unsigned operation,
    Arcadia_Engine_Visuals_DisplayDevice* self
  )
{
  switch (operation) {
    case 0: Arcadia_Engine_Visuals_DisplayDevice_getCurrentDisplayMode(thread, self); break;
    case 1: Arcadia_Engine_Visuals_DisplayDevice_getAvailableDisplayModes(thread, self); break;
    case 2: Arcadia_Engine_Visuals_DisplayDevice_getBounds(thread, self, NULL, NULL, NULL, NULL); break;
    case 3: Arcadia_Engine_Visuals_DisplayDevice_getName(thread, self); break;
    case 4: Arcadia_Engine_Visuals_DisplayDevice_getId(thread, self); break;
  }
}

static void
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (size_t i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 0);
  DisplayDeviceProbe* self = (void*)create0(thread, _DisplayDeviceProbe_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Visuals_DisplayDevice_getCurrentDisplayMode(thread, (void*)self) == self->mode);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Visuals_DisplayDevice_getAvailableDisplayModes(thread, (void*)self) == self->modes);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Visuals_DisplayDevice_getName(thread, (void*)self) == self->name && Arcadia_Engine_Visuals_DisplayDevice_getId(thread, (void*)self) == self->id);
  Arcadia_Integer32Value left, top, right, bottom;
  Arcadia_Engine_Visuals_DisplayDevice_getBounds(thread, (void*)self, &left, &top, &right, &bottom);
  Arcadia_Tests_assertTrue(thread, left == INT32_MIN && top == -17 && right == INT32_MAX && bottom == 123);
  Arcadia_Engine_Visuals_DisplayDevice_getBounds(thread, (void*)self, &left, &left, &left, &left);
  Arcadia_Tests_assertTrue(thread, left == 123);
  Arcadia_Engine_Visuals_DisplayDevice_getBounds(thread, (void*)self, NULL, &top, NULL, &bottom);
  Arcadia_Tests_assertTrue(thread, top == -17 && bottom == 123);
  self->mode = NULL; self->modes = NULL; self->name = NULL; self->id = NULL;
  for (unsigned operation = 0; operation < 5; ++operation) call(thread, operation, (void*)self);
  Arcadia_Engine_Visuals_DisplayDevice* unbound = (void*)create0(thread, _Arcadia_Engine_Visuals_DisplayDevice_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, unbound);
  Arcadia_String* wrong = Arcadia_String_createFromCxxString(thread, "wrong receiver");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, wrong);
  for (unsigned operation = 0; operation < 5; ++operation) {
    fail = true;
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      call(thread, operation, (void*)self);
      Arcadia_Thread_popJumpTarget(thread); Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationFailed && Arcadia_ValueStack_getSize(thread) == depth);
      Arcadia_Thread_setStatus(thread, 0);
    }
    fail = false;
    for (unsigned receiver = 0; receiver < 3; ++receiver) {
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (Arcadia_JumpTarget_save(&jump)) {
        call(thread, operation, receiver == 0 ? unbound : receiver == 1 ? NULL : (void*)wrong);
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, false);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == (receiver == 0 ? Arcadia_Status_OperationInvalid : Arcadia_Status_ArgumentTypeInvalid));
        Arcadia_Thread_setStatus(thread, 0);
      }
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
    }
  }
  Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget arity;
  Arcadia_Thread_pushJumpTarget(thread, &arity);
  if (Arcadia_JumpTarget_save(&arity)) {
    Arcadia_ValueStack_pushNatural64Value(thread, 1);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    _Arcadia_EndCreate0(thread, _Arcadia_Engine_Visuals_DisplayDevice_getType(thread), depth);
    Arcadia_Thread_popJumpTarget(thread); Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_NumberOfArgumentsInvalid && Arcadia_ValueStack_getSize(thread) == depth);
    Arcadia_Thread_setStatus(thread, 0);
  }
  Arcadia_ValueStack_popValues(thread, 3);
  self = (void*)create0(thread, _DisplayDeviceProbe_getType(thread));
  Arcadia_String* returned = Arcadia_Engine_Visuals_DisplayDevice_getName(thread, (void*)self);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, returned);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, returned, "display name", sizeof("display name") - 1));
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed == 2 && Arcadia_ValueStack_getSize(thread) == base + 300);
  Arcadia_ValueStack_popValues(thread, 300);
}

int
Arcadia_Engine_Tests_Backends_displayDevice
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
