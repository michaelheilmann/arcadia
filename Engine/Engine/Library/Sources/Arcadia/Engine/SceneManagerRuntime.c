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
// FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License
// for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include "Arcadia/Engine/Include.h"

Arcadia_Signal*
Arcadia_Engine_createSceneManagerSignalImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine
  )
{
  if (!engine) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_Signal_create(thread);
}

Arcadia_BooleanValue
Arcadia_Engine_scenesAreIdenticalImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* left,
    Arcadia_Engine_Scene* right
  )
{
  return left == right;
}

void
Arcadia_Engine_emitSceneChangedImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Signal* signal,
    Arcadia_Engine_SceneManager* sender,
    Arcadia_Engine_Scene* oldScene,
    Arcadia_Engine_Scene* newScene
  )
{
  if (!signal || !sender) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    if (oldScene) Arcadia_ValueStack_pushObjectReferenceValue(thread, oldScene);
    else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
    if (newScene) Arcadia_ValueStack_pushObjectReferenceValue(thread, newScene);
    else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_Signal_emit(thread, signal, (Arcadia_Object*)sender);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}
