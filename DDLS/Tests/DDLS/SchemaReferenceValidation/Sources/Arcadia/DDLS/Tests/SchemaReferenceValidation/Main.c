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
#include <string.h>

static Arcadia_DDLS_SchemaNode*
readDDLS
  (
    Arcadia_Thread* thread,
    const char* source
  )
{
  Arcadia_DDLS_DefaultReader* reader = Arcadia_DDLS_DefaultReader_create(thread);
  Arcadia_DDLS_Node* ddlsNode = Arcadia_DDLS_DefaultReader_run(thread, reader, Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, source, strlen(source))));
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
  Arcadia_ByteArray* input1 = Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, source, strlen(source)));
  Arcadia_UnicodeCodePointReader* input = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, input1));
  Arcadia_DDL_Node* ddlNode = Arcadia_DDL_DefaultReader_run(thread, reader, input);
  return ddlNode;
}

static const char* DDLS_TYPE =
  "{\n"
  "  kind : \"Schema\",\n"
  "  name : \"Type\",\n"
  "  definition : {\n"
  "    kind : \"String\",\n"
  "  },\n"
  "},\n"
  ;

static const char* DDLS_COLOR_COMPONENT =
  "{\n"
  "  kind : \"Schema\",\n"
  "  name : \"ColorComponent\",\n"
  "  definition : {\n"
  "    kind : \"Number\",\n"
  "  },\n"
  "},\n"
  ;

static const char* DDLS_COLOR =
  "{\n"
  "  kind : \"Schema\",\n"
  "  name : \"Color\",\n"
  "  definition : \n"
  "  {\n"
  "  kind : \"Map\",\n"
  "  entries : [\n"
  "      {\n"
  "        kind : \"MapEntry\",\n"
  "        name : \"type\",\n"
  "        type : {\n"
  "          kind : \"SchemaReference\",\n"
  "          name : \"Type\",\n"
  "        },\n"
  "      },\n"
  "      {\n"
  "        kind : \"MapEntry\",\n"
  "        name : \"red\",\n"
  "        type : {\n"
  "          kind : \"SchemaReference\",\n"
  "          name : \"ColorComponent\",\n"
  "        },\n"
  "      },\n"
  "      {\n"
  "        kind : \"MapEntry\",\n"
  "        name : \"green\",\n"
  "        type : {\n"
  "          kind : \"SchemaReference\",\n"
  "          name : \"ColorComponent\",\n"
  "        },\n"
  "      },\n"
  "      {\n"
  "        kind : \"MapEntry\",\n"
  "        name : \"blue\",\n"
  "        type : {\n"
  "          kind : \"SchemaReference\",\n"
  "          name : \"ColorComponent\",\n"
  "        },\n"
  "      },\n"
  "    ],\n"
  "  },\n"
  "},\n"
  ;

static void
test1
  (
    Arcadia_Thread* thread
  )
{
  const char* DDL =
    "{\n"
    "  type : \"Color\",\n"
    "  red : 255,\n"
    "  green : 0,\n"
    "  blue : 0,\n"
    "},\n"
    ;
  Arcadia_DDL_Node* node = readDDL(thread, DDL);
  Arcadia_DDLS_ValidationContext* validationContext = Arcadia_DDLS_ValidationContext_create(thread);
  Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_COLOR));
  Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_TYPE));
  Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_COLOR_COMPONENT));
  Arcadia_DDLS_ValidationContext_run(thread, validationContext, Arcadia_String_createFromCxxString(thread, u8"Color"), node);
}

// "$.type" not defined
static void
testRejectTypeNotDefined
  (
    Arcadia_Thread* thread
  )
{
  const char* DDL =
    "{\n"
    "  red : 255,\n"
    "  green : 0,\n"
    "  blue : 0,\n"
    "},\n"
    ;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_DDL_Node* node = readDDL(thread, DDL);
    Arcadia_DDLS_ValidationContext* validationContext = Arcadia_DDLS_ValidationContext_create(thread);
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_COLOR));
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_TYPE));
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_COLOR_COMPONENT));
    Arcadia_DDLS_ValidationContext_run(thread, validationContext, Arcadia_String_createFromCxxString(thread, u8"Color"), node);
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

// "$.red" not defined
static void
testRejectRedNotDefined
  (
    Arcadia_Thread* thread
  )
{
  const char* DDL =
    "{\n"
    "  type : \"Color\",\n"
    "  green : 0,\n"
    "  blue : 0,\n"
    "},\n"
    ;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_DDL_Node* node = readDDL(thread, DDL);
    Arcadia_DDLS_ValidationContext* validationContext = Arcadia_DDLS_ValidationContext_create(thread);
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_COLOR));
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_TYPE));
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_COLOR_COMPONENT));
    Arcadia_DDLS_ValidationContext_run(thread, validationContext, Arcadia_String_createFromCxxString(thread, u8"Color"), node);
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

