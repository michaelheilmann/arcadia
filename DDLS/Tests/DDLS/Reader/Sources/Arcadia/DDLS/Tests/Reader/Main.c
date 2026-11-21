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

#include "Arcadia/DDLS/Tests/Reader/Schema1.h"

#include "Arcadia/DDLS/Include.h"
#include <string.h>

static void
testAcceptChoice1
  (
    Arcadia_Thread* thread
  )
{
  const char* DDLS =
    "{\n"
    "  kind : \"Schema\",\n"
    "  name : \"AnyScalar\",\n"
    "  definition : \n"
    "  {\n"
    "    kind : \"Choice\",\n"
    "    choices : [\n"
    "      { kind : \"Boolean\" },\n"
    "      { kind : \"String\" },\n"
    "      { kind : \"Number\" },\n"
    "      { kind : \"Void\" },\n"
    "    ],\n"
    "  },\n"
    "}\n"
    ;
  Arcadia_DDLS_DefaultReader* ddlsReader =
    Arcadia_DDLS_DefaultReader_create
      (
        thread
      );
  Arcadia_DDLS_Node* ddlsNode =
    Arcadia_DDLS_DefaultReader_run
      (
        thread,
        ddlsReader,
        Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create
          (
            thread,
            DDLS,
            strlen(DDLS)
          ))
      );
  ddlsNode = NULL;
}

static void
testAcceptListElements
  (
    Arcadia_Thread* thread
  )
{
  const char* ddls =
    "{ kind : \"Schema\", name : \"Numbers\", definition : "
    "{ kind : \"List\", elements : { kind : \"Number\" } } }";
  Arcadia_DDLS_DefaultReader* reader = Arcadia_DDLS_DefaultReader_create(thread);
  Arcadia_DDLS_DefaultReader_run(thread, reader,
    Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, ddls, strlen(ddls))));
}

static void
testRejectListEntry
  (
    Arcadia_Thread* thread
  )
{
  const char* ddls =
    "{ kind : \"Schema\", name : \"Numbers\", definition : "
    "{ kind : \"List\", entry : { kind : \"Number\" } } }";
  Arcadia_DDLS_DefaultReader* reader = Arcadia_DDLS_DefaultReader_create(thread);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_DDLS_DefaultReader_run(thread, reader,
      Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, ddls, strlen(ddls))));
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
testRejectEmptyChoice
  (
    Arcadia_Thread* thread
  )
{
  const char* ddls =
    "{ kind : \"Schema\", name : \"EmptyChoice\", definition : "
    "{ kind : \"Choice\", choices : [] } }";
  Arcadia_DDLS_DefaultReader* reader = Arcadia_DDLS_DefaultReader_create(thread);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_DDLS_DefaultReader_run(thread, reader,
      Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, ddls, strlen(ddls))));
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


#define OPTIONAL_SCHEMA(field) \
  "{ kind : \"Schema\", name : \"Optional\", definition : { kind : \"Map\", entries : [" \
  "{ kind : \"MapEntry\", name : \"value\", type : { kind : \"String\" }, " field " } ] } }"

static void
testOptionalBooleanSchemas
  (
    Arcadia_Thread* thread
  )
{
  static const struct {
    const char* source;
    Arcadia_BooleanValue accepted;
  } cases[] = {
    { OPTIONAL_SCHEMA("optional : true,"), Arcadia_BooleanValue_True },
    { OPTIONAL_SCHEMA("optional : false,"), Arcadia_BooleanValue_True },
    { OPTIONAL_SCHEMA(""), Arcadia_BooleanValue_True },
    { OPTIONAL_SCHEMA("optional : \"true\","), Arcadia_BooleanValue_False },
    { OPTIONAL_SCHEMA("optional : \"false\","), Arcadia_BooleanValue_False },
    { OPTIONAL_SCHEMA("optional : 1,"), Arcadia_BooleanValue_False },
    { OPTIONAL_SCHEMA("optional : void,"), Arcadia_BooleanValue_False },
    { OPTIONAL_SCHEMA("optional : [],"), Arcadia_BooleanValue_False },
    { OPTIONAL_SCHEMA("optional : {},"), Arcadia_BooleanValue_False },
  };
  for (Arcadia_SizeValue i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    Arcadia_DDLS_DefaultReader* reader = Arcadia_DDLS_DefaultReader_create(thread);
    if (Arcadia_BooleanValue_True == cases[i].accepted) {
      Arcadia_DDLS_DefaultReader_run(thread, reader,
        Arcadia_ByteArray_createByteArray(thread,
          Arcadia_RuntimeByteArray_create(thread, cases[i].source, strlen(cases[i].source))));
    } else {
      Arcadia_JumpTarget target;
      Arcadia_Thread_pushJumpTarget(thread, &target);
      if (Arcadia_JumpTarget_save(&target)) {
        Arcadia_DDLS_DefaultReader_run(thread, reader,
          Arcadia_ByteArray_createByteArray(thread,
            Arcadia_RuntimeByteArray_create(thread, cases[i].source, strlen(cases[i].source))));
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

#undef OPTIONAL_SCHEMA

static int
runOptionalBooleanSchemas
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testOptionalBooleanSchemas) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runAcceptMap
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Reader_testAccept1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runAcceptSchemaReferences
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Reader_testAccept2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectMissingKind
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Reader_testReject1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectMissingEntries
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Reader_testReject2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectEntriesNotList
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Reader_testReject3) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectDuplicateKind
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Reader_testReject4) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectDuplicateEntryName
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Reader_testReject5) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runAcceptChoice
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testAcceptChoice1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runAcceptListElements
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testAcceptListElements) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectListEntry
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRejectListEntry) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRejectEmptyChoice
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRejectEmptyChoice) ? EXIT_SUCCESS : EXIT_FAILURE;
}

