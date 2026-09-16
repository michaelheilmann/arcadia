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

#include <stdlib.h>
#include <string.h>

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/FileSystem/Include.h"
#include "Arcadia/Logging/Include.h"
#include "Arcadia/DDLS/Include.h"

static Arcadia_DDLS_SchemaNode*
readDDLS
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* filePath
  )
{
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_ByteArrayBuilder* input0 = Arcadia_FileSystem_getFileContents(thread, fileSystem, filePath);
  Arcadia_ByteArray* input = Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, Arcadia_ByteArrayBuilder_getBytes(thread, input0), Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, input0)));
  Arcadia_DDLS_DefaultReader* reader = Arcadia_DDLS_DefaultReader_create(thread);
  Arcadia_DDLS_Node* ddlsNode = Arcadia_DDLS_DefaultReader_run(thread, reader, input);
  return (Arcadia_DDLS_SchemaNode*)ddlsNode;
}

static Arcadia_DDL_Node*
readDDL
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* filePath
  )
{
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_ByteArrayBuilder* input0 = Arcadia_FileSystem_getFileContents(thread, fileSystem, filePath);
  Arcadia_ByteArray* input1 = Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, Arcadia_ByteArrayBuilder_getBytes(thread, input0), Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, input0)));
  Arcadia_UnicodeCodePointReader* input = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, input1));
  Arcadia_DDL_DefaultReader* reader = (Arcadia_DDL_DefaultReader*)Arcadia_DDL_DefaultReader_create(thread);
  Arcadia_DDL_Node* ddlNode = Arcadia_DDL_DefaultReader_run(thread, reader, input);
  return ddlNode;
}

