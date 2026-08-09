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

#define ARCADIA_DDL_MODULE (1)
#include "Arcadia/DDL/Writer/Unparser.h"

#include "Arcadia/DDL/Include.h"

struct Arcadia_DataDefinitionLanguage_UnparserDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_DataDefinitionLanguage_Unparser {
  Arcadia_Object _parent;

  Arcadia_Unicode_Encoder* encoder;

  Arcadia_String* colon;
  Arcadia_String* comma;
  Arcadia_String* doubleQuotes;
  Arcadia_String* listClosingDelimiter;
  Arcadia_String* listOpeningDelimiter;
  Arcadia_String* mapClosingDelimiter;
  Arcadia_String* mapOpeningDelimiter;
  Arcadia_String* newline;
  Arcadia_String* whitespace;
};

static void
whitespace
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self,
    Arcadia_ByteArrayBuilder* targetBuffer,
    Arcadia_SizeValue count
  );

static void
Arcadia_DataDefinitionLanguage_Unparser_writeBooleanValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self,
    Arcadia_DDL_BooleanNode* node,
    Arcadia_SizeValue indent,
    Arcadia_ByteArrayBuilder* targetBuffer
  );

static void
Arcadia_DataDefinitionLanguage_Unparser_writeListValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self,
    Arcadia_DDL_ListNode* node,
    Arcadia_SizeValue indent,
    Arcadia_ByteArrayBuilder* targetBuffer
  );

static void
Arcadia_DataDefinitionLanguage_Unparser_writeMapValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self,
    Arcadia_DDL_MapNode* node,
    Arcadia_SizeValue indent,
    Arcadia_ByteArrayBuilder* targetBuffer
  );

static void
Arcadia_DataDefinitionLanguage_Unparser_writeNumberValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self,
    Arcadia_DDL_NumberNode* node,
    Arcadia_SizeValue indent,
    Arcadia_ByteArrayBuilder* targetBuffer
  );

static void
Arcadia_DataDefinitionLanguage_Unparser_writeStringValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self,
    Arcadia_DDL_StringNode* node,
    Arcadia_SizeValue indent,
    Arcadia_ByteArrayBuilder* targetBuffer
  );

static void
Arcadia_DataDefinitionLanguage_Unparser_writeValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self,
    Arcadia_DDL_Node* node,
    Arcadia_SizeValue indent,
    Arcadia_ByteArrayBuilder* targetBuffer
  );

static void
Arcadia_DataDefinitionLanguage_Unparser_writeVoidValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self,
    Arcadia_DDL_VoidNode* node,
    Arcadia_SizeValue indent,
    Arcadia_ByteArrayBuilder* targetBuffer
  );

static void
Arcadia_DataDefinitionLanguage_Unparser_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self
  );

static void
Arcadia_DataDefinitionLanguage_Unparser_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_UnparserDispatch* self
  );

static void
Arcadia_DataDefinitionLanguage_Unparser_visit
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DataDefinitionLanguage_Unparser_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DataDefinitionLanguage_Unparser_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DataDefinitionLanguage_Unparser_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DataDefinitionLanguage.Unparser", Arcadia_DataDefinitionLanguage_Unparser,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);
static void
whitespace
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self,
    Arcadia_ByteArrayBuilder* targetBuffer,
    Arcadia_SizeValue count
  )
{
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    Arcadia_Natural32Value codePoint = ' ';
    Arcadia_Unicode_Encoder_encodeCodePoints(thread, self->encoder, &codePoint, 1, targetBuffer);
  }
}

static void
Arcadia_DataDefinitionLanguage_Unparser_writeBooleanValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self,
    Arcadia_DDL_BooleanNode* node,
    Arcadia_SizeValue indent,
    Arcadia_ByteArrayBuilder* targetBuffer
  )
{
  Arcadia_Unicode_Encoder_encodeString(thread, self->encoder, node->value, targetBuffer);
}

static void
Arcadia_DataDefinitionLanguage_Unparser_writeListValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self,
    Arcadia_DDL_ListNode* node,
    Arcadia_SizeValue indent,
    Arcadia_ByteArrayBuilder* targetBuffer
  )
{
  Arcadia_Unicode_Encoder_encodeString(thread, self->encoder, self->listOpeningDelimiter, targetBuffer);
  Arcadia_Unicode_Encoder_encodeString(thread, self->encoder, self->newline, targetBuffer);
  indent += 2;
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)node->elements); i < n; ++i) {
    Arcadia_DDL_Node* elementNode = (Arcadia_DDL_Node*)Arcadia_List_getObjectReferenceValueAt(thread, node->elements, i);
    whitespace(thread, self, targetBuffer, indent);
    Arcadia_DataDefinitionLanguage_Unparser_writeValue(thread, self, elementNode, indent, targetBuffer);
    Arcadia_Unicode_Encoder_encodeString(thread, self->encoder, self->comma, targetBuffer);
    Arcadia_Unicode_Encoder_encodeString(thread, self->encoder, self->newline, targetBuffer);
  }
  indent -= 2;
  whitespace(thread, self, targetBuffer, indent);
  Arcadia_Unicode_Encoder_encodeString(thread, self->encoder, self->listClosingDelimiter, targetBuffer);
}

