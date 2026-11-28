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
#include <math.h>

static void
draft
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_NumericEditState* state,
    char const* input
  )
{
  Arcadia_Engine_UI_TextEditState* editor = Arcadia_Engine_UI_NumericEditState_getEditor(thread, state);
  Arcadia_String* text = Arcadia_String_createFromCxxString(thread, input);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)text);
  Arcadia_Engine_UI_TextEditState_moveEnd(thread, editor, false);
  Arcadia_Engine_UI_TextEditState_moveHome(thread, editor, true);
  Arcadia_Engine_UI_TextEditState_insertText(thread, editor, text);
  Arcadia_ValueStack_popValues(thread, 1);
}

static void
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Engine_UI_NumericEditState* state = Arcadia_Engine_UI_NumericEditState_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)state);
  Arcadia_Engine_UI_NumericEditState_setValue(thread, state, Arcadia_Value_makeInteger64Value(5));
  Arcadia_Engine_UI_NumericEditState_setBounds(thread, state, Arcadia_Value_makeInteger64Value(-10), Arcadia_Value_makeInteger64Value(10));
  static char const* const invalid[] = { "", "-", ".", "1.5", "1e1", "11", "-11", "9223372036854775808", "1x" };
  for (Arcadia_SizeValue i = 0; i < sizeof(invalid) / sizeof(invalid[0]); ++i) {
    draft(thread, state, invalid[i]);
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_NumericEditState_commit(thread, state) == Arcadia_Engine_UI_NumericEditStateResult_Rejected);
    Arcadia_Value value = Arcadia_Engine_UI_NumericEditState_getValue(thread, state);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_getInteger64Value(&value) == 5);
  }
  draft(thread, state, "-10");
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_NumericEditState_commit(thread, state) == Arcadia_Engine_UI_NumericEditStateResult_Changed);
  draft(thread, state, "-010");
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_NumericEditState_commit(thread, state) == Arcadia_Engine_UI_NumericEditStateResult_Unchanged);
  draft(thread, state, "-");
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_NumericEditState_cancel(thread, state));
  Arcadia_Value absent = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
  Arcadia_Engine_UI_NumericEditState_setBounds(thread, state, absent, absent);
  Arcadia_Engine_UI_NumericEditState_setValue(thread, state, Arcadia_Value_makeReal64Value(0.5));
  static char const* const invalidReal[] = { ".", "1e", "1e+", "1^2", "NaN", "inf", "1e9999", "1,5" };
  for (Arcadia_SizeValue i = 0; i < sizeof(invalidReal) / sizeof(invalidReal[0]); ++i) {
    draft(thread, state, invalidReal[i]);
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_NumericEditState_commit(thread, state) == Arcadia_Engine_UI_NumericEditStateResult_Rejected);
    Arcadia_Value value = Arcadia_Engine_UI_NumericEditState_getValue(thread, state);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_getReal64Value(&value) == 0.5);
  }
  draft(thread, state, "-1.25e1");
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_NumericEditState_commit(thread, state) == Arcadia_Engine_UI_NumericEditStateResult_Changed);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Value value = Arcadia_Engine_UI_NumericEditState_getValue(thread, state);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_getReal64Value(&value) == -12.5);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_NumericEditState_commit(thread, state) == Arcadia_Engine_UI_NumericEditStateResult_Unchanged);
  Arcadia_ValueStack_popValues(thread, 1);
}

int
Arcadia_Engine_UI_Tests_numericEditState
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
