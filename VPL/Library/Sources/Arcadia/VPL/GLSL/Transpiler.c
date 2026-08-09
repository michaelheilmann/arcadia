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
#include "Arcadia/VPL/GLSL/Transpiler.h"

#include "Arcadia/Logging/Include.h"
#include "Arcadia/Languages/Include.h"
#include "Arcadia/VPL/Tree/Include.h"
#include "Arcadia/VPL/Symbols/Include.h"
#include "Arcadia/VPL/ResolvePhase.h"
#include "Arcadia/VPL/Configure.h"

#include <string.h>

static const char ZEROTERMINATOR =
  '\0'
  ;

static const char* HEADER =
  "#version 420 core\n"
  ;

static Arcadia_String*
S
  (
    Arcadia_Thread* thread,
    const char* string
  )
{ return Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), string); }

static void
emitCxxString
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* target,
    const char* p
  );

static void
emitString
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* target,
    Arcadia_String* p
  );

static void
writeType
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Arcadia_VPL_Symbols_Symbol* type,
    Arcadia_ByteArrayBuilder* target
  );

static void
writeVariableScalar
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Context context,
    Arcadia_VPL_Symbols_Program* program,
    Arcadia_VPL_Symbols_VariableScalar* variableScalar,
    Arcadia_ByteArrayBuilder* target
  );

static void
writeConstantScalar
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Context context,
    Arcadia_Map* constantMapping,
    Arcadia_VPL_Symbols_Program* program,
    Arcadia_VPL_Symbols_ConstantScalar* constantScalar,
    Arcadia_ByteArrayBuilder* target
  );

static void
writeConstantRecord
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Context context,
    Arcadia_Map* constantMapping,
    Arcadia_VPL_Symbols_Program* program,
    Arcadia_VPL_Symbols_ConstantRecord* constantBlock,
    Arcadia_ByteArrayBuilder* target
  );

static void
Arcadia_VPL_Backends_GLSL_Transpiler_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self
  );

static void
Arcadia_VPL_Backends_GLSL_Transpiler_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_TranspilerDispatch* self
  );

static void
Arcadia_VPL_Backends_GLSL_Transpiler_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_Backends_GLSL_Transpiler_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_Backends_GLSL_Transpiler_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_Backends_GLSL_Transpiler_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.VPL.Backends.GLSL.Transpiler", Arcadia_VPL_Backends_GLSL_Transpiler,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_VPL_Backends_GLSL_Transpiler_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_Backends_GLSL_Transpiler);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->symbolNameMapping = NULL;
  Arcadia_LeaveConstructor(Arcadia_VPL_Backends_GLSL_Transpiler);
}

static void
Arcadia_VPL_Backends_GLSL_Transpiler_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_TranspilerDispatch* self
  )
{ }

static void
Arcadia_VPL_Backends_GLSL_Transpiler_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self
  )
{
  if (self->symbolNameMapping) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->symbolNameMapping);
  }
}

Arcadia_VPL_Backends_GLSL_Transpiler*
Arcadia_VPL_Backends_GLSL_Transpiler_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_Backends_GLSL_Transpiler);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_VPL_Backends_GLSL_Transpiler);
}

static void
emitCxxString
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* target,
    const char* p
  )
{ Arcadia_ByteArrayBuilder_insertBackBytes(thread, target, p, strlen(p)); }

static void
emitString
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* target,
    Arcadia_String* p
  )
{ Arcadia_ByteArrayBuilder_insertBackBytes(thread, target, Arcadia_String_getBytes(thread, p), Arcadia_String_getNumberOfBytes(thread, p)); }

static void
writeType
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Arcadia_VPL_Symbols_Symbol* type,
    Arcadia_ByteArrayBuilder* target
  )
{
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)type, _Arcadia_VPL_Symbols_BuiltinType_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value nameValue = Arcadia_Map_get(thread, self->symbolNameMapping, Arcadia_Value_makeObjectReferenceValue(type));
  if (Arcadia_Value_isVoidValue(&nameValue)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_String* nameString = (Arcadia_String*)Arcadia_Value_getObjectReferenceValueChecked(thread, nameValue, _Arcadia_String_getType(thread));
  emitString(thread, target, nameString);
}

