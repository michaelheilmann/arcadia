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

#include "Arcadia/DDLS/Implementation/Diagnostics.h"

#include "Arcadia/FileSystem/Include.h"
#include "Arcadia/DDLS/Extensions.h"

static void
Arcadia_DDLS_Diagnostics_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Diagnostics* self
  );

static void
Arcadia_DDLS_Diagnostics_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DiagnosticsDispatch* self
  );

static void
Arcadia_DDLS_Diagnostics_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Diagnostics* self
  );

static Arcadia_String*
expectedToString
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Diagnostics* self,
    Arcadia_DDLS_Node* ddlsNode
  );

static Arcadia_String*
receivedToString
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Diagnostics* self,
    Arcadia_DDL_Node* node
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDLS_Diagnostics_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDLS_Diagnostics_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDLS_Diagnostics_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDLS_Diagnostics_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDLS_Diagnostics_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDLS_Diagnostics_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDLS.Diagnostics", Arcadia_DDLS_Diagnostics,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_Arcadia_DDLS_Diagnostics_typeOperations);

static void
Arcadia_DDLS_Diagnostics_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Diagnostics* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDLS_Diagnostics);
  //
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  self->stringBuffer = Arcadia_StringBuilder_create(thread);
  self->stringTable = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Languages_StringTable_getType(thread));
  //
#define Define(Name, Value) \
  Arcadia_StringBuilder_clear(thread, self->stringBuffer); \
  Arcadia_StringBuilder_insertBackCxxString(thread, self->stringBuffer, Value); \
  self->Name = Arcadia_Languages_StringTable_getOrCreateString(thread, self->stringTable, self->stringBuffer);

  Define(BOOLEAN, u8"Boolean");
  Define(LIST, u8"List");
  Define(MAP, u8"Map");
  Define(NUMBER, u8"Number");
  Define(SCHEMA, u8"Schema");
  Define(SCHEMAREFERENCE, u8"SchemaReference");
  Define(STRING, u8"String");
  Define(VOID, u8"Void");

#undef Define
  //
  Arcadia_LeaveConstructor(Arcadia_DDLS_Diagnostics);
}

static void
Arcadia_DDLS_Diagnostics_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DiagnosticsDispatch* self
  )
{ }

static void
Arcadia_DDLS_Diagnostics_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Diagnostics* self
  )
{
  if (self->stringBuffer) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->stringBuffer);
  }

#define Define(Name, Value) \
  if (self->Name) {\
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->Name); \
  }

  Define(BOOLEAN, u8"Boolean");
  Define(LIST, u8"List");
  Define(MAP, u8"Map");
  Define(NUMBER, u8"Number");
  Define(SCHEMA, u8"Schema");
  Define(SCHEMAREFERENCE, u8"SchemaReference");
  Define(STRING, u8"String");
  Define(VOID, u8"Void");

#undef Define

  if (self->stringTable) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->stringTable);
  }
}

static Arcadia_String*
expectedToString
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Diagnostics* self,
    Arcadia_DDLS_Node* ddlsNode
  )
{
  if (Arcadia_DDLS_isBoolean(thread, ddlsNode)) {
    return self->BOOLEAN;
  } else if (Arcadia_DDLS_isList(thread, ddlsNode)) {
    return self->LIST;
  } else if (Arcadia_DDLS_isMap(thread, ddlsNode)) {
    return self->MAP;
  } else if (Arcadia_DDLS_isNumber(thread, ddlsNode)) {
    return self->NUMBER;
  } else if (Arcadia_DDLS_isSchema(thread, ddlsNode)) {
    return self->SCHEMA;
  } else if (Arcadia_DDLS_isSchemaReference(thread, ddlsNode)) {
    return self->SCHEMAREFERENCE;
  } else if (Arcadia_DDLS_isString(thread, ddlsNode)) {
    return self->STRING;
  } else if (Arcadia_DDLS_isVoid(thread, ddlsNode)) {
    return self->VOID;
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static Arcadia_String*
receivedToString
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Diagnostics* self,
    Arcadia_DDL_Node* node
  )
{
  if (Arcadia_DDL_Node_isBoolean(thread, node)) {
    return self->BOOLEAN;
  } else if (Arcadia_DDL_Node_isList(thread, node)) {
    return self->LIST;
  } else if (Arcadia_DDL_Node_isMap(thread, node)) {
    return self->MAP;
  } else if (Arcadia_DDL_Node_isNumber(thread, node)) {
    return self->NUMBER;
  } else if (Arcadia_DDL_Node_isString(thread, node)) {
    return self->STRING;
  } else if (Arcadia_DDL_Node_isVoid(thread, node)) {
    return self->VOID;
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
}

Arcadia_DDLS_Diagnostics*
Arcadia_DDLS_Diagnostics_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTable* stringTable
  )
{
  _Arcadia_BeginCreate(Arcadia_DDLS_Diagnostics);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)stringTable);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDLS_Diagnostics);
}

