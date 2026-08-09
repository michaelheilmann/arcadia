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
#include "Arcadia/VPL/Tree/VariableDefnNode.h"

#include "Arcadia/Languages/Include.h"

static Arcadia_String*
S
  (
    Arcadia_Thread* thread,
    const char* string
  )
{ return Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), string); }

static void
Arcadia_VPL_Tree_VariableDefnNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_VariableDefnNode* self
  );

static void
Arcadia_VPL_Tree_VariableDefnNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_VariableDefnNodeDispatch* self
  );

static void
Arcadia_VPL_Tree_VariableDefnNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_VariableDefnNode* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_Tree_VariableDefnNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_Tree_VariableDefnNode_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_Tree_VariableDefnNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};


Arcadia_defineObjectType(u8"Arcadia.VPL.Tree.VariableDefnNode", Arcadia_VPL_Tree_VariableDefnNode,
                         u8"Arcadia.VPL.Tree.Node", Arcadia_VPL_Tree_Node,
                         &_typeOperations);

static void
Arcadia_VPL_Tree_VariableDefnNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_VariableDefnNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_Tree_VariableDefnNode);
  {
    Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_VPL_Tree_NodeFlags_VariableDefinition);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->name = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_String_getType(thread));
  self->type = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
  self->typeSymbol = NULL;
  self->modifiers = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_LeaveConstructor(Arcadia_VPL_Tree_VariableDefnNode);
}

static void
Arcadia_VPL_Tree_VariableDefnNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_VariableDefnNodeDispatch* self
  )
{ }

static void
Arcadia_VPL_Tree_VariableDefnNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_VariableDefnNode* self
  )
{
  if (self->name) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->name);
  }
  if (self->type) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->type);
  }
  if (self->typeSymbol) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->typeSymbol);
  }
  if (self->modifiers) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->modifiers);
  }
}

Arcadia_VPL_Tree_VariableDefnNode*
Arcadia_VPL_Tree_VariableDefnNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name,
    Arcadia_String* type
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_Tree_VariableDefnNode);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)type);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_VPL_Tree_VariableDefnNode);
}

Arcadia_BooleanValue
Arcadia_VPL_Tree_VariableDefnNode_isVertex
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_VariableDefnNode* self
  )
{ return Arcadia_List_contains(thread, self->modifiers, Arcadia_Value_makeObjectReferenceValue(S(thread, u8"vertex"))); }

Arcadia_BooleanValue
Arcadia_VPL_Tree_VariableDefnNode_isFragment
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_VariableDefnNode* self
  )
{ return Arcadia_List_contains(thread, self->modifiers, Arcadia_Value_makeObjectReferenceValue(S(thread, u8"fragment"))); }

Arcadia_BooleanValue
Arcadia_VPL_Tree_VariableDefnNode_isPixel
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_VariableDefnNode* self
  )
{ return Arcadia_List_contains(thread, self->modifiers, Arcadia_Value_makeObjectReferenceValue(S(thread, u8"pixel"))); }

Arcadia_BooleanValue
Arcadia_VPL_Tree_VariableDefnNode_isConstant
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_VariableDefnNode* self
  )
{ return Arcadia_List_contains(thread, self->modifiers, Arcadia_Value_makeObjectReferenceValue(S(thread, u8"constant"))); }

Arcadia_BooleanValue
Arcadia_VPL_Tree_VariableDefnNode_isVariable
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_VariableDefnNode* self
  )
{ return Arcadia_List_contains(thread, self->modifiers, Arcadia_Value_makeObjectReferenceValue(S(thread, u8"variable"))); }