static void
writeVariableScalar
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Context context,
    Arcadia_VPL_Symbols_Program* program,
    Arcadia_VPL_Symbols_VariableScalar* variableScalar,
    Arcadia_ByteArrayBuilder* target
  )
{
  switch (context) {
    case Context_VertexShader: {
      if ((variableScalar->flags & Arcadia_VPL_Symbols_VariableScalarFlags_VertexProcessing) == Arcadia_VPL_Symbols_VariableScalarFlags_VertexProcessing) {
        emitCxxString(thread, target, "layout(location = ");
        emitString(thread, target, Arcadia_String_createFromInteger32(thread, variableScalar->location));
        emitCxxString(thread, target, u8") ");
        emitCxxString(thread, target, u8"in");
        emitCxxString(thread, target, u8" ");
        writeType(thread, self, variableScalar->type, target);
        emitCxxString(thread, target, u8" ");

        Arcadia_String* mangledName = Arcadia_Languages_mangleName(thread, Arcadia_Value_makeObjectReferenceValue(variableScalar->name));
        Arcadia_Map_set(thread, self->symbolNameMapping, Arcadia_Value_makeObjectReferenceValue(variableScalar), Arcadia_Value_makeObjectReferenceValue(mangledName), NULL, NULL);

        emitString(thread, target, mangledName);

        emitCxxString(thread, target, u8";\n");
      } else if ((variableScalar->flags & Arcadia_VPL_Symbols_VariableScalarFlags_FragmentProcessing) == Arcadia_VPL_Symbols_VariableScalarFlags_FragmentProcessing) {
        if (Arcadia_VPL_Symbols_VariableScalarFlags_PositionSemantics == (variableScalar->flags & Arcadia_VPL_Symbols_VariableScalarFlags_PositionSemantics)) {
          Arcadia_Map_set(thread, self->symbolNameMapping, Arcadia_Value_makeObjectReferenceValue(variableScalar), Arcadia_Value_makeObjectReferenceValue(S(thread, "gl_Position")), NULL, NULL);
        } else {
          emitCxxString(thread, target, u8"out");
          emitCxxString(thread, target, u8" ");
          writeType(thread, self, variableScalar->type, target);
          emitCxxString(thread, target, u8" ");

          Arcadia_String* mangledName = Arcadia_Languages_mangleName(thread, Arcadia_Value_makeObjectReferenceValue(variableScalar->name));
          Arcadia_Map_set(thread, self->symbolNameMapping, Arcadia_Value_makeObjectReferenceValue(variableScalar), Arcadia_Value_makeObjectReferenceValue(mangledName), NULL, NULL);

          emitString(thread, target, mangledName);

          emitCxxString(thread, target, u8";\n");
        }
      } else if ((variableScalar->flags & Arcadia_VPL_Symbols_VariableScalarFlags_PixelProcessing) == Arcadia_VPL_Symbols_VariableScalarFlags_PixelProcessing) {
        /*Intentionally empty.*/
      } else {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      }
    } break;
    case Context_FragmentShader: {
      if ((variableScalar->flags & Arcadia_VPL_Symbols_VariableScalarFlags_VertexProcessing) == Arcadia_VPL_Symbols_VariableScalarFlags_VertexProcessing) {
        /*Intentionally empty.*/
      } else if ((variableScalar->flags & Arcadia_VPL_Symbols_VariableScalarFlags_FragmentProcessing) == Arcadia_VPL_Symbols_VariableScalarFlags_FragmentProcessing) {
        emitCxxString(thread, target, u8"in");
        emitCxxString(thread, target, u8" ");
        writeType(thread, self, variableScalar->type, target);
        emitCxxString(thread, target, u8" ");

        Arcadia_String* mangledName = Arcadia_Languages_mangleName(thread, Arcadia_Value_makeObjectReferenceValue(variableScalar->name));
        Arcadia_Map_set(thread, self->symbolNameMapping, Arcadia_Value_makeObjectReferenceValue(variableScalar), Arcadia_Value_makeObjectReferenceValue(mangledName), NULL, NULL);

        emitString(thread, target, mangledName);

        emitCxxString(thread, target, u8";\n");
      } else if ((variableScalar->flags & Arcadia_VPL_Symbols_VariableScalarFlags_PixelProcessing) == Arcadia_VPL_Symbols_VariableScalarFlags_PixelProcessing) {
        emitCxxString(thread, target, u8"out");
        emitCxxString(thread, target, u8" ");
        writeType(thread, self, variableScalar->type, target);
        emitCxxString(thread, target, u8" ");

        Arcadia_String* mangledName = Arcadia_Languages_mangleName(thread, Arcadia_Value_makeObjectReferenceValue(variableScalar->name));
        Arcadia_Map_set(thread, self->symbolNameMapping, Arcadia_Value_makeObjectReferenceValue(variableScalar), Arcadia_Value_makeObjectReferenceValue(mangledName), NULL, NULL);

        emitString(thread, target, mangledName);

        emitCxxString(thread, target, u8";\n");
      } else {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      }
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
}

static void
writeConstantScalar
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Context context,
    Arcadia_Map* constantMapping,
    Arcadia_VPL_Symbols_Program* program,
    Arcadia_VPL_Symbols_ConstantScalar* constantScalar,
    Arcadia_ByteArrayBuilder* target
  )
{
  // <uniform> : 'uniform' <type> <instance name>;
  Arcadia_String* name = Arcadia_VPL_Symbols_Symbol_getName(thread, (Arcadia_VPL_Symbols_Symbol*)constantScalar);

  // Add the VPL constant scalar name to GL uniform scalar name to the map.
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackString(thread, stringBuffer, name);
  Arcadia_StringBuilder_insertBackCodePoint(thread, stringBuffer, '\0');
  Arcadia_String* glName = Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer));
  Arcadia_Map_set(thread, constantMapping, Arcadia_Value_makeObjectReferenceValue(name), Arcadia_Value_makeObjectReferenceValue(glName), NULL, NULL);
  Arcadia_Map_set(thread, self->symbolNameMapping, Arcadia_Value_makeObjectReferenceValue(constantScalar), Arcadia_Value_makeObjectReferenceValue(name), NULL, NULL);

  emitCxxString(thread, target, u8"uniform");
  emitCxxString(thread, target, u8"  ");
  writeType(thread, self, constantScalar->type, target);
  emitCxxString(thread, target, u8"  ");
  emitString(thread, target, constantScalar->name);
  emitCxxString(thread, target, u8";\n");
}

