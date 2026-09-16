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

#include "Arcadia/MILC/Include.h"

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
  Arcadia_List_insertBackObjectReferenceValue(thread, constructorParameters, Arcadia_MILC_AST_FieldDefinitionNode_create(thread, 0, Arcadia_String_createFromCxxString(thread, u8"red"), Arcadia_MILC_AST_IdentifierNode_create(thread, 0, makeNatural8(thread))));
  Arcadia_List_insertBackObjectReferenceValue(thread, constructorParameters, Arcadia_MILC_AST_FieldDefinitionNode_create(thread, 0, Arcadia_String_createFromCxxString(thread, u8"green"), Arcadia_MILC_AST_IdentifierNode_create(thread, 0, makeNatural8(thread))));
  Arcadia_List_insertBackObjectReferenceValue(thread, constructorParameters, Arcadia_MILC_AST_FieldDefinitionNode_create(thread, 0, Arcadia_String_createFromCxxString(thread, u8"blue"), Arcadia_MILC_AST_IdentifierNode_create(thread, 0, makeNatural8(thread))));
  Arcadia_MILC_AST_ConstructorDefinitionNode* constructor = Arcadia_MILC_AST_ConstructorDefinitionNode_create(thread, 0, NULL, constructorParameters, NULL);

  Arcadia_List* classBody = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_List_insertBackObjectReferenceValue(thread, classBody, constructor);
  Arcadia_List_insertBackObjectReferenceValue(thread, classBody, Arcadia_MILC_AST_FieldDefinitionNode_create(thread, 0, Arcadia_String_createFromCxxString(thread, u8"red"), Arcadia_MILC_AST_IdentifierNode_create(thread, 0, makeNatural8(thread))));
  Arcadia_List_insertBackObjectReferenceValue(thread, classBody, Arcadia_MILC_AST_FieldDefinitionNode_create(thread, 0, Arcadia_String_createFromCxxString(thread, u8"green"), Arcadia_MILC_AST_IdentifierNode_create(thread, 0, makeNatural8(thread))));
  Arcadia_List_insertBackObjectReferenceValue(thread, classBody, Arcadia_MILC_AST_FieldDefinitionNode_create(thread, 0, Arcadia_String_createFromCxxString(thread, u8"blue"), Arcadia_MILC_AST_IdentifierNode_create(thread, 0, makeNatural8(thread))));

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

  Arcadia_MILC_AST_ClassDefinitionNode_create
    (
      thread,
      0,
      Arcadia_MILC_AST_IdentifierNode_create(thread, 0, className),
      Arcadia_MILC_AST_IdentifierNode_create(thread, 0, extendedClassName),
      classBody
    );
}

int
main
  (
    int argc,
    char** argv
  )
{
  if (!Arcadia_Tests_safeExecute(&test1)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
