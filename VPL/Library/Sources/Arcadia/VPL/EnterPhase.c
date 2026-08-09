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
#include "Arcadia/VPL/EnterPhase.h"

#include "Arcadia/VPL/Symbols/Include.h"
#include "Arcadia/VPL/Tree/Include.h"
#include "Arcadia/VPL/ModifiersUtilities.h"

static void
Arcadia_VPL_EnterPhase_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhase* self
  );

static void
Arcadia_VPL_EnterPhase_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhaseDispatch* self
  );

static void
Arcadia_VPL_EnterPhase_visit
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhase* self
  );

static void
Arcadia_VPL_EnterPhase_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhase* self
  );

static void
conflictingDefinitionError
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhase* self,
    Arcadia_VPL_Symbols_Program* program
  );

static void
createProgramScope
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhase* self,
    Arcadia_VPL_Symbols_Program* program
  );

static void
onConstantRecordField
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhase* self,
    Arcadia_VPL_Symbols_Variable* field,
    Arcadia_VPL_Symbols_Program* program
  );

static void
onConstantRecord
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhase* self,
    Arcadia_VPL_Symbols_ConstantRecord* constantRecord,
    Arcadia_VPL_Symbols_Program* program
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_EnterPhase_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_VPL_EnterPhase_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_EnterPhase_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_EnterPhase_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.EnterPhase", Arcadia_VPL_EnterPhase,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_VPL_EnterPhase_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhase* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_EnterPhase);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->worklist = (Arcadia_Deque*)Arcadia_ArrayDeque_create(thread);
  Arcadia_LeaveConstructor(Arcadia_VPL_EnterPhase);
}

static void
Arcadia_VPL_EnterPhase_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhaseDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_VPL_EnterPhase_visit
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhase* self
  )
{
  if (self->worklist) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->worklist);
  }
}

static void
Arcadia_VPL_EnterPhase_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhase* self
  )
{/*Intentionally empty.*/}

Arcadia_VPL_EnterPhase*
Arcadia_VPL_EnterPhase_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_EnterPhase);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_VPL_EnterPhase);
}

static void
conflictingDefinitionError
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhase* self,
    Arcadia_VPL_Symbols_Program* program
  )
{
  /*@todo Add and use exceptions. The concept is already in Arcadia.Ring2 and Arcadia.Languages but it was not yet used so far.*/
  Arcadia_logf(Arcadia_LogFlags_Error, "conflicting definition\n");
  Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
  Arcadia_Thread_jump(thread);
}

// Create and populate a program level scope:
// Enter the "builtin type symbol"s.
// Enter the "vertex stage symbol" and the "fragment stage symbol".
static void
createProgramScope
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhase* self,
    Arcadia_VPL_Symbols_Program* program
  )
{
  program->scope = Arcadia_Languages_Scope_create(thread, NULL);

  if (!Arcadia_Languages_Scope_enter(thread, program->scope, program->MAT4->name, (Arcadia_Object*)program->MAT4)) {
    conflictingDefinitionError(thread, self, program);
  }
  if (!Arcadia_Languages_Scope_enter(thread, program->scope, program->SAMPLER2D->name, (Arcadia_Object*)program->SAMPLER2D)) {
    conflictingDefinitionError(thread, self, program);
  }
  if (!Arcadia_Languages_Scope_enter(thread, program->scope, program->VEC2->name, (Arcadia_Object*)program->VEC2)) {
    conflictingDefinitionError(thread, self, program);
  }
  if (!Arcadia_Languages_Scope_enter(thread, program->scope, program->VEC3->name, (Arcadia_Object*)program->VEC3)) {
    conflictingDefinitionError(thread, self, program);
  }
  if (!Arcadia_Languages_Scope_enter(thread, program->scope, program->VEC4->name, (Arcadia_Object*)program->VEC4)) {
    conflictingDefinitionError(thread, self, program);
  }
  if (!Arcadia_Languages_Scope_enter(thread, program->scope, program->VOID->name, (Arcadia_Object*)program->VOID)) {
    conflictingDefinitionError(thread, self, program);
  }

  program->vertexStage = Arcadia_VPL_Symbols_Stage_create(thread, program, Arcadia_VPL_Symbols_StageFlags_Vertex);
  program->vertexStage->scope = Arcadia_Languages_Scope_create(thread, program->scope);
  if (!Arcadia_Languages_Scope_enter(thread, program->scope, Arcadia_String_createFromCxxString(thread, u8"<vertex stage>"), (Arcadia_Object*)program->vertexStage)) {
    conflictingDefinitionError(thread, self, program);
  }

  program->fragmentStage = Arcadia_VPL_Symbols_Stage_create(thread, program, Arcadia_VPL_Symbols_StageFlags_Fragment);
  program->fragmentStage->scope = Arcadia_Languages_Scope_create(thread, program->scope);
  if (!Arcadia_Languages_Scope_enter(thread, program->scope, Arcadia_String_createFromCxxString(thread, u8"<fragment stage>"), (Arcadia_Object*)program->fragmentStage)) {
    conflictingDefinitionError(thread, self, program);
  }
}