static void
writeConstantRecord
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Context context,
    Arcadia_Map* constantMapping,
    Arcadia_VPL_Symbols_Program* program,
    Arcadia_VPL_Symbols_ConstantRecord* constantBlock,
    Arcadia_ByteArrayBuilder* target
  )
{
  // <uniform block> : 'layout(std140)' 'uniform' <name> '{' <fields> '} '_'<name> ';'
  // <fields>: <field> ';' <fields>
  //         |
  // <field> : <type> <name>

  // - the index of a member of an uniform block definition `'uniform' <block name> <block definition> <instance name> ';'`
  // is obtained by `glGetProgramUniformLocation(program, "<block name>.<member name>")`.
  //
  // - the uniform block definition `'uniform' <block name> <block definition> <instance name> ';'`
  // is obtained by `glGetUniformBlockIndex(program, "<block name>")`.
  //
  // - <instance name> is like a namespace. If there are to uniform blocks of block names A and B and
  // both have a member of name x, then there would be a naming conflict as x is declared in global
  // namespace. You can avoid this conflict by asssigning different instance names to A and  B.
  // If you assign instance names a to A and b to B then x of A is defined in namespace a and x of B is defined in namespace b.
  // Note that no two uniform blocks may have the same instance name.
  //
  // - <instance name> and <block name> of a block must be different.

  Arcadia_String* name = Arcadia_VPL_Symbols_Symbol_getName(thread, (Arcadia_VPL_Symbols_Symbol*)constantBlock);

  // Add the VPL constant block name to GL uniform block name to the map.
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackString(thread, stringBuffer, name);
  Arcadia_StringBuilder_insertBackCodePoint(thread, stringBuffer, '\0');
  Arcadia_String* glName = Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer));
  Arcadia_Map_set(thread, constantMapping, Arcadia_Value_makeObjectReferenceValue(name), Arcadia_Value_makeObjectReferenceValue(glName), NULL, NULL);

  // Generate an <instance name>.
  Arcadia_StringBuilder_clear(thread, stringBuffer);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, "_");
  Arcadia_StringBuilder_insertBackString(thread, stringBuffer, name);
  Arcadia_StringBuilder_insertBackString(thread, stringBuffer, Arcadia_String_createFromNatural32(thread, self->numberOfConstantBlocks++));
  Arcadia_String* glInstanceName = Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer));
  Arcadia_Map_set(thread, self->symbolNameMapping, Arcadia_Value_makeObjectReferenceValue(constantBlock), Arcadia_Value_makeObjectReferenceValue(glInstanceName), NULL, NULL);

  Arcadia_List* fields = Arcadia_VPL_Symbols_ConstantRecord_getFields(thread, constantBlock);
  emitCxxString(thread, target, u8"layout(std140) uniform ");
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, target, Arcadia_String_getBytes(thread, name), Arcadia_String_getNumberOfBytes(thread, name));
  emitCxxString(thread, target, u8" {\n");
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)fields); i < n; ++i) {
    Arcadia_VPL_Symbols_Variable* field = Arcadia_List_getObjectReferenceValueAt(thread, fields, i);
    emitCxxString(thread, target, u8"  ");
    writeType(thread, self, field->type, target);
    emitCxxString(thread, target, u8" ");
    emitString(thread, target, field->name);
    emitCxxString(thread, target, u8";\n");
  }
  emitCxxString(thread, target, u8"}");
  emitString(thread, target, glInstanceName);
  emitCxxString(thread, target, u8";\n");
}

