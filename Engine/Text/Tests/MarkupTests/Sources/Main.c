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

#include <stdlib.h>
#include <string.h>

#include "Arcadia/Collections/Include.h"
#include "Arcadia/Engine/Text/Include.h"
#include "Arcadia/Languages/Include.h"

static Arcadia_List*
parse
  (
    Arcadia_Thread* thread,
    char const* source
  )
{
  Arcadia_Engine_Text_MarkupParser* parser = Arcadia_Engine_Text_MarkupParser_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)parser);
  Arcadia_String* input = Arcadia_String_createFromCxxString(thread, source);
  Arcadia_Languages_Parser_setInput(thread, (Arcadia_Languages_Parser*)parser,
                                    (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, input)));
  Arcadia_Value result = Arcadia_Languages_Parser_run(thread, (Arcadia_Languages_Parser*)parser);
  Arcadia_List* runs = (Arcadia_List*)Arcadia_Value_getObjectReferenceValueChecked(thread, result, _Arcadia_ArrayList_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)runs);
  Arcadia_ValueStack_popValues(thread, 2);
  return runs;
}

static Arcadia_Engine_Text_MarkupTextRun*
getRun
  (
    Arcadia_Thread* thread,
    Arcadia_List* runs,
    Arcadia_SizeValue index
  )
{
  return (Arcadia_Engine_Text_MarkupTextRun*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, runs, index, _Arcadia_Engine_Text_MarkupTextRun_getType(thread));
}

static void
assertStringEquals
  (
    Arcadia_Thread* thread,
    Arcadia_String* string,
    char const* expected
  )
{
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, string, expected, strlen(expected)));
}

static void
parseRuns
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_List* runs = parse(thread,
                             "[rect=canvas align=center size=48 color=Colors.White]Hello, World!\n"
                             "[align=right x=10 y=20 width=300 height=40 size=16 color=Colors.Yellow fixed=true]{fps}");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)runs);
  Arcadia_Tests_assertTrue(thread, 2 == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)runs));

  Arcadia_Engine_Text_MarkupTextRun* first = getRun(thread, runs, 0);
  assertStringEquals(thread, first->text, "Hello, World!");
  assertStringEquals(thread, first->glyphSource, "Hello, World!");
  Arcadia_Tests_assertTrue(thread, NULL == first->dynamicToken);
  Arcadia_Tests_assertTrue(thread, 48 == first->pixelSize);
  assertStringEquals(thread, first->colorName, "Colors.White");
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Text_TextAlignment_Center == first->alignment);
  Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False == first->hasRectangle);
  Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False == first->fixedPlacement);

  Arcadia_Engine_Text_MarkupTextRun* second = getRun(thread, runs, 1);
  assertStringEquals(thread, second->text, "");
  assertStringEquals(thread, second->glyphSource, "fps");
  assertStringEquals(thread, second->dynamicToken, "fps");
  Arcadia_Tests_assertTrue(thread, 16 == second->pixelSize);
  assertStringEquals(thread, second->colorName, "Colors.Yellow");
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Text_TextAlignment_Right == second->alignment);
  Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_True == second->hasRectangle);
  Arcadia_Tests_assertTrue(thread, 10 == second->rectangleLeft);
  Arcadia_Tests_assertTrue(thread, 20 == second->rectangleBottom);
  Arcadia_Tests_assertTrue(thread, 300 == second->rectangleWidth);
  Arcadia_Tests_assertTrue(thread, 40 == second->rectangleHeight);
  Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_True == second->fixedPlacement);

  Arcadia_ValueStack_popValues(thread, 1);
}

static void
rejectUnknownAttribute
  (
    Arcadia_Thread* thread
  )
{
  parse(thread, "[unknown=value]Text");
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&parseRuns)) {
    return EXIT_FAILURE;
  }
  if (Arcadia_Tests_safeExecute(&rejectUnknownAttribute)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
