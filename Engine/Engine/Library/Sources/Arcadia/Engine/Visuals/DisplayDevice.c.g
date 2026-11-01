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
#include "Arcadia/Engine/Visuals/DisplayDevice.h"
#include "Arcadia/Engine/Visuals/DisplayMode.h"
#include "Arcadia/Collections/Include.h"

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

Arcadia_Engine_Visuals_DisplayMode*
Arcadia_Engine_Visuals_DisplayDevice_getCurrentDisplayMode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_DisplayDevice_getType(thread)));
  Arcadia_Engine_Visuals_DisplayDeviceDispatch* dispatch = (Arcadia_Engine_Visuals_DisplayDeviceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->getCurrentDisplayMode) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_Engine_Visuals_DisplayMode* result = dispatch->getCurrentDisplayMode(thread, (Arcadia_Engine_Visuals_DisplayDevice*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

Arcadia_List*
Arcadia_Engine_Visuals_DisplayDevice_getAvailableDisplayModes
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_DisplayDevice_getType(thread)));
  Arcadia_Engine_Visuals_DisplayDeviceDispatch* dispatch = (Arcadia_Engine_Visuals_DisplayDeviceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->getAvailableDisplayModes) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_List* result = dispatch->getAvailableDisplayModes(thread, (Arcadia_Engine_Visuals_DisplayDevice*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

void
Arcadia_Engine_Visuals_DisplayDevice_getBounds
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self,
    Arcadia_Integer32Value* p0,
    Arcadia_Integer32Value* p1,
    Arcadia_Integer32Value* p2,
    Arcadia_Integer32Value* p3
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_DisplayDevice_getType(thread)));
  Arcadia_Engine_Visuals_DisplayDeviceDispatch* dispatch = (Arcadia_Engine_Visuals_DisplayDeviceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->getBounds) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  dispatch->getBounds(thread, (Arcadia_Engine_Visuals_DisplayDevice*)self, p0, p1, p2, p3);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

Arcadia_String*
Arcadia_Engine_Visuals_DisplayDevice_getName
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_DisplayDevice_getType(thread)));
  Arcadia_Engine_Visuals_DisplayDeviceDispatch* dispatch = (Arcadia_Engine_Visuals_DisplayDeviceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->getName) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_String* result = dispatch->getName(thread, (Arcadia_Engine_Visuals_DisplayDevice*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

Arcadia_String*
Arcadia_Engine_Visuals_DisplayDevice_getId
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_DisplayDevice_getType(thread)));
  Arcadia_Engine_Visuals_DisplayDeviceDispatch* dispatch = (Arcadia_Engine_Visuals_DisplayDeviceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->getId) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_String* result = dispatch->getId(thread, (Arcadia_Engine_Visuals_DisplayDevice*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_DisplayDevice);
  if (_numberOfArguments != 0) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  Arcadia_Object_setType(thread, (Arcadia_Object*)self, _type);
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_DisplayDevice);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self
  )
{
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDeviceDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_DisplayDeviceDispatch*)self)->getCurrentDisplayMode = NULL;
  ((Arcadia_Engine_Visuals_DisplayDeviceDispatch*)self)->getAvailableDisplayModes = NULL;
  ((Arcadia_Engine_Visuals_DisplayDeviceDispatch*)self)->getBounds = NULL;
  ((Arcadia_Engine_Visuals_DisplayDeviceDispatch*)self)->getName = NULL;
  ((Arcadia_Engine_Visuals_DisplayDeviceDispatch*)self)->getId = NULL;
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

Arcadia_defineObjectType(u8"Arcadia.Engine.Visuals.DisplayDevice", Arcadia_Engine_Visuals_DisplayDevice,
                         u8"Arcadia.Object", Arcadia_Object,
                         &typeOperations);
