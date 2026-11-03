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

#include "Arcadia/Testing/Include.h"

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
  "}\n"
  ;

static const char* DDLS_COLOR_COMPONENT =
  "{\n"
  "  kind : \"Schema\",\n"
  "  name : \"ColorComponent\",\n"
  "  definition : {\n"
  "    kind : \"Number\",\n"
  "  },\n"
  "}\n"
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
  "}\n"
  ;

static void
expectCyclicSchemaReference
  (
    Arcadia_Thread* thread,
    const char* firstSchema,
    const char* secondSchema
  )
{
  Arcadia_DDL_Node* node = readDDL(thread, "void");
  Arcadia_DDLS_ValidationContext* context = Arcadia_DDLS_ValidationContext_create(thread);
  Arcadia_DDLS_ValidationContext_addSchema(thread, context, readDDLS(thread, firstSchema));
  if (secondSchema) {
    Arcadia_DDLS_ValidationContext_addSchema(thread, context, readDDLS(thread, secondSchema));
  }
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_DDLS_ValidationContext_run(thread, context, Arcadia_String_createFromCxxString(thread, u8"A"), node);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    if (status != Arcadia_Status_SemanticalError) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
      Arcadia_Thread_jump(thread);
    }
  }
}

static void
testRejectSelfCycle
  (
    Arcadia_Thread* thread
  )
{
  expectCyclicSchemaReference(thread,
    "{ kind : \"Schema\", name : \"A\", definition : { kind : \"SchemaReference\", name : \"A\" } }", NULL);
}

static void
testRejectMutualCycle
  (
    Arcadia_Thread* thread
  )
{
  expectCyclicSchemaReference(thread,
    "{ kind : \"Schema\", name : \"A\", definition : { kind : \"SchemaReference\", name : \"B\" } }",
    "{ kind : \"Schema\", name : \"B\", definition : { kind : \"SchemaReference\", name : \"A\" } }");
}

static void
testRejectChoiceCycle
  (
    Arcadia_Thread* thread
  )
{
  expectCyclicSchemaReference(thread,
    "{ kind : \"Schema\", name : \"A\", definition : { kind : \"Choice\", choices : ["
    "{ kind : \"SchemaReference\", name : \"A\" }] } }", NULL);
}

static void
testRejectNestedCycle
  (
    Arcadia_Thread* thread
  )
{
  expectCyclicSchemaReference(thread,
    "{ kind : \"Schema\", name : \"A\", definition : { kind : \"Map\", entries : ["
    "{ kind : \"MapEntry\", name : \"children\", type : { kind : \"List\", "
    "elements : { kind : \"SchemaReference\", name : \"A\" } } }] } }", NULL);
}

// B and C both refer to D; revisiting D on another path is not a cycle.
static void
testAcceptSharedAcyclicReference
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_DDL_Node* value = readDDL(thread, "42");
  Arcadia_DDLS_ValidationContext* context = Arcadia_DDLS_ValidationContext_create(thread);
  Arcadia_DDLS_ValidationContext_addSchema(thread, context, readDDLS(thread,
    "{ kind : \"Schema\", name : \"A\", definition : { kind : \"Choice\", choices : ["
    "{ kind : \"SchemaReference\", name : \"B\" },"
    "{ kind : \"SchemaReference\", name : \"C\" }] } }"));
  Arcadia_DDLS_ValidationContext_addSchema(thread, context, readDDLS(thread,
    "{ kind : \"Schema\", name : \"B\", definition : { kind : \"SchemaReference\", name : \"D\" } }"));
  Arcadia_DDLS_ValidationContext_addSchema(thread, context, readDDLS(thread,
    "{ kind : \"Schema\", name : \"C\", definition : { kind : \"SchemaReference\", name : \"D\" } }"));
  Arcadia_DDLS_ValidationContext_addSchema(thread, context, readDDLS(thread,
    "{ kind : \"Schema\", name : \"D\", definition : { kind : \"Number\" } }"));
  Arcadia_DDLS_ValidationContext_run(thread, context, Arcadia_String_createFromCxxString(thread, u8"A"), value);
}

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
    "}\n"
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
    "}\n"
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
    "}\n"
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
    "}\n"
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
    "}\n"
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
    "}\n"
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
    "}\n"
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
    "}\n"
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
    "}\n"
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

static int
runAcceptColor
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&test1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectTypeNotDefined
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRejectTypeNotDefined) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectRedNotDefined
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRejectRedNotDefined) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectGreenNotDefined
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRejectGreenNotDefined) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectBlueNotDefined
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRejectBlueNotDefined) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectTypeDefinedTwice
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRejectTypeDefinedTwice) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectRedDefinedTwice
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRejectRedDefinedTwice) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectGreenDefinedTwice
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRejectGreenDefinedTwice) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectBlueDefinedTwice
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRejectBlueDefinedTwice) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectSelfCycle
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRejectSelfCycle) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectMutualCycle
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRejectMutualCycle) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectChoiceCycle
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRejectChoiceCycle) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectNestedCycle
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRejectNestedCycle) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runAcceptSharedAcyclicReference
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testAcceptSharedAcyclicReference) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "AcceptColor", &runAcceptColor },
    { "RejectTypeNotDefined", &runRejectTypeNotDefined },
    { "RejectRedNotDefined", &runRejectRedNotDefined },
    { "RejectGreenNotDefined", &runRejectGreenNotDefined },
    { "RejectBlueNotDefined", &runRejectBlueNotDefined },
    { "RejectTypeDefinedTwice", &runRejectTypeDefinedTwice },
    { "RejectRedDefinedTwice", &runRejectRedDefinedTwice },
    { "RejectGreenDefinedTwice", &runRejectGreenDefinedTwice },
    { "RejectBlueDefinedTwice", &runRejectBlueDefinedTwice },
    { "RejectSelfCycle", &runRejectSelfCycle },
    { "RejectMutualCycle", &runRejectMutualCycle },
    { "RejectChoiceCycle", &runRejectChoiceCycle },
    { "RejectNestedCycle", &runRejectNestedCycle },
    { "AcceptSharedAcyclicReference", &runAcceptSharedAcyclicReference },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
