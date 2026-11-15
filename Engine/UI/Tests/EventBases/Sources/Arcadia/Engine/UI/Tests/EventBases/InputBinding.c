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

#include "Arcadia/Engine/UI/Tests/EventBases/InputBinding.h"
#include "Arcadia/Engine/UI/Include.h"
#include "Arcadia/Testing/Include.h"

static Arcadia_Natural32Value notifications;

static void
onChanged
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getNatural8Value(thread, 0) == 2);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1,
    _Arcadia_Engine_UI_InputBindingService_getType(thread)) != NULL);
  notifications++;
}

static void
key
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* canvas,
    Arcadia_Engine_Input_KeyboardKeyAction action,
    Arcadia_Engine_Input_KeyboardKey code
  )
{
  Arcadia_Engine_Input_KeyboardKeyEvent* event = Arcadia_Engine_Input_KeyboardKeyEvent_create(thread, 1, action, code);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)event);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent(thread, (Arcadia_Object*)canvas, event));
  Arcadia_ValueStack_popValues(thread, 1);
}

static void
button
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* canvas,
    Arcadia_Engine_Input_MouseButtonAction action,
    Arcadia_Natural64Value code
  )
{
  Arcadia_Engine_Input_MouseButtonEvent* event = Arcadia_Engine_Input_MouseButtonEvent_create(thread, 1, action, code, 20, 20);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)event);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (Arcadia_Object*)canvas, event));
  Arcadia_ValueStack_popValues(thread, 1);
}