static void
onParameterVariableDefinitionTree
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Arcadia_VPL_Tree_Node* tree,
    Arcadia_ByteArrayBuilder* target
  );

static void
onExprTree
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Arcadia_VPL_Tree_Node* tree,
    Arcadia_ByteArrayBuilder* target
  );

static void
onNameExprTree
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Arcadia_VPL_Tree_NameExprNode* tree,
    Arcadia_ByteArrayBuilder* target
  );

static void
onStatementListTree
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Arcadia_List* statementList,
    Arcadia_ByteArrayBuilder* target
  );

static void
onProcedureSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Arcadia_VPL_Symbols_Procedure* symbol,
    Arcadia_ByteArrayBuilder* target
  );

static void
onParameterVariableDefinitionTree
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Arcadia_VPL_Tree_Node* tree,
    Arcadia_ByteArrayBuilder* target
  )
{
  if (tree->flags != Arcadia_VPL_Tree_NodeFlags_VariableDefinition) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  writeType(thread, self, ((Arcadia_VPL_Tree_VariableDefnNode*)tree)->typeSymbol, target);
  emitCxxString(thread, target, " ");
  emitString(thread, target, ((Arcadia_VPL_Tree_VariableDefnNode*)tree)->name);
}

static void
onExprTree
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Arcadia_VPL_Tree_Node* tree,
    Arcadia_ByteArrayBuilder* target
  )
{
  switch (tree->flags) {
    case Arcadia_VPL_Tree_NodeFlags_BinaryExpr: {
      Arcadia_VPL_Tree_BinaryExprNode* binaryExpression = (Arcadia_VPL_Tree_BinaryExprNode*)tree;

      switch (binaryExpression->kind) {
        case Arcadia_VPL_Tree_BinaryExprNodeFlags_Access: {
          emitCxxString(thread, target, "(");
          onExprTree(thread, self, binaryExpression->lhs, target);
          emitCxxString(thread, target, ".");
          onExprTree(thread, self, binaryExpression->rhs, target);
          emitCxxString(thread, target, ")");
        } break;
        case Arcadia_VPL_Tree_BinaryExprNodeFlags_Add: {
          emitCxxString(thread, target, "(");
          onExprTree(thread, self, binaryExpression->lhs, target);
          emitCxxString(thread, target, " + ");
          onExprTree(thread, self, binaryExpression->rhs, target);
          emitCxxString(thread, target, ")");
        } break;
        case Arcadia_VPL_Tree_BinaryExprNodeFlags_Subtract: {
          emitCxxString(thread, target, "(");
          onExprTree(thread, self, binaryExpression->lhs, target);
          emitCxxString(thread, target, " - ");
          onExprTree(thread, self, binaryExpression->rhs, target);
          emitCxxString(thread, target, ")");
        } break;
        case Arcadia_VPL_Tree_BinaryExprNodeFlags_Multiply: {
          emitCxxString(thread, target, "(");
          onExprTree(thread, self, binaryExpression->lhs, target);
          emitCxxString(thread, target, " * ");
          onExprTree(thread, self, binaryExpression->rhs, target);
          emitCxxString(thread, target, ")");
        } break;
        case Arcadia_VPL_Tree_BinaryExprNodeFlags_Divide: {
          emitCxxString(thread, target, "(");
          onExprTree(thread, self, binaryExpression->lhs, target);
          emitCxxString(thread, target, " / ");
          onExprTree(thread, self, binaryExpression->rhs, target);
          emitCxxString(thread, target, ")");
        } break;
        case Arcadia_VPL_Tree_BinaryExprNodeFlags_Assignment: {
          emitCxxString(thread, target, u8"  ");
          onExprTree(thread, self, binaryExpression->lhs, target);
          emitCxxString(thread, target, u8" = ");
          onExprTree(thread, self, binaryExpression->rhs, target);
          emitCxxString(thread, target, ";\n");
        } break;
      };
    } break;

    case Arcadia_VPL_Tree_NodeFlags_NameExpr: {
      onNameExprTree(thread, self, (Arcadia_VPL_Tree_NameExprNode*)tree, target);
    } break;
    case Arcadia_VPL_Tree_NodeFlags_NumberExpr: {
      emitCxxString(thread, target, "(");
      emitString(thread, target, ((Arcadia_VPL_Tree_NumberExprNode*)tree)->literal);
      emitCxxString(thread, target, ")");
    } break;
    case Arcadia_VPL_Tree_NodeFlags_CallExpr: {
      Arcadia_VPL_Tree_CallExprNode* callExprNode = (Arcadia_VPL_Tree_CallExprNode*)tree;
      //emitCxxString(thread, target, "(");
      onExprTree(thread, self, callExprNode->target, target);
      //emitCxxString(thread, target, ")");
      emitCxxString(thread, target, "(");
      if (Arcadia_Collection_getSize(thread, (Arcadia_Collection*)callExprNode->arguments) > 0) {
        Arcadia_VPL_Tree_Node* argument;
        argument = (Arcadia_VPL_Tree_Node*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, callExprNode->arguments, 0, _Arcadia_VPL_Tree_Node_getType(thread));
        onExprTree(thread, self, argument, target);
        for (Arcadia_SizeValue i = 1, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)callExprNode->arguments); i < n; ++i) {
          argument = (Arcadia_VPL_Tree_Node*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, callExprNode->arguments, i, _Arcadia_VPL_Tree_Node_getType(thread));
          emitCxxString(thread, target, ", ");
          onExprTree(thread, self, argument, target);
        }
      }
      emitCxxString(thread, target, ")");
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
}

