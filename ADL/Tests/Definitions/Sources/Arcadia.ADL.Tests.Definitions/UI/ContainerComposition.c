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

#include "Arcadia.ADL.Tests.Definitions/UI/ContainerComposition.h"

#include "Arcadia/ADL/Include.h"
#include "Arcadia/Engine/UI/Include.h"

static void
assertRecordRejected
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions,
    char const* record
  )
{
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_ADL_Context_readFromString(thread, context, definitions,
      Arcadia_String_createFromCxxString(thread, record), Arcadia_BooleanValue_False);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (Arcadia_Thread_getStatus(thread) != Arcadia_Status_SemanticalError) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  }
}

void
Arcadia_ADL_Tests_UI_ContainerCompositionTest1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Tests_assertTrue(thread, 0.33 == Arcadia_Engine_UI_SliderNode_projectValue(0.333, 0.0, 1.0, 0.01));
  Arcadia_Tests_assertTrue(thread, 0.34 == Arcadia_Engine_UI_SliderNode_projectValue(0.335, 0.0, 1.0, 0.01));
  Arcadia_Tests_assertTrue(thread, 0.0 == Arcadia_Engine_UI_SliderNode_projectValue(-0.02, 0.0, 1.0, 0.01));
  Arcadia_Tests_assertTrue(thread, 1.0 == Arcadia_Engine_UI_SliderNode_projectValue(1.02, 0.0, 1.0, 0.01));

  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  static const char* rootDefinitionDDL =
    "{ type : \"UI.StackPanel\", name : \"Test.Root\", left : 0, bottom : 0, width : 100, height : 100, vertical : \"true\", gap : 0, paddingLeft : 0, paddingBottom : 0, paddingRight : 0, paddingTop : 0, mainAlignment : \"Center\", crossAlignment : \"Center\", children : [ \"Test.Parent\" ] }";
  static const char* parentDefinitionDDL =
    "{ type : \"UI.StackPanel\", name : \"Test.Parent\", left : 0, bottom : 0, width : 100, height : 100, vertical : \"true\", gap : 0, paddingLeft : 0, paddingBottom : 0, paddingRight : 0, paddingTop : 0, mainAlignment : \"Center\", crossAlignment : \"Center\", children : [ \"Test.Child\" ] }";
  static const char* childDefinitionDDL =
    "{ type : \"UI.StackPanel\", name : \"Test.Child\", left : 0, bottom : 0, width : 100, height : 100, vertical : \"true\", gap : 0, paddingLeft : 0, paddingBottom : 0, paddingRight : 0, paddingTop : 0, mainAlignment : \"Center\", crossAlignment : \"Center\", children : [] }";
  static const char* labelDefinitionDDL =
    "{ type : \"UI.Label\", name : \"Test.Label\", left : 0, bottom : 0, width : 100, height : 24, text : \"Example\", horizontalAlignment : \"Center\", verticalAlignment : \"Center\", red : 1, green : 2, blue : 3, alpha : 255, pixelSize : 18 }";
  static const char* segmentedChoiceDefinitionDDL =
    "{ type : \"UI.SegmentedChoice\", name : \"Test.Tabs\", left : 0, bottom : 0, width : 100, height : 24, selectedIndex : 1, labels : [ \"One\", \"Two\" ], pixelSize : 20, borderWidth : 1, borderRed : 65, borderGreen : 87, borderBlue : 102 }";
  static const char* sliderDefinitionDDL =
    "{ type : \"UI.Slider\", name : \"Test.Slider\", left : 0, bottom : 0, width : 120, height : 24, minimum : 0.0, maximum : 1.0, value : 0.75, step : 0.05 }";
  static const char* toggleDefinitionDDL =
    "{ type : \"UI.Toggle\", name : \"Test.Toggle\", left : 0, bottom : 0, width : 96, height : 44, checked : \"true\" }";
  static const char* buttonDefinitionDDL =
    "{ type : \"UI.Button\", name : \"Test.Button\", left : 0, bottom : 0, width : 120, height : 40, text : \"Action\", pixelSize : 20, borderWidth : 1, borderRed : 65, borderGreen : 87, borderBlue : 102 }";
  static const char* unknownFieldDDL =
    "{ type : \"UI.Toggle\", name : \"Test.InvalidUnknown\", left : 0, bottom : 0, width : 96, height : 44, checked : \"true\", unexpected : 1 }";
  static const char* missingFieldDDL =
    "{ type : \"UI.Toggle\", name : \"Test.InvalidMissing\", left : 0, bottom : 0, width : 96, height : 44 }";
  static const char* invalidButtonPropertyDDL =
    "{ type : \"UI.Button\", name : \"Test.InvalidButton\", left : 0, bottom : 0, width : 120, height : 40, text : \"Action\", pixelSize : \"20\" }";
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, rootDefinitionDDL), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, parentDefinitionDDL), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, childDefinitionDDL), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, labelDefinitionDDL), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, segmentedChoiceDefinitionDDL), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, sliderDefinitionDDL), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, toggleDefinitionDDL), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, buttonDefinitionDDL), Arcadia_BooleanValue_False);

  Arcadia_ADL_UI_StackPanelDefinition* rootDefinition = (Arcadia_ADL_UI_StackPanelDefinition*)Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, u8"Test.Root"));
  Arcadia_ADL_UI_LabelDefinition* labelDefinition = (Arcadia_ADL_UI_LabelDefinition*)Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, u8"Test.Label"));
  Arcadia_ADL_UI_SegmentedChoiceDefinition* segmentedChoiceDefinition = (Arcadia_ADL_UI_SegmentedChoiceDefinition*)Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, u8"Test.Tabs"));
  Arcadia_ADL_UI_SliderDefinition* sliderDefinition = (Arcadia_ADL_UI_SliderDefinition*)Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, u8"Test.Slider"));
  Arcadia_ADL_UI_ToggleDefinition* toggleDefinition = (Arcadia_ADL_UI_ToggleDefinition*)Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, u8"Test.Toggle"));
  Arcadia_ADL_UI_ButtonDefinition* buttonDefinition = (Arcadia_ADL_UI_ButtonDefinition*)Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, u8"Test.Button"));
  Arcadia_Tests_assertTrue(thread, 18 == labelDefinition->pixelSize);
  Arcadia_Tests_assertTrue(thread, 1 == segmentedChoiceDefinition->selectedIndex);
  Arcadia_Tests_assertTrue(thread, 2 == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)segmentedChoiceDefinition->segmentLabels));
  Arcadia_Tests_assertTrue(thread, 20 == segmentedChoiceDefinition->pixelSize);
  Arcadia_Tests_assertTrue(thread, 1 == segmentedChoiceDefinition->borderWidth);
  Arcadia_Tests_assertTrue(thread, 20 == buttonDefinition->pixelSize);
  Arcadia_Tests_assertTrue(thread, 1 == buttonDefinition->borderWidth);
  Arcadia_Tests_assertTrue(thread, 0.75 == sliderDefinition->value);
  Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_True == toggleDefinition->checked);

  assertRecordRejected(thread, context, definitions, unknownFieldDDL);
  assertRecordRejected(thread, context, definitions, missingFieldDDL);
  assertRecordRejected(thread, context, definitions, invalidButtonPropertyDDL);

  Arcadia_Engine_UI_StackPanelNode* firstRoot = Arcadia_Engine_UI_StackPanelNode_createFromDefinition(thread, NULL, rootDefinition);
  Arcadia_Engine_UI_StackPanelNode* secondRoot = Arcadia_Engine_UI_StackPanelNode_createFromDefinition(thread, NULL, rootDefinition);
  Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Engine_UI_Container_getChildCount(thread, (Arcadia_Object*)firstRoot));
  Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Engine_UI_Container_getChildCount(thread, (Arcadia_Object*)secondRoot));

  Arcadia_Object* firstParent = Arcadia_Engine_UI_Container_getChildAt(thread, (Arcadia_Object*)firstRoot, 0);
  Arcadia_Object* secondParent = Arcadia_Engine_UI_Container_getChildAt(thread, (Arcadia_Object*)secondRoot, 0);
  Arcadia_Tests_assertTrue(thread, NULL != firstParent);
  Arcadia_Tests_assertTrue(thread, NULL != secondParent);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, firstParent, _Arcadia_Engine_UI_StackPanelNode_getType(thread)));
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, secondParent, _Arcadia_Engine_UI_StackPanelNode_getType(thread)));
  Arcadia_Tests_assertTrue(thread, firstParent != secondParent);
  Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Engine_UI_Container_getChildCount(thread, firstParent));

  Arcadia_Object* firstChild = Arcadia_Engine_UI_Container_getChildAt(thread, firstParent, 0);
  Arcadia_Tests_assertTrue(thread, NULL != firstChild);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, firstChild, _Arcadia_Engine_UI_StackPanelNode_getType(thread)));
}
