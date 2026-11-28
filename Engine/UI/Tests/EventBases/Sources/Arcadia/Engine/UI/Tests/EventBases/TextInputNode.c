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
test(Arcadia_Thread* thread)
{
  Arcadia_Engine_UI_TextInputNode* node = Arcadia_Engine_UI_TextInputNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)node);
  // Explicit UTF-8 bytes avoid dependence on the compiler's source encoding.
  static const Arcadia_Natural8Value bytes[] = { 0x61, 0xc3, 0xa9 };
  Arcadia_Value value = Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, bytes, sizeof(bytes)));
  Arcadia_ValueStack_pushValue(thread, &value);
  Arcadia_String* text = Arcadia_String_create(thread, value);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)text);
  Arcadia_Tests_assertEqualBytes(thread, bytes, sizeof(bytes), Arcadia_String_getBytes(thread, text), Arcadia_String_getNumberOfBytes(thread, text));
  Arcadia_Tests_assertEqualSize(thread, 2, Arcadia_String_getNumberOfCodePoints(thread, text));
  Arcadia_Engine_UI_TextEditState* editor = Arcadia_Engine_UI_TextInputNode_getEditor(thread, node);
  Arcadia_Tests_assertFalse(thread, Arcadia_Engine_UI_TextInputNode_insertText(thread, node, text));
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_HitTestable_hitTest(thread, (Arcadia_Object*)node, 0, 0));
  Arcadia_Tests_assertFalse(thread, Arcadia_Engine_UI_HitTestable_hitTest(thread, (Arcadia_Object*)node, 160, 0));
  Arcadia_Engine_UI_Focusable_focus(thread, (Arcadia_Object*)node);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_TextInputNode_insertText(thread, node, text));
  Arcadia_Tests_assertEqualSize(thread, 2, Arcadia_Engine_UI_TextEditState_getCaret(thread, editor));
  Arcadia_String* actual = Arcadia_Engine_UI_TextEditState_getText(thread, editor);
  Arcadia_Tests_assertEqualBytes(thread, bytes, sizeof(bytes), Arcadia_String_getBytes(thread, actual), Arcadia_String_getNumberOfBytes(thread, actual));
  Arcadia_Engine_UI_TextInputNode_setReadOnly(thread, node, true);
  Arcadia_Tests_assertFalse(thread, Arcadia_Engine_UI_TextInputNode_insertText(thread, node, text));
  Arcadia_Engine_UI_TextInputNode_setReadOnly(thread, node, false);
  Arcadia_Engine_UI_TextInputNode_setEnabled(thread, node, false);
  Arcadia_Tests_assertFalse(thread, Arcadia_Engine_UI_Focusable_isFocused(thread, (Arcadia_Object*)node));
  Arcadia_Tests_assertFalse(thread, Arcadia_Engine_UI_HitTestable_hitTest(thread, (Arcadia_Object*)node, 0, 0));
  Arcadia_Tests_assertFalse(thread, Arcadia_Engine_UI_TextInputNode_insertText(thread, node, text));
  Arcadia_Engine_UI_TextInputNode_setEnabled(thread, node, true);
  Arcadia_Engine_UI_Focusable_focus(thread, (Arcadia_Object*)node);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertEqualSize(thread, 2, Arcadia_Engine_UI_TextEditState_getCaret(thread, editor));
  Arcadia_Engine_UI_Focusable_blur(thread, (Arcadia_Object*)node);
  Arcadia_Tests_assertFalse(thread, Arcadia_Engine_UI_TextInputNode_insertText(thread, node, text));
  Arcadia_ValueStack_popValues(thread, 2);
}

int
Arcadia_Engine_UI_Tests_textInputNode(void)
{
  return Arcadia_Tests_safeExecute(&test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
