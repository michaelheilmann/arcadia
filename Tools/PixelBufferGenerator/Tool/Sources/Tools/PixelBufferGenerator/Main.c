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
#include "Arcadia/DDL/Include.h"
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Logging/Include.h"
#include "Arcadia/Print/Include.h"
#include "Arcadia/PixelBufferIO/Include.h"
#include "Arcadia/ADL/Include.h"

Arcadia_NoReturn() static void
raiseNotAPixelBufferDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_String* key
  )
{
  Arcadia_ADL_Diagnostics_raiseNotAPixelBufferDefinition(thread, key);
}

Arcadia_NoReturn() static void
raiseADLPixelBufferOperationNotYetSupported
  (
    Arcadia_Thread* thread,
    Arcadia_String* key
  )
{
  Arcadia_ADL_Diagnostics_raisePixelBufferOperationNotYetSupported(thread, key);
}

Arcadia_NoReturn() static void
raiseADLDefinitionNotFound
  (
    Arcadia_Thread* thread,
    Arcadia_String* key
  )
{
  Arcadia_ADL_Diagnostics_raiseDefinitionNotFound(thread, key);
}

static Arcadia_ADL_Definition*
loadADL
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String *pathString
  )
{
  // (1)
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FilePath* path = Arcadia_FileSystem_getWorkingDirectory(thread, fileSystem);
  Arcadia_FilePath* temporary = Arcadia_FilePath_parseGeneric(thread, pathString);
  Arcadia_FilePath_append(thread, path, temporary);
  // (2)
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ByteArrayBuilder* contents = Arcadia_FileSystem_getFileContents(thread, fileSystem, path);
  // (3)
  return Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(contents)), Arcadia_BooleanValue_True);
}