static void
onConstantRecordField
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhase* self,
    Arcadia_VPL_Symbols_Variable* field,
    Arcadia_VPL_Symbols_Program* program
  )
{
  Arcadia_VPL_Symbols_ConstantRecord* enclosing = (Arcadia_VPL_Symbols_ConstantRecord*)field->enclosing;
  Arcadia_Object* object = Arcadia_Languages_Scope_lookup(thread, enclosing->scope, field->name, Arcadia_BooleanValue_False);
  if (object) {
    conflictingDefinitionError(thread, self, program);
  }
  Arcadia_Languages_Scope_enter(thread, enclosing->scope, field->name, (Arcadia_Object*)field);
}

static void
onConstantRecord
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhase* self,
    Arcadia_VPL_Symbols_ConstantRecord* constantRecord,
    Arcadia_VPL_Symbols_Program* program
  )
{
  // Create the constant record scope if it does not exist yet.
  if (!constantRecord->scope) {
    constantRecord->scope = Arcadia_Languages_Scope_create(thread, program->scope);
  }
  // Add the fields to the scope.
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)constantRecord->fields); i < n; ++i) {
    Arcadia_VPL_Symbols_Variable* field = (Arcadia_VPL_Symbols_Variable*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, constantRecord->fields, i, _Arcadia_VPL_Symbols_Variable_getType(thread));
    onConstantRecordField(thread, self, field, program);
  }
}

