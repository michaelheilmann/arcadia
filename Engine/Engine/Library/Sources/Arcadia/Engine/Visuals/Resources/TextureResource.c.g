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
#include "Arcadia/Engine/Visuals/Resources/TextureResource.h"
#include "Arcadia/Engine/Visuals/BackendContext.h"
#include "Arcadia/Engine/Visuals/Resource.h"
#include "Arcadia/Engine/Visuals/TextureAddressMode.h"
#include "Arcadia/Engine/Visuals/TextureFilter.h"
#include "Arcadia/Media/Include.h"

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

static void
Arcadia_Engine_Visuals_TextureResource_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Engine_Visuals_BackendContext* p0
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
  (void)((Arcadia_Engine_Visuals_BackendContext*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Engine_Visuals_BackendContext_getType(thread)));
  pdlPut(thread, base, 1, pdlGet(thread, base, 5));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 4); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[2];
    args[0] = Arcadia_ValueStack_getValue(thread, 0);
    Arcadia_Value receiver = pdlGet(thread, base, 1);
    pdlRequire(thread, Arcadia_Value_isObjectReferenceValue(&receiver) && Arcadia_Value_getObjectReferenceValue(&receiver) == (Arcadia_Object*)self);
    Arcadia_ValueStack_pushValue(thread, &args[0]);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_Type_getOperations(_Arcadia_Engine_Visuals_Resource_getType(thread))->objectTypeOperations->construct(thread, (Arcadia_Object*)self);
    Arcadia_Object_setType(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_TextureResource_getType(thread));
  pdlPut(thread, base, 0, receiver);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 6, pdlGet(thread, base, 0));
pdlReturn:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value v = pdlGet(thread, base, 0); pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return;
}

Arcadia_Engine_Visuals_TextureAddressMode
Arcadia_Engine_Visuals_TextureResource_getAddressModeU
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_TextureResource_getType(thread)));
  Arcadia_Engine_Visuals_TextureResourceDispatch* dispatch = (Arcadia_Engine_Visuals_TextureResourceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->getAddressModeU) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_Engine_Visuals_TextureAddressMode result = dispatch->getAddressModeU(thread, (Arcadia_Engine_Visuals_TextureResource*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

Arcadia_Engine_Visuals_TextureAddressMode
Arcadia_Engine_Visuals_TextureResource_getAddressModeV
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_TextureResource_getType(thread)));
  Arcadia_Engine_Visuals_TextureResourceDispatch* dispatch = (Arcadia_Engine_Visuals_TextureResourceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->getAddressModeV) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_Engine_Visuals_TextureAddressMode result = dispatch->getAddressModeV(thread, (Arcadia_Engine_Visuals_TextureResource*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

Arcadia_Integer32Value
Arcadia_Engine_Visuals_TextureResource_getHeight
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_TextureResource_getType(thread)));
  Arcadia_Engine_Visuals_TextureResourceDispatch* dispatch = (Arcadia_Engine_Visuals_TextureResourceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->getHeight) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_Integer32Value result = dispatch->getHeight(thread, (Arcadia_Engine_Visuals_TextureResource*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

Arcadia_Engine_Visuals_TextureFilter
Arcadia_Engine_Visuals_TextureResource_getMagnificationFilter
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_TextureResource_getType(thread)));
  Arcadia_Engine_Visuals_TextureResourceDispatch* dispatch = (Arcadia_Engine_Visuals_TextureResourceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->getMagnificationFilter) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_Engine_Visuals_TextureFilter result = dispatch->getMagnificationFilter(thread, (Arcadia_Engine_Visuals_TextureResource*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

Arcadia_Engine_Visuals_TextureFilter
Arcadia_Engine_Visuals_TextureResource_getMinificationFilter
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_TextureResource_getType(thread)));
  Arcadia_Engine_Visuals_TextureResourceDispatch* dispatch = (Arcadia_Engine_Visuals_TextureResourceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->getMinificationFilter) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_Engine_Visuals_TextureFilter result = dispatch->getMinificationFilter(thread, (Arcadia_Engine_Visuals_TextureResource*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

Arcadia_Integer32Value
Arcadia_Engine_Visuals_TextureResource_getWidth
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_TextureResource_getType(thread)));
  Arcadia_Engine_Visuals_TextureResourceDispatch* dispatch = (Arcadia_Engine_Visuals_TextureResourceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->getWidth) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_Integer32Value result = dispatch->getWidth(thread, (Arcadia_Engine_Visuals_TextureResource*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

void
Arcadia_Engine_Visuals_TextureResource_setAddressModeU
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Engine_Visuals_TextureAddressMode p0
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_TextureResource_getType(thread)));
  Arcadia_Engine_Visuals_TextureResourceDispatch* dispatch = (Arcadia_Engine_Visuals_TextureResourceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->setAddressModeU) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  dispatch->setAddressModeU(thread, (Arcadia_Engine_Visuals_TextureResource*)self, p0);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

