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
#include "Arcadia/DDLS/Tests/Validation/Default.h"
#include <stdio.h>
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

// Any accepts every DDL value kind.
static void
Arcadia_DDLS_Tests_Validation_anyAccept1
  (
    Arcadia_Thread* thread
  )
{
  static const char* DDLS =
    "{\n"
    "  kind : \"Schema\",\n"
    "  name : \"MySchema\",\n"
    "  definition : { kind : \"Any\" },\n"
    "}\n"
    ;
  static const char* DDL[] = {
    "true",
    "false",
    "void",
    "0",
    "1",
    "-1",
    "0.1",
    "-0.1",
    "\"w\"",
    "[]",
    "[1, true, \"w\"]",
    "{}",
    "{ a : 1, b : \"w\" }",
  };
  for (Arcadia_SizeValue i = 0; i < sizeof(DDL) / sizeof(DDL[0]); ++i) {
    Arcadia_DDLS_SchemaNode* ddlsNode = readDDLS(thread, DDLS);
    Arcadia_DDL_Node* node = readDDL(thread, DDL[i]);
    Arcadia_DDLS_ValidationContext* validationContext = Arcadia_DDLS_ValidationContext_create(thread);
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, ddlsNode);
    Arcadia_DDLS_ValidationContext_run(thread, validationContext, Arcadia_String_createFromCxxString(thread, u8"MySchema"), node);
  }
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
    "}\n"
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