static void
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Engine* engine = Arcadia_Engine_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)engine);
  if (!engine->visualsNodeFactory)
    engine->visualsNodeFactory = (Arcadia_Engine_NodeFactory*)Arcadia_Engine_Visuals_NodeFactory_create(thread);
  Arcadia_Engine_UI_CanvasNode* canvas = Arcadia_Engine_UI_CanvasNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)canvas);
  Arcadia_Engine_UI_CanvasNode_setVisualsCanvasSize(thread, canvas, 300, 200);
  Arcadia_Engine_UI_InputBindingService* service = Arcadia_Engine_UI_InputBindingService_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)service);
  Arcadia_String* jump = Arcadia_String_createFromCxxString(thread, "Jump");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)jump);
  Arcadia_String* sprint = Arcadia_String_createFromCxxString(thread, "Sprint");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)sprint);
  Arcadia_Engine_UI_InputBindingService_addAction(thread, service, jump);
  Arcadia_Engine_UI_InputBindingService_addAction(thread, service, sprint);
  Arcadia_Slot* connection = Arcadia_Signal_connect(thread,
    Arcadia_Engine_UI_InputBindingService_getChangedSignal(thread, service), (Arcadia_Object*)service, &onChanged);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)connection);
  notifications = 0;
  Arcadia_Engine_UI_InputBinding invalid = {
    .device = Arcadia_Engine_UI_InputBindingDevice_Keyboard,
    .code = Arcadia_Engine_Input_KeyboardKey_Tabulator,
    .bound = Arcadia_BooleanValue_True,
  };
  Arcadia_Tests_assertTrue(thread,
    Arcadia_Engine_UI_InputBindingService_assign(thread, service, jump, invalid) == Arcadia_Engine_UI_InputBindingResult_Rejected);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_UI_InputBindingService_get(thread, service, jump).bound && !notifications);

  Arcadia_Engine_UI_InputBindingService_beginCapture(thread, service, canvas, jump);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_InputBindingService_getCaptureState(thread, service) == Arcadia_Engine_UI_InputCaptureState_Listening);
  key(thread, canvas, Arcadia_Engine_Input_KeyboardKeyAction_Pressed, Arcadia_Engine_Input_KeyboardKey_Tabulator);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_InputBindingService_getCaptureState(thread, service) == Arcadia_Engine_UI_InputCaptureState_Cancelled);
  key(thread, canvas, Arcadia_Engine_Input_KeyboardKeyAction_Released, Arcadia_Engine_Input_KeyboardKey_Tabulator);
  Arcadia_Tests_assertTrue(thread, !canvas->suppressCaptureKeyRelease && !canvas->captureOwner);

  Arcadia_Engine_UI_InputBindingService_beginCapture(thread, service, canvas, jump);
  key(thread, canvas, Arcadia_Engine_Input_KeyboardKeyAction_Pressed, Arcadia_Engine_Input_KeyboardKey_LeftShift);
  key(thread, canvas, Arcadia_Engine_Input_KeyboardKeyAction_Pressed, Arcadia_Engine_Input_KeyboardKey_K);
  Arcadia_Tests_assertTrue(thread,
    Arcadia_Engine_UI_InputBindingService_getCaptureState(thread, service) == Arcadia_Engine_UI_InputCaptureState_Committed &&
    Arcadia_Engine_UI_InputBindingService_get(thread, service, jump).code == Arcadia_Engine_Input_KeyboardKey_K &&
    Arcadia_Engine_UI_InputBindingService_get(thread, service, jump).modifiers == Arcadia_Engine_UI_InputBindingModifier_Shift &&
    !canvas->captureOwner && notifications == 1);
  key(thread, canvas, Arcadia_Engine_Input_KeyboardKeyAction_Released, Arcadia_Engine_Input_KeyboardKey_K);
  Arcadia_Tests_assertTrue(thread, !canvas->suppressCaptureKeyRelease);

  Arcadia_Engine_UI_InputBindingService_beginCapture(thread, service, canvas, sprint);
  key(thread, canvas, Arcadia_Engine_Input_KeyboardKeyAction_Pressed, Arcadia_Engine_Input_KeyboardKey_RightShift);
  key(thread, canvas, Arcadia_Engine_Input_KeyboardKeyAction_Pressed, Arcadia_Engine_Input_KeyboardKey_K);
  Arcadia_Tests_assertTrue(thread,
    Arcadia_Engine_UI_InputBindingService_getCaptureState(thread, service) == Arcadia_Engine_UI_InputCaptureState_Conflict &&
    Arcadia_Engine_UI_InputBindingService_getConflictingAction(thread, service) != NULL &&
    !Arcadia_Engine_UI_InputBindingService_get(thread, service, sprint).bound && notifications == 1);
  key(thread, canvas, Arcadia_Engine_Input_KeyboardKeyAction_Released, Arcadia_Engine_Input_KeyboardKey_K);
  key(thread, canvas, Arcadia_Engine_Input_KeyboardKeyAction_Pressed, Arcadia_Engine_Input_KeyboardKey_Escape);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_InputBindingService_getCaptureState(thread, service) ==
    Arcadia_Engine_UI_InputCaptureState_Cancelled);
  key(thread, canvas, Arcadia_Engine_Input_KeyboardKeyAction_Released, Arcadia_Engine_Input_KeyboardKey_Escape);

  Arcadia_Engine_UI_InputBindingService_beginCapture(thread, service, canvas, sprint);
  button(thread, canvas, Arcadia_Engine_Input_MouseButtonAction_Pressed, 0);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_InputBindingService_getCaptureState(thread, service) ==
    Arcadia_Engine_UI_InputCaptureState_Committed &&
    Arcadia_Engine_UI_InputBindingService_get(thread, service, sprint).device == Arcadia_Engine_UI_InputBindingDevice_MouseButton &&
    Arcadia_Engine_UI_InputBindingService_get(thread, service, sprint).code == 0 && notifications == 2);
  button(thread, canvas, Arcadia_Engine_Input_MouseButtonAction_Released, 0);
  Arcadia_Tests_assertTrue(thread, !canvas->suppressCaptureButtonRelease);

  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Engine_UI_InputBindingService_removeAction(thread, service, sprint);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_InputBindingService_get(thread, service, jump).bound && notifications == 2);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

int
Arcadia_Engine_UI_Tests_inputBinding
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&test) ? EXIT_SUCCESS : EXIT_FAILURE; }
