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

#include "Arcadia/Engine/UI/Tests/EventBases/Interfaces.h"
#include "Arcadia/Engine/UI/Include.h"
#include <stddef.h>
#include <math.h>

// Independent ABI models keep receiver/parameter types and slot order pinned.
typedef void
VoidOperation
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );
typedef Arcadia_BooleanValue
BoolOperation
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );
typedef Arcadia_BooleanValue
HitOperation
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  );
typedef void
PairOperation
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  );
typedef void
RenderOperation
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Visuals_EnterPassNode* pass
  );
typedef Arcadia_BooleanValue
KeyOperation
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  );
typedef Arcadia_BooleanValue
PointerOperation
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  );
typedef Arcadia_BooleanValue
ButtonOperation
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  );
typedef Arcadia_Natural64Value
CountOperation
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );
typedef Arcadia_Object*
ChildOperation
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Natural64Value index
  );
typedef void
ObjectOperation
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* child
  );
typedef Arcadia_Value
GetValueOperation
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );
typedef void
SetValueOperation
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Value value
  );
struct ActivatableABI { Arcadia_InterfaceDispatch _parent; VoidOperation* activate; };
struct HitTestableABI { Arcadia_InterfaceDispatch _parent; HitOperation* hitTest; };
struct FocusableABI { Arcadia_InterfaceDispatch _parent; VoidOperation* focus; VoidOperation* blur; BoolOperation* isFocused; };
struct ScrollableABI { Arcadia_InterfaceDispatch _parent; PairOperation* scrollBy; PairOperation* scrollTo; };
struct RenderableABI { Arcadia_InterfaceDispatch _parent; RenderOperation* render; };
struct KeyboardInputABI { Arcadia_InterfaceDispatch _parent; KeyOperation* handleKeyboardKeyEvent; };
struct PointerInputABI {
  Arcadia_InterfaceDispatch _parent;
  PointerOperation* handleMousePointerEvent;
  ButtonOperation* handleMouseButtonEvent;
};
struct ContainerABI {
  Arcadia_InterfaceDispatch _parent;
  CountOperation* getChildCount;
  ChildOperation* getChildAt;
  ObjectOperation* addChild;
  ObjectOperation* removeChild;
};
struct ValueControlABI {
  Arcadia_InterfaceDispatch _parent;
  GetValueOperation* getValue;
  SetValueOperation* setValue;
};
#define CHECK_SIZE(Name) \
  _Static_assert(sizeof(Arcadia_Engine_UI_##Name##Dispatch) == sizeof(struct Name##ABI), #Name " dispatch size"); \
  _Static_assert(_Alignof(Arcadia_Engine_UI_##Name##Dispatch) == _Alignof(struct Name##ABI), #Name " dispatch alignment");
#define CHECK_SLOT(Name, Slot, Type) \
  _Static_assert(offsetof(Arcadia_Engine_UI_##Name##Dispatch, Slot) == offsetof(struct Name##ABI, Slot), #Name "." #Slot " offset"); \
  _Static_assert(_Generic(((Arcadia_Engine_UI_##Name##Dispatch*)0)->Slot, Type*: 1, default: 0), #Name "." #Slot " slot type"); \
  _Static_assert(_Generic(&Arcadia_Engine_UI_##Name##_##Slot, Type*: 1, default: 0), #Name "." #Slot " public type");
CHECK_SIZE(Activatable)
CHECK_SIZE(HitTestable)
CHECK_SIZE(Focusable)
CHECK_SIZE(Scrollable)
CHECK_SIZE(Renderable)
CHECK_SIZE(KeyboardInput)
CHECK_SIZE(PointerInput)
CHECK_SIZE(Container)
CHECK_SIZE(ValueControl)
CHECK_SLOT(Activatable, activate, VoidOperation)
CHECK_SLOT(HitTestable, hitTest, HitOperation)
CHECK_SLOT(Focusable, focus, VoidOperation)
CHECK_SLOT(Focusable, blur, VoidOperation)
CHECK_SLOT(Focusable, isFocused, BoolOperation)
CHECK_SLOT(Scrollable, scrollBy, PairOperation)
CHECK_SLOT(Scrollable, scrollTo, PairOperation)
CHECK_SLOT(Renderable, render, RenderOperation)
CHECK_SLOT(KeyboardInput, handleKeyboardKeyEvent, KeyOperation)
CHECK_SLOT(PointerInput, handleMousePointerEvent, PointerOperation)
CHECK_SLOT(PointerInput, handleMouseButtonEvent, ButtonOperation)
CHECK_SLOT(Container, getChildCount, CountOperation)
CHECK_SLOT(Container, getChildAt, ChildOperation)
CHECK_SLOT(Container, addChild, ObjectOperation)
CHECK_SLOT(Container, removeChild, ObjectOperation)
CHECK_SLOT(ValueControl, getValue, GetValueOperation)
CHECK_SLOT(ValueControl, setValue, SetValueOperation)
#undef CHECK_SLOT
#undef CHECK_SIZE

enum {
  Activate, Hit, Focus, Blur, IsFocused, ScrollBy, ScrollTo, Render, Keyboard,
  Pointer, Button, ChildCount, ChildAt, AddChild, RemoveChild, GetValue, SetValue,
  OperationCount
};
typedef struct UICapabilitiesProbe UICapabilitiesProbe;
typedef struct UICapabilitiesProbeDispatch UICapabilitiesProbeDispatch;
typedef struct UIPartialProbe UIPartialProbe;
typedef struct UIPartialProbeDispatch UIPartialProbeDispatch;
typedef struct UIUnboundProbe UIUnboundProbe;
typedef struct UIUnboundProbeDispatch UIUnboundProbeDispatch;
struct UICapabilitiesProbe {
  Arcadia_Object _parent;
  Arcadia_String* label;
  Arcadia_SizeValue calls[OperationCount];
  Arcadia_Integer32Value x, y;
  Arcadia_BooleanValue focused, handled, specialFocus;
  Arcadia_Object* lastArgument; // Borrowed capture; callers own argument roots.
  Arcadia_Object* child;
  Arcadia_Value value;
  Arcadia_Natural64Value count, index;
};
struct UICapabilitiesProbeDispatch { Arcadia_ObjectDispatch _parent; };
struct UIPartialProbe { UICapabilitiesProbe _parent; };
struct UIPartialProbeDispatch { UICapabilitiesProbeDispatch _parent; };
struct UIUnboundProbe { Arcadia_Object _parent; };
struct UIUnboundProbeDispatch { Arcadia_ObjectDispatch _parent; };
Arcadia_declareObjectType(u8"Tests.UICapabilitiesProbe", UICapabilitiesProbe, u8"Arcadia.Object");
Arcadia_declareObjectType(u8"Tests.UIPartialProbe", UIPartialProbe, u8"Tests.UICapabilitiesProbe");
Arcadia_declareObjectType(u8"Tests.UIUnboundProbe", UIUnboundProbe, u8"Arcadia.Object");
static Arcadia_SizeValue expectedDepth;
static Arcadia_SizeValue destroyed;
static Arcadia_BooleanValue failCallbacks;

static Arcadia_Object*
create0
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  return _Arcadia_EndCreate0(thread, type, base);
}

static void
constructProbe
  (
    Arcadia_Thread* thread,
    UICapabilitiesProbe* self
  )
{
  Arcadia_EnterConstructor(UICapabilitiesProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->label = NULL; self->lastArgument = NULL;
  self->child = NULL;
  self->value = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
  self->count = self->index = 0;
  self->x = self->y = 0;
  self->focused = self->handled = self->specialFocus = false;
  for (size_t i = 0; i < OperationCount; ++i) self->calls[i] = 0;
  Arcadia_Object_setType(thread, (Arcadia_Object*)self, _type);
  self->label = Arcadia_String_createFromCxxString(thread, "UI interface payload");
  Arcadia_LeaveConstructor(UICapabilitiesProbe);
}

static void
constructPartial
  (
    Arcadia_Thread* thread,
    UIPartialProbe* self
  )
{
  Arcadia_EnterConstructor(UIPartialProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  Arcadia_LeaveConstructor(UIPartialProbe);
}

static void
constructUnbound
  (
    Arcadia_Thread* thread,
    UIUnboundProbe* self
  )
{
  Arcadia_EnterConstructor(UIUnboundProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  Arcadia_LeaveConstructor(UIUnboundProbe);
}

static void
visitProbe
  (
    Arcadia_Thread* thread,
    UICapabilitiesProbe* self
  )
{
  if (self->label) Arcadia_Object_visit(thread, (Arcadia_Object*)self->label);
  if (self->child) Arcadia_Object_visit(thread, self->child);
  Arcadia_Value_visit(thread, &self->value);
}

static void
destructProbe
  (
    Arcadia_Thread* thread,
    UICapabilitiesProbe* self
  )
{ destroyed++; }

static UICapabilitiesProbe*
record
  (
    Arcadia_Thread* thread,
    Arcadia_Object* receiver,
    unsigned operation
  )
{
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == expectedDepth);
  if (failCallbacks) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  UICapabilitiesProbe* self = (UICapabilitiesProbe*)receiver;
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, self->label, "UI interface payload", sizeof("UI interface payload") - 1));
  self->calls[operation]++;
  return self;
}

static void
activate
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{ record(thread, self, Activate); }

static Arcadia_BooleanValue
hitTest
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  )
{
  UICapabilitiesProbe* probe = record(thread, self, Hit);
  probe->x = x; probe->y = y;
  return x < y;
}

static void
focus
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{ record(thread, self, Focus)->focused = true; }

static void
specialFocus
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{ UICapabilitiesProbe* probe = record(thread, self, Focus); probe->focused = true; probe->specialFocus = true; }

static void
blur
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{ record(thread, self, Blur)->focused = false; }

static Arcadia_BooleanValue
isFocused
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{ return record(thread, self, IsFocused)->focused; }

static void
scrollBy
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  )
{ UICapabilitiesProbe* probe = record(thread, self, ScrollBy); probe->x = x; probe->y = y; }

static void
scrollTo
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  )
{ UICapabilitiesProbe* probe = record(thread, self, ScrollTo); probe->x = x; probe->y = y; }

static void
render
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Visuals_EnterPassNode* pass
  )
{
  UICapabilitiesProbe* probe = record(thread, self, Render);
  if (pass) Arcadia_Tests_assertTrue(thread, pass->worldToViewMatrix != NULL);
  probe->lastArgument = (Arcadia_Object*)pass;
}

static Arcadia_BooleanValue
keyboard
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  UICapabilitiesProbe* probe = record(thread, self, Keyboard);
  if (event) Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event) == Arcadia_Engine_Input_KeyboardKey_Return);
  probe->lastArgument = (Arcadia_Object*)event;
  return probe->handled;
}

static void
initActivatable
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ActivatableDispatch* self
  )
{ self->activate = &activate; }

static void
initHit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_HitTestableDispatch* self
  )
{ self->hitTest = &hitTest; }

static void
initFocus
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_FocusableDispatch* self
  )
{ self->focus = &focus; self->blur = &blur; self->isFocused = &isFocused; }

static void
initScroll
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollableDispatch* self
  )
{ self->scrollBy = &scrollBy; self->scrollTo = &scrollTo; }

