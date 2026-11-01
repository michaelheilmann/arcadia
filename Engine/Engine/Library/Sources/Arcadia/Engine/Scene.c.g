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
#include "Arcadia/Engine/Scene.h"
#include "Arcadia/Engine/Engine.h"
#include "Arcadia/Engine/Input/KeyboardKeyEvent.h"
#include "Arcadia/Engine/Input/MouseButtonEvent.h"
#include "Arcadia/Engine/Input/MousePointerEvent.h"
#include "Arcadia/Engine/SceneManager.h"
#include "Arcadia/Engine/createSceneQuitRequestSignal.h"
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

static Arcadia_Integer32Value
pdlAsInteger32
  (
    Arcadia_Thread* t,
    Arcadia_Value v
  )
{
  pdlRequire(t, Arcadia_Value_isInteger32Value(&v));
  return Arcadia_Value_getInteger32Value(&v);
}

static Arcadia_Real64Value
pdlAsReal64
  (
    Arcadia_Thread* t,
    Arcadia_Value v
  )
{
  pdlRequire(t, Arcadia_Value_isReal64Value(&v));
  return Arcadia_Value_getReal64Value(&v);
}

static void
Arcadia_Engine_Scene_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Engine* p0,
    Arcadia_Engine_SceneManager* p1
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
  pdlPut(thread, base, 6, pdlBoxObjectReference(self));
  pdlPut(thread, base, 4, pdlBoxObjectReference(p0));
  pdlPut(thread, base, 5, pdlBoxObjectReference(p1));
  (void)((Arcadia_Engine*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Engine_getType(thread)));
  (void)((Arcadia_Engine_SceneManager*)pdlAsObjectReference(thread, pdlGet(thread, base, 5), _Arcadia_Engine_SceneManager_getType(thread)));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 4); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 5); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[3];
    args[0] = Arcadia_ValueStack_getValue(thread, 1);
    args[1] = Arcadia_ValueStack_getValue(thread, 0);
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_Engine_createSceneQuitRequestSignal(thread, (Arcadia_Engine*)pdlAsObjectReference(thread, args[0], _Arcadia_Engine_getType(thread)), (Arcadia_Engine_SceneManager*)pdlAsObjectReference(thread, args[1], _Arcadia_Engine_SceneManager_getType(thread)))));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 7, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 6));
  pdlPut(thread, base, 2, pdlGet(thread, base, 7));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Scene_getType(thread))) {
      ((Arcadia_Engine_Scene*)Arcadia_Value_getObjectReferenceValue(&receiver))->applicationQuitRequestSignal = (Arcadia_Signal*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_Signal_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 6));
  pdlPut(thread, base, 2, pdlGet(thread, base, 4));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Scene_getType(thread))) {
      ((Arcadia_Engine_Scene*)Arcadia_Value_getObjectReferenceValue(&receiver))->engine = (Arcadia_Engine*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_Engine_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 6));
  pdlPut(thread, base, 2, pdlGet(thread, base, 5));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Scene_getType(thread))) {
      ((Arcadia_Engine_Scene*)Arcadia_Value_getObjectReferenceValue(&receiver))->sceneManager = (Arcadia_Engine_SceneManager*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_Engine_SceneManager_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
pdlReturn:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value v = pdlGet(thread, base, 0); pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return;
}

static void
Arcadia_Engine_Scene_updateAudials_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Real64Value p0,
    Arcadia_Integer32Value p1,
    Arcadia_Integer32Value p2
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
  pdlPut(thread, base, 7, pdlBoxObjectReference(self));
  pdlPut(thread, base, 4, Arcadia_Value_makeReal64Value(p0));
  pdlPut(thread, base, 5, Arcadia_Value_makeInteger32Value(p1));
  pdlPut(thread, base, 6, Arcadia_Value_makeInteger32Value(p2));
  (void)((Arcadia_Real64Value)pdlAsReal64(thread, pdlGet(thread, base, 4)));
  (void)((Arcadia_Integer32Value)pdlAsInteger32(thread, pdlGet(thread, base, 5)));
  (void)((Arcadia_Integer32Value)pdlAsInteger32(thread, pdlGet(thread, base, 6)));
pdlReturn:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value v = pdlGet(thread, base, 0); pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return;
}