Arcadia_VPL_Symbols_VariableScalar*
Step1_onVariableScalarEnter
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhase* self,
    Arcadia_VPL_Symbols_Program* program,
    Arcadia_Integer32Value index,
    Arcadia_VPL_Tree_VariableDefnNode* defn
  )
{
  if (!Arcadia_VPL_Tree_VariableDefnNode_isVariable(thread, defn)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // TODO: Create once in constructor for semantical analysis.
  Arcadia_VPL_ModifiersUtilities* modifiersUtilities = Arcadia_VPL_ModifiersUtilities_create(thread);
  Arcadia_Natural8Value flags = Arcadia_VPL_ModifiersUtilities_variableScalarModifiers(thread, modifiersUtilities, defn->modifiers);

  Arcadia_VPL_Symbols_Symbol* typeSymbol = NULL;
  if (flags & Arcadia_VPL_Symbols_VariableScalarFlags_FragmentProcessing) {
    typeSymbol = (Arcadia_VPL_Symbols_Symbol*)Arcadia_Languages_Scope_lookup(thread, program->fragmentStage->scope, defn->type, Arcadia_BooleanValue_True);
  } else if (flags & Arcadia_VPL_Symbols_VariableScalarFlags_PixelProcessing) {
    // TODO: We should add a pixelStage for orthogonality.
    typeSymbol = (Arcadia_VPL_Symbols_Symbol*)Arcadia_Languages_Scope_lookup(thread, program->scope, defn->type, Arcadia_BooleanValue_True);
  } else if (flags & Arcadia_VPL_Symbols_VariableScalarFlags_VertexProcessing) {
    typeSymbol = (Arcadia_VPL_Symbols_Symbol*)Arcadia_Languages_Scope_lookup(thread, program->vertexStage->scope, defn->type, Arcadia_BooleanValue_True);
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  if (!typeSymbol || !Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)typeSymbol, _Arcadia_VPL_Symbols_BuiltinType_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  defn->typeSymbol = typeSymbol;

  Arcadia_VPL_Symbols_VariableScalar* sym =
    Arcadia_VPL_Symbols_VariableScalar_create
      (
        thread,
        index,
        flags,
        defn->name,
        defn->typeSymbol
      );

  if (flags & Arcadia_VPL_Symbols_VariableScalarFlags_VertexProcessing) {
    Arcadia_List_insertBackObjectReferenceValue(thread, program->vertexStage->variableScalars, (Arcadia_Object*)sym);
    Arcadia_Languages_Scope_enter(thread, program->vertexStage->scope, sym->name, (Arcadia_Object*)sym);
  } else if (flags & Arcadia_VPL_Symbols_VariableScalarFlags_FragmentProcessing) {
    Arcadia_List_insertBackObjectReferenceValue(thread, program->vertexStage->variableScalars, (Arcadia_Object*)sym); // as output
    Arcadia_Languages_Scope_enter(thread, program->vertexStage->scope, sym->name, (Arcadia_Object*)sym); // as output

    Arcadia_List_insertBackObjectReferenceValue(thread, program->fragmentStage->variableScalars, (Arcadia_Object*)sym);
    Arcadia_Languages_Scope_enter(thread, program->fragmentStage->scope, sym->name, (Arcadia_Object*)sym);
  } else if (flags & Arcadia_VPL_Symbols_VariableScalarFlags_PixelProcessing) {
    Arcadia_List_insertBackObjectReferenceValue(thread, program->fragmentStage->variableScalars, (Arcadia_Object*)sym); // as output
    Arcadia_Languages_Scope_enter(thread, program->fragmentStage->scope, sym->name, (Arcadia_Object*)sym); // as output
  }
  return sym;
}

Arcadia_VPL_Symbols_Procedure*
Step1_onProcedureEnter
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhase* self,
    Arcadia_VPL_Symbols_Program* programSymbol,
    Arcadia_VPL_Tree_ProcedureDefnNode* procedureDefnNode
  )
{
  // TODO: Create once in constructor for semantical analysis.
  Arcadia_VPL_ModifiersUtilities* modifiersUtilities = Arcadia_VPL_ModifiersUtilities_create(thread);
  Arcadia_Natural8Value flags = Arcadia_VPL_ModifiersUtilities_procedureModifiers(thread, modifiersUtilities, procedureDefnNode->modifiers);

  Arcadia_VPL_Symbols_Procedure* procedureSymbol = NULL;
  Arcadia_Languages_Scope* enclosingScope = NULL;
  if (flags & Arcadia_VPL_Symbols_ProcedureFlags_VertexStage) {
    enclosingScope = programSymbol->vertexStage->scope;
    procedureSymbol = Arcadia_VPL_Symbols_Procedure_create(thread, flags, procedureDefnNode->name, (Arcadia_VPL_Symbols_Symbol*)programSymbol->vertexStage, (Arcadia_VPL_Tree_Node*)procedureDefnNode);
  } else if (flags & Arcadia_VPL_Symbols_ProcedureFlags_FragmentStage) {
    enclosingScope = programSymbol->fragmentStage->scope;
    procedureSymbol = Arcadia_VPL_Symbols_Procedure_create(thread, flags, procedureDefnNode->name, (Arcadia_VPL_Symbols_Symbol*)programSymbol->fragmentStage, (Arcadia_VPL_Tree_Node*)procedureDefnNode);
  } else if (flags & Arcadia_VPL_Symbols_ProcedureFlags_Builtin) {
    enclosingScope = programSymbol->scope;
    procedureSymbol = Arcadia_VPL_Symbols_Procedure_create(thread, flags, procedureDefnNode->name, (Arcadia_VPL_Symbols_Symbol*)programSymbol, (Arcadia_VPL_Tree_Node*)procedureDefnNode);
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  // Resolve the return type symbol.
  Arcadia_Object* returnValueTypeSymbol = NULL;
  returnValueTypeSymbol = Arcadia_Languages_Scope_lookup(thread, enclosingScope, procedureDefnNode->returnValueType->name, Arcadia_BooleanValue_True);
  if (!returnValueTypeSymbol || !Arcadia_Object_isInstanceOf(thread, returnValueTypeSymbol, _Arcadia_VPL_Symbols_BuiltinType_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_LexicalError);
    Arcadia_Thread_jump(thread);
  }
  procedureSymbol->returnValueType = (Arcadia_VPL_Symbols_BuiltinType*)returnValueTypeSymbol;
  procedureDefnNode->returnValueType->symbol = (Arcadia_VPL_Symbols_Symbol*)returnValueTypeSymbol;
  // Create the scope.
  procedureSymbol->scope = Arcadia_Languages_Scope_create(thread, enclosingScope);
  // Enter the parameters.
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)procedureDefnNode->parameters); i < n; ++i) {
    Arcadia_VPL_Tree_VariableDefnNode* parameterDefnNode = (Arcadia_VPL_Tree_VariableDefnNode*)Arcadia_List_getObjectReferenceValueAt(thread, procedureDefnNode->parameters, i);
    // Resolve the paramter type symbol.
    Arcadia_VPL_Symbols_Symbol* typeSymbol = (Arcadia_VPL_Symbols_Symbol*)Arcadia_Languages_Scope_lookup(thread, programSymbol->scope, parameterDefnNode->type, Arcadia_BooleanValue_True);
    if (!typeSymbol || !Arcadia_Object_isInstanceOf(thread, returnValueTypeSymbol, _Arcadia_VPL_Symbols_BuiltinType_getType(thread))) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    parameterDefnNode->typeSymbol = typeSymbol;
    Arcadia_VPL_Symbols_Variable* parameterSymbol = Arcadia_VPL_Symbols_Variable_create(thread, parameterDefnNode->name, typeSymbol, (Arcadia_VPL_Symbols_Symbol*)procedureSymbol, (Arcadia_VPL_Tree_Node*)parameterDefnNode);
    Arcadia_List_insertBackObjectReferenceValue(thread, procedureSymbol->parameters, (Arcadia_Object*)parameterSymbol);
    Arcadia_Languages_Scope_enter(thread, procedureSymbol->scope, parameterSymbol->name, (Arcadia_Object*)parameterSymbol);
  }
  Arcadia_Languages_Scope_enter(thread, enclosingScope, procedureSymbol->name, (Arcadia_Object*)procedureSymbol);
  return procedureSymbol;
}