static void
initRender
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_RenderableDispatch* self
  )
{ self->render = &render; }

static void
initKeyboard
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_KeyboardInputDispatch* self
  )
{ self->handleKeyboardKeyEvent = &keyboard; }

static void
initPartialFocus
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_FocusableDispatch* self
  )
{ self->focus = &specialFocus; }

static Arcadia_BooleanValue
pointerInput
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  )
{
  UICapabilitiesProbe* probe = record(thread, self, Pointer);
  probe->lastArgument = (void*)event;
  return probe->handled;
}

static Arcadia_BooleanValue
buttonInput
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{
  UICapabilitiesProbe* probe = record(thread, self, Button);
  probe->lastArgument = (void*)event;
  return probe->handled;
}

static Arcadia_Natural64Value
childCount
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  return record(thread, self, ChildCount)->count;
}

static Arcadia_Object*
childAt
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Natural64Value index
  )
{
  UICapabilitiesProbe* probe = record(thread, self, ChildAt);
  probe->index = index;
  return probe->child;
}

static void
addChild
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* child
  )
{
  record(thread, self, AddChild)->child = child;
}

static void
removeChild
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* child
  )
{
  UICapabilitiesProbe* probe = record(thread, self, RemoveChild);
  Arcadia_Tests_assertTrue(thread, probe->child == child);
  probe->child = NULL;
}