// Expects 
// `--help`
// `--configuration=<path>`
static void
main1
  (
    Arcadia_Thread* thread,
    int argc,
    char** argv
  )
{
  Arcadia_Log* log = (Arcadia_Log*)Arcadia_ConsoleLog_create(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ADL_Definition* definition = NULL;

  Arcadia_Value target, start;
  Arcadia_Value_setVoidValue(&target,Arcadia_VoidValue_Void);
  Arcadia_Value_setVoidValue(&start, Arcadia_VoidValue_Void);

  Arcadia_List* arguments = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  for (int argi = 1; argi < argc; ++argi) {
    Arcadia_String* argument = Arcadia_String_createFromCxxString(thread, argv[argi]);
    Arcadia_List_insertBackObjectReferenceValue(thread, arguments, (Arcadia_ObjectReferenceValue)argument);
  }
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)arguments); i < n; ++i) {
    Arcadia_String* argumentString = (Arcadia_String*)Arcadia_List_getObjectReferenceValueAt(thread, arguments, i);
    Arcadia_UnicodeCodePointReader* r = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, argumentString));
    Arcadia_CommandLineArgument* argument = Arcadia_CommandLine_parseArgument(thread, (Arcadia_UnicodeCodePointReader*)r);
    if (argument->syntacticalError) {
      Arcadia_CommandLine_invalidCommandLineArgumentError(thread, argumentString, log);
    }
    if (Arcadia_String_isEqualTo_pn(thread, argument->name, u8"definition", sizeof(u8"definition") - 1)) {
      if (!argument->value) {
        Arcadia_CommandLine_raiseNoValueError(thread, argument->name, log);
      }
      definition = loadADL(thread, definitions, argument->value);
    } else if (Arcadia_String_isEqualTo_pn(thread, argument->name, u8"target", sizeof(u8"target") - 1)) {
      if (!argument->value) {
        Arcadia_CommandLine_raiseNoValueError(thread, argument->name, log);
      }
      Arcadia_Value_setObjectReferenceValue(&target, argument->value);
    } else if (Arcadia_String_isEqualTo_pn(thread, argument->name, u8"start", sizeof(u8"start") - 1)) {
      if (!argument->value) {
        Arcadia_CommandLine_raiseNoValueError(thread, argument->name, log);
      }
      Arcadia_Value_setObjectReferenceValue(&start, argument->value);
    } else {
      Arcadia_CommandLine_raiseUnknownArgumentError(thread, argument->name, argument->value, log);
    }
    Arcadia_FileHandle* fileHandle = Arcadia_FileSystem_createFileHandle(thread, Arcadia_FileSystem_getOrCreate(thread));
    Arcadia_FileHandle_openStandardOutput(thread, fileHandle);
    Arcadia_SizeValue numberOfArguments = 0;
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)argument->name); numberOfArguments++;
    if (argument->value) {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)Arcadia_String_createFromCxxString(thread, u8" = ")); numberOfArguments++;
      Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)argument->value); numberOfArguments++;
    }
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)Arcadia_String_createFromCxxString(thread, u8"\n")); numberOfArguments++;
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)fileHandle); numberOfArguments++;
    Arcadia_ValueStack_pushNatural8Value(thread, numberOfArguments);
    Arcadia_print(thread);
  }
  if (!Arcadia_Value_isInstanceOf(thread, &target,_Arcadia_String_getType(thread))) {
    Arcadia_CommandLine_raiseRequiredArgumentMissingError(thread, Arcadia_String_createFromCxxString(thread, u8"target"), log);
  }
  if (!Arcadia_Value_isInstanceOf(thread, &start, _Arcadia_String_getType(thread))) {
    Arcadia_CommandLine_raiseRequiredArgumentMissingError(thread, Arcadia_String_createFromCxxString(thread, u8"start"), log);
  }
  Arcadia_Media_PixelBuffer* pixelBuffer = NULL;
  Arcadia_ADL_Definitions_link(thread, definitions);
  definition = (Arcadia_ADL_Definition*)Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_Value_getObjectReferenceValueChecked(thread, start, _Arcadia_String_getType(thread)));
  if (!definition) {
    raiseADLDefinitionNotFound(thread, (Arcadia_String*)Arcadia_Value_getObjectReferenceValueChecked(thread, start, _Arcadia_String_getType(thread)));
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_PixelBufferDefinition_getType(thread))) {
    Arcadia_ADL_PixelBufferDefinition* p = (Arcadia_ADL_PixelBufferDefinition*)definition;
    pixelBuffer = Arcadia_Media_PixelBuffer_create(thread, 0, p->width, p->height, Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8);
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)p->operations); i < n; ++i) {
      Arcadia_ADL_Reference* e = (Arcadia_ADL_Reference*)Arcadia_List_getObjectReferenceValueAt(thread, p->operations, i);
      Arcadia_Media_PixelBufferOperation* operation;
      if (!e->definition) {
        raiseADLPixelBufferOperationNotYetSupported(thread, e->definitionName);
      }
      if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)e->definition, _Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_getType(thread))) {
        operation = (Arcadia_Media_PixelBufferOperation*)Arcadia_Media_PixelBufferOperations_CheckerboardFill_create(thread, (Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition*)e->definition);
      } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)e->definition, _Arcadia_ADL_PixelBufferOperations_FillOperationDefinition_getType(thread))) {
        operation = (Arcadia_Media_PixelBufferOperation*)Arcadia_Media_PixelBufferOperations_Fill_create(thread, (Arcadia_ADL_PixelBufferOperations_FillOperationDefinition*)e->definition);
      } else {
        raiseADLPixelBufferOperationNotYetSupported(thread, e->definitionName);
      }
      Arcadia_Media_PixelBufferOperation_apply(thread, operation, pixelBuffer);
    }
  } else {
    raiseNotAPixelBufferDefinition(thread, definition->name); // this is an internal error
  }
  Arcadia_Imaging_ImageManager* imageManager = Arcadia_Imaging_ImageManager_getOrCreate(thread);
  Arcadia_String* extension = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"png", sizeof(u8"png") - 1)));
  Arcadia_List* writers = Arcadia_Imaging_ImageManager_getWriters(thread, imageManager, extension);
  if (!Arcadia_Collection_getSize(thread, (Arcadia_Collection*)writers)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
    Arcadia_Thread_popJumpTarget(thread);
  }

  Arcadia_Imaging_ImageWriter* writer = (Arcadia_Imaging_ImageWriter*)Arcadia_List_getObjectReferenceValueAt(thread, writers, 0);
  Arcadia_List* pixelBufferList = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_List_insertBackObjectReferenceValue(thread, pixelBufferList, pixelBuffer);

  Arcadia_Imaging_ImageWriterParameters* parameters = Arcadia_Imaging_ImageWriterParameters_createFile(thread, (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&target), extension);
  Arcadia_Imaging_ImageWriter_write(thread, writer, pixelBufferList, parameters);
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