Arcadia_VPL_Symbols_ConstantScalar*
Step1_onConstantScalarEnter
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhase* self,
    Arcadia_VPL_Symbols_Program* program,
    Arcadia_VPL_Tree_VariableDefnNode* defn
  )
{
  if (!Arcadia_VPL_Tree_VariableDefnNode_isConstant(thread, defn)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // Resolve type.
  Arcadia_VPL_Symbols_Symbol* varTypeSym = (Arcadia_VPL_Symbols_Symbol*)Arcadia_Languages_Scope_lookup(thread, program->scope, defn->type, Arcadia_BooleanValue_True);
  if (!varTypeSym) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_VPL_Symbols_ConstantScalar* sym = Arcadia_VPL_Symbols_ConstantScalar_create(thread, defn->name, varTypeSym, program);
  Arcadia_List_insertBackObjectReferenceValue(thread, program->constants, (Arcadia_Object*)sym);
  if (!Arcadia_Languages_Scope_enter(thread, program->scope, sym->name, (Arcadia_Object*)sym)) {
    conflictingDefinitionError(thread, self, program);
  }
  return sym;
}

Arcadia_VPL_Symbols_ConstantRecord*
Step1_onConstantRecordEnter
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhase* self,
    Arcadia_VPL_Symbols_Program* program,
    Arcadia_VPL_Tree_ConstantRecordDefnNode* defn
  )
{
  //
  Arcadia_VPL_Symbols_ConstantRecord* sym = Arcadia_VPL_Symbols_ConstantRecord_create(thread, defn->name, program, (Arcadia_VPL_Tree_Node*)defn);
  if (!Arcadia_Languages_Scope_enter(thread, program->scope, sym->name, (Arcadia_Object*)sym)) {
    conflictingDefinitionError(thread, self, program);
  }
  sym->scope = Arcadia_Languages_Scope_create(thread, program->scope);
  Arcadia_List_insertBackObjectReferenceValue(thread, program->constants, sym);
  //
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)defn->fields); i < n; ++i) {
    Arcadia_VPL_Tree_VariableDefnNode* varDefn = (Arcadia_VPL_Tree_VariableDefnNode*)Arcadia_List_getObjectReferenceValueAt(thread, defn->fields, i);
    // Resolve type.
    Arcadia_VPL_Symbols_Symbol* varTypeSym = (Arcadia_VPL_Symbols_Symbol*)Arcadia_Languages_Scope_lookup(thread, program->scope, varDefn->type, Arcadia_BooleanValue_True);
    if (!varTypeSym) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_VPL_Symbols_Variable* varSym = Arcadia_VPL_Symbols_Variable_create(thread, varDefn->name, varTypeSym, (Arcadia_VPL_Symbols_Symbol*)sym, (Arcadia_VPL_Tree_Node*)varDefn);
    if (!Arcadia_Languages_Scope_enter(thread, sym->scope, varSym->name, (Arcadia_Object*)varSym)) {
      conflictingDefinitionError(thread, self, program);
    }
    Arcadia_List_insertBackObjectReferenceValue(thread, sym->fields, varSym);
  }
  return sym;
}

