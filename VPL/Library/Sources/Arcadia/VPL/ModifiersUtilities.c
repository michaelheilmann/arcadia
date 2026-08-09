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

#define ARCADIA_VPL_PRIVATE (1)
#include "Arcadia/VPL/ModifiersUtilities.h"

#include "Arcadia/Languages/Include.h"

static Arcadia_String*
S
  (
    Arcadia_Thread* thread,
    const char* string
  )
{ return Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), string); }

static void
Arcadia_VPL_ModifiersUtilities_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ModifiersUtilities* self
  );

static void
Arcadia_VPL_ModifiersUtilities_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ModifiersUtilitiesDispatch* self
  );

static void
Arcadia_VPL_ModifiersUtilities_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ModifiersUtilities* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_ModifiersUtilities_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_ModifiersUtilities_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_ModifiersUtilities_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.ModifieresUtilities", Arcadia_VPL_ModifiersUtilities,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_VPL_ModifiersUtilities_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ModifiersUtilities* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_ModifiersUtilities);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->temporary = (Arcadia_Set*)Arcadia_HashSet_create(thread);
  self->BUILTIN = S(thread, u8"builtin");
  self->CONSTANT = S(thread, u8"constant");
  self->FRAGMENT = S(thread, u8"fragment");
  self->PIXEL = S(thread, u8"pixel");
  self->POSITION = S(thread, u8"position");
  self->VARIABLE = S(thread, u8"variable");
  self->VERTEX = S(thread, u8"vertex");
  self->ALL = (Arcadia_Set*)Arcadia_HashSet_create(thread);
  Arcadia_Set_add(thread, self->ALL, Arcadia_Value_makeObjectReferenceValue(self->BUILTIN), NULL);
  Arcadia_Set_add(thread, self->ALL, Arcadia_Value_makeObjectReferenceValue(self->CONSTANT), NULL);
  Arcadia_Set_add(thread, self->ALL, Arcadia_Value_makeObjectReferenceValue(self->FRAGMENT), NULL);
  Arcadia_Set_add(thread, self->ALL, Arcadia_Value_makeObjectReferenceValue(self->PIXEL), NULL);
  Arcadia_Set_add(thread, self->ALL, Arcadia_Value_makeObjectReferenceValue(self->POSITION), NULL);
  Arcadia_Set_add(thread, self->ALL, Arcadia_Value_makeObjectReferenceValue(self->VARIABLE), NULL);
  Arcadia_Set_add(thread, self->ALL, Arcadia_Value_makeObjectReferenceValue(self->VERTEX), NULL);
  Arcadia_LeaveConstructor(Arcadia_VPL_ModifiersUtilities);
}

static void
Arcadia_VPL_ModifiersUtilities_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ModifiersUtilitiesDispatch* self
  )
{ }

static void
Arcadia_VPL_ModifiersUtilities_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ModifiersUtilities* self
  )
{
  if (self->temporary) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->temporary);
  }
  if (self->ALL) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->ALL);
  }

  if (self->BUILTIN) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->BUILTIN);
  }
  if (self->CONSTANT) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->CONSTANT);
  }
  if (self->FRAGMENT) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->FRAGMENT);
  }
  if (self->PIXEL) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->PIXEL);
  }
  if (self->POSITION) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->POSITION);
  }
  if (self->VARIABLE) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->VARIABLE);
  }
  if (self->VERTEX) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->VERTEX);
  }
}

Arcadia_VPL_ModifiersUtilities*
Arcadia_VPL_ModifiersUtilities_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_ModifiersUtilities);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_VPL_ModifiersUtilities);
}

