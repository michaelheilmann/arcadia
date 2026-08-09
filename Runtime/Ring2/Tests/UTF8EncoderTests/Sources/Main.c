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

#include "Arcadia/Ring2/Include.h"
#include <stdlib.h>
#include <string.h>

static void
regressionTest1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ByteArrayBuilder* encodingByteBuffer = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_Unicode_Encoder* encoder = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
  Arcadia_Unicode_Encoder_encodeString(thread, encoder, Arcadia_String_createFromCxxString(thread, u8".."), encodingByteBuffer);
  
  Arcadia_String* u = Arcadia_String_createFromCxxString(thread, u8"..");
  Arcadia_String* v = Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(encodingByteBuffer));

  Arcadia_Value t = Arcadia_Value_makeObjectReferenceValue(v);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)u, &t));
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&regressionTest1)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
