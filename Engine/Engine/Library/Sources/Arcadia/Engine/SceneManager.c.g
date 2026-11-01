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
#include "Arcadia/Engine/SceneManager.h"
#include "Arcadia/Engine/Engine.h"
#include "Arcadia/Engine/Scene.h"
#include "Arcadia/Engine/createSceneManagerSignal.h"
#include "Arcadia/Engine/emitSceneChanged.h"
#include "Arcadia/Engine/scenesAreIdentical.h"
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

static Arcadia_BooleanValue
pdlAsBoolean
  (
    Arcadia_Thread* t,
    Arcadia_Value v
  )
{
  pdlRequire(t, Arcadia_Value_isBooleanValue(&v));
  return Arcadia_Value_getBooleanValue(&v);
}

static void
Arcadia_Engine_SceneManager_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_SceneManager* self,
    Arcadia_Engine* p0
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
  for (Arcadia_SizeValue i = 0; i < 7; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 5, pdlBoxObjectReference(self));
  pdlPut(thread, base, 4, pdlBoxObjectReference(p0));
  (void)((Arcadia_Engine*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Engine_getType(thread)));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 4); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[2];
    args[0] = Arcadia_ValueStack_getValue(thread, 0);
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_Engine_createSceneManagerSignal(thread, (Arcadia_Engine*)pdlAsObjectReference(thread, args[0], _Arcadia_Engine_getType(thread)))));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 6, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 5));
  pdlPut(thread, base, 2, pdlGet(thread, base, 4));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_SceneManager_getType(thread))) {
      ((Arcadia_Engine_SceneManager*)Arcadia_Value_getObjectReferenceValue(&receiver))->engine = (Arcadia_Engine*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_Engine_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 5));
  pdlPut(thread, base, 2, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_SceneManager_getType(thread))) {
      ((Arcadia_Engine_SceneManager*)Arcadia_Value_getObjectReferenceValue(&receiver))->scene = (Arcadia_Engine_Scene*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_Engine_Scene_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 5));
  pdlPut(thread, base, 2, pdlGet(thread, base, 6));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_SceneManager_getType(thread))) {
      ((Arcadia_Engine_SceneManager*)Arcadia_Value_getObjectReferenceValue(&receiver))->sceneChangedEvent = (Arcadia_Signal*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_Signal_getType(thread));
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

static Arcadia_Engine_Scene*
Arcadia_Engine_SceneManager_getScene_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_SceneManager* self
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
  pdlPut(thread, base, 4, pdlBoxObjectReference(self));
  pdlPut(thread, base, 1, pdlGet(thread, base, 4));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_SceneManager_getType(thread))) {
  pdlPut(thread, base, 0, pdlBoxObjectReference(((Arcadia_Engine_SceneManager*)Arcadia_Value_getObjectReferenceValue(&receiver))->scene));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 5, pdlGet(thread, base, 0));
  pdlPut(thread, base, 0, pdlGet(thread, base, 5));
  goto pdlReturn;
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread);
pdlReturn:;
  (void)((Arcadia_Engine_Scene*)pdlAsObjectReference(thread, pdlGet(thread, base, 0), _Arcadia_Engine_Scene_getType(thread)));
  Arcadia_Engine_Scene* result = (Arcadia_Engine_Scene*)pdlAsObjectReference(thread, pdlGet(thread, base, 0), _Arcadia_Engine_Scene_getType(thread));
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return result;
}