// A mismatch is not a schema error: the next alternative may accept.
static Arcadia_DDLS_SchemaNode*
readChoiceSchema
  (
    Arcadia_Thread* thread,
    const char* definition
  )
{
  char source[1024];
  int length = snprintf(source, sizeof(source), "{ kind : \"Schema\", name : \"MySchema\", definition : %s }", definition);
  if (length < 0 || (size_t)length >= sizeof(source)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  return readDDLS(thread, source);
}

static void
Arcadia_DDLS_Tests_Validation_choiceLaterAlternative
  (
    Arcadia_Thread* thread
  )
{
  static const struct {
    const char* definition;
    const char* value;
  } cases[] = {
    { "{ kind : \"Choice\", choices : [{ kind : \"Number\" }, { kind : \"String\" }] }", "\"text\"" },
    { "{ kind : \"Choice\", choices : [{ kind : \"String\" }, { kind : \"Any\" }] }", "42" },
    { "{ kind : \"Choice\", choices : ["
      "{ kind : \"Map\", entries : [{ kind : \"MapEntry\", name : \"required\", type : { kind : \"String\" } }] },"
      "{ kind : \"Any\" }] }", "{}" },
    { "{ kind : \"Choice\", choices : ["
      "{ kind : \"List\", elements : { kind : \"String\" } },"
      "{ kind : \"List\", elements : { kind : \"Any\" } }] }", "[1]" },
  };
  for (Arcadia_SizeValue i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    Arcadia_DDLS_SchemaNode* schema = readChoiceSchema(thread, cases[i].definition);
    Arcadia_DDL_Node* value = readDDL(thread, cases[i].value);
    Arcadia_DDLS_ValidationContext* context = Arcadia_DDLS_ValidationContext_create(thread);
    Arcadia_DDLS_ValidationContext_addSchema(thread, context, schema);
    Arcadia_DDLS_ValidationContext_run(thread, context, Arcadia_String_createFromCxxString(thread, u8"MySchema"), value);
  }
}

static void
Arcadia_DDLS_Tests_Validation_choiceFailure
  (
    Arcadia_Thread* thread,
    const char* definition
  )
{
  Arcadia_DDLS_SchemaNode* schema = readChoiceSchema(thread, definition);
  Arcadia_DDL_Node* value = readDDL(thread, "42");
  Arcadia_DDLS_ValidationContext* context = Arcadia_DDLS_ValidationContext_create(thread);
  Arcadia_DDLS_ValidationContext_addSchema(thread, context, schema);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_DDLS_ValidationContext_run(thread, context, Arcadia_String_createFromCxxString(thread, u8"MySchema"), value);
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
Arcadia_DDLS_Tests_Validation_choiceAllMismatch
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_DDLS_Tests_Validation_choiceFailure(thread,
    "{ kind : \"Choice\", choices : [{ kind : \"String\" }, { kind : \"Boolean\" }] }");
}

static void
Arcadia_DDLS_Tests_Validation_choiceStructuralError
  (
    Arcadia_Thread* thread
  )
{
  // A missing reference must not be treated as a mismatch allowing Any to succeed.
  Arcadia_DDLS_Tests_Validation_choiceFailure(thread,
    "{ kind : \"Choice\", choices : ["
    "{ kind : \"SchemaReference\", name : \"Missing\" }, { kind : \"Any\" }] }");
}

static void
Arcadia_DDLS_Tests_Validation_choiceUnusedStructuralError
  (
    Arcadia_Thread* thread
  )
{
  // Any matches first, but a later alternative still contains an invalid reference.
  Arcadia_DDLS_Tests_Validation_choiceFailure(thread,
    "{ kind : \"Choice\", choices : ["
    "{ kind : \"Any\" }, { kind : \"SchemaReference\", name : \"Missing\" }] }");
}

static void
Arcadia_DDLS_Tests_Validation_choiceUnusedNestedStructuralError
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_DDLS_Tests_Validation_choiceFailure(thread,
    "{ kind : \"Choice\", choices : ["
    "{ kind : \"Any\" }, { kind : \"Map\", entries : ["
    "{ kind : \"MapEntry\", name : \"items\", type : "
    "{ kind : \"List\", elements : { kind : \"SchemaReference\", name : \"Missing\" } } }] }] }");
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
    "}\n"
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
    "        optional : true,\n"
    "        type : {\n"
    "          kind : \"String\",\n"
    "        },\n"
    "      },\n"
    "    ],\n"
    "  },\n"
    "}\n"
    ;

  const char* DDL[] =
    {
      "{\n"
      "  type : \"MySchema\",\n"
      "}\n",
      "{\n"
      "  type : \"MySchema\",\n"
      "  optionalEntry : \"a\",\n"
      "}\n",
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
    "        optional : true,\n"
    "        type : {\n"
    "          kind : \"String\",\n"
    "        },\n"
    "      },\n"
    "    ],\n"
    "  },\n"
    "}\n"
    ;

  const char* DDL =
    "{\n"
    "  optionalEntry : \"a\",\n"
    "}\n"
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


#define OPTIONAL_VALIDATION_SCHEMA(field) \
  "{ kind : \"Schema\", name : \"MySchema\", definition : { kind : \"Map\", entries : [" \
  "{ kind : \"MapEntry\", name : \"label\", type : { kind : \"String\" }, " field " }," \
  "{ kind : \"MapEntry\", name : \"present\", type : { kind : \"Void\" } } ] } }"

static void
testOptionalBooleanValidation
  (
    Arcadia_Thread* thread
  )
{
  static const struct {
    const char* schema;
    const char* value;
    Arcadia_BooleanValue accepted;
  } cases[] = {
    { OPTIONAL_VALIDATION_SCHEMA("optional : true,"), "{ present : void }", Arcadia_BooleanValue_True },
    { OPTIONAL_VALIDATION_SCHEMA("optional : true,"), "{ label : \"x\", present : void }", Arcadia_BooleanValue_True },
    { OPTIONAL_VALIDATION_SCHEMA("optional : true,"), "{ label : void, present : void }", Arcadia_BooleanValue_False },
    { OPTIONAL_VALIDATION_SCHEMA("optional : true,"), "{ present : \"x\" }", Arcadia_BooleanValue_False },
    { OPTIONAL_VALIDATION_SCHEMA("optional : true,"), "{ present : void, extra : 1 }", Arcadia_BooleanValue_False },
    { OPTIONAL_VALIDATION_SCHEMA("optional : true,"), "{}", Arcadia_BooleanValue_False },
    { OPTIONAL_VALIDATION_SCHEMA("optional : false,"), "{ label : \"x\", present : void }", Arcadia_BooleanValue_True },
    { OPTIONAL_VALIDATION_SCHEMA("optional : false,"), "{ present : void }", Arcadia_BooleanValue_False },
    { OPTIONAL_VALIDATION_SCHEMA(""), "{ label : \"x\", present : void }", Arcadia_BooleanValue_True },
    { OPTIONAL_VALIDATION_SCHEMA(""), "{ present : void }", Arcadia_BooleanValue_False },
  };
  for (Arcadia_SizeValue i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    Arcadia_DDLS_SchemaNode* schema = readDDLS(thread, cases[i].schema);
    Arcadia_DDL_Node* value = readDDL(thread, cases[i].value);
    Arcadia_DDLS_ValidationContext* context = Arcadia_DDLS_ValidationContext_create(thread);
    Arcadia_DDLS_ValidationContext_addSchema(thread, context, schema);
    if (Arcadia_BooleanValue_True == cases[i].accepted) {
      Arcadia_DDLS_ValidationContext_run(thread, context, Arcadia_String_createFromCxxString(thread, u8"MySchema"), value);
    } else {
      Arcadia_JumpTarget target;
      Arcadia_Thread_pushJumpTarget(thread, &target);
      if (Arcadia_JumpTarget_save(&target)) {
        Arcadia_DDLS_ValidationContext_run(thread, context, Arcadia_String_createFromCxxString(thread, u8"MySchema"), value);
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
  }
}

#undef OPTIONAL_VALIDATION_SCHEMA

static int
runOptionalBooleanValidation
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testOptionalBooleanValidation) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runAcceptColor
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_test1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectTypeNotDefined
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_testRejectTypeNotDefined) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectRedNotDefined
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_testRejectRedNotDefined) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectGreenNotDefined
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_testRejectGreenNotDefined) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectBlueNotDefined
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_testRejectBlueNotDefined) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectTypeDefinedTwice
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_testRejectTypeDefinedTwice) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectRedDefinedTwice
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_testRejectRedDefinedTwice) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectGreenDefinedTwice
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_testRejectGreenDefinedTwice) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectBlueDefinedTwice
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_testRejectBlueDefinedTwice) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runAcceptAny
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_anyAccept1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runAcceptChoice
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_choiceAccept1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectChoice
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_choiceReject1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runChoiceLaterAlternative
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_choiceLaterAlternative) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runChoiceAllMismatch
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_choiceAllMismatch) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runChoiceStructuralError
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_choiceStructuralError) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runChoiceUnusedStructuralError
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_choiceUnusedStructuralError) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runChoiceUnusedNestedStructuralError
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_choiceUnusedNestedStructuralError) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runAcceptOptional
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_optionalAccept1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectOptional
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Validation_optionalReject1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testStringEnumerationValidation
  (
    Arcadia_Thread* thread
  )
{
  static const struct {
    const char* schema;
    const char* value;
    Arcadia_BooleanValue valid;
  } cases[] = {
    { "{ kind : \"Schema\", name : \"Enum\", definition : { kind : \"String\", enum : [\"Nearest\", \"Linear\"] } }", "\"Nearest\"", Arcadia_BooleanValue_True },
    { "{ kind : \"Schema\", name : \"Enum\", definition : { kind : \"String\", enum : [\"Nearest\", \"Linear\"] } }", "\"nearest\"", Arcadia_BooleanValue_False },
    { "{ kind : \"Schema\", name : \"Enum\", definition : { kind : \"String\", enum : [\"Nearest\", \"Linear\"] } }", "\"Other\"", Arcadia_BooleanValue_False },
    { "{ kind : \"Schema\", name : \"Enum\", definition : { kind : \"String\", enum : [\"Nearest\", \"Linear\"] } }", "1", Arcadia_BooleanValue_False },
    { "{ kind : \"Schema\", name : \"Enum\", definition : { kind : \"String\", enum : [\"\", \"Other\"] } }", "\"\"", Arcadia_BooleanValue_True },
    { "{ kind : \"Schema\", name : \"Enum\", definition : { kind : \"String\", enum : [\"A\"] } }", "\"\\u{41}\"", Arcadia_BooleanValue_True },
    { "{ kind : \"Schema\", name : \"Enum\", definition : { kind : \"String\", enum : [\"\\u{E9}\"] } }", "\"e\\u{301}\"", Arcadia_BooleanValue_False },
    { "{ kind : \"Schema\", name : \"Enum\", definition : { kind : \"String\" } }", "\"Unlisted\"", Arcadia_BooleanValue_True },
    { "{ kind : \"Schema\", name : \"Enum\", definition : { kind : \"Choice\", choices : [{kind:\"String\",enum:[\"Nearest\"]}, {kind:\"Any\"}] } }", "\"Other\"", Arcadia_BooleanValue_True },
    { "{ kind : \"Schema\", name : \"Enum\", definition : { kind : \"Choice\", choices : [{kind:\"Number\"}, {kind:\"String\",enum:[\"Nearest\"]}] } }", "\"Nearest\"", Arcadia_BooleanValue_True },
    { "{ kind : \"Schema\", name : \"Enum\", definition : { kind : \"List\", elements : {kind:\"String\",enum:[\"Nearest\"]} } }", "[\"Nearest\"]", Arcadia_BooleanValue_True },
    { "{ kind : \"Schema\", name : \"Enum\", definition : { kind : \"List\", elements : {kind:\"String\",enum:[\"Nearest\"]} } }", "[\"Other\"]", Arcadia_BooleanValue_False },
    { "{ kind : \"Schema\", name : \"Enum\", definition : { kind : \"Map\", entries : [{kind:\"MapEntry\",name:\"filter\",type:{kind:\"String\",enum:[\"Linear\"]}}] } }", "{filter:\"Linear\"}", Arcadia_BooleanValue_True },
    { "{ kind : \"Schema\", name : \"Enum\", definition : { kind : \"Map\", entries : [{kind:\"MapEntry\",name:\"filter\",type:{kind:\"String\",enum:[\"Linear\"]}}] } }", "{filter:\"Other\"}", Arcadia_BooleanValue_False },
  };
  for (Arcadia_SizeValue i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    Arcadia_DDLS_SchemaNode* schema = readDDLS(thread, cases[i].schema);
    Arcadia_DDL_Node* value = readDDL(thread, cases[i].value);
    Arcadia_DDLS_ValidationContext* context = Arcadia_DDLS_ValidationContext_create(thread);
    Arcadia_DDLS_ValidationContext_addSchema(thread, context, schema);
    Arcadia_JumpTarget target;
    Arcadia_Thread_pushJumpTarget(thread, &target);
    if (Arcadia_JumpTarget_save(&target)) {
      Arcadia_DDLS_ValidationContext_run(thread, context, Arcadia_String_createFromCxxString(thread, u8"Enum"), value);
      Arcadia_Thread_popJumpTarget(thread);
      if (Arcadia_BooleanValue_False == cases[i].valid) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
        Arcadia_Thread_jump(thread);
      }
    } else {
      Arcadia_Status status = Arcadia_Thread_getStatus(thread);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
      if (Arcadia_BooleanValue_True == cases[i].valid || Arcadia_Status_SemanticalError != status) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
        Arcadia_Thread_jump(thread);
      }
    }
  }
}