void
Arcadia_Engine_Scene_updateAudials
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Real64Value p0,
    Arcadia_Integer32Value p1,
    Arcadia_Integer32Value p2
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value selfValue = pdlBoxObjectReference(self);
  Arcadia_ValueStack_pushValue(thread, &selfValue);
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Scene_getType(thread)));
  Arcadia_Engine_SceneDispatch* dispatch = (Arcadia_Engine_SceneDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->updateAudials) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  dispatch->updateAudials(thread, (Arcadia_Engine_Scene*)self, p0, p1, p2);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

static void
Arcadia_Engine_Scene_updateLogics_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Real64Value p0
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
  for (Arcadia_SizeValue i = 0; i < 6; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 5, pdlBoxObjectReference(self));
  pdlPut(thread, base, 4, Arcadia_Value_makeReal64Value(p0));
  (void)((Arcadia_Real64Value)pdlAsReal64(thread, pdlGet(thread, base, 4)));
pdlReturn:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value v = pdlGet(thread, base, 0); pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return;
}

void
Arcadia_Engine_Scene_updateLogics
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Real64Value p0
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value selfValue = pdlBoxObjectReference(self);
  Arcadia_ValueStack_pushValue(thread, &selfValue);
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Scene_getType(thread)));
  Arcadia_Engine_SceneDispatch* dispatch = (Arcadia_Engine_SceneDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->updateLogics) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  dispatch->updateLogics(thread, (Arcadia_Engine_Scene*)self, p0);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

static void
Arcadia_Engine_Scene_updateVisuals_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Real64Value p0,
    Arcadia_Integer32Value p1,
    Arcadia_Integer32Value p2
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
  pdlPut(thread, base, 7, pdlBoxObjectReference(self));
  pdlPut(thread, base, 4, Arcadia_Value_makeReal64Value(p0));
  pdlPut(thread, base, 5, Arcadia_Value_makeInteger32Value(p1));
  pdlPut(thread, base, 6, Arcadia_Value_makeInteger32Value(p2));
  (void)((Arcadia_Real64Value)pdlAsReal64(thread, pdlGet(thread, base, 4)));
  (void)((Arcadia_Integer32Value)pdlAsInteger32(thread, pdlGet(thread, base, 5)));
  (void)((Arcadia_Integer32Value)pdlAsInteger32(thread, pdlGet(thread, base, 6)));
pdlReturn:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value v = pdlGet(thread, base, 0); pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return;
}

void
Arcadia_Engine_Scene_updateVisuals
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Real64Value p0,
    Arcadia_Integer32Value p1,
    Arcadia_Integer32Value p2
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value selfValue = pdlBoxObjectReference(self);
  Arcadia_ValueStack_pushValue(thread, &selfValue);
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Scene_getType(thread)));
  Arcadia_Engine_SceneDispatch* dispatch = (Arcadia_Engine_SceneDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->updateVisuals) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  dispatch->updateVisuals(thread, (Arcadia_Engine_Scene*)self, p0, p1, p2);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

static void
Arcadia_Engine_Scene_handleKeyboardKeyEvent_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* p0
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
  for (Arcadia_SizeValue i = 0; i < 6; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 5, pdlBoxObjectReference(self));
  pdlPut(thread, base, 4, pdlBoxObjectReference(p0));
  (void)((Arcadia_Engine_Input_KeyboardKeyEvent*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Engine_Input_KeyboardKeyEvent_getType(thread)));
pdlReturn:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value v = pdlGet(thread, base, 0); pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return;
}

void
Arcadia_Engine_Scene_handleKeyboardKeyEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* p0
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value selfValue = pdlBoxObjectReference(self);
  Arcadia_ValueStack_pushValue(thread, &selfValue);
  { Arcadia_Value value = pdlBoxObjectReference(p0); Arcadia_ValueStack_pushValue(thread, &value); }
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Scene_getType(thread)));
  Arcadia_Engine_SceneDispatch* dispatch = (Arcadia_Engine_SceneDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->handleKeyboardKeyEvent) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  pdlRequire(thread, !p0 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p0, _Arcadia_Engine_Input_KeyboardKeyEvent_getType(thread)));
  dispatch->handleKeyboardKeyEvent(thread, (Arcadia_Engine_Scene*)self, p0);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

static void
Arcadia_Engine_Scene_handleMouseButtonEvent_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Engine_Input_MouseButtonEvent* p0
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
  for (Arcadia_SizeValue i = 0; i < 6; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 5, pdlBoxObjectReference(self));
  pdlPut(thread, base, 4, pdlBoxObjectReference(p0));
  (void)((Arcadia_Engine_Input_MouseButtonEvent*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Engine_Input_MouseButtonEvent_getType(thread)));
