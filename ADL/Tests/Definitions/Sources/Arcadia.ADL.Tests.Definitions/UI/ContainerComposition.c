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

static void
testInlineButtonColors
  (
    Arcadia_Thread* thread
  )
{
  // Both objects are used after a deliberate GC run below. The context
  // singleton is weak, and raw C locals are not ARMS roots.
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ADL_Context_readFromString(thread, context, definitions,
    Arcadia_String_createFromCxxString(thread,
      "{ type: \"Color\", name: \"Test.NamedColor\", red: 3, green: 6, blue: 9 }"), false);
  Arcadia_ADL_UI_ButtonDefinition* button = (Arcadia_ADL_UI_ButtonDefinition*)Arcadia_ADL_Context_readFromString(
    thread, context, definitions,
    Arcadia_String_createFromCxxString(thread,
      "{ type: \"UI.Button\", name: \"Test.InlineButton\", left: 0, bottom: 0, width: 20, height: 10, text: \"Go\", "
      "backgroundColor: { type: \"Color\", red: 13, green: 79, blue: 227, alpha: 0 }, "
      "hoveredBackgroundColor: { type: \"Color\", red: 13, green: 79, blue: 227 }, "
      "pressedBackgroundColor: \"Test.NamedColor\" }"), false);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)button);
  Arcadia_ADL_Definition_link(thread, (Arcadia_ADL_Definition*)button);
  Arcadia_ADL_ColorDefinition* base = (Arcadia_ADL_ColorDefinition*)button->backgroundColor->definition;
  Arcadia_ADL_ColorDefinition* hovered = (Arcadia_ADL_ColorDefinition*)button->hoveredBackgroundColor->definition;
  Arcadia_Tests_assertTrue(thread, base != NULL && hovered != NULL && base != hovered);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_ADL_Definition*)base)->name == NULL);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_ADL_Definition*)hovered)->name == NULL);
  Arcadia_Tests_assertEqualNatural64(thread, 0, base->alpha);
  Arcadia_Tests_assertEqualNatural64(thread, 255, hovered->alpha);
  Arcadia_Tests_assertEqualNatural64(thread, 13, base->red);
  Arcadia_Tests_assertTrue(thread, button->disabledBackgroundColor->definition == (Arcadia_ADL_Definition*)base);
  Arcadia_Tests_assertTrue(thread, button->focusedBackgroundColor->definition == (Arcadia_ADL_Definition*)base);
  Arcadia_Tests_assertTrue(thread, button->pressedBackgroundColor->definition != (Arcadia_ADL_Definition*)base);
  Arcadia_Tests_assertTrue(thread, button->pressedBackgroundColor->definition != (Arcadia_ADL_Definition*)hovered);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertEqualNatural64(thread, 227, base->blue);
  Arcadia_ValueStack_popValues(thread, 1);

  // Invalid inline maps must fail while reading the containing button.
  assertRecordRejected(thread, context, definitions,
    "{ type: \"UI.Button\", name: \"Test.BadInlineName\", left: 0, bottom: 0, width: 20, height: 10, text: \"X\", backgroundColor: { type: \"Color\", name: \"\", red: 1, green: 2, blue: 3 } }");
  assertRecordRejected(thread, context, definitions,
    "{ type: \"UI.Button\", name: \"Test.BadInlineType\", left: 0, bottom: 0, width: 20, height: 10, text: \"X\", backgroundColor: { type: \"UI.Label\", red: 1, green: 2, blue: 3 } }");
  assertRecordRejected(thread, context, definitions,
    "{ type: \"UI.Button\", name: \"Test.BadInlineMissing\", left: 0, bottom: 0, width: 20, height: 10, text: \"X\", backgroundColor: { type: \"Color\", red: 1, green: 2 } }");
  assertRecordRejected(thread, context, definitions,
    "{ type: \"UI.Button\", name: \"Test.BadInlineExtra\", left: 0, bottom: 0, width: 20, height: 10, text: \"X\", backgroundColor: { type: \"Color\", red: 1, green: 2, blue: 3, unexpected: 1 } }");
  assertRecordRejected(thread, context, definitions,
    "{ type: \"UI.Button\", name: \"Test.BadInlineKind\", left: 0, bottom: 0, width: 20, height: 10, text: \"X\", backgroundColor: 123 }");
  Arcadia_ValueStack_popValues(thread, 2);
}

