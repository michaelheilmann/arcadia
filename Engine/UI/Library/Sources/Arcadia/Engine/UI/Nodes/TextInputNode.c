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

#define ARCADIA_ENGINE_UI_PRIVATE (1)
#include "Arcadia/Engine/UI/Nodes/TextInputNode.h"
#include "Arcadia/Engine/UI/Include.h"

static void
constructImpl(Arcadia_Thread* thread, Arcadia_Engine_UI_TextInputNode* self)
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_TextInputNode);
  if (_numberOfArguments != 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->editor = NULL;
  self->enabled = true;
  self->readOnly = false;
  self->focused = false;
  self->shiftSides = 0;
  self->label = NULL;
  self->backendContext = NULL;
  self->scrollX = 0.f;
  self->dragging = false;
  Arcadia_Engine_UI_WidgetNode_setInitialBounds(thread, (Arcadia_Engine_UI_WidgetNode*)self, 0, 0, 160, 32);
  self->editor = Arcadia_Engine_UI_TextEditState_create(thread);
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_TextInputNode);
}

static void
visitImpl(Arcadia_Thread* thread, Arcadia_Engine_UI_TextInputNode* self)
{
  if (self->editor) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->editor);
  }
  if (self->label) Arcadia_Object_visit(thread, (Arcadia_Object*)self->label);
  if (self->backendContext) Arcadia_Object_visit(thread, (Arcadia_Object*)self->backendContext);
}

static void
focusImpl(Arcadia_Thread* thread, Arcadia_Object* object)
{
  Arcadia_Engine_UI_TextInputNode* self = (Arcadia_Engine_UI_TextInputNode*)object;
  self->focused = self->enabled;
}

static void
blurImpl(Arcadia_Thread* thread, Arcadia_Object* object)
{
  Arcadia_Engine_UI_TextInputNode* self = (Arcadia_Engine_UI_TextInputNode*)object;
  self->focused = false;
  self->shiftSides = 0;
  self->dragging = false;
}

static Arcadia_BooleanValue
isFocusedImpl(Arcadia_Thread* thread, Arcadia_Object* object)
{
  return ((Arcadia_Engine_UI_TextInputNode*)object)->focused;
}

static Arcadia_BooleanValue
hitTestImpl(Arcadia_Thread* thread, Arcadia_Object* object, Arcadia_Integer32Value x, Arcadia_Integer32Value y)
{
  Arcadia_Engine_UI_TextInputNode* self = (Arcadia_Engine_UI_TextInputNode*)object;
  Arcadia_Engine_UI_WidgetNode* widget = (Arcadia_Engine_UI_WidgetNode*)self;
  return self->enabled && x >= widget->left && y >= widget->bottom &&
         (Arcadia_Integer64Value)x < (Arcadia_Integer64Value)widget->left + widget->width &&
         (Arcadia_Integer64Value)y < (Arcadia_Integer64Value)widget->bottom + widget->height;
}

static Arcadia_BooleanValue
keyImpl(Arcadia_Thread* thread, Arcadia_Object* object, Arcadia_Engine_Input_KeyboardKeyEvent* event)
{
  Arcadia_Engine_UI_TextInputNode* self = (Arcadia_Engine_UI_TextInputNode*)object;
  if (!self->enabled || !self->focused) {
    return false;
  }
  Arcadia_Engine_Input_KeyboardKey key = Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event);
  Arcadia_BooleanValue pressed = Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) == Arcadia_Engine_Input_KeyboardKeyAction_Pressed;
  if (key == Arcadia_Engine_Input_KeyboardKey_LeftShift || key == Arcadia_Engine_Input_KeyboardKey_RightShift) {
    Arcadia_Natural8Value side = key == Arcadia_Engine_Input_KeyboardKey_LeftShift ? 1 : 2;
    if (pressed) {
      self->shiftSides |= side;
    } else {
      self->shiftSides &= ~side;
    }
    return true;
  }
  Arcadia_BooleanValue extend = self->shiftSides != 0;
  switch (key) {
    case Arcadia_Engine_Input_KeyboardKey_Left:
      if (pressed) Arcadia_Engine_UI_TextEditState_moveLeft(thread, self->editor, extend);
      return true;
    case Arcadia_Engine_Input_KeyboardKey_Right:
      if (pressed) Arcadia_Engine_UI_TextEditState_moveRight(thread, self->editor, extend);
      return true;
    case Arcadia_Engine_Input_KeyboardKey_Home:
      if (pressed) Arcadia_Engine_UI_TextEditState_moveHome(thread, self->editor, extend);
      return true;
    case Arcadia_Engine_Input_KeyboardKey_End:
      if (pressed) Arcadia_Engine_UI_TextEditState_moveEnd(thread, self->editor, extend);
      return true;
    case Arcadia_Engine_Input_KeyboardKey_Backspace:
      if (pressed && !self->readOnly) Arcadia_Engine_UI_TextEditState_backspace(thread, self->editor);
      return true;
    case Arcadia_Engine_Input_KeyboardKey_Delete:
      if (pressed && !self->readOnly) Arcadia_Engine_UI_TextEditState_deleteForward(thread, self->editor);
      return true;
    case Arcadia_Engine_Input_KeyboardKey_Return:
      if (pressed && !self->readOnly) Arcadia_Engine_UI_TextEditState_commit(thread, self->editor);
      return true;
    case Arcadia_Engine_Input_KeyboardKey_Escape:
      if (pressed && !self->readOnly) Arcadia_Engine_UI_TextEditState_cancel(thread, self->editor);
      return true;
    default:
      return false;
  }
}

