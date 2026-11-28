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

#if !defined(ARCADIA_ENGINE_UI_TEXTEDITSTATE_H_INCLUDED)
#define ARCADIA_ENGINE_UI_TEXTEDITSTATE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif

#include "Arcadia/Engine/Include.h"

/// Backend-independent single-line editing state. Keep the receiver and input
/// strings rooted during calls. All positions are Unicode codepoint boundaries,
/// not byte offsets or grapheme indices. Do not mutate the owned draft directly.
/// Input rejects NUL, CR, LF, NEL, LINE SEPARATOR and PARAGRAPH SEPARATOR.
/// Boolean editing results report text changes, not selection changes.
/// getText returns an immutable snapshot; commit preserves selection and
/// cancel restores committed text and collapses selection at its end.
Arcadia_declareObjectType(u8"Arcadia.Engine.UI.TextEditState", Arcadia_Engine_UI_TextEditState,
                          u8"Arcadia.Object")

struct Arcadia_Engine_UI_TextEditStateDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Engine_UI_TextEditState {
  Arcadia_Object _parent;
  Arcadia_StringBuilder* draft;
  Arcadia_String* committed;
  Arcadia_SizeValue anchor;
  Arcadia_SizeValue caret;
};

Arcadia_Engine_UI_TextEditState*
Arcadia_Engine_UI_TextEditState_create
  (
    Arcadia_Thread* thread
  );

Arcadia_String*
Arcadia_Engine_UI_TextEditState_getText
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self
  );

Arcadia_String*
Arcadia_Engine_UI_TextEditState_getCommittedText
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self
  );

Arcadia_SizeValue
Arcadia_Engine_UI_TextEditState_getCaret
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self
  );

Arcadia_SizeValue
Arcadia_Engine_UI_TextEditState_getAnchor
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self
  );

Arcadia_BooleanValue
Arcadia_Engine_UI_TextEditState_isDirty
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self
  );

void
Arcadia_Engine_UI_TextEditState_setSelection
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self,
    Arcadia_SizeValue anchor,
    Arcadia_SizeValue caret
  );

void
Arcadia_Engine_UI_TextEditState_setText
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self,
    Arcadia_String* text
  );

Arcadia_BooleanValue
Arcadia_Engine_UI_TextEditState_insertText
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self,
    Arcadia_String* text
  );

Arcadia_BooleanValue
Arcadia_Engine_UI_TextEditState_backspace
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self
  );

Arcadia_BooleanValue
Arcadia_Engine_UI_TextEditState_deleteForward
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self
  );

void
Arcadia_Engine_UI_TextEditState_moveLeft
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self,
    Arcadia_BooleanValue extend
  );

void
Arcadia_Engine_UI_TextEditState_moveRight
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self,
    Arcadia_BooleanValue extend
  );

void
Arcadia_Engine_UI_TextEditState_moveHome
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self,
    Arcadia_BooleanValue extend
  );

void
Arcadia_Engine_UI_TextEditState_moveEnd
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self,
    Arcadia_BooleanValue extend
  );

Arcadia_BooleanValue
Arcadia_Engine_UI_TextEditState_commit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self
  );

Arcadia_BooleanValue
Arcadia_Engine_UI_TextEditState_cancel
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TextEditState* self
  );

#endif // ARCADIA_ENGINE_UI_TEXTEDITSTATE_H_INCLUDED
