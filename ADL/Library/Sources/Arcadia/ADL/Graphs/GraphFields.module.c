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
#include "Arcadia/ADL/Graphs/GraphFields.module.h"

#include "Arcadia/ADL/Graphs/Graphs.module.h"

Arcadia_DDL_MapEntryNode*
Arcadia_ADL_GraphFields_getEntryOrNull
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* mapNode,
    Arcadia_String* key
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)mapNode->entries); i < n; ++i) {
    Arcadia_DDL_MapEntryNode* mapEntryNode =
      (Arcadia_DDL_MapEntryNode*)
      Arcadia_List_getObjectReferenceValueCheckedAt
        (
          thread,
          (Arcadia_List*)mapNode->entries,
          i,
          _Arcadia_DDL_MapEntryNode_getType(thread)
        );
    Arcadia_DDL_NameNode* keyNode = mapEntryNode->key;
    Arcadia_Value t = Arcadia_Value_makeObjectReferenceValue(key);
    if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)keyNode->value, &t)) {
      return mapEntryNode;
    }
  }
  return NULL;
}

Arcadia_String*
Arcadia_ADL_GraphFields_getStringField
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* mapNode,
    const char* cxxKey
  )
{
  Arcadia_String* key = Arcadia_String_createFromCxxString(thread, cxxKey);
  Arcadia_DDL_MapEntryNode* entry = Arcadia_ADL_GraphFields_getEntryOrNull(thread, mapNode, key);
  if (NULL == entry) {
    Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"missing required field `", key, u8"`");
  }
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)entry->value, _Arcadia_DDL_StringNode_getType(thread))) {
    Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"field `", key, u8"` must be a string");
  }
  return ((Arcadia_DDL_StringNode*)entry->value)->value;
}

Arcadia_String*
Arcadia_ADL_GraphFields_getNonEmptyStringField
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* mapNode,
    const char* cxxKey
  )
{
  Arcadia_String* value = Arcadia_ADL_GraphFields_getStringField(thread, mapNode, cxxKey);
  if (Arcadia_ADL_Graphs_stringIsEmpty(thread, value)) {
    Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"field `", Arcadia_String_createFromCxxString(thread, cxxKey), u8"` must not be empty");
  }
  return value;
}
