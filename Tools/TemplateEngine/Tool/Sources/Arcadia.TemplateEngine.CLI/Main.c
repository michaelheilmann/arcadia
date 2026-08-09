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
#include "Arcadia/Logging/Include.h"
#include "Arcadia.TemplateEngine/Context.h"
#include "Arcadia.TemplateEngine/Environment.h"

static void
help
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"Arcadia.TemplateEngine [options]\n");

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"[options]:\n");

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"--source=<path to source file>                \n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"  required. The path to the source file.      \n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"  Path must be enclosed by double quotes      \n");

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"--target=<path to target file>                \n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"  required. The path to the target file.      \n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"  Path must be enclosed by double quotes      \n");

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"--dependencies=<path to dependencies file>    \n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"  required. The path to the dependencies file.\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"  Path must be enclosed by double quotes      \n");

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"--environment=<path to environment file>      \n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"  optional. The path to the environment file. \n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"  Path must be enclosed by double quotes      \n");


  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FileHandle* fileHandle = Arcadia_FileSystem_createFileHandle(thread, fileSystem);
  Arcadia_FileHandle_openStandardOutput(thread, fileHandle);
  Arcadia_FileHandle_writeStringBuffer(thread, fileHandle, stringBuffer);

  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}

void
Arcadia_CommandLine_fileNotFoundError
  (
    Arcadia_Thread* thread,
    Arcadia_String* path
  )
{
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"file `");
  Arcadia_StringBuilder_insertBackString(thread, stringBuffer, path);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"` not found\n");

  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FileHandle* fileHandle = Arcadia_FileSystem_createFileHandle(thread, fileSystem);
  Arcadia_FileHandle_openStandardOutput(thread, fileHandle);
  Arcadia_FileHandle_writeStringBuffer(thread, fileHandle, stringBuffer);

  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}

void
Arcadia_CommandLine_requiredArgumentMissingError
  (
    Arcadia_Thread* thread,
    Arcadia_String* key
  )
{
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"required command-line argument `");
  Arcadia_StringBuilder_insertBackString(thread, stringBuffer, key);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"` missing\n");

  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FileHandle* fileHandle = Arcadia_FileSystem_createFileHandle(thread, fileSystem);
  Arcadia_FileHandle_openStandardOutput(thread, fileHandle);
  Arcadia_FileHandle_writeStringBuffer(thread, fileHandle, stringBuffer);

  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}

/*
 * @todo Use the call stack functionality to ensure proper stack unwinding.
 * @param arguments List of strings, a string represents a command-line argument.
 */