// "$.green" not defined
static void
testRejectGreenNotDefined
  (
    Arcadia_Thread* thread
  )
{
  const char* DDL =
    "{\n"
    "  type : \"Color\",\n"
    "  red : 255,\n"
    "  blue : 0,\n"
    "},\n"
    ;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_DDL_Node* node = readDDL(thread, DDL);
    Arcadia_DDLS_ValidationContext* validationContext = Arcadia_DDLS_ValidationContext_create(thread);
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_COLOR));
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_TYPE));
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_COLOR_COMPONENT));
    Arcadia_DDLS_ValidationContext_run(thread, validationContext, Arcadia_String_createFromCxxString(thread, u8"Color"), node);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (Arcadia_Thread_getStatus(thread) != Arcadia_Status_SemanticalError) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    }
  }
}

// "$.blue" not defined
static void
testRejectBlueNotDefined
  (
    Arcadia_Thread* thread
  )
{
  const char* DDL =
    "{\n"
    "  type : \"Color\",\n"
    "  red : 255,\n"
    "  green : 0,\n"
    "},\n"
    ;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_DDL_Node* node = readDDL(thread, DDL);
    Arcadia_DDLS_ValidationContext* validationContext = Arcadia_DDLS_ValidationContext_create(thread);
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_COLOR));
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_TYPE));
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_COLOR_COMPONENT));
    Arcadia_DDLS_ValidationContext_run(thread, validationContext, Arcadia_String_createFromCxxString(thread, u8"Color"), node);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (Arcadia_Thread_getStatus(thread) != Arcadia_Status_SemanticalError) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    }
  }
}

// "$.type" is defined twice
static void
testRejectTypeDefinedTwice
  (
    Arcadia_Thread* thread
  )
{
  const char* DDL =
    "{\n"
    "  type : \"Color\",\n"
    "  red : 255,\n"
    "  blue : 0,\n"
    "  green : 0,\n"
    "  type : \"Color\",\n"
    "},\n"
    ;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_DDL_Node* node = readDDL(thread, DDL);
    Arcadia_DDLS_ValidationContext* validationContext = Arcadia_DDLS_ValidationContext_create(thread);
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_COLOR));
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_TYPE));
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_COLOR_COMPONENT));
    Arcadia_DDLS_ValidationContext_run(thread, validationContext, Arcadia_String_createFromCxxString(thread, u8"Color"), node);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (Arcadia_Thread_getStatus(thread) != Arcadia_Status_SemanticalError) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    }
  }
}

// "$.red" is defined twice
static void
testRejectRedDefinedTwice
  (
    Arcadia_Thread* thread
  )
{
  const char* DDL =
    "{\n"
    "  type : \"Color\",\n"
    "  red : 255,\n"
    "  blue : 0,\n"
    "  green : 0,\n"
    "  red : 255,\n"
    "},\n"
    ;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_DDL_Node* node = readDDL(thread, DDL);
    Arcadia_DDLS_ValidationContext* validationContext = Arcadia_DDLS_ValidationContext_create(thread);
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_COLOR));
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_TYPE));
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_COLOR_COMPONENT));
    Arcadia_DDLS_ValidationContext_run(thread, validationContext, Arcadia_String_createFromCxxString(thread, u8"Color"), node);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (Arcadia_Thread_getStatus(thread) != Arcadia_Status_SemanticalError) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    }
  }
}

// "$.green" is defined twice
static void
testRejectGreenDefinedTwice
  (
    Arcadia_Thread* thread
  )
{
  const char* DDL =
    "{\n"
    "  type : \"Color\",\n"
    "  red : 255,\n"
    "  blue : 0,\n"
    "  green : 0,\n"
    "  green : 0,\n"
    "},\n"
    ;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_DDL_Node* node = readDDL(thread, DDL);
    Arcadia_DDLS_ValidationContext* validationContext = Arcadia_DDLS_ValidationContext_create(thread);
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_COLOR));
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_TYPE));
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_COLOR_COMPONENT));
    Arcadia_DDLS_ValidationContext_run(thread, validationContext, Arcadia_String_createFromCxxString(thread, u8"Color"), node);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (Arcadia_Thread_getStatus(thread) != Arcadia_Status_SemanticalError) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    }
  }
}

// "$.blue" is defined twice
static void
testRejectBlueDefinedTwice
  (
    Arcadia_Thread* thread
  )
{
  const char* DDL =
    "{\n"
    "  type : \"Color\",\n"
    "  red : 255,\n"
    "  blue : 0,\n"
    "  green : 0,\n"
    "  blue : 0,\n"
    "},\n"
    ;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_DDL_Node* node = readDDL(thread, DDL);
    Arcadia_DDLS_ValidationContext* validationContext = Arcadia_DDLS_ValidationContext_create(thread);
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_COLOR));
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_TYPE));
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)readDDLS(thread, DDLS_COLOR_COMPONENT));
    Arcadia_DDLS_ValidationContext_run(thread, validationContext, Arcadia_String_createFromCxxString(thread, u8"Color"), node);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (Arcadia_Thread_getStatus(thread) != Arcadia_Status_SemanticalError) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
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
  if (!Arcadia_Tests_safeExecute(&test1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&testRejectTypeNotDefined)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&testRejectRedNotDefined)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&testRejectGreenNotDefined)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&testRejectBlueNotDefined)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&testRejectTypeDefinedTwice)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&testRejectRedDefinedTwice)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&testRejectGreenDefinedTwice)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&testRejectBlueDefinedTwice)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
