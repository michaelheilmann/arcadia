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

int
main
  (
    int argc,
    char** argv
  )
{
  if (!Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Reader_testAccept1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Reader_testAccept2)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Reader_testReject1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Reader_testReject2)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Reader_testReject3)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Reader_testReject4)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_DDLS_Tests_Reader_testReject5)) {
    return EXIT_FAILURE;
  }
  //
  if (!Arcadia_Tests_safeExecute(&testAcceptChoice1)) {
    return EXIT_FAILURE;
  }
  //
  return EXIT_SUCCESS;
}
