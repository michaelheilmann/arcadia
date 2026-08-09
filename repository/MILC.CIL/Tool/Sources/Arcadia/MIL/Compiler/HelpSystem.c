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

#include "Arcadia/MIL/Compiler/HelpSystem.h"

static void
General_showHelp
  (
    Arcadia_Thread* thread
  );


static void
MIL2C_showHelp
  (
    Arcadia_Thread* thread
  );

static void
MIL2MIL_showHelp
  (
    Arcadia_Thread* thread
  );

static void
General_showHelp
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_StringBuilder* text = Arcadia_StringBuilder_create(thread);

  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"Arcadia.MIL.Compiler --<tool name> [<tool arguments>]\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"<tool name> can be one of the following:\n");

  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"--help     Display the help.\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"           You can specifiy a help topic to display the help on by specified `--topic=<topic>` for `<tool arguments>`.\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"           If no topic is specified or the topic `general` is specified, then this help is displayed.\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"           The following help topics are currently available:\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"           1) `general`\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"           2) `mil2c`\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"           2) `mil2mil`\n");

  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"--mil2c    Invoke the MIL to C compiler.\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"--mil2mil  Invoke the MIL to MIL compiler.\n");

  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FileHandle* fileHandle = Arcadia_FileSystem_createFileHandle(thread, fileSystem);
  Arcadia_FileHandle_writeStringBuffer(thread, fileHandle, text);
}

static void
MIL2C_showHelp
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_StringBuilder* text = Arcadia_StringBuilder_create(thread);

  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"Arcadia.MIL.Compiler --mil2c --configuration=<configuration>\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"\n");

  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"--configuration=<configuration> A DDL configuration file.\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"                                Consists of a single map with a list of module directories:\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"                                `{`\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"                                  `modulePaths = [<module paths>]`\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"                                `}`\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"<module paths> i a list of paths to modules. If a path is relative, then it is interpreted as relative to the current working directory.\n");

  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FileHandle* fileHandle = Arcadia_FileSystem_createFileHandle(thread, fileSystem);
  Arcadia_FileHandle_writeStringBuffer(thread, fileHandle, text);
}

static void
MIL2MIL_showHelp
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_StringBuilder* text = Arcadia_StringBuilder_create(thread);

  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"Arcadia.MIL.Compiler --mil2mil --configuration=<configuration>\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"\n");

  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"--configuration=<configuration> A DDL configuration file.\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"                                Consists of a single map with a list of module directories:\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"                                `{`\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"                                  `modulePaths = [<module paths>]`\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"                                `}`\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, u8"<module paths> i a list of paths to modules. If a path is relative, then it is interpreted as relative to the current working directory.\n");

  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FileHandle* fileHandle = Arcadia_FileSystem_createFileHandle(thread, fileSystem);
  Arcadia_FileHandle_writeStringBuffer(thread, fileHandle, text);
}

void
Help_invoke
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context,
    Arcadia_List* arguments
  )
{
  Arcadia_String* topicName = NULL;
  Arcadia_Value TOPIC = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"topic"));
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)arguments); i < n; ++i) {
    Arcadia_CommandLineArgument* argument = (Arcadia_CommandLineArgument*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, arguments, i, _Arcadia_CommandLineArgument_getType(thread));
    if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)argument->name, &TOPIC)) {
      if (topicName) {
        Arcadia_CommandLine_raiseAlreadySpecifiedError(thread, argument->name, context->log);
      }
      if (!argument->value) {
        Arcadia_CommandLine_raiseNoValueError(thread, argument->name, context->log);
      }
      topicName = argument->value;
    } else {
      Arcadia_CommandLine_invalidCommandLineArgumentError(thread, argument->name, context->log);
    }
  }
  if (!topicName || Arcadia_String_isEqualTo_pn(thread, topicName, u8"general", sizeof(u8"general") - 1)) {
    General_showHelp(thread);
  } else if (topicName && Arcadia_String_isEqualTo_pn(thread, topicName, u8"mil2c", sizeof(u8"mil2c") - 1)) {
    MIL2C_showHelp(thread);
  } else if (topicName && Arcadia_String_isEqualTo_pn(thread, topicName, u8"mil2mil", sizeof(u8"mil2mil") - 1)) {
    MIL2MIL_showHelp(thread);
  } else {
    General_showHelp(thread);
    Arcadia_CommandLine_raiseValueInvalidError(thread, Arcadia_Value_getObjectReferenceValue(&TOPIC), topicName, context->log);
  }
}
