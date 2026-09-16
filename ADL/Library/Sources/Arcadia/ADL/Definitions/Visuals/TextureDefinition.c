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

#define ARCADIA_ADL_MODULE (1)
#include "Arcadia/ADL/Definitions/Visuals/TextureDefinition.h"

#include "Arcadia/ADL/Context.h"
#include "Arcadia/ADL/Reader.module.h"
#include "Arcadia/ADL/Reference.h"
#include "Arcadia/ADL/Definitions/Visuals/Include.h"

static void
Arcadia_ADL_TextureDefinition_linkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureDefinition* self
  );

static void
Arcadia_ADL_TextureDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureDefinition* self
  );

static void
Arcadia_ADL_TextureDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureDefinition* self
  );

static void
Arcadia_ADL_TextureDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureDefinitionDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_TextureDefinition_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_TextureDefinition_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_TextureDefinition_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.TextureDefinition", Arcadia_ADL_TextureDefinition,
                         u8"Arcadia.ADL.Definition", Arcadia_ADL_Definition,
                         &_typeOperations);

static void
Arcadia_ADL_TextureDefinition_linkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureDefinition* self
  )
{
  Arcadia_ADL_Reference_resolve(thread, self->pixelBuffer);
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self->pixelBuffer->definition, _Arcadia_ADL_PixelBufferDefinition_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ADL_Definition_link(thread, self->pixelBuffer->definition);
}

static void
Arcadia_ADL_TextureDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureDefinition* self
  )
{
  if (self->pixelBuffer) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->pixelBuffer);
  }
}

static void
Arcadia_ADL_TextureDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureDefinition* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_TextureDefinition);
  {
    Arcadia_Value definitions, name;
    definitions = Arcadia_ValueStack_getValue(thread, 7);
    name = Arcadia_ValueStack_getValue(thread, 6);
    Arcadia_ValueStack_pushValue(thread, &definitions);
    Arcadia_ValueStack_pushValue(thread, &name);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (7 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_EnumerationValue magnificationFilter = Arcadia_ValueStack_getEnumerationValue(thread, 4),
                           minificationFilter = Arcadia_ValueStack_getEnumerationValue(thread, 3),
                           addressModeU = Arcadia_ValueStack_getEnumerationValue(thread, 2),
                           addressModeV = Arcadia_ValueStack_getEnumerationValue(thread, 1);
  if (magnificationFilter.type != _Arcadia_ADL_TextureFilter_getType(thread)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (minificationFilter.type != _Arcadia_ADL_TextureFilter_getType(thread)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (addressModeU.type != _Arcadia_ADL_TextureAddressMode_getType(thread)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (addressModeV.type != _Arcadia_ADL_TextureAddressMode_getType(thread)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->pixelBuffer = Arcadia_ADL_Reference_create(thread, ((Arcadia_ADL_Definition*)self)->definitions,
                                                   Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 5, _Arcadia_String_getType(thread)));
  self->magnificationFilter = (Arcadia_ADL_TextureFilter)magnificationFilter.value;
  self->minificationFilter = (Arcadia_ADL_TextureFilter)minificationFilter.value;
  self->addressModeU = (Arcadia_ADL_TextureAddressMode)addressModeU.value;
  self->addressModeV = (Arcadia_ADL_TextureAddressMode)addressModeV.value;
  Arcadia_LeaveConstructor(Arcadia_ADL_TextureDefinition);
}

static void
Arcadia_ADL_TextureDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureDefinitionDispatch* self
  )
{
  ((Arcadia_ADL_DefinitionDispatch*)self)->link = (void (*)(Arcadia_Thread*, Arcadia_ADL_Definition*)) & Arcadia_ADL_TextureDefinition_linkImpl;
}

Arcadia_ADL_TextureDefinition*
Arcadia_ADL_TextureDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_String* pixelBufferName,
    Arcadia_ADL_TextureFilter magnificationFilter,
    Arcadia_ADL_TextureFilter minificationFilter,
    Arcadia_ADL_TextureAddressMode addressModeU,
    Arcadia_ADL_TextureAddressMode addressModeV
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_TextureDefinition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)pixelBufferName);
  Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_ADL_TextureFilter_getType(thread), magnificationFilter));
  Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_ADL_TextureFilter_getType(thread), minificationFilter));
  Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_ADL_TextureAddressMode_getType(thread), addressModeU));
  Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_ADL_TextureAddressMode_getType(thread), addressModeV));
  Arcadia_ValueStack_pushNatural8Value(thread, 7);
  _Arcadia_EndCreate(Arcadia_ADL_TextureDefinition);
}
