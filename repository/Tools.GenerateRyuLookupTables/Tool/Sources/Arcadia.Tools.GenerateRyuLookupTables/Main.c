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

#include <stdio.h> // @todo Remove references to `stdio.h`.
#include <stdlib.h>
#include <string.h>

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Logging/Include.h"
#include "Arcadia.Tools.GenerateRyuLookupTables/generateReal32Table.h"
#include "Arcadia.Tools.GenerateRyuLookupTables/generateReal64Table.h"
#include "Arcadia.Tools.GenerateRyuLookupTables/Configure.h"

void
main1
  (
    Arcadia_Thread* thread,
    int argc,
    char** argv
  )
{
  Arcadia_Log* log = (Arcadia_Log*)Arcadia_ConsoleLog_create(thread);
  Arcadia_Value target;
  Arcadia_Value_setVoidValue(&target, Arcadia_VoidValue_Void);
  Arcadia_List* arguments = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  for (int argi = 1; argi < argc; ++argi) {
    Arcadia_String* argument = Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, argv[argi], strlen(argv[argi])));
    Arcadia_List_insertBackObjectReferenceValue(thread, arguments, (Arcadia_ObjectReferenceValue)argument);
  }
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)arguments); i < n; ++i) {
    Arcadia_String* argumentString = (Arcadia_String*)Arcadia_List_getObjectReferenceValueAt(thread, arguments, i);
    Arcadia_UnicodeCodePointReader* r = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, argumentString));
    Arcadia_CommandLineArgument* argument = Arcadia_CommandLine_parseArgument(thread, (Arcadia_UnicodeCodePointReader*)r);
    if (argument->syntacticalError) {
      Arcadia_CommandLine_invalidCommandLineArgumentError(thread, argumentString, log);
    }
    if (Arcadia_String_isEqualTo_pn(thread, argument->name, u8"target", sizeof(u8"target") - 1)) {
      if (!argument->value) {
        Arcadia_CommandLine_raiseNoValueError(thread, argument->name, log);
      }
      Arcadia_Value_setObjectReferenceValue(&target, argument->value);
    } else {
      Arcadia_CommandLine_raiseUnknownArgumentError(thread, argument->name, argument->value, log);
    }
    // TODO Remove references to fwrite.
    fwrite(Arcadia_String_getBytes(thread, argument->name), 1, Arcadia_String_getNumberOfBytes(thread, argument->name), stdout);
    if (argument->value) {
      // TODO Remove references to fwrite.
      fwrite(u8"=", 1, sizeof(u8"=") - 1, stdout);
      // TODO Remove references to fwrite.
      fwrite(Arcadia_String_getBytes(thread, argument->value), 1, Arcadia_String_getNumberOfBytes(thread, argument->value), stdout);
    }
    // TODO Remove references to fwrite.
    fwrite(u8"\n", 1, sizeof(u8"\n") - 1, stdout);
  }
  if (Arcadia_Value_isVoidValue(&target)) {
    Arcadia_CommandLine_raiseRequiredArgumentMissingError(thread, Arcadia_String_createFromCxxString(thread, u8"target"), log);
  }

  Arcadia_String* pathPrefixString = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&target);
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FilePath* pathPrefix = Arcadia_FilePath_parseNative(thread, pathPrefixString);
  Arcadia_StringBuilder* resultString = Arcadia_StringBuilder_create(thread);
  Arcadia_ByteArrayBuilder* resultBytes = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_FilePath* path;

  Arcadia_Tools_RyuLookupTableGenerator_generateReal64Table(thread, Arcadia_BooleanValue_True, resultString);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, resultBytes, Arcadia_StringBuilder_getBytes(thread, resultString), Arcadia_StringBuilder_getNumberOfBytes(thread, resultString));
  path = Arcadia_FilePath_clone(thread, pathPrefix);
  Arcadia_FilePath_append(thread, path, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"BigTablesReal64.i")));
  Arcadia_FileSystem_setFileContents(thread, fileSystem, path, resultBytes);
  Arcadia_StringBuilder_clear(thread, resultString);
  Arcadia_ByteArrayBuilder_clear(thread, resultBytes);

  Arcadia_Tools_RyuLookupTableGenerator_generateReal64Table(thread, Arcadia_BooleanValue_False, resultString);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, resultBytes, Arcadia_StringBuilder_getBytes(thread, resultString), Arcadia_StringBuilder_getNumberOfBytes(thread, resultString));
  path = Arcadia_FilePath_clone(thread, pathPrefix);
  Arcadia_FilePath_append(thread, path, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"SmallTablesReal64.i")));
  Arcadia_FileSystem_setFileContents(thread, fileSystem, path, resultBytes);
  Arcadia_StringBuilder_clear(thread, resultString);
  Arcadia_ByteArrayBuilder_clear(thread, resultBytes);

  Arcadia_Tools_RyuLookupTableGenerator_generateReal32Table(thread, resultString);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, resultBytes, Arcadia_StringBuilder_getBytes(thread, resultString), Arcadia_StringBuilder_getNumberOfBytes(thread, resultString));
  path = Arcadia_FilePath_clone(thread, pathPrefix);
  Arcadia_FilePath_append(thread, path, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"TablesReal32.i")));
  Arcadia_FileSystem_setFileContents(thread, fileSystem, path, resultBytes);
  Arcadia_StringBuilder_clear(thread, resultString);
  Arcadia_ByteArrayBuilder_clear(thread, resultBytes);
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
