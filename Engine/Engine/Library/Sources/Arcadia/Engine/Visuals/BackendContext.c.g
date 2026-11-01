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
#include "Arcadia/Engine/Visuals/BackendContext.h"
#include "Arcadia/Engine/BackendContext.h"
#include "Arcadia/Engine/Visuals/BlendFunction.h"
#include "Arcadia/Engine/Visuals/ColorSource.h"
#include "Arcadia/Engine/Visuals/Resources/ConstantBufferResource.h"
#include "Arcadia/Engine/Visuals/Resources/EnterPassResource.h"
#include "Arcadia/Engine/Visuals/Resources/FrameBufferResource.h"
#include "Arcadia/Engine/Visuals/Icon.h"
#include "Arcadia/Engine/Visuals/Resources/MaterialResource.h"
#include "Arcadia/Engine/Visuals/Resources/ModelResource.h"
#include "Arcadia/Engine/Visuals/Resources/ProgramResource.h"
#include "Arcadia/Engine/Visuals/Resources/TextureResource.h"
#include "Arcadia/Engine/Visuals/Resources/VertexBufferResource.h"
#include "Arcadia/Engine/Visuals/Window.h"
#include "Arcadia/Engine/Visuals/createVisualsWindowsList.h"
#include "Arcadia/Collections/Include.h"
#include "Arcadia/Media/Include.h"
#include "Arcadia/VPL/Include.h"

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
Arcadia_Engine_Visuals_BackendContext_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
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
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    Arcadia_Value args[1];
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_Engine_Visuals_createVisualsWindowsList(thread)));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 5, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 4));
  pdlPut(thread, base, 2, pdlGet(thread, base, 5));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Visuals_BackendContext_getType(thread))) {
      ((Arcadia_Engine_Visuals_BackendContext*)Arcadia_Value_getObjectReferenceValue(&receiver))->windows = (Arcadia_List*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_List_getType(thread));
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

Arcadia_Engine_Visuals_Icon*
Arcadia_Engine_Visuals_BackendContext_createIcon
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self,
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_BackendContext_getType(thread)));
  Arcadia_Engine_Visuals_BackendContextDispatch* dispatch = (Arcadia_Engine_Visuals_BackendContextDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->createIcon) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  pdlRequire(thread, !p0 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p0, _Arcadia_Media_PixelBuffer_getType(thread)));
  Arcadia_Engine_Visuals_Icon* result = dispatch->createIcon(thread, (Arcadia_Engine_Visuals_BackendContext*)self, p0);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

Arcadia_Engine_Visuals_Window*
Arcadia_Engine_Visuals_BackendContext_createWindow
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_BackendContext_getType(thread)));
  Arcadia_Engine_Visuals_BackendContextDispatch* dispatch = (Arcadia_Engine_Visuals_BackendContextDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->createWindow) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_Engine_Visuals_Window* result = dispatch->createWindow(thread, (Arcadia_Engine_Visuals_BackendContext*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

Arcadia_List*
Arcadia_Engine_Visuals_BackendContext_getDisplayDevices
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_BackendContext_getType(thread)));
  Arcadia_Engine_Visuals_BackendContextDispatch* dispatch = (Arcadia_Engine_Visuals_BackendContextDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->getDisplayDevices) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_List* result = dispatch->getDisplayDevices(thread, (Arcadia_Engine_Visuals_BackendContext*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

Arcadia_Engine_Visuals_ConstantBufferResource*
Arcadia_Engine_Visuals_BackendContext_createConstantBufferResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_BackendContext_getType(thread)));
  Arcadia_Engine_Visuals_BackendContextDispatch* dispatch = (Arcadia_Engine_Visuals_BackendContextDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->createConstantBufferResource) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_Engine_Visuals_ConstantBufferResource* result = dispatch->createConstantBufferResource(thread, (Arcadia_Engine_Visuals_BackendContext*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

Arcadia_Engine_Visuals_FrameBufferResource*
Arcadia_Engine_Visuals_BackendContext_createFrameBufferResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_BackendContext_getType(thread)));
  Arcadia_Engine_Visuals_BackendContextDispatch* dispatch = (Arcadia_Engine_Visuals_BackendContextDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->createFrameBufferResource) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_Engine_Visuals_FrameBufferResource* result = dispatch->createFrameBufferResource(thread, (Arcadia_Engine_Visuals_BackendContext*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

Arcadia_Engine_Visuals_EnterPassResource*
Arcadia_Engine_Visuals_BackendContext_createEnterPassResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_BackendContext_getType(thread)));
  Arcadia_Engine_Visuals_BackendContextDispatch* dispatch = (Arcadia_Engine_Visuals_BackendContextDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->createEnterPassResource) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_Engine_Visuals_EnterPassResource* result = dispatch->createEnterPassResource(thread, (Arcadia_Engine_Visuals_BackendContext*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

Arcadia_Engine_Visuals_MaterialResource*
Arcadia_Engine_Visuals_BackendContext_createMaterialResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self,
    Arcadia_BooleanValue p0,
    Arcadia_Engine_Visuals_BlendFunction p1,
    Arcadia_Engine_Visuals_BlendFunction p2,
    Arcadia_Engine_Visuals_ColorSource p3,
    Arcadia_Engine_Visuals_TextureResource* p4,
    Arcadia_Engine_Visuals_ProgramResource* p5
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
  { Arcadia_Value value = pdlBoxObjectReference(p4); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p5); Arcadia_ValueStack_pushValue(thread, &value); }
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_BackendContext_getType(thread)));
  Arcadia_Engine_Visuals_BackendContextDispatch* dispatch = (Arcadia_Engine_Visuals_BackendContextDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->createMaterialResource) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  pdlRequire(thread, !p4 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p4, _Arcadia_Engine_Visuals_TextureResource_getType(thread)));
  pdlRequire(thread, !p5 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p5, _Arcadia_Engine_Visuals_ProgramResource_getType(thread)));
  Arcadia_Engine_Visuals_MaterialResource* result = dispatch->createMaterialResource(thread, (Arcadia_Engine_Visuals_BackendContext*)self, p0, p1, p2, p3, p4, p5);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

