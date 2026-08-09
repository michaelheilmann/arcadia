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
#include "Arcadia/MIL/Compiler/HelpSystem.h"
#include "Arcadia/DDLS/Include.h"
#include "Arcadia/DDL/Include.h"
#include <stdlib.h>
#include <string.h>

static const char* SCHEMA = 
  "{\n"
  "  kind : \"Schema\",\n"
  "  name : \"Machine Interface Language Compiler Configuration File\",\n"
  "  definition : {\n"
  "    kind : \"Map\",\n"
  "    entries : [\n"
  "      {\n"
  "        kind : \"MapEntry\",\n"
  "        name : \"modulePaths\",\n"
  "        type : {\n"
  "          kind : \"List\",\n"
  "          entry : {\n"
  "            kind :  \"String\",\n"
  "          },\n"
  "        },\n"
  "      },\n"
  "    ],\n"
  "  },\n"
  "}\n"
  ;

/// @error Arcadia_Status_SemanticalError if no entry of the specified name was found
/// @error Arcadia_Status_SemanticalError if an entry of the specified name was found but its value is not a list
static inline Arcadia_DDL_ListNode*
Arcadia_DDL_MapNode_getListByName
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
      Arcadia_DDL_Node* valueNode = mapEntryNode->value;
      if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)valueNode, _Arcadia_DDL_ListNode_getType(thread))) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_DDL_ListNode* listNode = (Arcadia_DDL_ListNode*)valueNode;
      return listNode;
    }
  }
  Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
  Arcadia_Thread_jump(thread);
}

/// Parse a path string. If parsing fails, ensure compilation fails.
static Arcadia_FilePath*
safeParsePath
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics,
    Arcadia_MILC_FileType fileType,
    Arcadia_String* pathString
  )
{
  Arcadia_JumpTarget jumpTarget;
  Arcadia_FilePath* path = NULL;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    path = Arcadia_FilePath_parseNative(thread, pathString);
    Arcadia_Thread_popJumpTarget(thread);
    return path;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Languages_Diagnostics_add(thread, diagnostics, (Arcadia_Languages_Diagnostic*)Arcadia_MILC_Diagnostics_InvalidPathDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, pathString));
    Arcadia_Thread_setRaisedValue(thread, Arcadia_Value_makeObjectReferenceValue(Arcadia_MILC_CompilationFailedException_create(thread)));
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ValueRaised);
    Arcadia_Thread_jump(thread);
  }
  if ((fileType == Arcadia_MILC_FileType_CompilationUnit || fileType == Arcadia_MILC_FileType_ConfigurationFile) && !Arcadia_FileSystem_regularFileExists(thread, Arcadia_FileSystem_getOrCreate(thread), path)) {
    Arcadia_Languages_Diagnostics_add
      (
        thread,
        diagnostics,
        (Arcadia_Languages_Diagnostic*)
        Arcadia_MILC_Diagnostics_FileNotFoundDiagnostic_create
          (
            thread,
            Arcadia_Languages_DiagnosticType_Error,
            Arcadia_Languages_InputFile_create(thread, path),
            fileType
          )
      );
    Arcadia_Thread_setRaisedValue(thread, Arcadia_Value_makeObjectReferenceValue(Arcadia_MILC_CompilationFailedException_create(thread)));
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ValueRaised);
    Arcadia_Thread_jump(thread);
  } else if (fileType == Arcadia_MILC_FileType_ModuleDirectory && !Arcadia_FileSystem_directoryFileExists(thread, Arcadia_FileSystem_getOrCreate(thread), path)) {
    Arcadia_Languages_Diagnostics_add
      (
        thread,
        diagnostics,
        (Arcadia_Languages_Diagnostic*)
        Arcadia_MILC_Diagnostics_FileNotFoundDiagnostic_create
          (
            thread,
            Arcadia_Languages_DiagnosticType_Error,
            Arcadia_Languages_InputFile_create(thread, path),
            fileType
          )
      );
    Arcadia_Thread_setRaisedValue(thread, Arcadia_Value_makeObjectReferenceValue(Arcadia_MILC_CompilationFailedException_create(thread)));
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ValueRaised);
    Arcadia_Thread_jump(thread);
  }
}