static void
onNameExprTree
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Arcadia_VPL_Tree_NameExprNode* node,
    Arcadia_ByteArrayBuilder* target
  )
{
  if (!node->symbol) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node->symbol, _Arcadia_VPL_Symbols_ConstantRecord_getType(thread))) {
    // Name refers to a constant record. Get the name from the VPL symbol -> GLSL name mapping.
    Arcadia_Value nameValue = Arcadia_Map_get(thread, self->symbolNameMapping, Arcadia_Value_makeObjectReferenceValue(node->symbol));
    if (Arcadia_Value_isVoidValue(&nameValue)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_String* nameString = (Arcadia_String*)Arcadia_Value_getObjectReferenceValueChecked(thread, nameValue, _Arcadia_String_getType(thread));
    emitString(thread, target, nameString);
  } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node->symbol, _Arcadia_VPL_Symbols_VariableScalar_getType(thread))) {
    // Name refers to a variable scalar. Get the name from the VPL symbol -> GLSL name mapping.
    Arcadia_Value nameValue = Arcadia_Map_get(thread, self->symbolNameMapping, Arcadia_Value_makeObjectReferenceValue(node->symbol));
    if (Arcadia_Value_isVoidValue(&nameValue)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_String* nameString = (Arcadia_String*)Arcadia_Value_getObjectReferenceValueChecked(thread, nameValue, _Arcadia_String_getType(thread));
    emitString(thread, target, nameString);
  } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node->symbol, _Arcadia_VPL_Symbols_BuiltinType_getType(thread))) {
    // Name refers to a builtin type. Get the name from the VPL symbol -> GLSL name mapping.
    Arcadia_Value nameValue = Arcadia_Map_get(thread, self->symbolNameMapping, Arcadia_Value_makeObjectReferenceValue(node->symbol));
    if (Arcadia_Value_isVoidValue(&nameValue)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_String* nameString = (Arcadia_String*)Arcadia_Value_getObjectReferenceValueChecked(thread, nameValue, _Arcadia_String_getType(thread));
    emitString(thread, target, nameString);
  } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node->symbol, _Arcadia_VPL_Symbols_Variable_getType(thread))) {
    emitString(thread, target, ((Arcadia_VPL_Symbols_Variable*)node->symbol)->name);
  } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node->symbol, _Arcadia_VPL_Symbols_Procedure_getType(thread))) {
    emitString(thread, target, ((Arcadia_VPL_Symbols_Procedure*)node->symbol)->name);
  } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node->symbol, _Arcadia_VPL_Symbols_ConstantScalar_getType(thread))) {
    emitString(thread, target, ((Arcadia_VPL_Symbols_ConstantScalar*)node->symbol)->name);
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static void
onStatementListTree
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Arcadia_List* statementList,
    Arcadia_ByteArrayBuilder* target
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)statementList); i < n; ++i) {
    Arcadia_VPL_Tree_Node* a = (Arcadia_VPL_Tree_Node*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, statementList, i,
                                                                                                     _Arcadia_VPL_Tree_Node_getType(thread));
    switch (a->flags) {
      case Arcadia_VPL_Tree_NodeFlags_VariableDefinition: {
        emitCxxString(thread, target, u8"  ");
        writeType(thread, self, ((Arcadia_VPL_Tree_VariableDefnNode*)a)->typeSymbol, target);
        emitCxxString(thread, target, u8" ");
        emitString(thread, target, ((Arcadia_VPL_Tree_VariableDefnNode*)a)->name);
        emitCxxString(thread, target, ";\n");
      } break;
      case Arcadia_VPL_Tree_NodeFlags_BinaryExpr:
      case Arcadia_VPL_Tree_NodeFlags_CallExpr:
      case Arcadia_VPL_Tree_NodeFlags_NumberExpr: {
        onExprTree(thread, self, a, target);
      } break;
      default: {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      } break;
    };
  }
}