static void
Arcadia_DataDefinitionLanguage_Unparser_writeMapValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self,
    Arcadia_DDL_MapNode* node,
    Arcadia_SizeValue indent,
    Arcadia_ByteArrayBuilder* targetBuffer
  )
{
  Arcadia_Unicode_Encoder_encodeString(thread, self->encoder, self->mapOpeningDelimiter, targetBuffer);
  Arcadia_Unicode_Encoder_encodeString(thread, self->encoder, self->newline, targetBuffer);
  indent += 2;
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)node->entries); i < n; ++i) {
    Arcadia_DDL_MapEntryNode* entryNode =
      (Arcadia_DDL_MapEntryNode*)Arcadia_List_getObjectReferenceValueAt(thread, node->entries, i);
    Arcadia_DDL_NameNode* keyNode = entryNode->key;
    whitespace(thread, self, targetBuffer, indent);
    Arcadia_Unicode_Encoder_encodeString(thread, self->encoder, keyNode->value, targetBuffer);
    Arcadia_Unicode_Encoder_encodeString(thread, self->encoder, self->whitespace, targetBuffer);
    Arcadia_Unicode_Encoder_encodeString(thread, self->encoder, self->colon, targetBuffer);
    Arcadia_Unicode_Encoder_encodeString(thread, self->encoder, self->whitespace, targetBuffer);
    Arcadia_DDL_Node* valueNode = entryNode->value;
    Arcadia_DataDefinitionLanguage_Unparser_writeValue(thread, self, valueNode, indent, targetBuffer);
    Arcadia_Unicode_Encoder_encodeString(thread, self->encoder, self->comma, targetBuffer);
    Arcadia_Unicode_Encoder_encodeString(thread, self->encoder, self->newline, targetBuffer);
  }
  indent -= 2;
  whitespace(thread, self, targetBuffer, indent);
  Arcadia_Unicode_Encoder_encodeString(thread, self->encoder, self->mapClosingDelimiter, targetBuffer);
}

static void
Arcadia_DataDefinitionLanguage_Unparser_writeNumberValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self,
    Arcadia_DDL_NumberNode* node,
    Arcadia_SizeValue indent,
    Arcadia_ByteArrayBuilder* targetBuffer
  )
{
  Arcadia_Unicode_Encoder_encodeString(thread, self->encoder, node->value, targetBuffer);
}

static void
Arcadia_DataDefinitionLanguage_Unparser_writeStringValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self,
    Arcadia_DDL_StringNode* node,
    Arcadia_SizeValue indent,
    Arcadia_ByteArrayBuilder* targetBuffer
  )
{
  Arcadia_String* string = node->value;
  Arcadia_Unicode_Encoder_encodeString(thread, self->encoder, self->doubleQuotes, targetBuffer);
  Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, string));
  while (Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
    Arcadia_Natural32Value codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
    // Escape all control characters [0,1f].
    if (codePoint <= 0x1f) {
      static const Arcadia_Natural32Value codePoints[] = { '\\', 'u', '0', '0' };
      Arcadia_Unicode_Encoder_encodeCodePoints(thread, self->encoder, codePoints, 4, targetBuffer);
      Arcadia_Natural32Value x = codePoint % 16;
      codePoint /= 16;
      Arcadia_Natural32Value y = codePoint % 16;
      codePoint /= 16;
      static const Arcadia_Natural32Value hexDigit[] = {
        '0',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        'A',
        'B',
        'C',
        'D',
        'E',
        'F'
      };
      Arcadia_Unicode_Encoder_encodeCodePoints(thread, self->encoder, &hexDigit[y], 1, targetBuffer);
      Arcadia_Unicode_Encoder_encodeCodePoints(thread, self->encoder, &hexDigit[x], 1, targetBuffer);
    } else {
      switch (codePoint) {
        case '"': {
          static const uint32_t codePoints[] = { '\\', '"' };
          Arcadia_Unicode_Encoder_encodeCodePoints(thread, self->encoder, codePoints, 2, targetBuffer);
        } break;
        case '\\': {
          static const uint32_t codePoints[] = { '\\', '\\' };
          Arcadia_Unicode_Encoder_encodeCodePoints(thread, self->encoder, codePoints, 2, targetBuffer);
        } break;
        default: {
          Arcadia_Unicode_Encoder_encodeCodePoints(thread, self->encoder, &codePoint, 1, targetBuffer);
        } break;
      };
    }
    Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
  }
  Arcadia_Unicode_Encoder_encodeString(thread, self->encoder, self->doubleQuotes, targetBuffer);
}