pdlReturn:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value v = pdlGet(thread, base, 0); pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return;
}

void
Arcadia_Engine_Scene_handleMouseButtonEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Engine_Input_MouseButtonEvent* p0
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value selfValue = pdlBoxObjectReference(self);
  Arcadia_ValueStack_pushValue(thread, &selfValue);
  { Arcadia_Value value = pdlBoxObjectReference(p0); Arcadia_ValueStack_pushValue(thread, &value); }
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Scene_getType(thread)));
  Arcadia_Engine_SceneDispatch* dispatch = (Arcadia_Engine_SceneDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->handleMouseButtonEvent) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  pdlRequire(thread, !p0 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p0, _Arcadia_Engine_Input_MouseButtonEvent_getType(thread)));
  dispatch->handleMouseButtonEvent(thread, (Arcadia_Engine_Scene*)self, p0);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

static void
Arcadia_Engine_Scene_handleMousePointerEvent_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Engine_Input_MousePointerEvent* p0
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
  for (Arcadia_SizeValue i = 0; i < 6; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 5, pdlBoxObjectReference(self));
  pdlPut(thread, base, 4, pdlBoxObjectReference(p0));
  (void)((Arcadia_Engine_Input_MousePointerEvent*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Engine_Input_MousePointerEvent_getType(thread)));
pdlReturn:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value v = pdlGet(thread, base, 0); pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return;
}

void
Arcadia_Engine_Scene_handleMousePointerEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Engine_Input_MousePointerEvent* p0
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value selfValue = pdlBoxObjectReference(self);
  Arcadia_ValueStack_pushValue(thread, &selfValue);
  { Arcadia_Value value = pdlBoxObjectReference(p0); Arcadia_ValueStack_pushValue(thread, &value); }
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Scene_getType(thread)));
  Arcadia_Engine_SceneDispatch* dispatch = (Arcadia_Engine_SceneDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->handleMousePointerEvent) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  pdlRequire(thread, !p0 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p0, _Arcadia_Engine_Input_MousePointerEvent_getType(thread)));
  dispatch->handleMousePointerEvent(thread, (Arcadia_Engine_Scene*)self, p0);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Scene);
  if (_numberOfArguments != 2) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->engine = (Arcadia_Engine*){0};
  self->sceneManager = (Arcadia_Engine_SceneManager*){0};
  self->applicationQuitRequestSignal = (Arcadia_Signal*){0};
  Arcadia_Object_setType(thread, (Arcadia_Object*)self, _type);
  { Arcadia_Value args[3];
    args[0] = Arcadia_ValueStack_getValue(thread, 2);
    args[1] = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_Engine_Scene_construct(thread, self, (Arcadia_Engine*)pdlAsObjectReference(thread, args[0], _Arcadia_Engine_getType(thread)), (Arcadia_Engine_SceneManager*)pdlAsObjectReference(thread, args[1], _Arcadia_Engine_SceneManager_getType(thread)));
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Scene);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self
  )
{
  if (self->engine) Arcadia_Object_visit(thread, (Arcadia_Object*)self->engine);
  if (self->sceneManager) Arcadia_Object_visit(thread, (Arcadia_Object*)self->sceneManager);
  if (self->applicationQuitRequestSignal) Arcadia_Object_visit(thread, (Arcadia_Object*)self->applicationQuitRequestSignal);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_SceneDispatch* self
  )
{
  ((Arcadia_Engine_SceneDispatch*)self)->updateAudials = Arcadia_Engine_Scene_updateAudials_impl;
  ((Arcadia_Engine_SceneDispatch*)self)->updateLogics = Arcadia_Engine_Scene_updateLogics_impl;
  ((Arcadia_Engine_SceneDispatch*)self)->updateVisuals = Arcadia_Engine_Scene_updateVisuals_impl;
  ((Arcadia_Engine_SceneDispatch*)self)->handleKeyboardKeyEvent = Arcadia_Engine_Scene_handleKeyboardKeyEvent_impl;
  ((Arcadia_Engine_SceneDispatch*)self)->handleMouseButtonEvent = Arcadia_Engine_Scene_handleMouseButtonEvent_impl;
  ((Arcadia_Engine_SceneDispatch*)self)->handleMousePointerEvent = Arcadia_Engine_Scene_handleMousePointerEvent_impl;
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)initializeDispatchImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Scene", Arcadia_Engine_Scene,
                         u8"Arcadia.Object", Arcadia_Object,
                         &typeOperations);