static Arcadia_Value
getValue
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  return record(thread, self, GetValue)->value;
}

static void
setValue
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Value value
  )
{
  record(thread, self, SetValue)->value = value;
}

static void
initPointer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PointerInputDispatch* self
  )
{
  self->handleMousePointerEvent = &pointerInput;
  self->handleMouseButtonEvent = &buttonInput;
}

static void
initContainer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ContainerDispatch* self
  )
{
  self->getChildCount = &childCount;
  self->getChildAt = &childAt;
  self->addChild = &addChild;
  self->removeChild = &removeChild;
}

static void
initValueControl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ValueControlDispatch* self
  )
{
  self->getValue = &getValue;
  self->setValue = &setValue;
}

static const Arcadia_ObjectType_Operations probeObjects = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructProbe,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitProbe,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructProbe,
};
static const Arcadia_Type_Operations probeOperations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &probeObjects };
static const Arcadia_ObjectType_Operations partialObjects = { Arcadia_ObjectType_Operations_Initializer, .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructPartial };
static const Arcadia_Type_Operations partialOperations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &partialObjects };
static const Arcadia_ObjectType_Operations unboundObjects = { Arcadia_ObjectType_Operations_Initializer, .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructUnbound };
static const Arcadia_Type_Operations unboundOperations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &unboundObjects };