static void
testDeclarativePanels
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  // Child panels have nonzero desired sizes from their padding alone.
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread,
    "{ type: \"UI.Panel\", name: \"Test.Declarative.First\", left: 0, bottom: 0, width: 30, height: 20, "
    "children: [], layout: { type: \"UI.StackLayout\", orientation: \"Horizontal\", "
    "paddingLeft: 15, paddingRight: 15, paddingBottom: 10, paddingTop: 10 } }"), false);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread,
    "{ type: \"UI.Panel\", name: \"Test.Declarative.Second\", left: 0, bottom: 0, width: 40, height: 30, "
    "children: [], layout: { type: \"UI.StackLayout\", orientation: \"Vertical\", "
    "paddingLeft: 20, paddingRight: 20, paddingBottom: 15, paddingTop: 15 } }"), false);
  Arcadia_ADL_UI_PanelDefinition* definition = (Arcadia_ADL_UI_PanelDefinition*)Arcadia_ADL_Context_readFromString(
    thread, context, definitions, Arcadia_String_createFromCxxString(thread,
    "{ type: \"UI.Panel\", name: \"Test.Declarative.Root\", left: 10, bottom: 20, width: 140, height: 90, "
    "children: [ \"Test.Declarative.First\", \"Test.Declarative.Second\" ], "
    "layout: { type: \"UI.StackLayout\", orientation: \"Vertical\", gap: 5, paddingLeft: 2, "
    "paddingBottom: 3, paddingRight: 4, paddingTop: 6, mainAlignment: \"Start\", crossAlignment: \"End\" } }"), false);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_UI_PanelDefinition_getType(thread)));
  Arcadia_Tests_assertTrue(thread, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)definition->childReferences) == 2);
  Arcadia_Tests_assertTrue(thread, definition->vertical && definition->gap == 5 && definition->paddingTop == 6);
  Arcadia_Object* object = Arcadia_Engine_UI_createNodeFromDefinition(thread, NULL, (Arcadia_ADL_Definition*)definition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, object);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, object, _Arcadia_Engine_UI_LayoutPanelNode_getType(thread)));
  Arcadia_Engine_UI_LayoutPanelNode* panel = (Arcadia_Engine_UI_LayoutPanelNode*)object;
  Arcadia_Engine_UI_StackLayout* layout = (Arcadia_Engine_UI_StackLayout*)Arcadia_Engine_UI_LayoutPanelNode_getLayout(thread, panel);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)layout, _Arcadia_Engine_UI_StackLayout_getType(thread)));
  Arcadia_Tests_assertTrue(thread, layout->settings.vertical && layout->settings.gap == 5);
  Arcadia_Tests_assertTrue(thread, layout->settings.mainAlignment == Arcadia_Engine_UI_StackPanelNodeAlignment_Start);
  Arcadia_Tests_assertTrue(thread, layout->settings.crossAlignment == Arcadia_Engine_UI_StackPanelNodeAlignment_End);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildCount(thread, object) == 2);
  Arcadia_Engine_UI_WidgetNode* first = (Arcadia_Engine_UI_WidgetNode*)Arcadia_Engine_UI_Container_getChildAt(thread, object, 0);
  Arcadia_Engine_UI_WidgetNode* second = (Arcadia_Engine_UI_WidgetNode*)Arcadia_Engine_UI_Container_getChildAt(thread, object, 1);
  Arcadia_Tests_assertTrue(thread, first != second && first->left == 116 && first->bottom == 84 && first->width == 30 && first->height == 20);
  Arcadia_Tests_assertTrue(thread, second->left == 106 && second->bottom == 49 && second->width == 40 && second->height == 30);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildAt(thread, object, 0) == (Arcadia_Object*)first);
  assertRecordRejected(thread, context, definitions,
    "{ type: \"UI.Panel\", name: \"Test.BadPanelType\", left: 0, bottom: 0, width: 10, height: 10, children: [], "
    "layout: { type: \"UI.GridLayout\", orientation: \"Vertical\" } }");
  assertRecordRejected(thread, context, definitions,
    "{ type: \"UI.Panel\", name: \"Test.BadPanelOrientation\", left: 0, bottom: 0, width: 10, height: 10, children: [], "
    "layout: { type: \"UI.StackLayout\", orientation: \"Diagonal\" } }");
  assertRecordRejected(thread, context, definitions,
    "{ type: \"UI.Panel\", name: \"Test.BadPanelGap\", left: 0, bottom: 0, width: 10, height: 10, children: [], "
    "layout: { type: \"UI.StackLayout\", orientation: \"Vertical\", gap: -1 } }");
  assertRecordRejected(thread, context, definitions,
    "{ type: \"UI.Panel\", name: \"Test.BadPanelPadding\", left: 0, bottom: 0, width: 10, height: 10, children: [], "
    "layout: { type: \"UI.StackLayout\", orientation: \"Vertical\", paddingLeft: -1 } }");
  assertRecordRejected(thread, context, definitions,
    "{ type: \"UI.Panel\", name: \"Test.BadPanelAlignment\", left: 0, bottom: 0, width: 10, height: 10, children: [], "
    "layout: { type: \"UI.StackLayout\", orientation: \"Vertical\", crossAlignment: \"Top\" } }");
  assertRecordRejected(thread, context, definitions,
    "{ type: \"UI.Panel\", name: \"Test.BadPanelExtra\", left: 0, bottom: 0, width: 10, height: 10, children: [], "
    "layout: { type: \"UI.StackLayout\", orientation: \"Vertical\", unsupported: 7 } }");
  assertRecordRejected(thread, context, definitions,
    "{ type: \"UI.Panel\", name: \"Test.BadPanelMissing\", left: 0, bottom: 0, width: 10, height: 10, children: [] }");
  Arcadia_ValueStack_popValues(thread, 3);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testDeclarativePanelChildLayouts
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread,
    "{ type: \"UI.Panel\", name: \"Test.Slots.First\", left: 0, bottom: 0, width: 30, height: 20, "
    "children: [], layout: { type: \"UI.StackLayout\", orientation: \"Horizontal\", "
    "paddingLeft: 15, paddingRight: 15, paddingTop: 10, paddingBottom: 10 } }"), false);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread,
    "{ type: \"UI.Panel\", name: \"Test.Slots.Second\", left: 0, bottom: 0, width: 40, height: 30, "
    "children: [], layout: { type: \"UI.StackLayout\", orientation: \"Horizontal\", "
    "paddingLeft: 20, paddingRight: 20, paddingTop: 15, paddingBottom: 15 } }"), false);
  Arcadia_ADL_UI_PanelDefinition* definition = (Arcadia_ADL_UI_PanelDefinition*)Arcadia_ADL_Context_readFromString(
    thread, context, definitions, Arcadia_String_createFromCxxString(thread,
    "{ type: \"UI.Panel\", name: \"Test.Slots.Root\", left: 10, bottom: 20, width: 140, height: 100, "
    "children: [\"Test.Slots.First\", \"Test.Slots.Second\"], "
    "childLayouts: [{ name: \"Test.Slots.Second\", marginLeft: 15, marginTop: 4, crossAlignment: \"End\" }], "
    "layout: { type: \"UI.StackLayout\", orientation: \"Vertical\", gap: 5, "
    "paddingLeft: 2, paddingRight: 4, paddingTop: 6, paddingBottom: 3, "
    "mainAlignment: \"Start\", crossAlignment: \"Start\" } }"), false);
  Arcadia_Tests_assertTrue(thread, definition->childLayoutCount == 2);
  Arcadia_Tests_assertTrue(thread, definition->childLayouts && !definition->childLayouts[0].configured);
  Arcadia_Tests_assertTrue(thread, definition->childLayouts[1].marginLeft == 15);
  Arcadia_Tests_assertTrue(thread, definition->childLayouts[1].marginTop == 4);
  Arcadia_Tests_assertTrue(thread, definition->childLayouts[1].hasCrossAlignment);
  Arcadia_Tests_assertTrue(thread, definition->childLayouts[1].crossAlignment == Arcadia_ADL_UI_PanelChildCrossAlignment_End);
  Arcadia_Object* object = Arcadia_Engine_UI_createNodeFromDefinition(thread, NULL, (Arcadia_ADL_Definition*)definition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, object);
  Arcadia_Engine_UI_LayoutPanelNode* panel = (Arcadia_Engine_UI_LayoutPanelNode*)object;
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildCount(thread, object) == 2);
  Arcadia_Engine_UI_WidgetNode* first = (Arcadia_Engine_UI_WidgetNode*)Arcadia_Engine_UI_Container_getChildAt(thread, object, 0);
  Arcadia_Engine_UI_WidgetNode* second = (Arcadia_Engine_UI_WidgetNode*)Arcadia_Engine_UI_Container_getChildAt(thread, object, 1);
  Arcadia_Engine_UI_StackLayoutSlot const* slots = Arcadia_Engine_UI_LayoutPanelNode_getChildLayoutSlots(thread, panel);
  Arcadia_Tests_assertTrue(thread, slots[0].marginLeft == 0 && slots[0].marginTop == 0);
  Arcadia_Tests_assertTrue(thread, slots[1].marginLeft == 15 && slots[1].marginTop == 4);
  Arcadia_Tests_assertTrue(thread, slots[1].hasAlignmentOverride);
  Arcadia_Tests_assertTrue(thread, slots[1].crossAlignment == Arcadia_Engine_UI_StackPanelNodeAlignment_End);
  Arcadia_Tests_assertTrue(thread, first->left == 12 && first->bottom == 94);
  Arcadia_Tests_assertTrue(thread, second->left == 106 && second->bottom == 55);
  Arcadia_Integer32Value width, height;
  Arcadia_Engine_UI_WidgetNode_getDesiredSize(thread, (Arcadia_Engine_UI_WidgetNode*)panel, &width, &height);
  Arcadia_Tests_assertTrue(thread, width == 61 && height == 68);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  assertRecordRejected(thread, context, definitions,
    "{ type: \"UI.Panel\", name: \"Test.BadSlots.Unknown\", left: 0, bottom: 0, width: 10, height: 10, "
    "children: [\"Test.Slots.First\"], childLayouts: [{ name: \"Does.Not.Exist\", marginLeft: 2 }], "
    "layout: { type: \"UI.StackLayout\", orientation: \"Vertical\" } }");
  assertRecordRejected(thread, context, definitions,
    "{ type: \"UI.Panel\", name: \"Test.BadSlots.Duplicate\", left: 0, bottom: 0, width: 10, height: 10, "
    "children: [\"Test.Slots.First\"], childLayouts: [{ name: \"Test.Slots.First\", marginLeft: 2 }, "
    "{ name: \"Test.Slots.First\", marginRight: 3 }], "
    "layout: { type: \"UI.StackLayout\", orientation: \"Vertical\" } }");
  assertRecordRejected(thread, context, definitions,
    "{ type: \"UI.Panel\", name: \"Test.BadSlots.Negative\", left: 0, bottom: 0, width: 10, height: 10, "
    "children: [\"Test.Slots.First\"], childLayouts: [{ name: \"Test.Slots.First\", marginTop: -1 }], "
    "layout: { type: \"UI.StackLayout\", orientation: \"Vertical\" } }");
  assertRecordRejected(thread, context, definitions,
    "{ type: \"UI.Panel\", name: \"Test.BadSlots.Alignment\", left: 0, bottom: 0, width: 10, height: 10, "
    "children: [\"Test.Slots.First\"], childLayouts: [{ name: \"Test.Slots.First\", crossAlignment: \"Above\" }], "
    "layout: { type: \"UI.StackLayout\", orientation: \"Vertical\" } }");
  assertRecordRejected(thread, context, definitions,
    "{ type: \"UI.Panel\", name: \"Test.BadSlots.Extra\", left: 0, bottom: 0, width: 10, height: 10, "
    "children: [\"Test.Slots.First\"], childLayouts: [{ name: \"Test.Slots.First\", mystery: 1 }], "
    "layout: { type: \"UI.StackLayout\", orientation: \"Vertical\" } }");
  Arcadia_ValueStack_popValues(thread, 3);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testDeclarativePanelBackground
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ADL_UI_PanelDefinition* definition = (Arcadia_ADL_UI_PanelDefinition*)Arcadia_ADL_Context_readFromString(
    thread, context, definitions, Arcadia_String_createFromCxxString(thread,
    "{ type: \"UI.Panel\", name: \"Test.Background.Root\", left: 0, bottom: 0, width: 120, height: 80, "
    "children: [], backgroundModel: \"Test.Background.Model\", backgroundBorderWidth: 3, "
    "layout: { type: \"UI.StackLayout\", orientation: \"Vertical\" } }"), false);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definition);
  Arcadia_Tests_assertTrue(thread, definition->backgroundModelName != NULL);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, definition->backgroundModelName, "Test.Background.Model", 21));
  Arcadia_Tests_assertTrue(thread, definition->backgroundBorderWidth == 3);
  // The ADL field is valid, but a missing model reference must be rejected
  // by the UI factory without attempting to create a graphics resource.
  Arcadia_SizeValue checkpoint = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Engine_UI_LayoutPanelNode_createFromDefinition(thread, NULL, definition);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_SemanticalError);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == checkpoint);
  }
  Arcadia_ADL_UI_PanelDefinition* plain = (Arcadia_ADL_UI_PanelDefinition*)Arcadia_ADL_Context_readFromString(
    thread, context, definitions, Arcadia_String_createFromCxxString(thread,
    "{ type: \"UI.Panel\", name: \"Test.Background.Plain\", left: 0, bottom: 0, width: 100, height: 60, "
    "children: [], layout: { type: \"UI.StackLayout\", orientation: \"Horizontal\" } }"), false);
  Arcadia_Tests_assertTrue(thread, plain->backgroundModelName == NULL && plain->backgroundBorderWidth == 0);
  assertRecordRejected(thread, context, definitions,
    "{ type: \"UI.Panel\", name: \"Test.Background.InvalidType\", left: 0, bottom: 0, width: 100, height: 60, "
    "children: [], backgroundModel: 42, layout: { type: \"UI.StackLayout\", orientation: \"Horizontal\" } }");
  assertRecordRejected(thread, context, definitions,
    "{ type: \"UI.Panel\", name: \"Test.Background.InvalidBorder\", left: 0, bottom: 0, width: 100, height: 60, "
    "children: [], backgroundBorderWidth: \"invalid\", layout: { type: \"UI.StackLayout\", orientation: \"Horizontal\" } }");
  Arcadia_ValueStack_popValues(thread, 3);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

