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

#include "Arcadia/DDLS/Include.h"

#include "Arcadia/DDL/Include.h"
#include "Arcadia/DDLS/Tests/Validation/Default.h"
#include <string.h>

static Arcadia_DDLS_SchemaNode*
readDDLS
  (
    Arcadia_Thread* thread,
    const char* source
  )
{
  Arcadia_DDLS_DefaultReader* reader = Arcadia_DDLS_DefaultReader_create(thread);
  Arcadia_ByteArray* x = Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, source, strlen(source)));
  //Arcadia_UnicodeCodePointReader* y = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, x));
  Arcadia_DDLS_Node* ddlsNode = Arcadia_DDLS_DefaultReader_run(thread, reader, x);
  return (Arcadia_DDLS_SchemaNode*)ddlsNode;
}

static Arcadia_DDL_Node*
readDDL
  (
    Arcadia_Thread* thread,
    const char* source
  )
{
  Arcadia_DDL_DefaultReader* reader = (Arcadia_DDL_DefaultReader*)Arcadia_DDL_DefaultReader_create(thread);
  Arcadia_ByteArray* x = Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, source, strlen(source)));
  Arcadia_UnicodeCodePointReader* y = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, x));
  Arcadia_DDL_Node* target = Arcadia_DDL_DefaultReader_run(thread, reader, y);
  return target;
}

// input are strings, the single choice is string => accept
static void
Arcadia_DDLS_Tests_Validation_choiceAccept1
  (
    Arcadia_Thread* thread
  )
{
  static const char* DDLS =
    "{\n"
    "  kind : \"Schema\",\n"
    "  name : \"MySchema\",\n"
    "  definition : \n"
    "  {\n"
    "  kind : \"Choice\",\n"
    "  choices : [\n"
    "      {\n"
    "        kind : \"String\",\n"
    "      },\n"
    "    ],\n"
    "  },\n"
    "},\n"
    ;

  const char* DDL[] =
    {
      "\"w\"",
    };
  for (Arcadia_SizeValue i = 0; i < sizeof(DDL) / sizeof(const char*); ++i) {
    Arcadia_DDLS_SchemaNode* ddlsNode = readDDLS(thread, DDLS);
    Arcadia_DDL_Node* node = readDDL(thread, DDL[i]);
    Arcadia_DDLS_ValidationContext* validationContext = Arcadia_DDLS_ValidationContext_create(thread);
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, ddlsNode);
    Arcadia_DDLS_ValidationContext_run(thread, validationContext, Arcadia_String_createFromCxxString(thread, u8"MySchema"), node);
  }
}

// input is true, false, void, some numbers, the single choice is string => reject
static void
Arcadia_DDLS_Tests_Validation_choiceReject1
  (
    Arcadia_Thread* thread
  )
{
  static const char* DDLS =
    "{\n"
    "  kind : \"Schema\",\n"
    "  name : \"MySchema\",\n"
    "  definition : \n"
    "  {\n"
    "  kind : \"Choice\",\n"
    "  choices : [\n"
    "      {\n"
    "        kind : \"String\",\n"
    "      },\n"
    "    ],\n"
    "  },\n"
    "},\n"
    ;

  const char* DDL[] =
    {
    "true",
    "false",
    "void",
    "1",
    "-1",
    "0.1",
    "-0.1",
    };
  for (Arcadia_SizeValue i = 0; i < sizeof(DDL) / sizeof(const char*); ++i) {
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      Arcadia_DDLS_SchemaNode* ddlsNode = readDDLS(thread, DDLS);
      Arcadia_DDL_Node* node = readDDL(thread, DDL[i]);
      Arcadia_DDLS_ValidationContext* validationContext = Arcadia_DDLS_ValidationContext_create(thread);
      Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, ddlsNode);
      Arcadia_DDLS_ValidationContext_run(thread, validationContext, Arcadia_String_createFromCxxString(thread, u8"MySchema"), node);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
      Arcadia_Thread_jump(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      if (Arcadia_Thread_getStatus(thread) != Arcadia_Status_SemanticalError) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
        Arcadia_Thread_jump(thread);
      } else {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
      }
    }
  }
}

