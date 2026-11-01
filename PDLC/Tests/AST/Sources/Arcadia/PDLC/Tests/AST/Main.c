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

#include "Arcadia/Testing/Include.h"

#include "Arcadia/PDLC/Include.h"
#include "Arcadia/PDLC/Tests/AST/Bases.h"

#include "Arcadia/Collections/Include.h"

static Arcadia_ImmutableList*
makeNatural8
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_List* temporary = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_List_insertBackObjectReferenceValue(thread, temporary, Arcadia_String_createFromCxxString(thread, u8"Arcadia"));
  Arcadia_List_insertBackObjectReferenceValue(thread, temporary, Arcadia_String_createFromCxxString(thread, u8"Natural8"));
  return Arcadia_ImmutableList_create(thread, Arcadia_Value_makeObjectReferenceValue(temporary));
}

static void
test1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_List* constructorParameters = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_List_insertBackObjectReferenceValue(thread, constructorParameters, Arcadia_PDLC_AST_FieldDefinitionNode_create(thread, 0, Arcadia_String_createFromCxxString(thread, u8"red"), Arcadia_PDLC_AST_IdentifierNode_create(thread, 0, makeNatural8(thread))));
  Arcadia_List_insertBackObjectReferenceValue(thread, constructorParameters, Arcadia_PDLC_AST_FieldDefinitionNode_create(thread, 0, Arcadia_String_createFromCxxString(thread, u8"green"), Arcadia_PDLC_AST_IdentifierNode_create(thread, 0, makeNatural8(thread))));
  Arcadia_List_insertBackObjectReferenceValue(thread, constructorParameters, Arcadia_PDLC_AST_FieldDefinitionNode_create(thread, 0, Arcadia_String_createFromCxxString(thread, u8"blue"), Arcadia_PDLC_AST_IdentifierNode_create(thread, 0, makeNatural8(thread))));
  Arcadia_PDLC_AST_ConstructorDefinitionNode* constructor = Arcadia_PDLC_AST_ConstructorDefinitionNode_create(thread, 0, NULL, constructorParameters, NULL);

  Arcadia_List* classBody = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_List_insertBackObjectReferenceValue(thread, classBody, constructor);
  Arcadia_List_insertBackObjectReferenceValue(thread, classBody, Arcadia_PDLC_AST_FieldDefinitionNode_create(thread, 0, Arcadia_String_createFromCxxString(thread, u8"red"), Arcadia_PDLC_AST_IdentifierNode_create(thread, 0, makeNatural8(thread))));
  Arcadia_List_insertBackObjectReferenceValue(thread, classBody, Arcadia_PDLC_AST_FieldDefinitionNode_create(thread, 0, Arcadia_String_createFromCxxString(thread, u8"green"), Arcadia_PDLC_AST_IdentifierNode_create(thread, 0, makeNatural8(thread))));
  Arcadia_List_insertBackObjectReferenceValue(thread, classBody, Arcadia_PDLC_AST_FieldDefinitionNode_create(thread, 0, Arcadia_String_createFromCxxString(thread, u8"blue"), Arcadia_PDLC_AST_IdentifierNode_create(thread, 0, makeNatural8(thread))));

  Arcadia_ImmutableList* className = NULL,
                       * extendedClassName = NULL;

  Arcadia_List* temporary = (Arcadia_List*)Arcadia_ArrayList_create(thread);

  Arcadia_Collection_clear(thread, (Arcadia_Collection*)temporary);
  Arcadia_List_insertBackObjectReferenceValue(thread, temporary, Arcadia_String_createFromCxxString(thread, "Arcadia"));
  Arcadia_List_insertBackObjectReferenceValue(thread, temporary, Arcadia_String_createFromCxxString(thread, "AssetDefinitionLanguage"));
  Arcadia_List_insertBackObjectReferenceValue(thread, temporary, Arcadia_String_createFromCxxString(thread, "ColorDefinition"));
  className = Arcadia_ImmutableList_create(thread, Arcadia_Value_makeObjectReferenceValue(temporary));

  Arcadia_Collection_clear(thread, (Arcadia_Collection*)temporary);
  Arcadia_List_insertBackObjectReferenceValue(thread, temporary, Arcadia_String_createFromCxxString(thread, "Arcadia"));
  Arcadia_List_insertBackObjectReferenceValue(thread, temporary, Arcadia_String_createFromCxxString(thread, "AssetDefinitionLanguage"));
  Arcadia_List_insertBackObjectReferenceValue(thread, temporary, Arcadia_String_createFromCxxString(thread, "Definition"));
  extendedClassName = Arcadia_ImmutableList_create(thread, Arcadia_Value_makeObjectReferenceValue(temporary));

  Arcadia_PDLC_AST_ClassDefinitionNode* node = Arcadia_PDLC_AST_ClassDefinitionNode_create
    (
      thread,
      0,
      Arcadia_BooleanValue_False,
      Arcadia_BooleanValue_True,
      Arcadia_PDLC_AST_IdentifierNode_create(thread, 0, className),
      Arcadia_PDLC_AST_IdentifierNode_create(thread, 0, extendedClassName),
      NULL,
      classBody
    );
  Arcadia_Tests_assertTrue(thread, node->isAbstract && !node->isNative);
}

static void
testReturnOperands
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_PDLC_AST_ReturnInstructionNode* bare = Arcadia_PDLC_AST_ReturnInstructionNode_create(thread, 0, NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, bare);
  Arcadia_PDLC_AST_OperandNode* operand = (void*)Arcadia_PDLC_AST_VariableOperandNode_create(thread, 0, Arcadia_String_createFromCxxString(thread, "result"));
  Arcadia_PDLC_AST_ReturnInstructionNode* valued = Arcadia_PDLC_AST_ReturnInstructionNode_create(thread, 0, operand);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, valued);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, !bare->operand && valued->operand == operand);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)valued->operand, _Arcadia_PDLC_AST_VariableOperandNode_getType(thread)));
  Arcadia_ValueStack_popValues(thread, 2);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static int
runClassDefinition
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&test1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runReturnOperands
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testReturnOperands) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "ClassDefinition", &runClassDefinition },
    { "ReturnOperands", &runReturnOperands },
    { "Bases", &Arcadia_PDLC_Tests_AST_bases },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