static void
_invoke
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context,
    Arcadia_List* arguments
  )
{
  // @todo The logic from this point on should be part of a method of the compiler context.
  Arcadia_String* configurationFilePathString = NULL;
  Arcadia_Value CONFIGURATION = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"configuration"));
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)arguments); i < n; ++i) {
    Arcadia_CommandLineArgument* argument = (Arcadia_CommandLineArgument*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, arguments, i, _Arcadia_CommandLineArgument_getType(thread));
    if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)argument->name, &CONFIGURATION)) {
      if (configurationFilePathString) {
        // @todo These errors must integrate with the log of the context.
        Arcadia_CommandLine_raiseAlreadySpecifiedError(thread, argument->name, context->log);
      }
      if (!argument->value) {
        // @todo These errors must integrate with the log of the context.
        Arcadia_CommandLine_raiseNoValueError(thread, argument->name, context->log);
      }
      configurationFilePathString = argument->value;
    } else {
      // @todo These errors must integrate with the log of the context.
      Arcadia_CommandLine_invalidCommandLineArgumentError(thread, argument->name, context->log);
    }
  }
  if (!configurationFilePathString) {
    // @todo These errors must integrate with the log of the context.
    Arcadia_CommandLine_raiseRequiredArgumentMissingError(thread, Arcadia_String_createFromCxxString(thread, u8"configuration"), context->log);
  }

  // (1) Create the compilation task.
  Arcadia_MILC_CompilationTask* compilationTask = Arcadia_MILC_CompilationTask_create(thread, context);

  Arcadia_FilePath* configurationFilePath = NULL;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_List* moduleDirectoryPaths = NULL;

  // (2) Search the configuration file.
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  configurationFilePath = safeParsePath(thread, context->diagnostics, Arcadia_MILC_FileType_ConfigurationFile, configurationFilePathString);
  #if defined(Arcadia_MILC_Configuration_ListConfigurationFiles) && 1 == Arcadia_MILC_Configuration_ListConfigurationFiles 
    Arcadia_Log_information(thread, context->log, Arcadia_String_createFromCxxString(thread, u8"configuration file `"));
    Arcadia_Log_information(thread, context->log, Arcadia_FilePath_toNative(thread, configurationFilePath, Arcadia_BooleanValue_True));
    Arcadia_Log_information(thread, context->log, Arcadia_String_createFromCxxString(thread, u8"`\n"));
  #endif
  // (3) Load the configuration from the configuration file.
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    /* @todo Add Arcadia.DDL.Reader and derive Arcadia.DDL.DefaultReader from it. */
    Arcadia_DDL_DefaultReader* readerDDL = (Arcadia_DDL_DefaultReader*)Arcadia_DDL_DefaultReader_create(thread);
    Arcadia_Languages_InputFile* inputFile = Arcadia_Languages_InputFile_create(thread, configurationFilePath);
    Arcadia_ByteArray* contents = Arcadia_Languages_InputFile_getContents(thread, inputFile);
    Arcadia_UnicodeCodePointReader* z = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, contents));
    Arcadia_DDL_Node* nodeDDL = Arcadia_DDL_DefaultReader_run(thread, readerDDL, z);
    /* @todo Add Arcadia.DDLS.Reader and derivce Arcadia.DDLS.DefaultReader from it. */
    Arcadia_DDLS_DefaultReader* readerDDLS = (Arcadia_DDLS_DefaultReader*)Arcadia_DDLS_DefaultReader_create(thread);
    Arcadia_DDLS_Node* nodeDDLS = Arcadia_DDLS_DefaultReader_run(thread, readerDDLS, Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, SCHEMA, strlen(SCHEMA))));
    Arcadia_DDLS_ValidationContext* validationContext = Arcadia_DDLS_ValidationContext_create(thread);
    if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)nodeDDLS, _Arcadia_DDLS_ValidationContext_getType(thread))) {
      Arcadia_Languages_Diagnostics_add
        (
          thread,
          context->diagnostics,
          (Arcadia_Languages_Diagnostic*)
          Arcadia_MILC_Diagnostics_ConfigurationFileInvalidDiagnostic_create
            (
              thread,
              Arcadia_Languages_DiagnosticType_Error,
              inputFile
            )
        );
      Arcadia_Thread_setRaisedValue(thread, Arcadia_Value_makeObjectReferenceValue(Arcadia_MILC_CompilationFailedException_create(thread)));
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ValueRaised);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)nodeDDLS);
    Arcadia_DDLS_ValidationContext_run(thread, validationContext, Arcadia_String_createFromCxxString(thread, u8"Machine Interface Language Compiler Configuration File"), nodeDDL);
    // (4) Obtain the list of configuration file paths.
    Arcadia_DDL_ListNode* source = Arcadia_DDL_MapNode_getListByName(thread, (Arcadia_DDL_MapNode*)nodeDDL, Arcadia_String_createFromCxxString(thread, u8"modulePaths"));
    moduleDirectoryPaths = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    for (Arcadia_SizeValue i = 0, n = Arcadia_DDL_ListNode_getNumberOfElements(thread, source); i < n; ++i) {
      Arcadia_DDL_StringNode* x = (Arcadia_DDL_StringNode*)Arcadia_DDL_ListNode_getElementAt(thread, source, i);
      /* @todo If the specified string is not a generic path, then we need to handle the resulting exception and emit an appropriate error message. */
      Arcadia_FilePath* y = safeParsePath(thread, context->diagnostics, Arcadia_MILC_FileType_CompilationUnit, x->value);
      Arcadia_List_insertBackObjectReferenceValue(thread, moduleDirectoryPaths, y);
    }
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_jump(thread);
  }
  // (4) Get the working directory path.
  Arcadia_FilePath* workingDirectory = Arcadia_FilePath_getParent(thread, configurationFilePath);