static int
runStringEnumerationValidation
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testStringEnumerationValidation) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testNumberBounds
  (
    Arcadia_Thread* thread
  )
{
  static const struct {
    const char* definition;
    const char* value;
    Arcadia_BooleanValue valid;
  } cases[] = {
    { "{kind:\"Number\"}", "-1", Arcadia_BooleanValue_True },
    { "{kind:\"Number\"}", "1e1000", Arcadia_BooleanValue_True },
    { "{kind:\"Number\"}", "\"1\"", Arcadia_BooleanValue_False },
    { "{kind:\"Number\",minimum:0}", "0", Arcadia_BooleanValue_True },
    { "{kind:\"Number\",minimum:0}", "+00.0", Arcadia_BooleanValue_True },
    { "{kind:\"Number\",minimum:0}", "-0.1", Arcadia_BooleanValue_False },
    { "{kind:\"Number\",exclusiveMinimum:0}", "0.0001", Arcadia_BooleanValue_True },
    { "{kind:\"Number\",exclusiveMinimum:0}", "-0", Arcadia_BooleanValue_False },
    { "{kind:\"Number\",exclusiveMinimum:0}", "0", Arcadia_BooleanValue_False },
    { "{kind:\"Number\",maximum:10}", "10", Arcadia_BooleanValue_True },
    { "{kind:\"Number\",maximum:10}", "-3", Arcadia_BooleanValue_True },
    { "{kind:\"Number\",maximum:10}", "10.0001", Arcadia_BooleanValue_False },
    { "{kind:\"Number\",exclusiveMaximum:10}", "9.999", Arcadia_BooleanValue_True },
    { "{kind:\"Number\",exclusiveMaximum:10}", "10", Arcadia_BooleanValue_False },
    { "{kind:\"Number\",minimum:0,exclusiveMinimum:5,maximum:10,exclusiveMaximum:10}", "6", Arcadia_BooleanValue_True },
    { "{kind:\"Number\",minimum:0,exclusiveMinimum:5,maximum:10,exclusiveMaximum:10}", "5", Arcadia_BooleanValue_False },
    { "{kind:\"Number\",minimum:0,exclusiveMinimum:5,maximum:10,exclusiveMaximum:10}", "10", Arcadia_BooleanValue_False },
    { "{kind:\"Number\",minimum:1,maximum:1}", "+01.00e0", Arcadia_BooleanValue_True },
    { "{kind:\"Number\",minimum:1,maximum:1}", "1.0001", Arcadia_BooleanValue_False },
    { "{kind:\"Number\",minimum:9007199254740993}", "9007199254740993", Arcadia_BooleanValue_True },
    { "{kind:\"Number\",minimum:9007199254740993}", "9007199254740992", Arcadia_BooleanValue_False },
    { "{kind:\"Number\",minimum:1e100000000000000000000}", "1e100000000000000000000", Arcadia_BooleanValue_True },
    { "{kind:\"Number\",minimum:1e100000000000000000000}", "9e99999999999999999999", Arcadia_BooleanValue_False },
    { "{kind:\"Number\",maximum:1e-100000000000000000000}", "1e-100000000000000000001", Arcadia_BooleanValue_True },
    { "{kind:\"Number\",maximum:1e-100000000000000000000}", "1e-99999999999999999999", Arcadia_BooleanValue_False },
    { "{kind:\"List\",elements:{kind:\"Number\",minimum:0}}", "[1,0]", Arcadia_BooleanValue_True },
    { "{kind:\"List\",elements:{kind:\"Number\",minimum:0}}", "[1,-1]", Arcadia_BooleanValue_False },
    { "{kind:\"Map\",entries:[{kind:\"MapEntry\",name:\"value\",type:{kind:\"Number\",maximum:5}}]}", "{value:6}", Arcadia_BooleanValue_False },
    { "{kind:\"Map\",entries:[{kind:\"MapEntry\",name:\"value\",type:{kind:\"Number\",maximum:5}}]}", "{value:5}", Arcadia_BooleanValue_True },
    { "{kind:\"Choice\",choices:[{kind:\"Number\",minimum:10},{kind:\"Number\",maximum:5}]}", "3", Arcadia_BooleanValue_True },
    { "{kind:\"Choice\",choices:[{kind:\"Number\",minimum:10},{kind:\"Number\",maximum:5}]}", "7", Arcadia_BooleanValue_False },
    { "{kind:\"Choice\",choices:[{kind:\"Number\",minimum:10},{kind:\"String\"}]}", "\"text\"", Arcadia_BooleanValue_True },
  };
  for (Arcadia_SizeValue i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    char schemaText[2048];
    int length = snprintf(schemaText, sizeof(schemaText),
      "{kind:\"Schema\",name:\"NumberBounds\",definition:%s}", cases[i].definition);
    if (length < 0 || (size_t)length >= sizeof(schemaText)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_DDLS_SchemaNode* schema = readDDLS(thread, schemaText);
    Arcadia_DDL_Node* value = readDDL(thread, cases[i].value);
    Arcadia_DDLS_ValidationContext* context = Arcadia_DDLS_ValidationContext_create(thread);
    Arcadia_DDLS_ValidationContext_addSchema(thread, context, schema);
    Arcadia_JumpTarget target;
    Arcadia_Thread_pushJumpTarget(thread, &target);
    if (Arcadia_JumpTarget_save(&target)) {
      Arcadia_DDLS_ValidationContext_run(thread, context, Arcadia_String_createFromCxxString(thread, u8"NumberBounds"), value);
      Arcadia_Thread_popJumpTarget(thread);
      if (Arcadia_BooleanValue_False == cases[i].valid) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
        Arcadia_Thread_jump(thread);
      }
    } else {
      Arcadia_Status status = Arcadia_Thread_getStatus(thread);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
      if (Arcadia_BooleanValue_True == cases[i].valid || Arcadia_Status_SemanticalError != status) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
        Arcadia_Thread_jump(thread);
      }
    }
  }
}