void
main1
  (
    Arcadia_Thread* thread,
    int argc,
    char** argv
  )
{
  Arcadia_Log* log = (Arcadia_Log*)Arcadia_ConsoleLog_create(thread);
  Arcadia_List* ddlsFiles = (Arcadia_List*)Arcadia_ArrayList_create(thread); // The list of DDLS files.
  Arcadia_String* ddlFile = NULL; // The DDL file.
  Arcadia_String* schema = NULL; // The schema used to validate the DDL file.

  Arcadia_List* arguments = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  for (int argi = 1; argi < argc; ++argi) {
    Arcadia_String* argument = Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, argv[argi], strlen(argv[argi])));
    Arcadia_List_insertBackObjectReferenceValue(thread, arguments, (Arcadia_ObjectReferenceValue)argument);
  }

  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)arguments); i < n; ++i) {
    Arcadia_String* argumentString = (Arcadia_String*)Arcadia_List_getObjectReferenceValueAt(thread, arguments, i);
    Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, argumentString));
    Arcadia_CommandLineArgument* argument = Arcadia_CommandLine_parseArgument(thread, reader);
    if (argument->syntacticalError) {
      Arcadia_CommandLine_invalidCommandLineArgumentError(thread, argumentString, log);
    }
    if (Arcadia_String_isEqualTo_pn(thread, argument->name, u8"ddls", sizeof(u8"ddls") - 1)) {
      // Append a DDLS file.
      if (!argument->value) {
        Arcadia_CommandLine_raiseNoValueError(thread, argument->name, log);
      }
      Arcadia_List_insertBackObjectReferenceValue(thread, ddlsFiles, argument->value);
    } else if (Arcadia_String_isEqualTo_pn(thread, argument->name, u8"schema", sizeof(u8"schema") - 1)) {
      // Store schema.
      if (!argument->value) {
        Arcadia_CommandLine_raiseNoValueError(thread, argument->name, log);
      }
      if (schema) {
        Arcadia_CommandLine_raiseAlreadySpecifiedError(thread, argument->name, log);
      }
      schema = argument->value;
    } else if (Arcadia_String_isEqualTo_pn(thread, argument->name, u8"ddl", sizeof(u8"ddl") - 1)) {
      // Store data file.
      if (!argument->value) {
        Arcadia_CommandLine_raiseNoValueError(thread, argument->name, log);
      }
      if (ddlFile) {
        Arcadia_CommandLine_raiseAlreadySpecifiedError(thread, argument->name, log);
      }
      ddlFile = argument->value;
    } else {
      Arcadia_CommandLine_raiseUnknownArgumentError(thread, argument->name, argument->value, log);
    }
  }
  if (!schema) {
    Arcadia_ByteArrayBuilder* encodingByteBuffer = Arcadia_ByteArrayBuilder_create(thread);
    Arcadia_Unicode_Encoder* encoder = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
    Arcadia_Unicode_Encoder_encodeString(thread, encoder, Arcadia_String_createFromCxxString(thread, u8"no schema name specified\n"), encodingByteBuffer);
    Arcadia_FileHandle* fileHandle = Arcadia_FileSystem_createFileHandle(thread, Arcadia_FileSystem_getOrCreate(thread));
    Arcadia_FileHandle_openStandardError(thread, fileHandle);
    Arcadia_FileHandle_writeByteBuffer(thread, fileHandle, encodingByteBuffer);
  }
  if (!ddlFile) {
    Arcadia_ByteArrayBuilder* encodingByteBuffer = Arcadia_ByteArrayBuilder_create(thread);
    Arcadia_Unicode_Encoder* encoder = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
    Arcadia_Unicode_Encoder_encodeString(thread, encoder, Arcadia_String_createFromCxxString(thread, u8"no DDL file specified\n"), encodingByteBuffer);
    Arcadia_FileHandle* fileHandle = Arcadia_FileSystem_createFileHandle(thread, Arcadia_FileSystem_getOrCreate(thread));
    Arcadia_FileHandle_openStandardError(thread, fileHandle);
    Arcadia_FileHandle_writeByteBuffer(thread, fileHandle, encodingByteBuffer);
  }
  if (Arcadia_Collection_isEmpty(thread, (Arcadia_Collection*)ddlsFiles)) {
    Arcadia_ByteArrayBuilder* encodingByteBuffer = Arcadia_ByteArrayBuilder_create(thread);
    Arcadia_Unicode_Encoder* encoder = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
    Arcadia_Unicode_Encoder_encodeString(thread, encoder, Arcadia_String_createFromCxxString(thread, u8"no DDLS file specified\n"), encodingByteBuffer);
    Arcadia_FileHandle* fileHandle = Arcadia_FileSystem_createFileHandle(thread, Arcadia_FileSystem_getOrCreate(thread));
    Arcadia_FileHandle_openStandardError(thread, fileHandle);
    Arcadia_FileHandle_writeByteBuffer(thread, fileHandle, encodingByteBuffer);
  }
  Arcadia_DDLS_ValidationContext* validationContext = Arcadia_DDLS_ValidationContext_create(thread);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)ddlsFiles); i < n; ++i) {
    Arcadia_String* pathString = (Arcadia_String*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, ddlsFiles, i, _Arcadia_String_getType(thread));
    Arcadia_FilePath* path = Arcadia_FilePath_parseNative(thread, pathString);
    Arcadia_DDLS_SchemaNode* ddlsNode = readDDLS(thread, path);
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, ddlsNode);
  }
  Arcadia_String* pathString = ddlFile;
  Arcadia_FilePath* path = Arcadia_FilePath_parseNative(thread, pathString);
  Arcadia_DDL_Node* ddlNode = readDDL(thread, path);
  Arcadia_DDLS_ValidationContext_run(thread, validationContext, schema, ddlNode);
}

int
main
  (
    int argc,
    char** argv
  )
{
  Arcadia_Process* process = NULL;
  if (Arcadia_Process_get(&process)) {
    return EXIT_FAILURE;
  }
  Arcadia_Thread* thread = Arcadia_Process_getThread(process);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    main1(thread, argc, argv);
  }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Status status = Arcadia_Thread_getStatus(thread);
  thread = NULL;
  Arcadia_Process_relinquish(process);
  process = NULL;
  if (status) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
