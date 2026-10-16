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
#include "Arcadia/VPL/VerifiedModule.h"
#include "Arcadia/VPL/IR/ObjectArguments.h"

struct Arcadia_VPL_VerifiedModuleDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_VerifiedModule {
  Arcadia_Object _parent;
  Arcadia_String* text;
};

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_VerifiedModule* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_VerifiedModule);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  if (1 != _numberOfArguments) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  self->text = (Arcadia_String*)Arcadia_VPL_IR_getRequiredObjectArgument(thread, 1, _Arcadia_String_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_VPL_VerifiedModule);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_VerifiedModule* self
  )
{
  if (self->text) Arcadia_Object_visit(thread, (Arcadia_Object*)self->text);
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.VerifiedModule", Arcadia_VPL_VerifiedModule,
                         u8"Arcadia.Object", Arcadia_Object, &typeOperations);

static Arcadia_VPL_VerifiedModule*
newSnapshot
  (
    Arcadia_Thread* thread,
    Arcadia_String* text
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_VerifiedModule);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)text);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_VPL_VerifiedModule);
}

Arcadia_VPL_VerifiedModule*
Arcadia_VPL_VerifiedModule_create
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Module* module,
    Arcadia_Languages_Diagnostics* diagnostics
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) {
    Arcadia_String* text = Arcadia_VPL_Text_write(thread, module, diagnostics);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)text);
    Arcadia_VPL_VerifiedModule* result = newSnapshot(thread, text);
    Arcadia_ValueStack_popValues(thread, 1);
    Arcadia_Thread_popJumpTarget(thread);
    return result;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
    Arcadia_Thread_jump(thread);
    return NULL;
  }
}

Arcadia_String*
Arcadia_VPL_VerifiedModule_getText
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_VerifiedModule* self
  )
{
  return self->text;
}

Arcadia_VPL_IR_Module*
Arcadia_VPL_VerifiedModule_materialize
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_VerifiedModule* self,
    Arcadia_Languages_Diagnostics* diagnostics
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)self);
    Arcadia_VPL_IR_Module* result = Arcadia_VPL_Text_read(thread, Arcadia_String_getBytes(thread, self->text), Arcadia_String_getNumberOfBytes(thread, self->text), diagnostics, NULL);
    Arcadia_ValueStack_popValues(thread, 1);
    Arcadia_Thread_popJumpTarget(thread);
    return result;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
    Arcadia_Thread_jump(thread);
    return NULL;
  }
}