static void
onProcedureSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Arcadia_VPL_Symbols_Procedure* symbol,
    Arcadia_ByteArrayBuilder* target
  )
{
  Arcadia_VPL_Tree_ProcedureDefnNode* node = (Arcadia_VPL_Tree_ProcedureDefnNode*)symbol->node;
  onNameExprTree(thread, self, ((Arcadia_VPL_Tree_ProcedureDefnNode*)node)->returnValueType, target);
  emitCxxString(thread, target, " ");
  emitString(thread, target, ((Arcadia_VPL_Tree_ProcedureDefnNode*)node)->name);
  emitCxxString(thread, target, "(");

  if (Arcadia_Collection_getSize(thread, (Arcadia_Collection*)node->parameters) > 0) {
    Arcadia_VPL_Tree_Node* parameter;
    parameter = (Arcadia_VPL_Tree_Node*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, node->parameters, 0, _Arcadia_VPL_Tree_Node_getType(thread));
    onParameterVariableDefinitionTree(thread, self, parameter, target);
    for (Arcadia_SizeValue i = 1, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)node->parameters); i < n; ++i) {
      parameter = (Arcadia_VPL_Tree_Node*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, node->parameters, i, _Arcadia_VPL_Tree_Node_getType(thread));
      emitCxxString(thread, target, ", ");
      onParameterVariableDefinitionTree(thread, self, parameter, target);
    }
  }

  emitCxxString(thread, target, ")");
  emitCxxString(thread, target, " ");
  emitCxxString(thread, target, "{\n");
  onStatementListTree(thread, self, node->body, target);
  emitCxxString(thread, target, "}\n");
}