Arcadia_Engine_Scene*
Arcadia_Engine_SceneManager_getScene
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_SceneManager* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_SceneManager_getType(thread)));
  Arcadia_Engine_Scene* result = Arcadia_Engine_SceneManager_getScene_impl(thread, self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

static void
Arcadia_Engine_SceneManager_setScene_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_SceneManager* self,
    Arcadia_Engine_Scene* p0
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
  for (Arcadia_SizeValue i = 0; i < 10; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 5, pdlBoxObjectReference(self));
  pdlPut(thread, base, 4, pdlBoxObjectReference(p0));
  (void)((Arcadia_Engine_Scene*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Engine_Scene_getType(thread)));
  pdlPut(thread, base, 1, pdlGet(thread, base, 5));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_SceneManager_getType(thread))) {
  pdlPut(thread, base, 0, pdlBoxObjectReference(((Arcadia_Engine_SceneManager*)Arcadia_Value_getObjectReferenceValue(&receiver))->scene));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 6, pdlGet(thread, base, 0));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 6); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 4); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[3];
    args[0] = Arcadia_ValueStack_getValue(thread, 1);
    args[1] = Arcadia_ValueStack_getValue(thread, 0);
  pdlPut(thread, base, 0, Arcadia_Value_makeBooleanValue(Arcadia_Engine_scenesAreIdentical(thread, (Arcadia_Engine_Scene*)pdlAsObjectReference(thread, args[0], _Arcadia_Engine_Scene_getType(thread)), (Arcadia_Engine_Scene*)pdlAsObjectReference(thread, args[1], _Arcadia_Engine_Scene_getType(thread)))));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 7, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 7));
  if (pdlAsBoolean(thread, pdlGet(thread, base, 1))) goto pdlLabel_done;
  pdlPut(thread, base, 1, pdlGet(thread, base, 5));
  pdlPut(thread, base, 2, pdlGet(thread, base, 4));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_SceneManager_getType(thread))) {
      ((Arcadia_Engine_SceneManager*)Arcadia_Value_getObjectReferenceValue(&receiver))->scene = (Arcadia_Engine_Scene*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_Engine_Scene_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 5));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_SceneManager_getType(thread))) {
  pdlPut(thread, base, 0, pdlBoxObjectReference(((Arcadia_Engine_SceneManager*)Arcadia_Value_getObjectReferenceValue(&receiver))->sceneChangedEvent));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 8, pdlGet(thread, base, 0));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 8); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 5); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 6); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 4); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[5];
    args[0] = Arcadia_ValueStack_getValue(thread, 3);
    args[1] = Arcadia_ValueStack_getValue(thread, 2);
    args[2] = Arcadia_ValueStack_getValue(thread, 1);
    args[3] = Arcadia_ValueStack_getValue(thread, 0);
    Arcadia_Engine_emitSceneChanged(thread, (Arcadia_Signal*)pdlAsObjectReference(thread, args[0], _Arcadia_Signal_getType(thread)), (Arcadia_Engine_SceneManager*)pdlAsObjectReference(thread, args[1], _Arcadia_Engine_SceneManager_getType(thread)), (Arcadia_Engine_Scene*)pdlAsObjectReference(thread, args[2], _Arcadia_Engine_Scene_getType(thread)), (Arcadia_Engine_Scene*)pdlAsObjectReference(thread, args[3], _Arcadia_Engine_Scene_getType(thread)));
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 9, pdlGet(thread, base, 0));
pdlLabel_done:;
pdlReturn:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value v = pdlGet(thread, base, 0); pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return;
}

void
Arcadia_Engine_SceneManager_setScene
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_SceneManager* self,
    Arcadia_Engine_Scene* p0
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_SceneManager_getType(thread)));
  pdlRequire(thread, !p0 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p0, _Arcadia_Engine_Scene_getType(thread)));
  Arcadia_Engine_SceneManager_setScene_impl(thread, self, p0);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_SceneManager* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_SceneManager);
  if (_numberOfArguments != 1) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->engine = (Arcadia_Engine*){0};
  self->scene = (Arcadia_Engine_Scene*){0};
  self->sceneChangedEvent = (Arcadia_Signal*){0};
  Arcadia_Object_setType(thread, (Arcadia_Object*)self, _type);
  { Arcadia_Value args[2];
    args[0] = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_Engine_SceneManager_construct(thread, self, (Arcadia_Engine*)pdlAsObjectReference(thread, args[0], _Arcadia_Engine_getType(thread)));
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_SceneManager);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_SceneManager* self
  )
{
  if (self->engine) Arcadia_Object_visit(thread, (Arcadia_Object*)self->engine);
  if (self->scene) Arcadia_Object_visit(thread, (Arcadia_Object*)self->scene);
  if (self->sceneChangedEvent) Arcadia_Object_visit(thread, (Arcadia_Object*)self->sceneChangedEvent);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_SceneManagerDispatch* self
  )
{
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

Arcadia_defineObjectType(u8"Arcadia.Engine.SceneManager", Arcadia_Engine_SceneManager,
                         u8"Arcadia.Object", Arcadia_Object,
                         &typeOperations);

Arcadia_Engine_SceneManager*
Arcadia_Engine_SceneManager_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* p0
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
  { Arcadia_Value value = pdlBoxObjectReference(p0); Arcadia_ValueStack_pushValue(thread, &value); }
  Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
  { Arcadia_Value value = pdlBoxObjectReference(p0); Arcadia_ValueStack_pushValue(thread, &value); }
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  Arcadia_Engine_SceneManager* result = _Arcadia_EndCreate0(thread, _Arcadia_Engine_SceneManager_getType(thread), argumentsBase);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}