static void
initFocus(Arcadia_Thread* thread, Arcadia_Engine_UI_FocusableDispatch* self)
{
  self->focus = &focusImpl;
  self->blur = &blurImpl;
  self->isFocused = &isFocusedImpl;
}

static void
initHit(Arcadia_Thread* thread, Arcadia_Engine_UI_HitTestableDispatch* self)
{
  self->hitTest = &hitTestImpl;
}

static void
initKeyboard(Arcadia_Thread* thread, Arcadia_Engine_UI_KeyboardInputDispatch* self)
{
  self->handleKeyboardKeyEvent = &keyImpl;
}


/* The label owns the atlas and model. The input supplies its own clipped
 * single-line geometry, with a pen independent of the clipping rectangle. */
static void
syncLabel(Arcadia_Thread* thread, Arcadia_Engine_UI_TextInputNode* self)
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    if (!self->label || Arcadia_StringBuilder_compareTo(thread, self->editor->draft,
        Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)self->label->text)) != 0) {
      Arcadia_String* text = Arcadia_Engine_UI_TextEditState_getText(thread, self->editor);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)text);
      if (!self->label) {
        Arcadia_String* left = Arcadia_String_createFromCxxString(thread, "Left");
        Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)left);
        Arcadia_String* center = Arcadia_String_createFromCxxString(thread, "Center");
        Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)center);
        self->label = Arcadia_Engine_UI_LabelNode_create(thread, self->backendContext, text,
          0, 0, 160, 32, left, center, 255, 255, 255, 255);
      } else {
        Arcadia_Engine_UI_LabelNode_setText(thread, self->label, text);
      }
    }
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
}

static Arcadia_Real32Value
advanceTo(Arcadia_Engine_UI_TextInputNode* self, Arcadia_SizeValue position)
{
  Arcadia_Real32Value x = 0.f;
  if (self->label) {
    for (Arcadia_SizeValue i = 0; i < position && i < self->label->numberOfGlyphs; ++i) {
      x += self->label->glyphs[i].glyphInformation.advanceX;
    }
  }
  return x;
}

static void
updateScroll(Arcadia_Thread* thread, Arcadia_Engine_UI_TextInputNode* self)
{
  Arcadia_Real32Value width = (Arcadia_Real32Value)self->parent.width - 8.f;
  if (width < 1.f) width = 1.f;
  Arcadia_Real32Value caret = advanceTo(self, Arcadia_Engine_UI_TextEditState_getCaret(thread, self->editor));
  Arcadia_Real32Value total = advanceTo(self, self->label->numberOfGlyphs);
  if (caret < self->scrollX) self->scrollX = caret;
  if (caret > self->scrollX + width - 1.f) self->scrollX = caret - width + 1.f;
  if (self->scrollX > total - width + 1.f) self->scrollX = total - width + 1.f;
  if (self->scrollX < 0.f) self->scrollX = 0.f;
}

