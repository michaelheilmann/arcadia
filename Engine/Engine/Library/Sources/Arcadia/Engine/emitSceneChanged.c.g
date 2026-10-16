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

#define ARCADIA_ENGINE_PRIVATE (1)
#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Engine/emitSceneChanged.h"
#include "Arcadia/Engine/Scene.h"
#include "Arcadia/Engine/SceneManager.h"
#include "Arcadia/Ring1/Include.h"

static Arcadia_Value
pdlGet
  (
    Arcadia_Thread* t,
    Arcadia_SizeValue b,
    Arcadia_SizeValue i
  )
{
  return Arcadia_ValueStack_getValue(t, Arcadia_ValueStack_getSize(t)-b-i-1);
}

static void
pdlPut
  (
    Arcadia_Thread* t,
    Arcadia_SizeValue b,
    Arcadia_SizeValue i,
    Arcadia_Value v
  )
{
  Arcadia_ValueStack_setValue(t, Arcadia_ValueStack_getSize(t)-b-i-1, &v);
}

static void
pdlRequire
  (
    Arcadia_Thread* t,
    Arcadia_BooleanValue b
  )
{
  if (!b) { Arcadia_Thread_setStatus(t, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(t); }
}

static Arcadia_Value
pdlBoxObjectReference
  (
    Arcadia_ObjectReferenceValue object
  )
{
  return object ? Arcadia_Value_makeObjectReferenceValue(object) : Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
}

static Arcadia_Object*
pdlAsObjectReference
  (
    Arcadia_Thread* thread,
    Arcadia_Value v,
    Arcadia_Type* type
  )
{
  if (Arcadia_Value_isVoidValue(&v)) return NULL;
  pdlRequire(thread, Arcadia_Value_isObjectReferenceValue(&v) && Arcadia_Value_getObjectReferenceValue(&v));
  return (Arcadia_Object*)Arcadia_Value_getObjectReferenceValueChecked(thread, v, type);
}

void
Arcadia_Engine_emitSceneChanged
  (
    Arcadia_Thread* thread,
    Arcadia_Signal* p0,
    Arcadia_Engine_SceneManager* p1,
    Arcadia_Engine_Scene* p2,
    Arcadia_Engine_Scene* p3
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0; i < 8; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 4, pdlBoxObjectReference(p0));
  pdlPut(thread, base, 5, pdlBoxObjectReference(p1));
  pdlPut(thread, base, 6, pdlBoxObjectReference(p2));
  pdlPut(thread, base, 7, pdlBoxObjectReference(p3));
  (void)((Arcadia_Signal*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Signal_getType(thread)));
  (void)((Arcadia_Engine_SceneManager*)pdlAsObjectReference(thread, pdlGet(thread, base, 5), _Arcadia_Engine_SceneManager_getType(thread)));
  (void)((Arcadia_Engine_Scene*)pdlAsObjectReference(thread, pdlGet(thread, base, 6), _Arcadia_Engine_Scene_getType(thread)));
  (void)((Arcadia_Engine_Scene*)pdlAsObjectReference(thread, pdlGet(thread, base, 7), _Arcadia_Engine_Scene_getType(thread)));
  extern void Arcadia_Engine_emitSceneChangedImpl(Arcadia_Thread* thread, Arcadia_Signal* p0, Arcadia_Engine_SceneManager* p1, Arcadia_Engine_Scene* p2, Arcadia_Engine_Scene* p3);
  Arcadia_Engine_emitSceneChangedImpl(thread, (Arcadia_Signal*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Signal_getType(thread)), (Arcadia_Engine_SceneManager*)pdlAsObjectReference(thread, pdlGet(thread, base, 5), _Arcadia_Engine_SceneManager_getType(thread)), (Arcadia_Engine_Scene*)pdlAsObjectReference(thread, pdlGet(thread, base, 6), _Arcadia_Engine_Scene_getType(thread)), (Arcadia_Engine_Scene*)pdlAsObjectReference(thread, pdlGet(thread, base, 7), _Arcadia_Engine_Scene_getType(thread)));
  goto pdlReturn;
pdlReturn:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value v = pdlGet(thread, base, 0); pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return;
}