static void
testNumberBoundsInvalidSchemas
  (
    Arcadia_Thread* thread
  )
{
  static const char* definitions[] = {
    "{kind:\"Number\",minimum:\"0\"}",
    "{kind:\"Number\",maximum:false}",
    "{kind:\"Number\",minimum:void}",
    "{kind:\"Number\",minimum:2,maximum:1}",
    "{kind:\"Number\",minimum:1,exclusiveMaximum:1}",
    "{kind:\"Number\",exclusiveMinimum:1,maximum:1}",
    "{kind:\"Number\",exclusiveMinimum:1,exclusiveMaximum:1}",
    "{kind:\"Number\",minimum:10,exclusiveMinimum:10,maximum:10}",
    "{kind:\"Number\",minimum:0,minimum:1}",
    "{kind:\"Number\",minimum:1,extra:0}",
    "{kind:\"List\",elements:{kind:\"Number\",minimum:2,maximum:1}}",
    "{kind:\"Choice\",choices:[{kind:\"Any\"},{kind:\"Number\",minimum:2,maximum:1}]}",
  };
  for (Arcadia_SizeValue i = 0; i < sizeof(definitions) / sizeof(definitions[0]); ++i) {
    char schemaText[2048];
    int length = snprintf(schemaText, sizeof(schemaText),
      "{kind:\"Schema\",name:\"NumberBounds\",definition:%s}", definitions[i]);
    if (length < 0 || (size_t)length >= sizeof(schemaText)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_JumpTarget target;
    Arcadia_Thread_pushJumpTarget(thread, &target);
    if (Arcadia_JumpTarget_save(&target)) {
      readDDLS(thread, schemaText);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
      Arcadia_Thread_jump(thread);
    } else {
      Arcadia_Status status = Arcadia_Thread_getStatus(thread);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
      if (Arcadia_Status_SemanticalError != status) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
        Arcadia_Thread_jump(thread);
      }
    }
  }
}