#if defined(Arcadia_MILC_Configuration_ListWorkingDirectory) && 1 == Arcadia_MILC_Configuration_ListWorkingDirectory
  Arcadia_Log_information(thread, context->log, Arcadia_String_createFromCxxString(thread, u8"working directory `"));
  Arcadia_Log_information(thread, context->log, Arcadia_FilePath_toNative(thread, workingDirectory, Arcadia_BooleanValue_True));
  Arcadia_Log_information(thread, context->log, Arcadia_String_createFromCxxString(thread, u8"`\n"));
#endif

  // (5) Run the compiler.
  Arcadia_MILC_CompilationTask_run(thread, compilationTask, workingDirectory, moduleDirectoryPaths);
}

static void
MIL2C_invoke
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context,
    Arcadia_List* arguments
  )
{
  _invoke(thread, context, arguments);
}

static void
MIL2MIL_invoke
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context,
    Arcadia_List* arguments
  )
{
  _invoke(thread, context, arguments);
}

static void
main1
  (
    Arcadia_Thread* thread,
    int argc,
    char** argv
  )
{
  Arcadia_MILC_Context* context1  = Arcadia_MILC_Context_create(thread);
  // (1) Create the native argument to a list of a list of strings.
  Arcadia_List* arguments = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  for (int argi = 1; argi < argc; ++argi) {
    Arcadia_String* argumentString = Arcadia_String_createFromCxxString(thread, argv[argi]);
    Arcadia_UnicodeCodePointReader* argumentReader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, argumentString));
    Arcadia_CommandLineArgument* commandLineArgument = Arcadia_CommandLine_parseArgument(thread, argumentReader);
    if (commandLineArgument->syntacticalError) {
      Arcadia_CommandLine_invalidCommandLineArgumentError(thread, commandLineArgument->name, context1->log);
    }

    Arcadia_List_insertBackObjectReferenceValue(thread, arguments, commandLineArgument);
  }

  // (2) There must be at least one argument `--<tool name>`.
  if (!Arcadia_Collection_getSize(thread, (Arcadia_Collection*)arguments)) {
    /* Invoke help without any arguments. */
    Help_invoke(thread, context1, (Arcadia_List*)Arcadia_ArrayList_create(thread));
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_String* toolName  = NULL;
  {
    Arcadia_CommandLineArgument* commandLineArgument = (Arcadia_CommandLineArgument*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, arguments, 0, _Arcadia_CommandLineArgument_getType(thread));
    if (commandLineArgument->syntacticalError) {
      Arcadia_CommandLine_invalidCommandLineArgumentError(thread, commandLineArgument->name, context1->log);
    }
    if (commandLineArgument->value) {
      Arcadia_CommandLine_raiseValueInvalidError(thread, commandLineArgument->name, commandLineArgument->value, context1->log);
    }
    toolName = commandLineArgument->name;
    Arcadia_List_removeFront(thread, arguments, 1);
  }
  if (Arcadia_String_isEqualTo_pn(thread, toolName, u8"mil2c", sizeof(u8"mil2c") - 1)) {
    MIL2C_invoke(thread, context1, arguments);
  } else if (Arcadia_String_isEqualTo_pn(thread, toolName, u8"mil2mil", sizeof(u8"mil2mil") - 1)) {
    MIL2MIL_invoke(thread, context1, arguments);
  } else if (Arcadia_String_isEqualTo_pn(thread, toolName, u8"help", sizeof(u8"help") - 1)) {
    Help_invoke(thread, context1, arguments);
  } else {
    /* Invoke help without any arguments. */
    Help_invoke(thread, context1, (Arcadia_List*)Arcadia_ArrayList_create(thread));
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
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