Arcadia_Engine_Visuals_ModelResource*
Arcadia_Engine_Visuals_BackendContext_createModelResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self,
    Arcadia_Engine_Visuals_ConstantBufferResource* p0,
    Arcadia_Engine_Visuals_VertexBufferResource* p1,
    Arcadia_Engine_Visuals_MaterialResource* p2
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
  { Arcadia_Value value = pdlBoxObjectReference(p1); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p2); Arcadia_ValueStack_pushValue(thread, &value); }
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_BackendContext_getType(thread)));
  Arcadia_Engine_Visuals_BackendContextDispatch* dispatch = (Arcadia_Engine_Visuals_BackendContextDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->createModelResource) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  pdlRequire(thread, !p0 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p0, _Arcadia_Engine_Visuals_ConstantBufferResource_getType(thread)));
  pdlRequire(thread, !p1 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p1, _Arcadia_Engine_Visuals_VertexBufferResource_getType(thread)));
  pdlRequire(thread, !p2 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p2, _Arcadia_Engine_Visuals_MaterialResource_getType(thread)));
  Arcadia_Engine_Visuals_ModelResource* result = dispatch->createModelResource(thread, (Arcadia_Engine_Visuals_BackendContext*)self, p0, p1, p2);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

Arcadia_Engine_Visuals_ProgramResource*
Arcadia_Engine_Visuals_BackendContext_createProgramResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self,
    Arcadia_VPL_VerifiedModule* p0
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_BackendContext_getType(thread)));
  Arcadia_Engine_Visuals_BackendContextDispatch* dispatch = (Arcadia_Engine_Visuals_BackendContextDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->createProgramResource) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  pdlRequire(thread, !p0 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p0, _Arcadia_VPL_VerifiedModule_getType(thread)));
  Arcadia_Engine_Visuals_ProgramResource* result = dispatch->createProgramResource(thread, (Arcadia_Engine_Visuals_BackendContext*)self, p0);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

Arcadia_Engine_Visuals_TextureResource*
Arcadia_Engine_Visuals_BackendContext_createTextureResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_BackendContext_getType(thread)));
  Arcadia_Engine_Visuals_BackendContextDispatch* dispatch = (Arcadia_Engine_Visuals_BackendContextDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->createTextureResource) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_Engine_Visuals_TextureResource* result = dispatch->createTextureResource(thread, (Arcadia_Engine_Visuals_BackendContext*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

Arcadia_Engine_Visuals_VertexBufferResource*
Arcadia_Engine_Visuals_BackendContext_createVertexBufferResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_BackendContext_getType(thread)));
  Arcadia_Engine_Visuals_BackendContextDispatch* dispatch = (Arcadia_Engine_Visuals_BackendContextDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->createVertexBufferResource) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_Engine_Visuals_VertexBufferResource* result = dispatch->createVertexBufferResource(thread, (Arcadia_Engine_Visuals_BackendContext*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_BackendContext);
  if (_numberOfArguments != 0) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->windows = (Arcadia_List*){0};
  Arcadia_Object_setType(thread, (Arcadia_Object*)self, _type);
  { Arcadia_Value args[1];
    Arcadia_Engine_Visuals_BackendContext_construct(thread, self);
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_BackendContext);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  )
{
  if (self->windows) Arcadia_Object_visit(thread, (Arcadia_Object*)self->windows);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContextDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_BackendContextDispatch*)self)->createIcon = NULL;
  ((Arcadia_Engine_Visuals_BackendContextDispatch*)self)->createWindow = NULL;
  ((Arcadia_Engine_Visuals_BackendContextDispatch*)self)->getDisplayDevices = NULL;
  ((Arcadia_Engine_Visuals_BackendContextDispatch*)self)->createConstantBufferResource = NULL;
  ((Arcadia_Engine_Visuals_BackendContextDispatch*)self)->createFrameBufferResource = NULL;
  ((Arcadia_Engine_Visuals_BackendContextDispatch*)self)->createEnterPassResource = NULL;
  ((Arcadia_Engine_Visuals_BackendContextDispatch*)self)->createMaterialResource = NULL;
  ((Arcadia_Engine_Visuals_BackendContextDispatch*)self)->createModelResource = NULL;
  ((Arcadia_Engine_Visuals_BackendContextDispatch*)self)->createProgramResource = NULL;
  ((Arcadia_Engine_Visuals_BackendContextDispatch*)self)->createTextureResource = NULL;
  ((Arcadia_Engine_Visuals_BackendContextDispatch*)self)->createVertexBufferResource = NULL;
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

Arcadia_defineObjectType(u8"Arcadia.Engine.Visuals.BackendContext", Arcadia_Engine_Visuals_BackendContext,
                         u8"Arcadia.Engine.BackendContext", Arcadia_Engine_BackendContext,
                         &typeOperations);
