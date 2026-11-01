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

#include "Arcadia/Engine/UI/Tests/EventBases/Alignment.h"
#include "Arcadia/Engine/UI/Include.h"
#include <string.h>

typedef Arcadia_Engine_UI_StackPanelNodeAlignment Alignment;

_Static_assert(Arcadia_Engine_UI_StackPanelNodeAlignment_Start == 0, "Start value");
_Static_assert(Arcadia_Engine_UI_StackPanelNodeAlignment_Center == 1, "Center value");
_Static_assert(Arcadia_Engine_UI_StackPanelNodeAlignment_End == 2, "End value");
_Static_assert(Arcadia_Engine_UI_StackPanelNodeAlignment_Stretch == 3, "Stretch value");
_Static_assert(sizeof(Alignment) == sizeof(Arcadia_Integer32Value), "enum size");
_Static_assert(_Alignof(Alignment) == _Alignof(Arcadia_Integer32Value), "enum alignment");
_Static_assert(_Generic(&_Arcadia_Engine_UI_StackPanelNodeAlignment_getType, Arcadia_TypeValue (*)(Arcadia_Thread*): 1, default: 0), "descriptor signature");
_Static_assert(_Generic(&Arcadia_Engine_UI_StackPanelNode_setAlignment, void (*)(Arcadia_Thread*, Arcadia_Engine_UI_StackPanelNode*, Alignment, Alignment): 1, default: 0), "setAlignment signature");
_Static_assert(_Generic(&Arcadia_Engine_UI_StackPanelNode_setChildCrossAlignment, void (*)(Arcadia_Thread*, Arcadia_Engine_UI_StackPanelNode*, Arcadia_Object*, Alignment): 1, default: 0), "child alignment signature");

static void
testMetadata
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_TypeValue type = _Arcadia_Engine_UI_StackPanelNodeAlignment_getType(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_getKind(thread, type) == Arcadia_TypeKind_Enumeration);
  Arcadia_Name* name = Arcadia_Type_getName(thread, type);
  const char expected[] = "Arcadia.Engine.UI.StackPanelNodeAlignment";
  Arcadia_Tests_assertTrue(thread, Arcadia_Name_getNumberOfBytes(thread, name) == sizeof(expected) - 1);
  Arcadia_Tests_assertTrue(thread, !memcmp(Arcadia_Name_getBytes(thread, name), expected, sizeof(expected) - 1));
  Arcadia_Tests_assertTrue(thread, Arcadia_getType(thread, name) == type);
  for (Arcadia_Integer32Value i = 0; i < 4; ++i) {
    Arcadia_Value value = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(type, i));
    Arcadia_ValueStack_pushValue(thread, &value);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, _Arcadia_Engine_UI_StackPanelNodeAlignment_getType(thread) == type);
  for (Arcadia_Integer32Value i = 0; i < 4; ++i) {
    Arcadia_Value value = Arcadia_ValueStack_getValue(thread, 3 - i);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isEnumerationValue(&value));
    Arcadia_EnumerationValue enumeration = Arcadia_Value_getEnumerationValue(&value);
    Arcadia_Tests_assertTrue(thread, enumeration.type == type && enumeration.value == i);
  }
  Arcadia_ValueStack_popValues(thread, 4);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testConsumers
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  Arcadia_Engine_UI_StackPanelNode* panel = Arcadia_Engine_UI_StackPanelNode_create(thread, 1, 2, 80, 90, true, 3);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, panel);
  Arcadia_Engine_UI_StackPanelNode* child = Arcadia_Engine_UI_StackPanelNode_create(thread, 0, 0, 10, 20, false, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, child);
  Arcadia_Tests_assertTrue(thread, panel->mainAlignment == Arcadia_Engine_UI_StackPanelNodeAlignment_Center && panel->crossAlignment == Arcadia_Engine_UI_StackPanelNodeAlignment_Center);
  Arcadia_Engine_UI_Container_addChild(thread, (void*)panel, (void*)child);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, !panel->slots[0].hasAlignmentOverride);
  for (unsigned main = 0; main < 4; ++main) {
    for (unsigned cross = 0; cross < 4; ++cross) {
      Arcadia_Engine_UI_StackPanelNode_setAlignment(thread, panel, (Alignment)main, (Alignment)cross);
      Arcadia_Tests_assertTrue(thread, panel->mainAlignment == (Alignment)main && panel->crossAlignment == (Alignment)cross);
    }
    Arcadia_Engine_UI_StackPanelNode_setChildCrossAlignment(thread, panel, (void*)child, (Alignment)main);
    Arcadia_Tests_assertTrue(thread, panel->slots[0].hasAlignmentOverride && panel->slots[0].crossAlignment == (Alignment)main);
    Arcadia_Engine_UI_StackPanelNode_clearChildCrossAlignment(thread, panel, (void*)child);
    Arcadia_Tests_assertTrue(thread, !panel->slots[0].hasAlignmentOverride);
  }
  const Arcadia_Integer32Value invalid[] = { -1, 4, INT32_MIN, INT32_MAX };
  for (unsigned i = 0; i < sizeof(invalid) / sizeof(invalid[0]); ++i) {
    for (unsigned operation = 0; operation < 3; ++operation) {
      Arcadia_Engine_UI_StackPanelNode_setAlignment(thread, panel, (Alignment)0, (Alignment)1);
      Arcadia_Engine_UI_StackPanelNode_setChildCrossAlignment(thread, panel, (void*)child, (Alignment)2);
      Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
      Arcadia_JumpTarget jump;
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (Arcadia_JumpTarget_save(&jump)) {
        if (operation == 0) Arcadia_Engine_UI_StackPanelNode_setAlignment(thread, panel, (Alignment)invalid[i], (Alignment)3);
        else if (operation == 1) Arcadia_Engine_UI_StackPanelNode_setAlignment(thread, panel, (Alignment)3, (Alignment)invalid[i]);
        else Arcadia_Engine_UI_StackPanelNode_setChildCrossAlignment(thread, panel, (void*)child, (Alignment)invalid[i]);
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, false);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_setStatus(thread, 0);
        Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
        Arcadia_Tests_assertTrue(thread, panel->mainAlignment == (Alignment)0 && panel->crossAlignment == (Alignment)1);
        Arcadia_Tests_assertTrue(thread, panel->slots[0].hasAlignmentOverride && panel->slots[0].crossAlignment == (Alignment)2);
      }
    }
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildAt(thread, (void*)panel, 0) == (Arcadia_Object*)child);
  Arcadia_Tests_assertTrue(thread, panel->slots[0].crossAlignment == (Alignment)2);
  Arcadia_ValueStack_popValues(thread, 301);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

int
Arcadia_Engine_UI_Tests_alignment
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(testMetadata)) return EXIT_FAILURE;
  return Arcadia_Tests_safeExecute(testConsumers) ? EXIT_SUCCESS : EXIT_FAILURE;
}