#define SPEC(Name, Initialize) { .interfaceType = _Arcadia_Engine_UI_##Name##_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_##Name##Dispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)Initialize }
Arcadia_defineObjectTypeWithInterfaces(u8"Tests.UICapabilitiesProbe", UICapabilitiesProbe, u8"Arcadia.Object", Arcadia_Object, &probeOperations,
  SPEC(Activatable, initActivatable), SPEC(HitTestable, initHit), SPEC(Focusable, initFocus),
  SPEC(Scrollable, initScroll), SPEC(Renderable, initRender), SPEC(KeyboardInput, initKeyboard),
  SPEC(PointerInput, initPointer), SPEC(Container, initContainer), SPEC(ValueControl, initValueControl))
Arcadia_defineObjectTypeWithInterfaces(u8"Tests.UIPartialProbe", UIPartialProbe, u8"Tests.UICapabilitiesProbe", UICapabilitiesProbe, &partialOperations, SPEC(Focusable, initPartialFocus))
Arcadia_defineObjectTypeWithInterfaces(u8"Tests.UIUnboundProbe", UIUnboundProbe, u8"Arcadia.Object", Arcadia_Object, &unboundOperations,
  SPEC(Activatable, NULL), SPEC(HitTestable, NULL), SPEC(Focusable, NULL),
  SPEC(Scrollable, NULL), SPEC(Renderable, NULL), SPEC(KeyboardInput, NULL),
  SPEC(PointerInput, NULL), SPEC(Container, NULL), SPEC(ValueControl, NULL))
#undef SPEC

static Arcadia_BooleanValue
call
  (
    Arcadia_Thread* thread,
    unsigned operation,
    Arcadia_Object* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y,
    Arcadia_Engine_Visuals_EnterPassNode* pass,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  switch (operation) {
    case Activate: Arcadia_Engine_UI_Activatable_activate(thread, self); break;
    case Hit: return Arcadia_Engine_UI_HitTestable_hitTest(thread, self, x, y);
    case Focus: Arcadia_Engine_UI_Focusable_focus(thread, self); break;
    case Blur: Arcadia_Engine_UI_Focusable_blur(thread, self); break;
    case IsFocused: return Arcadia_Engine_UI_Focusable_isFocused(thread, self);
    case ScrollBy: Arcadia_Engine_UI_Scrollable_scrollBy(thread, self, x, y); break;
    case ScrollTo: Arcadia_Engine_UI_Scrollable_scrollTo(thread, self, x, y); break;
    case Render: Arcadia_Engine_UI_Renderable_render(thread, self, pass); break;
    case Keyboard: return Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent(thread, self, event);
    case Pointer: return Arcadia_Engine_UI_PointerInput_handleMousePointerEvent(thread, self, NULL);
    case Button: return Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, self, NULL);
    case ChildCount: Arcadia_Engine_UI_Container_getChildCount(thread, self); break;
    case ChildAt: Arcadia_Engine_UI_Container_getChildAt(thread, self, UINT64_MAX); break;
    case AddChild: Arcadia_Engine_UI_Container_addChild(thread, self, NULL); break;
    case RemoveChild: Arcadia_Engine_UI_Container_removeChild(thread, self, NULL); break;
    case GetValue: Arcadia_Engine_UI_ValueControl_getValue(thread, self); break;
    case SetValue: Arcadia_Engine_UI_ValueControl_setValue(thread, self, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void)); break;
  }
  return false;
}

