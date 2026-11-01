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

#include "Arcadia/Engine/Tests/InputEvents/Cases.h"
#include "Arcadia/Engine/Include.h"

static const Arcadia_Engine_Input_KeyboardKey keys[] = {
#define withAliases
#define Define(name, value, description) Arcadia_Engine_Input_KeyboardKey_##name,
#define Alias(name, aliased) Arcadia_Engine_Input_KeyboardKey_##name,
#include "Arcadia/Engine/Input/KeyboardKey.i"
#undef Alias
#undef Define
#undef withAliases
};

static Arcadia_Engine_Input_KeyboardKeyEvent*
create
  (
    Arcadia_Thread* thread,
    Arcadia_Value* args,
    Arcadia_Natural8Value count
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_Natural8Value i = 0; i < count; ++i) Arcadia_ValueStack_pushValue(thread, &args[i]);
  Arcadia_ValueStack_pushNatural8Value(thread, count);
  return _Arcadia_EndCreate0(thread, _Arcadia_Engine_Input_KeyboardKeyEvent_getType(thread), base);
}

static void
expectFailure
  (
    Arcadia_Thread* thread,
    Arcadia_Value* args,
    Arcadia_Natural8Value count,
    Arcadia_Status status
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    create(thread, args, count);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == status && Arcadia_ValueStack_getSize(thread) == base);
    Arcadia_Thread_setStatus(thread, 0);
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
  Arcadia_Natural64Value timestamps[] = { 0, 1, Arcadia_Natural64Value_Maximum };
  for (size_t t = 0; t < sizeof(timestamps) / sizeof(timestamps[0]); ++t) {
    for (size_t k = 0; k < sizeof(keys) / sizeof(keys[0]); ++k) {
      for (Arcadia_Integer32Value action = Arcadia_Engine_Input_KeyboardKeyAction_Pressed; action <= Arcadia_Engine_Input_KeyboardKeyAction_Released; ++action) {
        Arcadia_Engine_Input_KeyboardKeyEvent* self = Arcadia_Engine_Input_KeyboardKeyEvent_create(thread, timestamps[t], action, keys[k]);
        Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
        Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
        Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)self)->timestamp == timestamps[t] && self->key == keys[k] && self->action == action);
        Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, self) == keys[k] && Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, self) == action);
        Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Event_getType(thread)));
        Arcadia_ValueStack_popValues(thread, 1);
      }
    }
  }
  Arcadia_Value args[] = {
    Arcadia_Value_makeNatural64Value(Arcadia_Natural64Value_Maximum),
    Arcadia_Value_makeInteger32Value(Arcadia_Engine_Input_KeyboardKeyAction_Released),
    Arcadia_Value_makeInteger32Value(Arcadia_Engine_Input_KeyboardKey_F12),
    Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void)
  };
  Arcadia_TypeValue enums[] = { _Arcadia_Engine_Input_KeyboardKeyAction_getType(thread), _Arcadia_Engine_Input_KeyboardKey_getType(thread) };
  for (unsigned representations = 0; representations < 4; ++representations) {
    args[1] = representations & 1 ? Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(enums[0], Arcadia_Engine_Input_KeyboardKeyAction_Released)) : Arcadia_Value_makeInteger32Value(Arcadia_Engine_Input_KeyboardKeyAction_Released);
    args[2] = representations & 2 ? Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(enums[1], Arcadia_Engine_Input_KeyboardKey_F12)) : Arcadia_Value_makeInteger32Value(Arcadia_Engine_Input_KeyboardKey_F12);
    Arcadia_Engine_Input_KeyboardKeyEvent* self = create(thread, args, 3);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
    Arcadia_Tests_assertTrue(thread, self->key == Arcadia_Engine_Input_KeyboardKey_F12 && self->action == Arcadia_Engine_Input_KeyboardKeyAction_Released && ((Arcadia_Engine_Event*)self)->timestamp == Arcadia_Natural64Value_Maximum);
    Arcadia_ValueStack_popValues(thread, 1);
  }
  args[1] = Arcadia_Value_makeInteger32Value(Arcadia_Engine_Input_KeyboardKeyAction_Released);
  args[2] = Arcadia_Value_makeInteger32Value(Arcadia_Engine_Input_KeyboardKey_F12);
  expectFailure(thread, args, 0, Arcadia_Status_NumberOfArgumentsInvalid);
  expectFailure(thread, args, 2, Arcadia_Status_NumberOfArgumentsInvalid);
  expectFailure(thread, args, 4, Arcadia_Status_NumberOfArgumentsInvalid);
  for (size_t i = 0; i < 3; ++i) {
    Arcadia_Value saved = args[i];
    args[i] = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
    expectFailure(thread, args, 3, Arcadia_Status_ArgumentTypeInvalid);
    args[i] = Arcadia_Value_makeNatural32Value(1);
    expectFailure(thread, args, 3, Arcadia_Status_ArgumentTypeInvalid);
    if (i) {
      args[i] = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(enums[i == 1 ? 1 : 0], 1));
      expectFailure(thread, args, 3, Arcadia_Status_ArgumentTypeInvalid);
    }
    args[i] = saved;
  }
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 300);
  Arcadia_ValueStack_popValues(thread, 300);
}

int
Arcadia_Engine_Tests_InputEvents_keyboardEvent
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