void
Arcadia_ADL_Tests_UI_ContainerCompositionTest1
  (
    Arcadia_Thread* thread
  )
{
  testInlineButtonColors(thread);
  testDeclarativePanels(thread);
  testDeclarativePanelChildLayouts(thread);
  testDeclarativePanelBackground(thread);
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
  static const char* buttonColorDDL =
    "{ type : \"Color\", name : \"Test.Button.Normal\", red : 31, green : 50, blue : 60 }";
  static const char* buttonDefinitionDDL =
    "{ type : \"UI.Button\", name : \"Test.Button\", left : 0, bottom : 0, width : 120, height : 40, text : \"Action\", backgroundColor : \"Test.Button.Normal\", pixelSize : 20, borderWidth : 1, borderRed : 65, borderGreen : 87, borderBlue : 102 }";
  static const char* unknownFieldDDL =
    "{ type : \"UI.Toggle\", name : \"Test.InvalidUnknown\", left : 0, bottom : 0, width : 96, height : 44, checked : \"true\", unexpected : 1 }";
  static const char* missingFieldDDL =
    "{ type : \"UI.Toggle\", name : \"Test.InvalidMissing\", left : 0, bottom : 0, width : 96, height : 44 }";
  static const char* invalidButtonPropertyDDL =
    "{ type : \"UI.Button\", name : \"Test.InvalidButton\", left : 0, bottom : 0, width : 120, height : 40, text : \"Action\", backgroundColor : \"Test.Button.Normal\", pixelSize : \"20\" }";
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, rootDefinitionDDL), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, parentDefinitionDDL), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, childDefinitionDDL), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, labelDefinitionDDL), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, segmentedChoiceDefinitionDDL), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, sliderDefinitionDDL), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, toggleDefinitionDDL), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, buttonColorDDL), Arcadia_BooleanValue_False);
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
  assertRecordRejected(thread, context, definitions,
    "{ type : \"UI.Button\", name : \"Test.MissingBackground\", left : 0, bottom : 0, width : 120, height : 40, text : \"Missing\" }");
  assertRecordRejected(thread, context, definitions,
    "{ type : \"UI.Button\", name : \"Test.LegacyBackground\", left : 0, bottom : 0, width : 120, height : 40, text : \"Legacy\", backgroundColor : \"Test.Button.Normal\", backgroundRed : 1, backgroundGreen : 2, backgroundBlue : 3 }");

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