static Arcadia_List*
getProcedures
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Context context,
    Arcadia_VPL_Symbols_Program* program
  )
{
  Arcadia_List* list = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_VPL_Symbols_Stage* stage = NULL;
  switch (context) {
    case Context_FragmentShader: {
      stage = program->fragmentStage;
    } break;
    case Context_VertexShader: {
      stage = program->vertexStage;
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
  Arcadia_List* objects = Arcadia_Map_getValues(thread, stage->scope->entries);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)objects); i < n; ++i) {
    Arcadia_Object* object = Arcadia_List_getObjectReferenceValueAt(thread, objects, i);
    if (Arcadia_Object_isInstanceOf(thread, object, _Arcadia_VPL_Symbols_Procedure_getType(thread))) {
      Arcadia_List_insertBackObjectReferenceValue(thread, list, object);
    }
  }
  return list;
}

static Arcadia_Map*
computeInitialSymbolNameMapping
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Program* program
  )
{
  Arcadia_Map* map = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_Map_set(thread, map, Arcadia_Value_makeObjectReferenceValue(program->MAT4), Arcadia_Value_makeObjectReferenceValue(S(thread, "mat4")), NULL, NULL);
  Arcadia_Map_set(thread, map, Arcadia_Value_makeObjectReferenceValue(program->SAMPLER2D), Arcadia_Value_makeObjectReferenceValue(S(thread, "sampler2D")), NULL, NULL);
  Arcadia_Map_set(thread, map, Arcadia_Value_makeObjectReferenceValue(program->VEC2), Arcadia_Value_makeObjectReferenceValue(S(thread, "vec2")), NULL, NULL);
  Arcadia_Map_set(thread, map, Arcadia_Value_makeObjectReferenceValue(program->VEC3), Arcadia_Value_makeObjectReferenceValue(S(thread, "vec3")), NULL, NULL);
  Arcadia_Map_set(thread, map, Arcadia_Value_makeObjectReferenceValue(program->VEC4), Arcadia_Value_makeObjectReferenceValue(S(thread, "vec4")), NULL, NULL);
  Arcadia_Map_set(thread, map, Arcadia_Value_makeObjectReferenceValue(program->VOID), Arcadia_Value_makeObjectReferenceValue(S(thread, "void")), NULL, NULL);
  return map;
}