static void
testDispatch
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (size_t i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 0);
  UICapabilitiesProbe* self = (void*)create0(thread, _UICapabilitiesProbe_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_Engine_Visuals_EnterPassNode* pass = Arcadia_Engine_Visuals_EnterPassNode_create(thread, NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, pass);
  Arcadia_Engine_Input_KeyboardKeyEvent* event = Arcadia_Engine_Input_KeyboardKeyEvent_create(thread, UINT64_MAX, Arcadia_Engine_Input_KeyboardKeyAction_Released, Arcadia_Engine_Input_KeyboardKey_Return);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, event);
  expectedDepth = Arcadia_ValueStack_getSize(thread);
  for (unsigned operation = 0; operation < OperationCount; ++operation) {
    call(thread, operation, (void*)self, -17, 23, pass, event);
    Arcadia_Tests_assertTrue(thread, self->calls[operation] == 1 && Arcadia_ValueStack_getSize(thread) == expectedDepth);
    if (operation == Keyboard) Arcadia_Tests_assertTrue(thread, self->lastArgument == (Arcadia_Object*)event);
  }
  Arcadia_Integer32Value pairs[][2] = { { INT32_MIN, INT32_MAX }, { INT32_MAX, INT32_MIN }, { 0, 0 }, { -3, 17 } };
  for (size_t i = 0; i < 4; ++i) {
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_HitTestable_hitTest(thread, (void*)self, pairs[i][0], pairs[i][1]) == (pairs[i][0] < pairs[i][1]));
    Arcadia_Tests_assertTrue(thread, self->x == pairs[i][0] && self->y == pairs[i][1]);
    Arcadia_Engine_UI_Scrollable_scrollBy(thread, (void*)self, pairs[i][0], pairs[i][1]);
    Arcadia_Tests_assertTrue(thread, self->x == pairs[i][0] && self->y == pairs[i][1]);
    Arcadia_Engine_UI_Scrollable_scrollTo(thread, (void*)self, pairs[i][1], pairs[i][0]);
    Arcadia_Tests_assertTrue(thread, self->x == pairs[i][1] && self->y == pairs[i][0]);
  }
  Arcadia_Engine_UI_Focusable_focus(thread, (void*)self);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Focusable_isFocused(thread, (void*)self));
  Arcadia_Engine_UI_Focusable_blur(thread, (void*)self);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_UI_Focusable_isFocused(thread, (void*)self));
  for (unsigned handled = 0; handled < 2; ++handled) {
    self->handled = handled;
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent(thread, (void*)self, event) == handled);
  }
  Arcadia_Engine_UI_Renderable_render(thread, (void*)self, NULL);
  Arcadia_Tests_assertTrue(thread, self->lastArgument == NULL);
  Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent(thread, (void*)self, NULL);
  UIPartialProbe* partial = (void*)create0(thread, _UIPartialProbe_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, partial);
  expectedDepth = Arcadia_ValueStack_getSize(thread);
  Arcadia_Engine_UI_Focusable_focus(thread, (void*)partial);
  Arcadia_Tests_assertTrue(thread, ((UICapabilitiesProbe*)partial)->specialFocus && Arcadia_Engine_UI_Focusable_isFocused(thread, (void*)partial));
  Arcadia_Engine_UI_Focusable_blur(thread, (void*)partial);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_UI_Focusable_isFocused(thread, (void*)partial));
  UIUnboundProbe* unbound = (void*)create0(thread, _UIUnboundProbe_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, unbound);
  Arcadia_String* missing = Arcadia_String_createFromCxxString(thread, "no UI interfaces");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, missing);
  expectedDepth = Arcadia_ValueStack_getSize(thread);
  for (unsigned operation = 0; operation < OperationCount; ++operation) {
    for (unsigned failure = 0; failure < 3; ++failure) {
      failCallbacks = failure == 0;
      Arcadia_JumpTarget jump;
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (Arcadia_JumpTarget_save(&jump)) {
        call(thread, operation, failure == 0 ? (void*)self : failure == 1 ? (void*)unbound : (void*)missing, -3, 17, pass, event);
        Arcadia_Thread_popJumpTarget(thread); Arcadia_Tests_assertTrue(thread, false);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == (failure == 0 ? Arcadia_Status_OperationFailed : Arcadia_Status_NotImplemented));
        Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == expectedDepth);
        Arcadia_Thread_setStatus(thread, 0);
      }
      failCallbacks = false;
    }
  }
  Arcadia_ValueStack_popValues(thread, 6);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed == 2 && Arcadia_ValueStack_getSize(thread) == base + 300);
  Arcadia_ValueStack_popValues(thread, 300);
}

