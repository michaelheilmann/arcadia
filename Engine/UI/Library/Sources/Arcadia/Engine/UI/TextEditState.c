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
#include "Arcadia/Engine/UI/TextEditState.h"

static void
invalid
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}

static Arcadia_SizeValue
validate
  (
    Arcadia_Thread* thread,
    Arcadia_String* text
  )
{
  if (!text) {
    invalid(thread);
  }
  Arcadia_Natural8Value const* bytes = Arcadia_String_getBytes(thread, text);
  Arcadia_SizeValue size = Arcadia_String_getNumberOfBytes(thread, text);
  for (Arcadia_SizeValue i = 0; i < size; ++i) {
    if (!bytes[i] || bytes[i] == '\r' || bytes[i] == '\n' ||
        (size - i >= 2 && bytes[i] == 0xc2 && bytes[i + 1] == 0x85) ||
        (size - i >= 3 && bytes[i] == 0xe2 && bytes[i + 1] == 0x80 &&
         (bytes[i + 2] == 0xa8 || bytes[i + 2] == 0xa9))) {
      invalid(thread);
    }
  }
  return Arcadia_String_getNumberOfCodePoints(thread, text);
}

static Arcadia_SizeValue
byteOffset
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self,
    Arcadia_SizeValue index
  )
{
  Arcadia_Natural8Value const* bytes = Arcadia_StringBuilder_getBytes(thread, self->draft);
  Arcadia_SizeValue size = Arcadia_StringBuilder_getNumberOfBytes(thread, self->draft);
  Arcadia_SizeValue offset = 0;
  while (index && offset < size) {
    ++offset;
    while (offset < size && (bytes[offset] & 0xc0) == 0x80) {
      ++offset;
    }
    --index;
  }
  return offset;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_TextEditState);
  if (_numberOfArguments != 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->draft = NULL;
  self->committed = NULL;
  self->anchor = self->caret = 0;
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)self);
    self->draft = Arcadia_StringBuilder_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)self->draft);
    self->committed = Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)self->draft));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_TextEditState);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self
  )
{
  if (self->draft) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->draft);
  }
  if (self->committed) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->committed);
  }
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};

static const Arcadia_TypeDescriptor_Operations typeOperations = {
  Arcadia_TypeDescriptor_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.UI.TextEditState", Arcadia_Engine_UI_TextEditState,
                         u8"Arcadia.Object", Arcadia_Object, &typeOperations)

Arcadia_Engine_UI_TextEditState*
Arcadia_Engine_UI_TextEditState_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_TextEditState);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_UI_TextEditState);
}

Arcadia_String*
Arcadia_Engine_UI_TextEditState_getText
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self
  )
{
  return Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)self->draft));
}

Arcadia_String*
Arcadia_Engine_UI_TextEditState_getCommittedText
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self
  )
{
  return self->committed;
}

Arcadia_SizeValue
Arcadia_Engine_UI_TextEditState_getCaret
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self
  )
{
  return self->caret;
}

Arcadia_SizeValue
Arcadia_Engine_UI_TextEditState_getAnchor
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self
  )
{
  return self->anchor;
}

Arcadia_BooleanValue
Arcadia_Engine_UI_TextEditState_isDirty
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self
  )
{
  return Arcadia_StringBuilder_compareTo(thread, self->draft, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)self->committed)) != 0;
}

void
Arcadia_Engine_UI_TextEditState_setSelection
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self,
    Arcadia_SizeValue anchor,
    Arcadia_SizeValue caret
  )
{
  Arcadia_SizeValue count = Arcadia_StringBuilder_getNumberOfCodePoints(thread, self->draft);
  if (anchor > count || caret > count) {
    invalid(thread);
  }
  self->anchor = anchor;
  self->caret = caret;
}

void
Arcadia_Engine_UI_TextEditState_setText
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self,
    Arcadia_String* text
  )
{
  Arcadia_SizeValue count = validate(thread, text);
  Arcadia_StringBuilder_replaceCodePointsAt(thread, self->draft, 0,
    Arcadia_StringBuilder_getNumberOfCodePoints(thread, self->draft), Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)text));
  self->committed = text;
  self->anchor = self->caret = count;
}