void
Arcadia_VPL_EnterPhase_run
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhase* self,
    Arcadia_VPL_Symbols_Program* program
  )
{
  Arcadia_Integer32Value variableScalarCount = 0;
  createProgramScope(thread, self, program);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)program->programDefnNode->elements); i < n; ++i) {
    Arcadia_VPL_Tree_Node* node = (Arcadia_VPL_Tree_Node*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, program->programDefnNode->elements, i, _Arcadia_VPL_Tree_Node_getType(thread));
    if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node, _Arcadia_VPL_Tree_VariableDefnNode_getType(thread))) {
      Arcadia_VPL_Tree_VariableDefnNode* defn = (Arcadia_VPL_Tree_VariableDefnNode*)Arcadia_List_getObjectReferenceValueAt(thread, program->programDefnNode->elements, i);
      if (Arcadia_VPL_Tree_VariableDefnNode_isConstant(thread, defn)) {
        Step1_onConstantScalarEnter(thread, self, program, defn);
      } else if (Arcadia_VPL_Tree_VariableDefnNode_isVariable(thread, defn)) {
        Step1_onVariableScalarEnter(thread, self, program, variableScalarCount++, defn);
      } else {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      }
    } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node, _Arcadia_VPL_Tree_ConstantRecordDefnNode_getType(thread))) {
      Arcadia_VPL_Tree_ConstantRecordDefnNode* defn = (Arcadia_VPL_Tree_ConstantRecordDefnNode*)Arcadia_List_getObjectReferenceValueAt(thread, program->programDefnNode->elements, i);
      Step1_onConstantRecordEnter(thread, self, program, defn);
    } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node, _Arcadia_VPL_Tree_ProcedureDefnNode_getType(thread))) {
      Arcadia_VPL_Tree_ProcedureDefnNode* defn = (Arcadia_VPL_Tree_ProcedureDefnNode*)Arcadia_List_getObjectReferenceValueAt(thread, program->programDefnNode->elements, i);
      Step1_onProcedureEnter(thread, self, program, defn);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
  }
}