void
Arcadia_DDLS_Diagnostics_unexpectedTypeError
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Diagnostics* self,
    Arcadia_DDLS_Node* ddlsNode,
    Arcadia_DDL_Node* node
  )
{
  Arcadia_StringBuilder_clear(thread, self->stringBuffer);
  Arcadia_StringBuilder_insertBackCxxString(thread, self->stringBuffer, u8"expected `");
  Arcadia_StringBuilder_insertBackString(thread, self->stringBuffer, expectedToString(thread, self, ddlsNode));
  Arcadia_StringBuilder_insertBackCxxString(thread, self->stringBuffer, u8"`, received `");
  Arcadia_StringBuilder_insertBackString(thread, self->stringBuffer, receivedToString(thread, self, node));
  Arcadia_StringBuilder_insertBackCxxString(thread, self->stringBuffer, u8"`\n");
  Arcadia_FileSystem* fs = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FileHandle* fd = Arcadia_FileSystem_createFileHandle(thread, fs);
  Arcadia_FileHandle_openStandardOutput(thread, fd);
  Arcadia_SizeValue bytesWritten;
  Arcadia_FileHandle_write(thread, fd, Arcadia_StringBuilder_getBytes(thread, self->stringBuffer),
                                       Arcadia_StringBuilder_getNumberOfBytes(thread, self->stringBuffer), &bytesWritten);
}

void
Arcadia_DDLS_Diagnostics_mapEntryAlreadyDefined
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Diagnostics* self,
    Arcadia_String* name
  )
{
  Arcadia_StringBuilder_clear(thread, self->stringBuffer);
  Arcadia_StringBuilder_insertBackCxxString(thread, self->stringBuffer, u8"a map entry of name `");
  Arcadia_StringBuilder_insertBackString(thread, self->stringBuffer, name);
  Arcadia_StringBuilder_insertBackCxxString(thread, self->stringBuffer, u8"` was already specified");
  Arcadia_StringBuilder_insertBackCxxString(thread, self->stringBuffer, u8"\n");
  Arcadia_FileSystem* fs = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FileHandle* fd = Arcadia_FileSystem_createFileHandle(thread, fs);
  Arcadia_FileHandle_openStandardOutput(thread, fd);
  Arcadia_SizeValue bytesWritten;
  Arcadia_FileHandle_write(thread, fd, Arcadia_StringBuilder_getBytes(thread, self->stringBuffer),
                                       Arcadia_StringBuilder_getNumberOfBytes(thread, self->stringBuffer), &bytesWritten);
}

void
Arcadia_DDLS_Diagnostics_mapEntryExistsError
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Diagnostics* self,
    Arcadia_String* name
  )
{
  Arcadia_StringBuilder_clear(thread, self->stringBuffer);
  Arcadia_StringBuilder_insertBackCxxString(thread, self->stringBuffer, u8"a map entry of name `");
  Arcadia_StringBuilder_insertBackString(thread, self->stringBuffer, name);
  Arcadia_StringBuilder_insertBackCxxString(thread, self->stringBuffer, u8"` exists but is not permitted by the type");
  Arcadia_StringBuilder_insertBackCxxString(thread, self->stringBuffer, u8"\n");
  Arcadia_FileSystem* fs = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FileHandle* fd = Arcadia_FileSystem_createFileHandle(thread, fs);
  Arcadia_FileHandle_openStandardOutput(thread, fd);
  Arcadia_SizeValue bytesWritten;
  Arcadia_FileHandle_write(thread, fd, Arcadia_StringBuilder_getBytes(thread, self->stringBuffer),
                                       Arcadia_StringBuilder_getNumberOfBytes(thread, self->stringBuffer), &bytesWritten);
}

void
Arcadia_DDLS_Diagnostics_mapEntryNotExistsError
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Diagnostics* self,
    Arcadia_String* name
  )
{
  Arcadia_StringBuilder_clear(thread, self->stringBuffer);
  Arcadia_StringBuilder_insertBackCxxString(thread, self->stringBuffer, u8"a map entry of name `");
  Arcadia_StringBuilder_insertBackString(thread, self->stringBuffer, name);
  Arcadia_StringBuilder_insertBackCxxString(thread, self->stringBuffer, u8"` not exists but is required by the type");
  Arcadia_StringBuilder_insertBackCxxString(thread, self->stringBuffer, u8"\n");
  Arcadia_FileSystem* fs = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FileHandle* fd = Arcadia_FileSystem_createFileHandle(thread, fs);
  Arcadia_FileHandle_openStandardOutput(thread, fd);
  Arcadia_SizeValue bytesWritten;
  Arcadia_FileHandle_write(thread, fd, Arcadia_StringBuilder_getBytes(thread, self->stringBuffer),
                                       Arcadia_StringBuilder_getNumberOfBytes(thread, self->stringBuffer), &bytesWritten);
}

void
Arcadia_DDLS_Diagnostics_unresolvedSchemaReferenceError
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Diagnostics* self,
    Arcadia_String* name
  )
{
  Arcadia_StringBuilder_clear(thread, self->stringBuffer);
  Arcadia_StringBuilder_insertBackCxxString(thread, self->stringBuffer, u8"unresolved schema reference `");
  Arcadia_StringBuilder_insertBackString(thread, self->stringBuffer, name);
  Arcadia_StringBuilder_insertBackCxxString(thread, self->stringBuffer, u8"`");
  Arcadia_StringBuilder_insertBackCxxString(thread, self->stringBuffer, u8"\n");
  Arcadia_FileSystem* fs = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FileHandle* fd = Arcadia_FileSystem_createFileHandle(thread, fs);
  Arcadia_FileHandle_openStandardOutput(thread, fd);
  Arcadia_SizeValue bytesWritten;
  Arcadia_FileHandle_write(thread, fd, Arcadia_StringBuilder_getBytes(thread, self->stringBuffer),
                                       Arcadia_StringBuilder_getNumberOfBytes(thread, self->stringBuffer), &bytesWritten);
}