static void
renderImpl(Arcadia_Thread* thread, Arcadia_Engine_UI_WidgetNode* widget,
           Arcadia_Engine_Visuals_EnterPassNode* pass)
{
  Arcadia_Engine_UI_TextInputNode* self = (Arcadia_Engine_UI_TextInputNode*)widget;
  if (widget->width <= 8 || widget->height <= 4) return;
  syncLabel(thread, self);
  updateScroll(thread, self);
  Arcadia_Engine_UI_LabelNode* label = self->label;
  if (!label->modelNode || !label->numberOfGlyphs) return;
  Arcadia_Media_VertexBuffer* buffer = Arcadia_Engine_Visuals_MeshNode_getVertexBuffer(thread, label->modelNode->mesh);
  Arcadia_Real32Value* vertices = (Arcadia_Real32Value*)Arcadia_ByteArray_getMutableBytes(thread, Arcadia_Media_VertexBuffer_getVertexBytes(thread, buffer));
  Arcadia_SizeValue capacity = Arcadia_Media_VertexBuffer_getNumberOfVertices(thread, buffer);
  for (Arcadia_SizeValue i = 0; i < capacity * 9; ++i) vertices[i] = 0.f;
  Arcadia_Engine_Text_GlyphPlacement* placements = Arcadia_Memory_allocateUnmanaged(thread, label->numberOfGlyphs * sizeof(Arcadia_Engine_Text_GlyphPlacement));
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Engine_UI_TextUtilities_copyGlyphPlacements(placements, label->glyphs, label->numberOfGlyphs);
    Arcadia_Integer32Value left = widget->left + 4;
    Arcadia_Integer32Value bottom = widget->bottom + 2;
    Arcadia_Integer32Value baseline = bottom + (widget->height - 4 + label->lineTop - label->lineBottom) / 2 - label->lineTop;
    Arcadia_Real32Value shade = self->enabled ? 1.f : 0.5f;
    Arcadia_Engine_Text_writeGlyphQuads(thread, placements, label->numberOfGlyphs,
      left - (Arcadia_Integer32Value)self->scrollX, baseline, true,
      left, bottom, widget->width - 8, widget->height - 4,
      (Arcadia_Real32Value)label->fontAtlasWidth, (Arcadia_Real32Value)label->fontAtlasHeight,
      shade, shade, shade, 1.f, vertices, capacity, 9);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Memory_deallocateUnmanaged(thread, placements);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Memory_deallocateUnmanaged(thread, placements);
    Arcadia_Thread_jump(thread);
  }
  label->modelNode->mesh->dirtyFlags |= 1;
  Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)label->modelNode, pass);
}

static void
renderInterface(Arcadia_Thread* thread, Arcadia_Object* object, Arcadia_Engine_Visuals_EnterPassNode* pass)
{
  renderImpl(thread, (Arcadia_Engine_UI_WidgetNode*)object, pass);
}

static void
setBackend(Arcadia_Thread* thread, Arcadia_Engine_Node* node, Arcadia_Engine_Visuals_BackendContext* backend)
{
  Arcadia_Engine_UI_TextInputNode* self = (Arcadia_Engine_UI_TextInputNode*)node;
  if (self->label) Arcadia_Engine_Node_setVisualsBackendContext(thread, (Arcadia_Engine_Node*)self->label, backend);
  self->backendContext = backend;
}

static void
initDispatch(Arcadia_Thread* thread, Arcadia_Engine_UI_TextInputNodeDispatch* self)
{
  ((Arcadia_Engine_UI_WidgetNodeDispatch*)self)->render = &renderImpl;
  ((Arcadia_Engine_NodeDispatch*)self)->setVisualsBackendContext = &setBackend;
}

static void
initRender(Arcadia_Thread* thread, Arcadia_Engine_UI_RenderableDispatch* self)
{
  self->render = &renderInterface;
}

static Arcadia_SizeValue
positionAt(Arcadia_Thread* thread, Arcadia_Engine_UI_TextInputNode* self, Arcadia_Integer32Value x)
{
  syncLabel(thread, self);
  updateScroll(thread, self);
  Arcadia_Real32Value local = (Arcadia_Real32Value)x - self->parent.left - 4.f + self->scrollX;
  Arcadia_Real32Value pen = 0.f;
  for (Arcadia_SizeValue i = 0; i < self->label->numberOfGlyphs; ++i) {
    Arcadia_Real32Value advance = self->label->glyphs[i].glyphInformation.advanceX;
    if (local < pen + advance * 0.5f) return i;
    pen += advance;
  }
  return self->label->numberOfGlyphs;
}

