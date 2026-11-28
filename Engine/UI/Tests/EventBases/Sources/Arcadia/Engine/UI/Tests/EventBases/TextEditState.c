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

#include "Arcadia/Testing/Include.h"
#include "Arcadia/Engine/UI/Include.h"

static void
check
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self,
    char const* expected,
    Arcadia_SizeValue size,
    Arcadia_SizeValue anchor,
    Arcadia_SizeValue caret
  )
{
  Arcadia_String* text = Arcadia_Engine_UI_TextEditState_getText(thread, self);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)text);
  Arcadia_Tests_assertEqualBytes(thread, expected, size, Arcadia_String_getBytes(thread, text), Arcadia_String_getNumberOfBytes(thread, text));
  Arcadia_Tests_assertEqualSize(thread, anchor, Arcadia_Engine_UI_TextEditState_getAnchor(thread, self));
  Arcadia_Tests_assertEqualSize(thread, caret, Arcadia_Engine_UI_TextEditState_getCaret(thread, self));
  Arcadia_ValueStack_popValues(thread, 1);
}

static void
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Engine_UI_TextEditState* self = Arcadia_Engine_UI_TextEditState_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)self);
  check(thread, self, "", 0, 0, 0);
  Arcadia_String* initial = Arcadia_String_createFromCxxString(thread, "a\xc3\xa9\xf0\x9f\x98\x80z");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)initial);
  Arcadia_Engine_UI_TextEditState_setText(thread, self, initial);
  Arcadia_ValueStack_popValues(thread, 1);
  check(thread, self, "a\xc3\xa9\xf0\x9f\x98\x80z", 8, 4, 4);
  Arcadia_Engine_UI_TextEditState_moveHome(thread, self, false);
  Arcadia_Engine_UI_TextEditState_moveRight(thread, self, false);
  Arcadia_Engine_UI_TextEditState_moveRight(thread, self, true);
  Arcadia_Engine_UI_TextEditState_moveRight(thread, self, true);
  Arcadia_String* replacement = Arcadia_String_createFromCxxString(thread, "X");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)replacement);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_TextEditState_insertText(thread, self, replacement));
  check(thread, self, "aXz", 3, 2, 2);
  Arcadia_Engine_UI_TextEditState_setSelection(thread, self, 2, 1);
  Arcadia_Tests_assertFalse(thread, Arcadia_Engine_UI_TextEditState_insertText(thread, self, replacement));
  check(thread, self, "aXz", 3, 2, 2);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_TextEditState_backspace(thread, self));
  check(thread, self, "az", 2, 1, 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_TextEditState_deleteForward(thread, self));
  check(thread, self, "a", 1, 1, 1);
  Arcadia_Tests_assertFalse(thread, Arcadia_Engine_UI_TextEditState_deleteForward(thread, self));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_TextEditState_cancel(thread, self));
  check(thread, self, "a\xc3\xa9\xf0\x9f\x98\x80z", 8, 4, 4);
  Arcadia_Engine_UI_TextEditState_setSelection(thread, self, 3, 1);
  Arcadia_Engine_UI_TextEditState_moveLeft(thread, self, false);
  check(thread, self, "a\xc3\xa9\xf0\x9f\x98\x80z", 8, 1, 1);
  Arcadia_Engine_UI_TextEditState_setSelection(thread, self, 1, 3);
  Arcadia_Engine_UI_TextEditState_moveRight(thread, self, false);
  check(thread, self, "a\xc3\xa9\xf0\x9f\x98\x80z", 8, 3, 3);
  Arcadia_Engine_UI_TextEditState_moveEnd(thread, self, false);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_TextEditState_backspace(thread, self));
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_TextEditState_commit(thread, self));
  Arcadia_Tests_assertFalse(thread, Arcadia_Engine_UI_TextEditState_commit(thread, self));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  check(thread, self, "a\xc3\xa9\xf0\x9f\x98\x80", 7, 3, 3);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, Arcadia_Engine_UI_TextEditState_getCommittedText(thread, self), "a\xc3\xa9\xf0\x9f\x98\x80", 7));
  Arcadia_String* invalid = Arcadia_String_createFromCxxString(thread, "a\nb");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)invalid);
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      if (i == 0) {
        Arcadia_Engine_UI_TextEditState_insertText(thread, self, invalid);
      } else if (i == 1) {
        Arcadia_Engine_UI_TextEditState_setText(thread, self, invalid);
      } else {
        Arcadia_Engine_UI_TextEditState_setSelection(thread, self, 0, 4);
      }
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Status status = Arcadia_Thread_getStatus(thread);
      Arcadia_Thread_setStatus(thread, 0);
      Arcadia_Tests_assertTrue(thread, status == Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
    }
    check(thread, self, "a\xc3\xa9\xf0\x9f\x98\x80", 7, 3, 3);
  }
  Arcadia_ValueStack_popValues(thread, 2);
}

int
Arcadia_Engine_UI_Tests_textEditState
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&test) ? EXIT_SUCCESS : EXIT_FAILURE;
}

