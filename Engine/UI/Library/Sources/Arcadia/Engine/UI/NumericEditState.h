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

#if !defined(ARCADIA_ENGINE_UI_NUMERICEDITSTATE_H_INCLUDED)
#define ARCADIA_ENGINE_UI_NUMERICEDITSTATE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif

#include "Arcadia/Engine/UI/TextEditState.h"

typedef enum Arcadia_Engine_UI_NumericEditStateResult {
  Arcadia_Engine_UI_NumericEditStateResult_Rejected,
  Arcadia_Engine_UI_NumericEditStateResult_Unchanged,
  Arcadia_Engine_UI_NumericEditStateResult_Changed,
} Arcadia_Engine_UI_NumericEditStateResult;

/// Backend-independent Integer64/Real64 editing. Initially Integer64 zero.
/// Bounds are Void (absent) or the same type as the committed value.
/// A type change requires clearing incompatible bounds first.
/// Rejected commits preserve both the draft and the last accepted value.
/// Accepted commits normalize text using Ring1's round-trip formatter.
/// Bounds are inclusive and checked against the converted typed value.
/// The borrowed editor is for draft editing/navigation only: do not call
/// its setText, commit or cancel directly. Root receiver during calls.
Arcadia_declareObjectType(u8"Arcadia.Engine.UI.NumericEditState", Arcadia_Engine_UI_NumericEditState,
                          u8"Arcadia.Object")

struct Arcadia_Engine_UI_NumericEditStateDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Engine_UI_NumericEditState {
  Arcadia_Object _parent;
  Arcadia_Engine_UI_TextEditState* editor;
  Arcadia_Value value;
  Arcadia_Value minimum;
  Arcadia_Value maximum;
};

Arcadia_Engine_UI_NumericEditState*
Arcadia_Engine_UI_NumericEditState_create
  (
    Arcadia_Thread* thread
  );

Arcadia_Engine_UI_TextEditState*
Arcadia_Engine_UI_NumericEditState_getEditor
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_NumericEditState* self
  );

Arcadia_Value
Arcadia_Engine_UI_NumericEditState_getValue
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_NumericEditState* self
  );

void
Arcadia_Engine_UI_NumericEditState_setValue
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_NumericEditState* self,
    Arcadia_Value value
  );

void
Arcadia_Engine_UI_NumericEditState_setBounds
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_NumericEditState* self,
    Arcadia_Value minimum,
    Arcadia_Value maximum
  );

Arcadia_Engine_UI_NumericEditStateResult
Arcadia_Engine_UI_NumericEditState_commit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_NumericEditState* self
  );

Arcadia_BooleanValue
Arcadia_Engine_UI_NumericEditState_cancel
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_NumericEditState* self
  );

#endif // ARCADIA_ENGINE_UI_NUMERICEDITSTATE_H_INCLUDED
