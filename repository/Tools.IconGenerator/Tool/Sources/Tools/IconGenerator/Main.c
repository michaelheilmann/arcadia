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
#include <stdio.h> // @todo Remove references to `stdio.h`.
#include <string.h>
#include "Arcadia/Logging/Include.h"
#include "Arcadia/PixelBufferIO/Include.h"

// @todo Remove references to `stdio.h`.
static void
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
    fwrite(Arcadia_String_getBytes(thread, argument->name), 1, Arcadia_String_getNumberOfBytes(thread, argument->name), stdout);
    if (argument->value) {
      fwrite(u8"=", 1, sizeof(u8"=") - 1, stdout);
      fwrite(Arcadia_String_getBytes(thread, argument->value), 1, Arcadia_String_getNumberOfBytes(thread, argument->value), stdout);
    }
    fwrite(u8"\n", 1, sizeof(u8"\n") - 1, stdout);
  }
  if (Arcadia_Value_isVoidValue(&target)) {
    Arcadia_CommandLine_raiseRequiredArgumentMissingError(thread, Arcadia_String_createFromCxxString(thread, u8"target"), log);
  }
  Arcadia_List* pixelBufferList = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_SizeValue sizes[] = {
    8,
    16,
    32,
    48,
    64,
    96,
    128,
    256,
  };
  Arcadia_Imaging_ImageManager* imageManager = Arcadia_Imaging_ImageManager_getOrCreate(thread);
  Arcadia_String* extension = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"ico", sizeof(u8"ico") - 1)));
  Arcadia_List* writers = Arcadia_Imaging_ImageManager_getWriters(thread, imageManager, extension);
  if (!Arcadia_Collection_getSize(thread, (Arcadia_Collection*)writers)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
    Arcadia_Thread_popJumpTarget(thread);
  }
  Arcadia_Imaging_ImageWriter* writer = (Arcadia_Imaging_ImageWriter*)Arcadia_List_getObjectReferenceValueAt(thread, writers, 0);
  for (Arcadia_SizeValue i = 0, n = sizeof(sizes) / sizeof(size_t); i < n; ++i) {
    Arcadia_Media_PixelBuffer* pixelBuffer = Arcadia_Media_PixelBuffer_create(thread, 0, sizes[i], sizes[i], Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8);
    Arcadia_List_insertBackObjectReferenceValue(thread, pixelBufferList, (Arcadia_ObjectReferenceValue)pixelBuffer);
  }
  Arcadia_Imaging_ImageWriterParameters* parameters = Arcadia_Imaging_ImageWriterParameters_createFile(thread, (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&target), extension);
  Arcadia_Imaging_ImageWriter_write(thread,
                                    writer,
                                    pixelBufferList,
                                    parameters);
}

int
main
  (
    int argc,
    char** argv
  )
{
  Arcadia_Status status;
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
  status = Arcadia_Thread_getStatus(thread);
  thread = NULL;
  Arcadia_Process_relinquish(process);
  process = NULL;
  if (status) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
