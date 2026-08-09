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
#include "Arcadia/ADL/Definitions/Visuals/CheckerboardFillOperationDefinition.h"

#include "Arcadia/ADL/Context.h"
#include "Arcadia/ADL/Reader.module.h"
#include "Arcadia/ADL/Reference.h"
#include "Arcadia/ADL/Definitions/Visuals/Include.h"

static void
Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_linkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition* self
  );

static void
Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition* self
  );

static void
Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition* self
  );

static void
Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinitionDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.PixelBufferOperations.CheckerboardFillOperationDefinition", Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition,
                         u8"Arcadia.ADL.Definition", Arcadia_ADL_Definition,
                         &_typeOperations);

static void
Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_linkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition* self
  )
{
  Arcadia_ADL_Reference_resolve(thread, self->firstCheckerColor);
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self->firstCheckerColor->definition, _Arcadia_ADL_ColorDefinition_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ADL_Reference_resolve(thread, self->secondCheckerColor);
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self->secondCheckerColor->definition, _Arcadia_ADL_ColorDefinition_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static void
Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition* self
  )
{
  if (self->firstCheckerColor) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->firstCheckerColor);
  }
  if (self->secondCheckerColor) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->secondCheckerColor);
  }
}

static void
Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition);
  {
    Arcadia_Value definitions, name;
    definitions = Arcadia_ValueStack_getValue(thread, 6);
    name = Arcadia_ValueStack_getValue(thread, 5);
    Arcadia_ValueStack_pushValue(thread, &definitions);
    Arcadia_ValueStack_pushValue(thread, &name);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (6 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  self->checkerWidth = Arcadia_ValueStack_getInteger32Value(thread, 4);
  self->checkerHeight = Arcadia_ValueStack_getInteger32Value(thread, 3);

  self->firstCheckerColor = Arcadia_ADL_Reference_create(thread, ((Arcadia_ADL_Definition*)self)->definitions,
                                                        (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_String_getType(thread)));
  self->secondCheckerColor = Arcadia_ADL_Reference_create(thread, ((Arcadia_ADL_Definition*)self)->definitions,
                                                         (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread)));

  Arcadia_LeaveConstructor(Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition);
}

static void
Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinitionDispatch* self
  )
{
  ((Arcadia_ADL_DefinitionDispatch*)self)->link = (void (*)(Arcadia_Thread*, Arcadia_ADL_Definition*)) & Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_linkImpl;
}

Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition*
Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_Integer32Value checkerWidth,
    Arcadia_Integer32Value checkerHeight,
    Arcadia_String* firstColorName,
    Arcadia_String* secondColorName
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushInteger32Value(thread, checkerWidth);
  Arcadia_ValueStack_pushInteger32Value(thread, checkerHeight);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)firstColorName);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)secondColorName);
  Arcadia_ValueStack_pushNatural8Value(thread, 6);
  _Arcadia_EndCreate(Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition);
}