static Arcadia_SizeValue activations;

static void
testPointerContainerValue
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  UICapabilitiesProbe* self = (void*)create0(thread, _UICapabilitiesProbe_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_Engine_Input_MousePointerEvent* pointer = Arcadia_Engine_Input_MousePointerEvent_create(thread, UINT64_MAX, Arcadia_Engine_Input_MousePointerAction_Moved, INT32_MIN, INT32_MAX);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, pointer);
  Arcadia_Engine_Input_MouseButtonEvent* button = Arcadia_Engine_Input_MouseButtonEvent_create(thread, UINT64_MAX, Arcadia_Engine_Input_MouseButtonAction_Released, UINT64_MAX, INT32_MAX, INT32_MIN);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, button);
  expectedDepth = Arcadia_ValueStack_getSize(thread);
  for (unsigned handled = 0; handled < 2; ++handled) {
    self->handled = handled;
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMousePointerEvent(thread, (void*)self, pointer) == handled);
    Arcadia_Tests_assertTrue(thread, self->lastArgument == (Arcadia_Object*)pointer && pointer->x == INT32_MIN && pointer->y == INT32_MAX);
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (void*)self, button) == handled);
    Arcadia_Tests_assertTrue(thread, self->lastArgument == (Arcadia_Object*)button && button->button == UINT64_MAX);
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMousePointerEvent(thread, (void*)self, NULL) == handled);
    Arcadia_Tests_assertTrue(thread, self->lastArgument == NULL);
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (void*)self, NULL) == handled);
    Arcadia_Tests_assertTrue(thread, self->lastArgument == NULL);
  }
  Arcadia_Natural64Value indices[] = { 0, 1, UINT32_MAX, (Arcadia_Natural64Value)UINT32_MAX + 1, UINT64_MAX };
  for (unsigned i = 0; i < sizeof(indices) / sizeof(indices[0]); ++i) {
    self->count = indices[i];
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildCount(thread, (void*)self) == indices[i]);
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildAt(thread, (void*)self, indices[i]) == NULL && self->index == indices[i]);
  }
  Arcadia_String* child = Arcadia_String_createFromCxxString(thread, "managed child");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, child);
  expectedDepth = Arcadia_ValueStack_getSize(thread);
  Arcadia_Engine_UI_Container_addChild(thread, (void*)self, (void*)child);
  Arcadia_ValueStack_popValues(thread, 1);
  expectedDepth = Arcadia_ValueStack_getSize(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildAt(thread, (void*)self, UINT64_MAX) == (Arcadia_Object*)child);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, child, "managed child", sizeof("managed child") - 1));
  Arcadia_Engine_UI_Container_removeChild(thread, (void*)self, (void*)child);
  Arcadia_Engine_UI_Container_addChild(thread, (void*)self, NULL);
  Arcadia_Engine_UI_Container_removeChild(thread, (void*)self, NULL);
  Arcadia_Value values[] = {
    Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void),
    Arcadia_Value_makeBooleanValue(false), Arcadia_Value_makeBooleanValue(true),
    Arcadia_Value_makeInteger8Value(INT8_MIN), Arcadia_Value_makeInteger16Value(INT16_MIN),
    Arcadia_Value_makeInteger32Value(INT32_MIN), Arcadia_Value_makeInteger64Value(INT64_MIN),
    Arcadia_Value_makeNatural8Value(UINT8_MAX), Arcadia_Value_makeNatural16Value(UINT16_MAX),
    Arcadia_Value_makeNatural32Value(UINT32_MAX), Arcadia_Value_makeNatural64Value(UINT64_MAX),
    Arcadia_Value_makeSizeValue(SIZE_MAX), Arcadia_Value_makeReal32Value(-17.5f), Arcadia_Value_makeReal64Value(23.5),
    Arcadia_Value_makeTypeValue(_UICapabilitiesProbe_getType(thread)),
  };
  for (unsigned i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
    Arcadia_ValueStack_pushValue(thread, &values[i]);
    expectedDepth = Arcadia_ValueStack_getSize(thread);
    Arcadia_Engine_UI_ValueControl_setValue(thread, (void*)self, values[i]);
    Arcadia_Value actual = Arcadia_Engine_UI_ValueControl_getValue(thread, (void*)self);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_getTag(&actual) == Arcadia_Value_getTag(&values[i]));
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isEqualTo(thread, &actual, &values[i]));
    Arcadia_ValueStack_popValues(thread, 1);
  }
  Arcadia_Real64Value special[] = { -0., INFINITY, -INFINITY, NAN };
  for (unsigned i = 0; i < sizeof(special) / sizeof(special[0]); ++i) {
    expectedDepth = Arcadia_ValueStack_getSize(thread);
    Arcadia_Engine_UI_ValueControl_setValue(thread, (void*)self, Arcadia_Value_makeReal64Value(special[i]));
    Arcadia_Value actual = Arcadia_Engine_UI_ValueControl_getValue(thread, (void*)self);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isReal64Value(&actual));
    Arcadia_Real64Value real = Arcadia_Value_getReal64Value(&actual);
    Arcadia_Tests_assertTrue(thread, isnan(special[i]) ? isnan(real) : real == special[i] && !!signbit(real) == !!signbit(special[i]));
  }
  // The probe's managed Value field is the sole payload root after setValue.
  Arcadia_String* payload = Arcadia_String_createFromCxxString(thread, "managed value");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, payload);
  expectedDepth = Arcadia_ValueStack_getSize(thread);
  Arcadia_Engine_UI_ValueControl_setValue(thread, (void*)self, Arcadia_Value_makeObjectReferenceValue(payload));
  Arcadia_ValueStack_popValues(thread, 1);
  expectedDepth = Arcadia_ValueStack_getSize(thread);
  Arcadia_Value actual = Arcadia_Engine_UI_ValueControl_getValue(thread, (void*)self);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_getObjectReferenceValue(&actual) == (Arcadia_Object*)payload);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, payload, "managed value", sizeof("managed value") - 1));
  Arcadia_Tests_assertTrue(thread, expectedDepth == base + 3);
  Arcadia_ValueStack_popValues(thread, 3);
}