static int
runNumberBounds
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNumberBounds) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runNumberBoundsInvalidSchemas
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNumberBoundsInvalidSchemas) ? EXIT_SUCCESS : EXIT_FAILURE;
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
    { "StringEnumerationValidation", &runStringEnumerationValidation },
    { "NumberBounds", &runNumberBounds },
    { "NumberBoundsInvalidSchemas", &runNumberBoundsInvalidSchemas },
    { "OptionalBooleanValidation", &runOptionalBooleanValidation },
    { "RejectTypeNotDefined", &runRejectTypeNotDefined },
    { "RejectRedNotDefined", &runRejectRedNotDefined },
    { "RejectGreenNotDefined", &runRejectGreenNotDefined },
    { "RejectBlueNotDefined", &runRejectBlueNotDefined },
    { "RejectTypeDefinedTwice", &runRejectTypeDefinedTwice },
    { "RejectRedDefinedTwice", &runRejectRedDefinedTwice },
    { "RejectGreenDefinedTwice", &runRejectGreenDefinedTwice },
    { "RejectBlueDefinedTwice", &runRejectBlueDefinedTwice },
    { "AcceptAny", &runAcceptAny },
    { "AcceptChoice", &runAcceptChoice },
    { "RejectChoice", &runRejectChoice },
    { "ChoiceLaterAlternative", &runChoiceLaterAlternative },
    { "ChoiceAllMismatch", &runChoiceAllMismatch },
    { "ChoiceStructuralError", &runChoiceStructuralError },
    { "ChoiceUnusedStructuralError", &runChoiceUnusedStructuralError },
    { "ChoiceUnusedNestedStructuralError", &runChoiceUnusedNestedStructuralError },
    { "AcceptOptional", &runAcceptOptional },
    { "RejectOptional", &runRejectOptional },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