static Arcadia_BooleanValue
mouseButton(Arcadia_Thread* thread, Arcadia_Object* object, Arcadia_Engine_Input_MouseButtonEvent* event)
{
  Arcadia_Engine_UI_TextInputNode* self = (Arcadia_Engine_UI_TextInputNode*)object;
  if (!self->enabled || event->button != 0) return false;
  if (event->action == Arcadia_Engine_Input_MouseButtonAction_Pressed) {
    Arcadia_SizeValue caret = positionAt(thread, self, event->x);
    Arcadia_SizeValue anchor = self->shiftSides ? Arcadia_Engine_UI_TextEditState_getAnchor(thread, self->editor) : caret;
    Arcadia_Engine_UI_TextEditState_setSelection(thread, self->editor, anchor, caret);
    self->dragging = true;
    return true;
  }
  if (event->action == Arcadia_Engine_Input_MouseButtonAction_Released && self->dragging) {
    self->dragging = false;
    Arcadia_SizeValue caret = positionAt(thread, self, event->x);
    Arcadia_Engine_UI_TextEditState_setSelection(thread, self->editor,
      Arcadia_Engine_UI_TextEditState_getAnchor(thread, self->editor), caret);
    return true;
  }
  return false;
}

static Arcadia_BooleanValue
mousePointer(Arcadia_Thread* thread, Arcadia_Object* object, Arcadia_Engine_Input_MousePointerEvent* event)
{
  Arcadia_Engine_UI_TextInputNode* self = (Arcadia_Engine_UI_TextInputNode*)object;
  if (!self->enabled || !self->dragging) return false;
  Arcadia_SizeValue caret = positionAt(thread, self, event->x);
  Arcadia_Engine_UI_TextEditState_setSelection(thread, self->editor,
    Arcadia_Engine_UI_TextEditState_getAnchor(thread, self->editor), caret);
  return true;
}

static void
initPointer(Arcadia_Thread* thread, Arcadia_Engine_UI_PointerInputDispatch* self)
{
  self->handleMouseButtonEvent = &mouseButton;
  self->handleMousePointerEvent = &mousePointer;
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initDispatch,
};

static const Arcadia_TypeDescriptor_Operations typeOperations = {
  Arcadia_TypeDescriptor_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Arcadia.Engine.UI.TextInputNode", Arcadia_Engine_UI_TextInputNode,
    u8"Arcadia.Engine.UI.WidgetNode", Arcadia_Engine_UI_WidgetNode, &typeOperations,
    {
      .interfaceType = _Arcadia_Engine_UI_Focusable_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_FocusableDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initFocus
    },
    {
      .interfaceType = _Arcadia_Engine_UI_HitTestable_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_HitTestableDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initHit
    },
    {
      .interfaceType = _Arcadia_Engine_UI_KeyboardInput_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_KeyboardInputDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initKeyboard
    },
    {
      .interfaceType = _Arcadia_Engine_UI_Renderable_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_RenderableDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initRender
    },
    {
      .interfaceType = _Arcadia_Engine_UI_PointerInput_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_PointerInputDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initPointer
    }
  )

Arcadia_Engine_UI_TextInputNode*
Arcadia_Engine_UI_TextInputNode_create(Arcadia_Thread* thread)
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_TextInputNode);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_UI_TextInputNode);
}

Arcadia_Engine_UI_TextEditState*
Arcadia_Engine_UI_TextInputNode_getEditor(Arcadia_Thread* thread, Arcadia_Engine_UI_TextInputNode* self)
{
  return self->editor;
}

void
Arcadia_Engine_UI_TextInputNode_setEnabled(Arcadia_Thread* thread, Arcadia_Engine_UI_TextInputNode* self, Arcadia_BooleanValue enabled)
{
  self->enabled = enabled;
  if (!enabled) {
    blurImpl(thread, (Arcadia_Object*)self);
  }
}

void
Arcadia_Engine_UI_TextInputNode_setReadOnly(Arcadia_Thread* thread, Arcadia_Engine_UI_TextInputNode* self, Arcadia_BooleanValue readOnly)
{
  self->readOnly = readOnly;
}

Arcadia_BooleanValue
Arcadia_Engine_UI_TextInputNode_insertText(Arcadia_Thread* thread, Arcadia_Engine_UI_TextInputNode* self, Arcadia_String* text)
{
  return self->enabled && self->focused && !self->readOnly &&
         Arcadia_Engine_UI_TextEditState_insertText(thread, self->editor, text);
}