static void
Arcadia_DataDefinitionLanguage_Unparser_writeValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self,
    Arcadia_DDL_Node* node,
    Arcadia_SizeValue indent,
    Arcadia_ByteArrayBuilder* targetBuffer
  )
{
  switch (node->type) {
    case Arcadia_DDL_NodeType_Boolean: {
      Arcadia_DataDefinitionLanguage_Unparser_writeBooleanValue(thread, self, (Arcadia_DDL_BooleanNode*)node, indent, targetBuffer);
    } break;
    case Arcadia_DDL_NodeType_List: {
      Arcadia_DataDefinitionLanguage_Unparser_writeListValue(thread, self, (Arcadia_DDL_ListNode*)node, indent, targetBuffer);
    } break;
    case Arcadia_DDL_NodeType_Map: {
      Arcadia_DataDefinitionLanguage_Unparser_writeMapValue(thread, self, (Arcadia_DDL_MapNode*)node, indent, targetBuffer);
    } break;
    case Arcadia_DDL_NodeType_Number: {
      Arcadia_DataDefinitionLanguage_Unparser_writeNumberValue(thread, self, (Arcadia_DDL_NumberNode*)node, indent, targetBuffer);
    } break;
    case Arcadia_DDL_NodeType_String: {
      Arcadia_DataDefinitionLanguage_Unparser_writeStringValue(thread, self, (Arcadia_DDL_StringNode*)node, indent, targetBuffer);
    } break;
    case Arcadia_DDL_NodeType_Void: {
      Arcadia_DataDefinitionLanguage_Unparser_writeVoidValue(thread, self, (Arcadia_DDL_VoidNode*)node, indent, targetBuffer);
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  }
}

static void
Arcadia_DataDefinitionLanguage_Unparser_writeVoidValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self,
    Arcadia_DDL_VoidNode* node,
    Arcadia_SizeValue indent,
    Arcadia_ByteArrayBuilder* targetBuffer
  )
{
  Arcadia_Unicode_Encoder_encodeString(thread, self->encoder, node->value, targetBuffer);
}

static void
Arcadia_DataDefinitionLanguage_Unparser_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DataDefinitionLanguage_Unparser);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  self->encoder = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Unicode_Encoder_getType(thread));
  //
  self->colon = Arcadia_String_createFromCxxString(thread, u8":");
  self->comma = Arcadia_String_createFromCxxString(thread, u8",");
  self->doubleQuotes = Arcadia_String_createFromCxxString(thread, u8"\"");
  self->listClosingDelimiter = Arcadia_String_createFromCxxString(thread, u8"]");
  self->listOpeningDelimiter = Arcadia_String_createFromCxxString(thread, u8"[");
  self->mapOpeningDelimiter= Arcadia_String_createFromCxxString(thread, u8"{");
  self->mapClosingDelimiter = Arcadia_String_createFromCxxString(thread, u8"}");
  self->newline = Arcadia_String_createFromCxxString(thread, u8"\n");
  self->whitespace = Arcadia_String_createFromCxxString(thread, u8" ");
  //
  Arcadia_LeaveConstructor(Arcadia_DataDefinitionLanguage_Unparser);
}

static void
Arcadia_DataDefinitionLanguage_Unparser_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_UnparserDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_DataDefinitionLanguage_Unparser_visit
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self
  )
{
  if (self->encoder) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->encoder);
  }
  if (self->colon) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->colon);
  }
  if (self->comma) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->comma);
  }
  if (self->doubleQuotes) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->doubleQuotes);
  }
  if (self->listClosingDelimiter) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->listClosingDelimiter);
  }
  if (self->listOpeningDelimiter) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->listOpeningDelimiter);
  }
  if (self->mapClosingDelimiter) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->mapClosingDelimiter);
  }
  if (self->mapOpeningDelimiter) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->mapOpeningDelimiter);
  }
  if (self->newline) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->newline);
  }
  if (self->whitespace) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->whitespace);
  }
}

Arcadia_DataDefinitionLanguage_Unparser*
Arcadia_DataDefinitionLanguage_Unparser_create
  (
    Arcadia_Thread* thread,
    Arcadia_Unicode_Encoder* encoder
  )
{
  _Arcadia_BeginCreate(Arcadia_DataDefinitionLanguage_Unparser);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)encoder);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DataDefinitionLanguage_Unparser);
}

void
Arcadia_DataDefinitionLanguage_Unparser_run
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self,
    Arcadia_DDL_Node* node,
    Arcadia_ByteArrayBuilder* targetBuffer
  )
{
  Arcadia_DataDefinitionLanguage_Unparser_writeValue(thread, self, node, Arcadia_SizeValue_Literal(0), targetBuffer);
  Arcadia_Unicode_Encoder_encodeString(thread, self->encoder, self->newline, targetBuffer);
}