Arcadia_BooleanValue
Arcadia_Engine_UI_TextEditState_insertText
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self,
    Arcadia_String* text
  )
{
  Arcadia_SizeValue count = validate(thread, text);
  Arcadia_SizeValue first = self->anchor < self->caret ? self->anchor : self->caret;
  Arcadia_SizeValue last = self->anchor > self->caret ? self->anchor : self->caret;
  Arcadia_SizeValue startByte = byteOffset(thread, self, first);
  Arcadia_SizeValue endByte = byteOffset(thread, self, last);
  Arcadia_SizeValue bytes = Arcadia_String_getNumberOfBytes(thread, text);
  Arcadia_BooleanValue changed = endByte - startByte != bytes;
  if (!changed && bytes) {
    changed = Arcadia_Memory_compare(thread, Arcadia_StringBuilder_getBytes(thread, self->draft) + startByte,
      Arcadia_String_getBytes(thread, text), bytes) != 0;
  }
  if (changed) {
    Arcadia_StringBuilder_replaceCodePointsAt(thread, self->draft, first, last - first, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)text));
  }
  self->anchor = self->caret = first + count;
  return changed;
}

Arcadia_BooleanValue
Arcadia_Engine_UI_TextEditState_backspace
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self
  )
{
  Arcadia_SizeValue first = self->anchor < self->caret ? self->anchor : self->caret;
  Arcadia_SizeValue last = self->anchor > self->caret ? self->anchor : self->caret;
  if (first == last) {
    if (!first) {
      return false;
    }
    --first;
  }
  Arcadia_StringBuilder_removeCodePointsAt(thread, self->draft, first, last - first);
  self->anchor = self->caret = first;
  return true;
}

Arcadia_BooleanValue
Arcadia_Engine_UI_TextEditState_deleteForward
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self
  )
{
  Arcadia_SizeValue first = self->anchor < self->caret ? self->anchor : self->caret;
  Arcadia_SizeValue last = self->anchor > self->caret ? self->anchor : self->caret;
  if (first == last) {
    if (last == Arcadia_StringBuilder_getNumberOfCodePoints(thread, self->draft)) {
      return false;
    }
    ++last;
  }
  Arcadia_StringBuilder_removeCodePointsAt(thread, self->draft, first, last - first);
  self->anchor = self->caret = first;
  return true;
}

void
Arcadia_Engine_UI_TextEditState_moveLeft
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self,
    Arcadia_BooleanValue extend
  )
{
  if (!extend && self->anchor != self->caret) {
    self->caret = self->anchor < self->caret ? self->anchor : self->caret;
  } else if (self->caret) {
    --self->caret;
  }
  if (!extend) {
    self->anchor = self->caret;
  }
}

void
Arcadia_Engine_UI_TextEditState_moveRight
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self,
    Arcadia_BooleanValue extend
  )
{
  if (!extend && self->anchor != self->caret) {
    self->caret = self->anchor > self->caret ? self->anchor : self->caret;
  } else if (self->caret < Arcadia_StringBuilder_getNumberOfCodePoints(thread, self->draft)) {
    ++self->caret;
  }
  if (!extend) {
    self->anchor = self->caret;
  }
}

void
Arcadia_Engine_UI_TextEditState_moveHome
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self,
    Arcadia_BooleanValue extend
  )
{
  self->caret = 0;
  if (!extend) {
    self->anchor = self->caret;
  }
}

void
Arcadia_Engine_UI_TextEditState_moveEnd
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self,
    Arcadia_BooleanValue extend
  )
{
  self->caret = Arcadia_StringBuilder_getNumberOfCodePoints(thread, self->draft);
  if (!extend) {
    self->anchor = self->caret;
  }
}

Arcadia_BooleanValue
Arcadia_Engine_UI_TextEditState_commit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self
  )
{
  if (!Arcadia_Engine_UI_TextEditState_isDirty(thread, self)) {
    return false;
  }
  self->committed = Arcadia_Engine_UI_TextEditState_getText(thread, self);
  return true;
}

Arcadia_BooleanValue
Arcadia_Engine_UI_TextEditState_cancel
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self
  )
{
  Arcadia_BooleanValue changed = Arcadia_Engine_UI_TextEditState_isDirty(thread, self);
  if (changed) {
    Arcadia_Engine_UI_TextEditState_setText(thread, self, self->committed);
  } else {
    self->anchor = self->caret = Arcadia_StringBuilder_getNumberOfCodePoints(thread, self->draft);
  }
  return changed;
}
