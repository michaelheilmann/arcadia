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
#include "Arcadia/ADL/Definitions/Visuals/MaterialDefinition.h"

#include "Arcadia/Logging/Include.h"
#include "Arcadia/ADL/Context.h"
#include "Arcadia/ADL/Reader.module.h"
#include "Arcadia/ADL/Reference.h"
#include "Arcadia/ADL/Definitions/Visuals/Include.h"

static void
Arcadia_ADL_MaterialDefinition_linkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialDefinition* self
  );

static void
Arcadia_ADL_MaterialDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialDefinition* self
  );

static void
Arcadia_ADL_MaterialDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialDefinition* self
  );

static void
Arcadia_ADL_MaterialDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialDefinitionDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_MaterialDefinition_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_MaterialDefinition_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_MaterialDefinition_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.MaterialDefinition", Arcadia_ADL_MaterialDefinition,
                         u8"Arcadia.ADL.Definition", Arcadia_ADL_Definition,
                         &_typeOperations);

static void
Arcadia_ADL_MaterialDefinition_linkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialDefinition* self
  )
{
  Arcadia_ADL_Reference_resolve(thread, self->ambientColorTexture);
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self->ambientColorTexture->definition, _Arcadia_ADL_TextureDefinition_getType(thread))) {
    Arcadia_Type* type = Arcadia_Object_getType(thread, (Arcadia_Object*)self->ambientColorTexture->definition);
    Arcadia_Name* typeName = Arcadia_Type_getName(thread, type);
    Arcadia_StringBuilder* stringBuilder = Arcadia_StringBuilder_create(thread);
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8"expected Arcadia.ADL.TextureDefinition, received ");
    Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, Arcadia_Name_getBytes(thread, typeName), Arcadia_Name_getNumberOfBytes(thread, typeName)))));
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8"\n");
    Arcadia_String* string = Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuilder));
    Arcadia_Log_error(thread, (Arcadia_Log*)Arcadia_ConsoleLog_create(thread), string);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static void
Arcadia_ADL_MaterialDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialDefinition* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_ADL_MaterialDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialDefinition* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_MaterialDefinition);
  {
    Arcadia_Value definitions, name;
    definitions = Arcadia_ValueStack_getValue(thread, 5);
    name = Arcadia_ValueStack_getValue(thread, 4);
    Arcadia_ValueStack_pushValue(thread, &definitions);
    Arcadia_ValueStack_pushValue(thread, &name);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (5 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_EnumerationValue materialType = Arcadia_ValueStack_getEnumerationValue(thread, 3),
                           ambientColorSource = Arcadia_ValueStack_getEnumerationValue(thread, 2);
  if (materialType.type != _Arcadia_ADL_MaterialType_getType(thread)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (ambientColorSource.type != _Arcadia_ADL_AmbientColorSource_getType(thread)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->materialType = (Arcadia_ADL_MaterialType)materialType.value;
  self->ambientColorSource = (Arcadia_ADL_AmbientColorSource)ambientColorSource.value;
  self->ambientColorTexture = Arcadia_ADL_Reference_create(thread, ((Arcadia_ADL_Definition*)self)->definitions,
                                                           Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread)));
  Arcadia_LeaveConstructor(Arcadia_ADL_MaterialDefinition);
}

static void
Arcadia_ADL_MaterialDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialDefinitionDispatch* self
  )
{
  ((Arcadia_ADL_DefinitionDispatch*)self)->link = (void (*)(Arcadia_Thread*, Arcadia_ADL_Definition*)) & Arcadia_ADL_MaterialDefinition_linkImpl;
}

Arcadia_ADL_MaterialDefinition*
Arcadia_ADL_MaterialDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_ADL_MaterialType materialType,
    Arcadia_ADL_AmbientColorSource ambientColorSource,
    Arcadia_String* ambientColorTextureName
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_MaterialDefinition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_ADL_MaterialType_getType(thread), materialType));
  Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_ADL_AmbientColorSource_getType(thread), ambientColorSource));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)ambientColorTextureName);
  Arcadia_ValueStack_pushNatural8Value(thread, 5);
  _Arcadia_EndCreate(Arcadia_ADL_MaterialDefinition);
}