void
Arcadia_Engine_Visuals_TextureResource_setAddressModeV
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Engine_Visuals_TextureAddressMode p0
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_TextureResource_getType(thread)));
  Arcadia_Engine_Visuals_TextureResourceDispatch* dispatch = (Arcadia_Engine_Visuals_TextureResourceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->setAddressModeV) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  dispatch->setAddressModeV(thread, (Arcadia_Engine_Visuals_TextureResource*)self, p0);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

void
Arcadia_Engine_Visuals_TextureResource_setHeight
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Integer32Value p0
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_TextureResource_getType(thread)));
  Arcadia_Engine_Visuals_TextureResourceDispatch* dispatch = (Arcadia_Engine_Visuals_TextureResourceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->setHeight) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  dispatch->setHeight(thread, (Arcadia_Engine_Visuals_TextureResource*)self, p0);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

void
Arcadia_Engine_Visuals_TextureResource_setMagnificationFilter
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Engine_Visuals_TextureFilter p0
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_TextureResource_getType(thread)));
  Arcadia_Engine_Visuals_TextureResourceDispatch* dispatch = (Arcadia_Engine_Visuals_TextureResourceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->setMagnificationFilter) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  dispatch->setMagnificationFilter(thread, (Arcadia_Engine_Visuals_TextureResource*)self, p0);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

void
Arcadia_Engine_Visuals_TextureResource_setMinificationFilter
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Engine_Visuals_TextureFilter p0
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_TextureResource_getType(thread)));
  Arcadia_Engine_Visuals_TextureResourceDispatch* dispatch = (Arcadia_Engine_Visuals_TextureResourceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->setMinificationFilter) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  dispatch->setMinificationFilter(thread, (Arcadia_Engine_Visuals_TextureResource*)self, p0);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

void
Arcadia_Engine_Visuals_TextureResource_setWidth
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Integer32Value p0
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_TextureResource_getType(thread)));
  Arcadia_Engine_Visuals_TextureResourceDispatch* dispatch = (Arcadia_Engine_Visuals_TextureResourceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->setWidth) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  dispatch->setWidth(thread, (Arcadia_Engine_Visuals_TextureResource*)self, p0);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

void
Arcadia_Engine_Visuals_TextureResource_setPixelBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Media_PixelBuffer* p0
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_TextureResource_getType(thread)));
  Arcadia_Engine_Visuals_TextureResourceDispatch* dispatch = (Arcadia_Engine_Visuals_TextureResourceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->setPixelBuffer) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  pdlRequire(thread, !p0 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p0, _Arcadia_Media_PixelBuffer_getType(thread)));
  dispatch->setPixelBuffer(thread, (Arcadia_Engine_Visuals_TextureResource*)self, p0);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_TextureResource);
  if (_numberOfArguments != 1) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  { Arcadia_Value args[2];
    args[0] = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_Engine_Visuals_TextureResource_construct(thread, self, (Arcadia_Engine_Visuals_BackendContext*)pdlAsObjectReference(thread, args[0], _Arcadia_Engine_Visuals_BackendContext_getType(thread)));
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_TextureResource);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
  )
{
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResourceDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->getAddressModeU = NULL;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->getAddressModeV = NULL;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->getHeight = NULL;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->getMagnificationFilter = NULL;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->getMinificationFilter = NULL;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->getWidth = NULL;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->setAddressModeU = NULL;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->setAddressModeV = NULL;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->setHeight = NULL;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->setMagnificationFilter = NULL;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->setMinificationFilter = NULL;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->setWidth = NULL;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->setPixelBuffer = NULL;
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

Arcadia_defineObjectType(u8"Arcadia.Engine.Visuals.TextureResource", Arcadia_Engine_Visuals_TextureResource,
                         u8"Arcadia.Engine.Visuals.Resource", Arcadia_Engine_Visuals_Resource,
                         &typeOperations);
