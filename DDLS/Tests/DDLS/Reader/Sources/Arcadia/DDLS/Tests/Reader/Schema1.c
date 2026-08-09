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

#include "Arcadia/DDLS/Tests/Reader/Schema1.h"

#include <string.h>

void
Arcadia_DDLS_Tests_Reader_testAccept1
  (
    Arcadia_Thread* thread
  )
{
  const char* DDLS =
    "{\n"
    "  kind : \"Schema\",\n"
    "  name : \"Color\",\n"
    "  definition : \n"
    "  {\n"
    "    kind : \"Map\",\n"
    "    entries : [\n"
    "      {\n"
    "        kind : \"MapEntry\",\n"
    "        name : \"type\",\n"
    "        type : {\n"
    "          kind : \"String\",\n"
    "        },\n"
    "      },\n"
    "      {\n"
    "        kind : \"MapEntry\",\n"
    "        name : \"red\",\n"
    "        type : {\n"
    "          kind : \"Number\",\n"
    "        },\n"
    "      },\n"
    "      {\n"
    "        kind : \"MapEntry\",\n"
    "        name : \"green\",\n"
    "        type : {\n"
    "          kind : \"Number\",\n"
    "        },\n"
    "      },\n"
    "      {\n"
    "        kind : \"MapEntry\",\n"
    "        name : \"blue\",\n"
    "        type : {\n"
    "          kind : \"Number\",\n"
    "        },\n"
    "      },\n"
    "    ],\n"
    "  },\n"
    "},\n"
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

void
Arcadia_DDLS_Tests_Reader_testAccept2
  (
    Arcadia_Thread* thread
  )
{
  const char* DDLS =
    "{\n"
    "  kind : \"Schema\",\n"
    "  name : \"Color\",\n"
    "  definition : \n"
    "  {\n"
    "    kind : \"Map\",\n"
    "    entries : [\n"
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
    "        },"
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

// "$.kind" is missing
void
Arcadia_DDLS_Tests_Reader_testReject1
  (
    Arcadia_Thread* thread
  )
{
  const char* DDLS =
    "{\n"
    "  kind : \"Schema\",\n"
    "  name : \"MySchema\",\n"
    "  definition : \n"
    "  {\n"
    "    entries : [\n"
    "    ],\n"
    "  },\n"
    "},\n"
    ;
  Arcadia_DDLS_DefaultReader* ddlsReader =
    Arcadia_DDLS_DefaultReader_create
      (
        thread
      );
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
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

// "$.entries" is missing
void
Arcadia_DDLS_Tests_Reader_testReject2
  (
    Arcadia_Thread* thread
  )
{
  const char* DDLS =
    "{\n"
    "  kind : \"Schema\",\n"
    "  name : \"MySchema\",\n"
    "  definition : \n"
    "  {\n"
    "    kind : \"Map\",\n"
    "  },\n"
    "},\n"
    ;
  Arcadia_DDLS_DefaultReader* ddlsReader =
    Arcadia_DDLS_DefaultReader_create
      (
        thread
      );
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
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

// "$.entries" is not a list
void
Arcadia_DDLS_Tests_Reader_testReject3
  (
    Arcadia_Thread* thread
  )
{
  const char* DDLS =
    "{\n"
    "  kind : \"Schema\",\n"
    "  name : \"MySchema\",\n"
    "  definition : \n"
    "  {\n"
    "    kind : \"Map\",\n"
    "    entries : \"x\",\n"
    "  },\n"
    "},\n"
    ;
  Arcadia_DDLS_DefaultReader* ddlsReader =
    Arcadia_DDLS_DefaultReader_create
    (
      thread
    );
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
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

// "$.kind" specified twice
void
Arcadia_DDLS_Tests_Reader_testReject4
  (
    Arcadia_Thread* thread
  )
{
  const char* DDLS =
    "{\n"
    "  kind : \"Schema\",\n"
    "  name : \"MySchema\",\n"
    "  definition : \n"
    "  {\n"
    "    kind : \"Map\",\n"
    "    kind : \"Map\",\n"
    "    entries : {\n"
    "    },\n"
    "  },\n"
    "},\n"
    ;
  Arcadia_DDLS_DefaultReader* ddlsReader =
    Arcadia_DDLS_DefaultReader_create
      (
        thread
      );
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
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

// "$.entries[0].name" and "$.entries[1].name" are equal
void
Arcadia_DDLS_Tests_Reader_testReject5
  (
    Arcadia_Thread* thread
  )
{
  const char* DDLS =
    "{\n"
    "  kind : \"Schema\",\n"
    "  name : \"MySchema\",\n"
    "  definition : \n"
    "  {\n"
    "    kind : \"Map\",\n"
    "    entries : [\n"
    "      {\n"
    "        kind : \"MapEntry\",\n"
    "        name : \"green\",\n"
    "        type : {\n"
    "          kind : \"Number\",\n"
    "        },\n"
    "      },\n"
    "      {\n"
    "        kind : \"MapEntry\",\n"
    "        name : \"green\",\n"
    "        type : {\n"
    "          kind : \"Number\",\n"
    "        },\n"
    "      },\n"
    "    ],\n"
    "  },\n"
    "},\n"
    ;
  Arcadia_DDLS_DefaultReader* ddlsReader =
    Arcadia_DDLS_DefaultReader_create
    (
      thread
    );
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
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
