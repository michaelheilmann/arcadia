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

#include "Arcadia/Engine/UI/Tests/EventBases/BindingRow.h"
#include "Arcadia/Engine/UI/Include.h"
#include "Arcadia/Testing/Include.h"

static Arcadia_Engine_UI_ButtonNode*
makeButton(Arcadia_Thread* thread, Arcadia_ADL_Definitions* definitions, char const* name)
{
  Arcadia_ADL_UI_ButtonDefinition* source = Arcadia_ADL_UI_ButtonDefinition_create(thread, definitions,
    Arcadia_String_createFromCxxString(thread, name), 0, 0, 90, 30,
    Arcadia_String_createFromCxxString(thread, name), Arcadia_String_createFromCxxString(thread, u8"Center"),
    Arcadia_String_createFromCxxString(thread, u8"Center"),
    Arcadia_String_createFromCxxString(thread, u8"Tests.BindingRow.Color"), NULL, NULL, NULL, NULL,
    16, 0, 0, 0, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)source);
  Arcadia_Engine_UI_ButtonNode* button = Arcadia_Engine_UI_ButtonNode_create(thread, NULL, source);
  Arcadia_ValueStack_popValues(thread, 1);
  return button;
}
static Arcadia_Engine_UI_LabelNode*
makeLabel(Arcadia_Thread* thread)
{
  return Arcadia_Engine_UI_LabelNode_create(thread, NULL,
    Arcadia_String_createFromCxxString(thread, u8"Listening: Esc to cancel"),
    0, 0, 260, 30, Arcadia_String_createFromCxxString(thread, u8"Left"),
    Arcadia_String_createFromCxxString(thread, u8"Center"), 220, 228, 240, 255);
}
static void
test(Arcadia_Thread* thread)
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Engine* engine = Arcadia_Engine_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)engine);
  if (!engine->visualsNodeFactory)
    engine->visualsNodeFactory = (Arcadia_Engine_NodeFactory*)Arcadia_Engine_Visuals_NodeFactory_create(thread);
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ADL_Context_readFromString(thread, context, definitions,
    Arcadia_String_createFromCxxString(thread, u8"{ type: \"Color\", name: \"Tests.BindingRow.Color\", red: 50, green: 70, blue: 90 }"), false);
  Arcadia_Engine_UI_CanvasNode* canvas = Arcadia_Engine_UI_CanvasNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)canvas);
  Arcadia_Engine_UI_InputBindingService* service = Arcadia_Engine_UI_InputBindingService_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)service);
  Arcadia_String* jump = Arcadia_String_createFromCxxString(thread, u8"Jump");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)jump);
  Arcadia_Engine_UI_InputBindingService_addAction(thread, service, jump);
  Arcadia_Engine_UI_ButtonNode* edit = makeButton(thread, definitions, u8"Tests.Edit");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)edit);
  Arcadia_Engine_UI_ButtonNode* reset = makeButton(thread, definitions, u8"Tests.Reset");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)reset);
  Arcadia_Engine_UI_LabelNode* value = makeLabel(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)value);
  Arcadia_Engine_UI_LabelNode* status = makeLabel(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)status);
  Arcadia_Engine_UI_InputBinding original = {
    .device = Arcadia_Engine_UI_InputBindingDevice_Keyboard,
    .code = Arcadia_Engine_Input_KeyboardKey_Space, .bound = Arcadia_BooleanValue_True,
  };
  Arcadia_Engine_UI_InputBindingService_assign(thread, service, jump, original);
  Arcadia_Engine_UI_BindingRowController* row = Arcadia_Engine_UI_BindingRowController_create(thread,
    service, canvas, jump, edit, reset, value, status, original);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)row);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, value->text, "Space", 5));
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_InputBindingService_get(thread, service, jump).bound);
  Arcadia_Engine_UI_InputBinding alt = original;
  alt.code = Arcadia_Engine_Input_KeyboardKey_K;
  alt.modifiers = Arcadia_Engine_UI_InputBindingModifier_Control;
  Arcadia_Engine_UI_InputBindingService_assign(thread, service, jump, alt);
  Arcadia_Engine_UI_BindingRowController_refresh(thread, row);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, value->text, "Ctrl+K", 6));
  Arcadia_Engine_UI_BindingRowController_reset(thread, row);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_InputBindingService_get(thread, service, jump).code == Arcadia_Engine_Input_KeyboardKey_Space);
  Arcadia_Engine_UI_BindingRowController_setEnabled(thread, row, Arcadia_BooleanValue_False);
  Arcadia_Tests_assertTrue(thread, !edit->enabled && !reset->enabled &&
    Arcadia_String_isEqualTo_pn(thread, status->text, "Disabled", 8));
  Arcadia_Engine_UI_BindingRowController_setEnabled(thread, row, Arcadia_BooleanValue_True);
  Arcadia_Engine_UI_StatusLabel_set(thread, status, Arcadia_Engine_UI_StatusSeverity_Warning,
    Arcadia_String_createFromCxxString(thread, u8"Conflict: choose another"));
  Arcadia_Tests_assertTrue(thread, status->red == 255 && status->green == 210 &&
    Arcadia_String_isEqualTo_pn(thread, status->text, "Conflict: choose another", 24));
  // Initial text reserves only enough geometry for "Listening: Esc to cancel".
  // A longer warning must grow its mesh, not clip to the original capacity.
  Arcadia_Engine_UI_StatusLabel_set(thread, status, Arcadia_Engine_UI_StatusSeverity_Warning,
    Arcadia_String_createFromCxxString(thread, u8"Warning: the selected resolution is unavailable for this display."));
  Arcadia_Tests_assertTrue(thread, status->numberOfGlyphs == 65 &&
    Arcadia_Media_VertexBuffer_getNumberOfVertices(thread,
      Arcadia_Engine_Visuals_MeshNode_getVertexBuffer(thread, status->modelNode->mesh)) >= 390);
  Arcadia_Engine_UI_LabelNode_setText(thread, status, Arcadia_String_createFromCxxString(thread, u8"OK"));
  Arcadia_Tests_assertTrue(thread, status->numberOfGlyphs == 2);
  Arcadia_Engine_UI_StatusLabel_set(thread, status, Arcadia_Engine_UI_StatusSeverity_Warning,
    Arcadia_String_createFromCxxString(thread, u8"Warning: the selected resolution is unavailable for this display."));
  Arcadia_Tests_assertTrue(thread, status->numberOfGlyphs == 65);
  Arcadia_Engine_UI_StatusLabel_set(thread, status, Arcadia_Engine_UI_StatusSeverity_Informational, NULL);
  Arcadia_Tests_assertTrue(thread, status->alpha == 0);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}
int
Arcadia_Engine_UI_Tests_bindingRow
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&test) ? EXIT_SUCCESS : EXIT_FAILURE; }