#define ENUM_SCHEMA(definition) "{ kind : \"Schema\", name : \"Enum\", definition : " definition " }"

static void
testStringEnumerations
  (
    Arcadia_Thread* thread
  )
{
  static const struct {
    const char* schema;
    Arcadia_BooleanValue valid;
  } cases[] = {
    { ENUM_SCHEMA("{ kind : \"String\" }"), Arcadia_BooleanValue_True },
    { ENUM_SCHEMA("{ kind : \"String\", enum : [\"Nearest\", \"Linear\", \"Cubic\"] }"), Arcadia_BooleanValue_True },
    { ENUM_SCHEMA("{ kind : \"String\", enum : [\"\", \"Other\"] }"), Arcadia_BooleanValue_True },
    { ENUM_SCHEMA("{ kind : \"String\", enum : [\"\\u{E9}\", \"e\\u{301}\"] }"), Arcadia_BooleanValue_True },
    { ENUM_SCHEMA("{ kind : \"String\", enum : [] }"), Arcadia_BooleanValue_False },
    { ENUM_SCHEMA("{ kind : \"String\", enum : \"Nearest\" }"), Arcadia_BooleanValue_False },
    { ENUM_SCHEMA("{ kind : \"String\", enum : void }"), Arcadia_BooleanValue_False },
    { ENUM_SCHEMA("{ kind : \"String\", enum : [1] }"), Arcadia_BooleanValue_False },
    { ENUM_SCHEMA("{ kind : \"String\", enum : [\"A\", 1] }"), Arcadia_BooleanValue_False },
    { ENUM_SCHEMA("{ kind : \"String\", enum : [\"Nearest\", \"Nearest\"] }"), Arcadia_BooleanValue_False },
    { ENUM_SCHEMA("{ kind : \"String\", enum : [\"A\", \"\\u{41}\"] }"), Arcadia_BooleanValue_False },
    { ENUM_SCHEMA("{ kind : \"String\", enum : [\"\", \"\"] }"), Arcadia_BooleanValue_False },
    { ENUM_SCHEMA("{ kind : \"Choice\", choices : [{ kind : \"Any\" }, { kind : \"String\", enum : [] }] }"), Arcadia_BooleanValue_False },
    { ENUM_SCHEMA("{ kind : \"List\", elements : { kind : \"String\", enum : [\"A\", \"A\"] } }"), Arcadia_BooleanValue_False },
    { ENUM_SCHEMA("{ kind : \"Map\", entries : [{kind:\"MapEntry\", name:\"filter\", type:{kind:\"String\", enum:[\"Linear\",\"Linear\"]}}] }"), Arcadia_BooleanValue_False },
  };
  for (Arcadia_SizeValue i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    Arcadia_DDLS_DefaultReader* reader = Arcadia_DDLS_DefaultReader_create(thread);
    Arcadia_JumpTarget target;
    Arcadia_Thread_pushJumpTarget(thread, &target);
    if (Arcadia_JumpTarget_save(&target)) {
      Arcadia_DDLS_DefaultReader_run(thread, reader,
        Arcadia_ByteArray_createByteArray(thread,
          Arcadia_RuntimeByteArray_create(thread, cases[i].schema, strlen(cases[i].schema))));
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

#undef ENUM_SCHEMA

static int
runStringEnumerations
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testStringEnumerations) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "AcceptMap", &runAcceptMap },
    { "StringEnumerations", &runStringEnumerations },
    { "OptionalBooleanSchemas", &runOptionalBooleanSchemas },
    { "AcceptSchemaReferences", &runAcceptSchemaReferences },
    { "RejectMissingKind", &runRejectMissingKind },
    { "RejectMissingEntries", &runRejectMissingEntries },
    { "RejectEntriesNotList", &runRejectEntriesNotList },
    { "RejectDuplicateKind", &runRejectDuplicateKind },
    { "RejectDuplicateEntryName", &runRejectDuplicateEntryName },
    { "AcceptChoice", &runAcceptChoice },
    { "AcceptListElements", &runAcceptListElements },
    { "RejectListEntry", &runRejectListEntry },
    { "RejectEmptyChoice", &runRejectEmptyChoice },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