void
Arcadia_VPL_Backends_GLSL_Transpiler_writeDefaultVertexShader
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Arcadia_VPL_Symbols_Program* program,
    Arcadia_Map* constantMapping,
    Arcadia_Map* vertexShaderVariableScalarMapping,
    Arcadia_ByteArrayBuilder* target
  )
{
  self->symbolNameMapping = computeInitialSymbolNameMapping(thread, program);
  self->numberOfConstantBlocks = 0;
  emitCxxString(thread, target, HEADER);

  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)program->vertexStage->variableScalars); i < n; ++i) {
    Arcadia_VPL_Symbols_VariableScalar* variableScalar =
      (Arcadia_VPL_Symbols_VariableScalar*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, program->vertexStage->variableScalars, i,
                                                                                         _Arcadia_VPL_Symbols_VariableScalar_getType(thread));
    writeVariableScalar(thread, self, Context_VertexShader, program, variableScalar, target);
  }

  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)program->constants); i < n; ++i) {
    Arcadia_VPL_Symbols_Constant* constant = (Arcadia_VPL_Symbols_Constant*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, program->constants, i, _Arcadia_VPL_Symbols_Constant_getType(thread));
    if (Arcadia_VPL_Symbols_Constant_getKind(thread, constant) == Arcadia_VPL_ConstantKind_Record) {
      writeConstantRecord(thread, self, Context_VertexShader, constantMapping, program, (Arcadia_VPL_Symbols_ConstantRecord*)constant, target);
    } else if (Arcadia_VPL_Symbols_Constant_getKind(thread, constant) == Arcadia_VPL_ConstantKind_Scalar) {
      writeConstantScalar(thread, self, Context_VertexShader, constantMapping, program, (Arcadia_VPL_Symbols_ConstantScalar*)constant, target);
    }
  }
  Arcadia_VPL_ResolvePhase_run(thread, Arcadia_VPL_ResolvePhase_create(thread, Arcadia_Languages_Diagnostics_create(thread, (Arcadia_Log*)Arcadia_ConsoleLog_create(thread))), program);
  Arcadia_List* procedures = getProcedures(thread, self, Context_VertexShader, program);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)procedures); i < n; ++i) {
    Arcadia_VPL_Symbols_Procedure* procedure = (Arcadia_VPL_Symbols_Procedure*)Arcadia_List_getObjectReferenceValueAt(thread, procedures, i);
    onProcedureSymbol(thread, self, procedure, target);
  }

  // the zero terminator
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, target, &ZEROTERMINATOR, 1);
}

void
Arcadia_VPL_Backends_GLSL_Transpiler_writeDefaultFragmentShader
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Arcadia_VPL_Symbols_Program* program,
    Arcadia_Map* constantMapping,
    Arcadia_Map* fragmentShaderVariableScalarMapping,
    Arcadia_String** fragmentColorOutput,
    Arcadia_ByteArrayBuilder* target
  )
{
  self->symbolNameMapping = computeInitialSymbolNameMapping(thread, program);
  self->numberOfConstantBlocks = 0;
  emitCxxString(thread, target, HEADER);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)program->fragmentStage->variableScalars); i < n; ++i) {
    Arcadia_VPL_Symbols_VariableScalar* variableScalar =
      (Arcadia_VPL_Symbols_VariableScalar*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, program->fragmentStage->variableScalars, i,
                                                                                         _Arcadia_VPL_Symbols_VariableScalar_getType(thread));
    writeVariableScalar(thread, self, Context_FragmentShader, program, variableScalar, target);
  }

  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)program->constants); i < n; ++i) {
    Arcadia_VPL_Symbols_Constant* constant = (Arcadia_VPL_Symbols_Constant*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, program->constants, i, _Arcadia_VPL_Symbols_Constant_getType(thread));
    if (Arcadia_VPL_Symbols_Constant_getKind(thread, constant) == Arcadia_VPL_ConstantKind_Record) {
      writeConstantRecord(thread, self, Context_FragmentShader, constantMapping, program, (Arcadia_VPL_Symbols_ConstantRecord*)constant, target);
    } else if (Arcadia_VPL_Symbols_Constant_getKind(thread, constant) == Arcadia_VPL_ConstantKind_Scalar) {
      writeConstantScalar(thread, self, Context_FragmentShader, constantMapping, program, (Arcadia_VPL_Symbols_ConstantScalar*)constant, target);
    }
  }
  Arcadia_VPL_ResolvePhase_run(thread, Arcadia_VPL_ResolvePhase_create(thread, Arcadia_Languages_Diagnostics_create(thread, (Arcadia_Log*)Arcadia_ConsoleLog_create(thread))), program);
  Arcadia_List* procedures = getProcedures(thread, self, Context_FragmentShader, program);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)procedures); i < n; ++i) {
    Arcadia_VPL_Symbols_Procedure* procedure = (Arcadia_VPL_Symbols_Procedure*)Arcadia_List_getObjectReferenceValueAt(thread, procedures, i);
    onProcedureSymbol(thread, self, procedure, target);
  }

  // the zero terminator
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, target, &ZEROTERMINATOR, 1);

  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"_2_fragmentColor");
  Arcadia_StringBuilder_insertBackCodePoint(thread, stringBuffer, '\0');
  *fragmentColorOutput = Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer));
}