// An optional entry may be omitted.
static void
Arcadia_DDLS_Tests_Validation_optionalAccept1
  (
    Arcadia_Thread* thread
  )
{
  static const char* DDLS =
    "{\n"
    "  kind : \"Schema\",\n"
    "  name : \"MySchema\",\n"
    "  definition : \n"
    "  {\n"
    "  kind : \"Map\",\n"
    "  entries : [\n"
    "      {\n"
    "        kind : \"MapEntry\",\n"
    "        name : \"type\",\n"
    "        type : {\n"
    "          kind : \"String\",\n"
    "        },\n"
    "      },\n"
    "      {\n"
    "        kind : \"MapEntry\",\n"
    "        name : \"optionalEntry\",\n"
    "        optional : \"true\",\n"
    "        type : {\n"
    "          kind : \"String\",\n"
    "        },\n"
    "      },\n"
    "    ],\n"
    "  },\n"
    "},\n"
    ;

  const char* DDL[] =
    {
      "{\n"
      "  type : \"MySchema\",\n"
      "},\n",
      "{\n"
      "  type : \"MySchema\",\n"
      "  optionalEntry : \"a\",\n"
      "},\n",
    };
  for (Arcadia_SizeValue i = 0; i < sizeof(DDL) / sizeof(const char*); ++i) {
    Arcadia_DDLS_SchemaNode* ddlsNode = readDDLS(thread, DDLS);
    Arcadia_DDL_Node* node = readDDL(thread, DDL[i]);
    Arcadia_DDLS_ValidationContext* validationContext = Arcadia_DDLS_ValidationContext_create(thread);
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, ddlsNode);
    Arcadia_DDLS_ValidationContext_run(thread, validationContext, Arcadia_String_createFromCxxString(thread, u8"MySchema"), node);
  }
}

// A required entry cannot be omitted even if another entry is optional.
static void
Arcadia_DDLS_Tests_Validation_optionalReject1
  (
    Arcadia_Thread* thread
  )
{
  static const char* DDLS =
    "{\n"
    "  kind : \"Schema\",\n"
    "  name : \"MySchema\",\n"
    "  definition : \n"
    "  {\n"
    "  kind : \"Map\",\n"
    "  entries : [\n"
    "      {\n"
    "        kind : \"MapEntry\",\n"
    "        name : \"type\",\n"
    "        type : {\n"
    "          kind : \"String\",\n"
    "        },\n"
    "      },\n"
    "      {\n"
    "        kind : \"MapEntry\",\n"
    "        name : \"optionalEntry\",\n"
    "        optional : \"true\",\n"
    "        type : {\n"
    "          kind : \"String\",\n"
    "        },\n"
    "      },\n"
    "    ],\n"
    "  },\n"
    "},\n"
    ;

  const char* DDL =
    "{\n"
    "  optionalEntry : \"a\",\n"
    "},\n"
    ;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_DDLS_SchemaNode* ddlsNode = readDDLS(thread, DDLS);
    Arcadia_DDL_Node* node = readDDL(thread, DDL);
    Arcadia_DDLS_ValidationContext* validationContext = Arcadia_DDLS_ValidationContext_create(thread);
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, ddlsNode);
    Arcadia_DDLS_ValidationContext_run(thread, validationContext, Arcadia_String_createFromCxxString(thread, u8"MySchema"), node);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (Arcadia_Thread_getStatus(thread) != Arcadia_Status_SemanticalError) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
      Arcadia_Thread_jump(thread);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    }
  }
}

int
main
  (
    int argc,
    char** argv
  )
{
  if (!Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_test1)) {
    return EXIT_FAILURE;
  }

  if (!Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_testRejectTypeNotDefined)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_testRejectRedNotDefined)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_testRejectGreenNotDefined)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_testRejectBlueNotDefined)) {
    return EXIT_FAILURE;
  }

  if (!Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_testRejectTypeDefinedTwice)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_testRejectRedDefinedTwice)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_testRejectGreenDefinedTwice)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_testRejectBlueDefinedTwice)) {
    return EXIT_FAILURE;
  }

  if (!Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_choiceAccept1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_choiceReject1)) {
    return EXIT_FAILURE;
  }

  if (!Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_optionalAccept1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_optionalReject1)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