static void
onActivate
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Natural8Value count = Arcadia_ValueStack_getNatural8Value(thread, 0);
  Arcadia_Tests_assertTrue(thread, count == 2);
  activations++;
  // Signal_emit, unlike an ordinary ForeignProcedure caller, owns cleanup.
}

static void
testWidgets
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Engine* engine = Arcadia_Engine_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, engine);
  engine->visualsNodeFactory = (Arcadia_Engine_NodeFactory*)Arcadia_Engine_Visuals_NodeFactory_create(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, definitions);
  Arcadia_ADL_UI_ButtonDefinition* source = Arcadia_ADL_UI_ButtonDefinition_create(thread, definitions,
    Arcadia_String_createFromCxxString(thread, "Tests.InterfaceButton"), 10, 20, 100, 40,
    Arcadia_String_createFromCxxString(thread, ""), Arcadia_String_createFromCxxString(thread, "Left"), Arcadia_String_createFromCxxString(thread, "Bottom"),
    NULL, NULL, NULL, NULL, NULL, 30, 40, 50, 16, 0, 0, 0, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, source);
  Arcadia_Engine_UI_ButtonNode* button = Arcadia_Engine_UI_ButtonNode_create(thread, NULL, source);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, button);
  Arcadia_Signal_connect(thread, Arcadia_Engine_UI_ButtonNode_getActivatedSignal(thread, button), (Arcadia_Object*)button, &onActivate);
  Arcadia_Engine_UI_Activatable_activate(thread, (void*)button);
  Arcadia_Tests_assertTrue(thread, activations == 1);
  Arcadia_Engine_UI_ButtonNode_setEnabled(thread, button, false);
  Arcadia_Engine_UI_Activatable_activate(thread, (void*)button);
  Arcadia_Tests_assertTrue(thread, activations == 1);
  Arcadia_Engine_UI_ButtonNode_setEnabled(thread, button, true);
  Arcadia_Engine_UI_Focusable_focus(thread, (void*)button);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Focusable_isFocused(thread, (void*)button));
  Arcadia_Engine_UI_Focusable_blur(thread, (void*)button);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_UI_Focusable_isFocused(thread, (void*)button));
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_HitTestable_hitTest(thread, (void*)button, 10, 20));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_UI_HitTestable_hitTest(thread, (void*)button, 110, 60));
  Arcadia_Engine_Input_KeyboardKeyEvent* event = Arcadia_Engine_Input_KeyboardKeyEvent_create(thread, 0, Arcadia_Engine_Input_KeyboardKeyAction_Released, Arcadia_Engine_Input_KeyboardKey_Return);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, event);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent(thread, (void*)button, event) && activations == 2);
  event->action = Arcadia_Engine_Input_KeyboardKeyAction_Pressed;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent(thread, (void*)button, event) && activations == 2);
  Arcadia_Engine_Visuals_EnterPassNode* pass = Arcadia_Engine_Visuals_EnterPassNode_create(thread, NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, pass);
  Arcadia_Engine_Visuals_ViewportNode* viewport = Arcadia_Engine_Visuals_ViewportNode_create(thread, NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, viewport);
  Arcadia_Engine_Visuals_EnterPassNode_setViewportNode(thread, pass, viewport);
  Arcadia_Engine_UI_Renderable_render(thread, (void*)button, pass);
  Arcadia_Engine_UI_ScrollPanelNode* panel = Arcadia_Engine_UI_ScrollPanelNode_create(thread, 0, 0, 100, 60);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, panel);
  Arcadia_Engine_UI_ScrollPanelNode_setContentSize(thread, panel, 500, 400);
  Arcadia_Engine_UI_Scrollable_scrollTo(thread, (void*)panel, 37, 53);
  Arcadia_Integer32Value x, y;
  Arcadia_Engine_UI_ScrollPanelNode_getScrollOffset(thread, panel, &x, &y);
  Arcadia_Tests_assertTrue(thread, x == 37 && y == 53);
  Arcadia_Engine_UI_Scrollable_scrollBy(thread, (void*)panel, 11, -7);
  Arcadia_Engine_UI_ScrollPanelNode_getScrollOffset(thread, panel, &x, &y);
  Arcadia_Tests_assertTrue(thread, x == 48 && y == 46);
  Arcadia_Engine_Input_MousePointerEvent* pointer = Arcadia_Engine_Input_MousePointerEvent_create(thread, 0, Arcadia_Engine_Input_MousePointerAction_Entered, 11, 22);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, pointer);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMousePointerEvent(thread, (void*)button, pointer) && button->hovered);
  Arcadia_Engine_UI_ButtonNode_setEnabled(thread, button, false);
  pointer->action = Arcadia_Engine_Input_MousePointerAction_Exited;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_UI_PointerInput_handleMousePointerEvent(thread, (void*)button, pointer) && !button->hovered);
  Arcadia_Engine_UI_ButtonNode_setEnabled(thread, button, true);
  Arcadia_Engine_Input_MouseButtonEvent* click = Arcadia_Engine_Input_MouseButtonEvent_create(thread, 0, Arcadia_Engine_Input_MouseButtonAction_Pressed, 0, 11, 22);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, click);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (void*)button, click) && button->pressed);
  click->action = Arcadia_Engine_Input_MouseButtonAction_Released;
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (void*)button, click) && !button->pressed);
  click->button = 1;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (void*)button, click));
  Arcadia_Engine_UI_Container_addChild(thread, (void*)panel, (void*)button);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildCount(thread, (void*)panel) == 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildAt(thread, (void*)panel, 0) == (Arcadia_Object*)button);
  Arcadia_Engine_UI_Container_removeChild(thread, (void*)panel, (void*)button);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildCount(thread, (void*)panel) == 0);
  Arcadia_Engine_UI_ToggleNode* toggle = Arcadia_Engine_UI_ToggleNode_create(thread, NULL, 0, 0, 80, 20, false);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, toggle);
  for (unsigned checked = 0; checked < 2; ++checked) {
    Arcadia_Engine_UI_ValueControl_setValue(thread, (void*)toggle, Arcadia_Value_makeBooleanValue(checked));
    Arcadia_Value value = Arcadia_Engine_UI_ValueControl_getValue(thread, (void*)toggle);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isBooleanValue(&value) && Arcadia_Value_getBooleanValue(&value) == checked);
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_ToggleNode_getChecked(thread, toggle) == checked);
  }
  Arcadia_JumpTarget invalidValue;
  Arcadia_Thread_pushJumpTarget(thread, &invalidValue);
  if (Arcadia_JumpTarget_save(&invalidValue)) {
    Arcadia_Engine_UI_ValueControl_setValue(thread, (void*)toggle, Arcadia_Value_makeInteger32Value(1));
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_setStatus(thread, 0);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 11);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Arcadia_Engine_UI_Tests_EventBases_interfaces
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(testDispatch)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(testPointerContainerValue)) return EXIT_FAILURE;
  return Arcadia_Tests_safeExecute(testWidgets) ? EXIT_SUCCESS : EXIT_FAILURE;
}