void
main1
  (
    Arcadia_Thread* thread
  )
{
  Context* context = Context_create(thread);
  Arcadia_Log* log = (Arcadia_Log*)Arcadia_ConsoleLog_create(thread);
  Arcadia_BooleanValue showHelp = Arcadia_BooleanValue_False;
  Arcadia_Value sourceFileValue = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void),
                targetFileValue = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void),
                environmentFileValue = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void),
                dependenciesFileValue = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void),
                logFileValue = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);

  if (Arcadia_ValueStack_getNatural8Value(thread, 0) != 1) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_List* arguments = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_List_getType(thread));
  Arcadia_ValueStack_popValues(thread, 2);

  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)arguments); i < n; ++i) {
    Arcadia_String* argumentString = (Arcadia_String*)Arcadia_List_getObjectReferenceValueAt(thread, arguments, i);
    Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, argumentString));
    Arcadia_CommandLineArgument* argument = Arcadia_CommandLine_parseArgument(thread, reader);
    if (argument->syntacticalError) {
      Arcadia_CommandLine_invalidCommandLineArgumentError(thread, argumentString, log);
    }
    if (Arcadia_String_isEqualTo_pn(thread, argument->name, u8"source", sizeof(u8"source") - 1)) {
      if (!argument->value) {
        Arcadia_CommandLine_raiseNoValueError(thread, argument->name, log);
      }
      Arcadia_Value_setObjectReferenceValue(&sourceFileValue, argument->value);
    } else if (Arcadia_String_isEqualTo_pn(thread, argument->name, u8"target", sizeof(u8"target") - 1)) {
      if (!argument->value) {
        Arcadia_CommandLine_raiseNoValueError(thread, argument->name, log);
      }
      Arcadia_Value_setObjectReferenceValue(&targetFileValue, argument->value);
    } else if (Arcadia_String_isEqualTo_pn(thread, argument->name, u8"environment", sizeof(u8"environment") - 1)) {
      if (!argument->value) {
        Arcadia_CommandLine_raiseNoValueError(thread, argument->name, log);
      }
      Arcadia_Value_setObjectReferenceValue(&environmentFileValue, argument->value);
    } else if (Arcadia_String_isEqualTo_pn(thread, argument->name, u8"dependencies", sizeof(u8"dependencies") - 1)) {
      if (!argument->value) {
        Arcadia_CommandLine_raiseNoValueError(thread, argument->name, log);
      }
      Arcadia_Value_setObjectReferenceValue(&dependenciesFileValue, argument->value);
    } else if (Arcadia_String_isEqualTo_pn(thread, argument->name, u8"log", sizeof(u8"log") - 1)) {
      if (!argument->value) {
        Arcadia_CommandLine_raiseNoValueError(thread, argument->name, log);
      }
      Arcadia_Value_setObjectReferenceValue(&logFileValue, argument->value);
    } else if (Arcadia_String_isEqualTo_pn(thread, argument->name, u8"help", sizeof(u8"help") - 1)) {
      if (argument->value) {
        Arcadia_CommandLine_invalidCommandLineArgumentError(thread, argument->name, log);
      }
      showHelp = Arcadia_BooleanValue_True;
    } else {
      Arcadia_CommandLine_raiseUnknownArgumentError(thread, argument->name, argument->value, log);
    }
  }

  if (showHelp) {
    help(thread);
    return;
  }

  Arcadia_JumpTarget jumpTarget;

  Arcadia_FilePath* sourceFilePath = NULL,
                  * targetFilePath = NULL,
                  * environmentFilePath = NULL,
                  * dependenciesFilePath = NULL;

  // `source` is required
  if (Arcadia_Value_isVoidValue(&sourceFileValue)) {
    Arcadia_CommandLine_requiredArgumentMissingError(thread, Arcadia_String_createFromCxxString(thread, u8"source"));
  }
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    sourceFilePath = Arcadia_FilePath_parseNative(thread, Arcadia_ArgumentsValidation_getObjectReferenceValue(thread, &sourceFileValue, _Arcadia_String_getType(thread)));
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_CommandLine_fileNotFoundError(thread, (Arcadia_String*)Arcadia_ArgumentsValidation_getObjectReferenceValue(thread, &sourceFileValue, _Arcadia_String_getType(thread)));
  }
  // `target` is required
  if (Arcadia_Value_isVoidValue(&targetFileValue)) {
    Arcadia_CommandLine_requiredArgumentMissingError(thread, Arcadia_String_createFromCxxString(thread, u8"target"));
  }
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    targetFilePath = Arcadia_FilePath_parseNative(thread, Arcadia_ArgumentsValidation_getObjectReferenceValue(thread, &targetFileValue, _Arcadia_String_getType(thread)));
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_CommandLine_fileNotFoundError(thread, (Arcadia_String*)Arcadia_ArgumentsValidation_getObjectReferenceValue(thread, &targetFileValue, _Arcadia_String_getType(thread)));
  }
  // `dependencies` is required.
  if (Arcadia_Value_isVoidValue(&dependenciesFileValue)) {
    Arcadia_CommandLine_requiredArgumentMissingError(thread, Arcadia_String_createFromCxxString(thread, u8"dependencies"));
  }
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    dependenciesFilePath = Arcadia_FilePath_parseNative(thread, Arcadia_ArgumentsValidation_getObjectReferenceValue(thread, &dependenciesFileValue, _Arcadia_String_getType(thread)));
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_CommandLine_fileNotFoundError(thread, (Arcadia_String*)Arcadia_ArgumentsValidation_getObjectReferenceValue(thread, &dependenciesFileValue, _Arcadia_String_getType(thread)));
  }
  // `environment` is optional.
  if (!Arcadia_Value_isVoidValue(&environmentFileValue)) {
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      environmentFilePath = Arcadia_FilePath_parseNative(thread, Arcadia_ArgumentsValidation_getObjectReferenceValue(thread, &environmentFileValue, _Arcadia_String_getType(thread)));
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_CommandLine_fileNotFoundError(thread, (Arcadia_String*)Arcadia_ArgumentsValidation_getObjectReferenceValue(thread, &environmentFileValue, _Arcadia_String_getType(thread)));
    }
  }

  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Environment* environment = NULL;
  if (environmentFilePath) {
    Arcadia_String* environmentString = NULL;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      environmentString = Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(Arcadia_FileSystem_getFileContents(thread, fileSystem, environmentFilePath)));
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_CommandLine_fileNotFoundError(thread, (Arcadia_String*)Arcadia_ArgumentsValidation_getObjectReferenceValue(thread, &environmentFileValue, _Arcadia_String_getType(thread)));
      Arcadia_Thread_jump(thread);
    }
    environment = Environment_loadString(thread, environmentString);
  } else {
    environment = Environment_create(thread, NULL);
  }
  context->targetFilePath = targetFilePath;
  context->sourceFilePath = sourceFilePath;
  context->environmentFilePath = environmentFilePath;
  context->dependenciesFilePath = dependenciesFilePath;

  environment->enclosing = context->environment;
  context->environment = environment;

  Arcadia_BooleanValue failed = Arcadia_BooleanValue_False;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Context_onRun(thread, context);
    Arcadia_FileSystem_setFileContents(thread, fileSystem, context->targetFilePath, context->targetBuffer);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    failed |= Arcadia_BooleanValue_True;
    
  }
  Arcadia_Languages_Diagnostics_emit(thread, context->diagnostics);
  failed |= Arcadia_Languages_Diagnostics_hasErrors(thread, context->diagnostics);
  if (failed) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
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
    Arcadia_List* arguments = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    for (int argi = 1; argi < argc; ++argi) {
      Arcadia_List_insertBackObjectReferenceValue(thread, arguments, (Arcadia_Object*)Arcadia_String_createFromCxxString(thread, argv[argi]));
    }
    Arcadia_ValueStack_pushObjectReferenceValue(thread,(Arcadia_Object*)arguments);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    main1(thread);
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