Arcadia_Natural8Value
Arcadia_VPL_ModifiersUtilities_variableScalarModifiers
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ModifiersUtilities* self,
    Arcadia_List* modifiers
  )
{
  Arcadia_Natural8Value flags = 0;
  // Assert no modifier is specified twice.
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)self->temporary);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)modifiers); i < n; ++i) {
    Arcadia_Value v = Arcadia_List_getAt(thread, modifiers, i);
    if (Arcadia_Set_contains(thread, self->temporary, v)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Set_add(thread, self->temporary, v, NULL);
  }
  // Assert the set contains the "variable" modifier.
  if (!Arcadia_Set_contains(thread, self->temporary, Arcadia_Value_makeObjectReferenceValue(self->VARIABLE))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  // Assert: Exactly one of `fragment`, `pixel`, and `vertex``must be specified.
  Arcadia_Natural8Value allStageFlags = Arcadia_VPL_Symbols_VariableScalarFlags_FragmentProcessing
                                      | Arcadia_VPL_Symbols_VariableScalarFlags_PixelProcessing
                                      | Arcadia_VPL_Symbols_VariableScalarFlags_VertexProcessing;
  if (Arcadia_Set_contains(thread, self->temporary, Arcadia_Value_makeObjectReferenceValue(self->FRAGMENT))) {
    flags |= Arcadia_VPL_Symbols_VariableScalarFlags_FragmentProcessing;
  }
  if (Arcadia_Set_contains(thread, self->temporary, Arcadia_Value_makeObjectReferenceValue(self->PIXEL))) {
    if (flags & allStageFlags) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    flags |= Arcadia_VPL_Symbols_VariableScalarFlags_PixelProcessing;
  }
  if (Arcadia_Set_contains(thread, self->temporary, Arcadia_Value_makeObjectReferenceValue(self->VERTEX))) {
    if (flags & allStageFlags) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    flags |= Arcadia_VPL_Symbols_VariableScalarFlags_VertexProcessing;
  }
  if (!flags) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  if (Arcadia_Set_contains(thread, self->temporary, Arcadia_Value_makeObjectReferenceValue(self->POSITION))) {
    flags |= Arcadia_VPL_Symbols_VariableScalarFlags_PositionSemantics;
  }

  return flags;
}

Arcadia_Natural8Value
Arcadia_VPL_ModifiersUtilities_procedureModifiers
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ModifiersUtilities* self,
    Arcadia_List* modifiers
  )
{
  Arcadia_Natural8Value flags = 0;
  // Assert no modifier is specified twice.
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)self->temporary);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)modifiers); i < n; ++i) {
    Arcadia_Value v = Arcadia_List_getAt(thread, modifiers, i);
    if (Arcadia_Set_contains(thread, self->temporary, v)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Set_add(thread, self->temporary, v, NULL);
  }
  // Assert: `builtin` cannnot be combined with any other flag.
  // If `builtin` is not specified, then at exactly one of `fragment`, `pixel`, and `vertex``must be specified.
  Arcadia_Natural8Value allStageFlags = Arcadia_VPL_Symbols_ProcedureFlags_FragmentStage
                                      | Arcadia_VPL_Symbols_ProcedureFlags_PixelStage
                                      | Arcadia_VPL_Symbols_ProcedureFlags_VertexStage;
  if (Arcadia_Set_contains(thread, self->temporary, Arcadia_Value_makeObjectReferenceValue(self->FRAGMENT))) {
    flags |= Arcadia_VPL_Symbols_ProcedureFlags_FragmentStage;
  }
  if (Arcadia_Set_contains(thread, self->temporary, Arcadia_Value_makeObjectReferenceValue(self->PIXEL))) {
    if (flags & allStageFlags) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    flags |= Arcadia_VPL_Symbols_ProcedureFlags_PixelStage;
  }
  if (Arcadia_Set_contains(thread, self->temporary, Arcadia_Value_makeObjectReferenceValue(self->VERTEX))) {
    if (flags & allStageFlags) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    flags |= Arcadia_VPL_Symbols_ProcedureFlags_VertexStage;
  }
  // At this point at most one of `fragment`, `pixel`, and `vertex` is specified.
  if (Arcadia_Set_contains(thread, self->temporary, Arcadia_Value_makeObjectReferenceValue(self->BUILTIN))) {
    if (flags & allStageFlags) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    flags |= Arcadia_VPL_Symbols_ProcedureFlags_Builtin;
  }
  if (!flags) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  return flags;
}

