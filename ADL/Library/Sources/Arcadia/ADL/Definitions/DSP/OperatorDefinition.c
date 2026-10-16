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

#define ARCADIA_ADL_PRIVATE (1)
#include "Arcadia/ADL/Definitions/DSP/OperatorDefinition.h"

static void linkImpl(Arcadia_Thread* thread, Arcadia_ADL_DSPOperatorDefinition* self);
static void visitImpl(Arcadia_Thread* thread, Arcadia_ADL_DSPOperatorDefinition* self);
static void constructImpl(Arcadia_Thread* thread, Arcadia_ADL_DSPOperatorDefinition* self);
static void initializeDispatchImpl(Arcadia_Thread* thread, Arcadia_ADL_DSPOperatorDefinitionDispatch* self);

static const Arcadia_ObjectType_Operations objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.DSPOperatorDefinition", Arcadia_ADL_DSPOperatorDefinition,
                         u8"Arcadia.ADL.Definition", Arcadia_ADL_Definition, &typeOperations);

static void
linkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPOperatorDefinition* self
  )
{
  if (self->input) {
    Arcadia_ADL_Reference_resolve(thread, self->input);
    Arcadia_ADL_Definition_link(thread, self->input->definition);
  }
  if (self->inputs) {
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->inputs); i < n; ++i) {
      Arcadia_ADL_Reference* input = (Arcadia_ADL_Reference*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, self->inputs, i, _Arcadia_ADL_Reference_getType(thread));
      Arcadia_ADL_Reference_resolve(thread, input);
      Arcadia_ADL_Definition_link(thread, input->definition);
    }
  }
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPOperatorDefinition* self
  )
{
  if (self->input) Arcadia_Object_visit(thread, (Arcadia_Object*)self->input);
  if (self->inputs) Arcadia_Object_visit(thread, (Arcadia_Object*)self->inputs);
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPOperatorDefinition* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_DSPOperatorDefinition);
  {
    Arcadia_Value definitions = Arcadia_ValueStack_getValue(thread, 6);
    Arcadia_Value name = Arcadia_ValueStack_getValue(thread, 5);
    Arcadia_ValueStack_pushValue(thread, &definitions);
    Arcadia_ValueStack_pushValue(thread, &name);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (6 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->kind = (Arcadia_ADL_DSPOperatorKind)Arcadia_ValueStack_getNatural32Value(thread, 4);
  if (Arcadia_ValueStack_isVoidValue(thread, 3)) self->input = NULL;
  else self->input = (Arcadia_ADL_Reference*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_ADL_Reference_getType(thread));
  if (Arcadia_ValueStack_isVoidValue(thread, 2)) self->inputs = NULL;
  else self->inputs = (Arcadia_List*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Object_getType(thread));
  self->parameter = Arcadia_ValueStack_getReal32Value(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_ADL_DSPOperatorDefinition);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPOperatorDefinitionDispatch* self
  )
{
  ((Arcadia_ADL_DefinitionDispatch*)self)->link = (void(*)(Arcadia_Thread*, Arcadia_ADL_Definition*))&linkImpl;
}

Arcadia_ADL_DSPOperatorDefinition*
Arcadia_ADL_DSPOperatorDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_ADL_DSPOperatorKind kind,
    Arcadia_ADL_Reference* input,
    Arcadia_List* inputs,
    Arcadia_Real32Value parameter
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_DSPOperatorDefinition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushNatural32Value(thread, (Arcadia_Natural32Value)kind);
  if (input) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)input); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (inputs) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)inputs); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushReal32Value(thread, parameter);
  Arcadia_ValueStack_pushNatural8Value(thread, 6);
  _Arcadia_EndCreate(Arcadia_ADL_DSPOperatorDefinition);
}
