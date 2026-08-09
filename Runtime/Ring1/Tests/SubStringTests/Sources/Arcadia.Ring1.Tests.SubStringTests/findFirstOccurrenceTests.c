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

#include "Arcadia.Ring1.Tests.SubStringTests/findFirstOccurrenceTests.h"

void
findFirstOccurrenceTests1
  (
    Arcadia_Thread* thread
  )
{
  {
    Arcadia_RuntimeUTF8String* string; 
    Arcadia_Value position;
    string = Arcadia_RuntimeUTF8String_create(thread, u8"", sizeof(u8"") - 1);
    position = Arcadia_RuntimeUTF8String_findFirstOccurrence(thread, string, 'a');
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isVoidValue(&position));
  }
  {
    Arcadia_RuntimeUTF8String* string;
    Arcadia_Value position;
    string = Arcadia_RuntimeUTF8String_create(thread, u8"b", sizeof(u8"b") - 1);
    position = Arcadia_RuntimeUTF8String_findFirstOccurrence(thread, string, 'a');
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isVoidValue(&position));
  }
  {
    Arcadia_RuntimeUTF8String* string;
    Arcadia_Value position;
    string = Arcadia_RuntimeUTF8String_create(thread, u8"bb", sizeof(u8"bb") - 1);
    position = Arcadia_RuntimeUTF8String_findFirstOccurrence(thread, string, 'a');
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isVoidValue(&position));
  }
  {
    Arcadia_RuntimeUTF8String* string;
    Arcadia_Value position;
    string = Arcadia_RuntimeUTF8String_create(thread, u8"bbb", sizeof(u8"bbb") - 1);
    position = Arcadia_RuntimeUTF8String_findFirstOccurrence(thread, string, 'a');
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isVoidValue(&position));
  }
}

void
findFirstOccurrenceTests2
  (
    Arcadia_Thread* thread
  )
{
  {
    Arcadia_RuntimeUTF8String* string;
    Arcadia_Value position;
    string = Arcadia_RuntimeUTF8String_create(thread, u8"abbb", sizeof(u8"abbb") - 1);
    position = Arcadia_RuntimeUTF8String_findFirstOccurrence(thread, string, 'a');
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isSizeValue(&position) && 0 == Arcadia_Value_getSizeValue(&position));
  }
  {
    Arcadia_RuntimeUTF8String* string;
    Arcadia_Value position;
    string = Arcadia_RuntimeUTF8String_create(thread, u8"babb", sizeof(u8"babb") - 1);
    position = Arcadia_RuntimeUTF8String_findFirstOccurrence(thread, string, 'a');
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isSizeValue(&position) && 1 == Arcadia_Value_getSizeValue(&position));
  }
  {
    Arcadia_RuntimeUTF8String* string;
    Arcadia_Value position;
    string = Arcadia_RuntimeUTF8String_create(thread, u8"bbab", sizeof(u8"bbab") - 1);
    position = Arcadia_RuntimeUTF8String_findFirstOccurrence(thread, string, 'a');
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isSizeValue(&position) && 2 == Arcadia_Value_getSizeValue(&position));
  }
  {
    Arcadia_RuntimeUTF8String* string;
    Arcadia_Value position;
    string = Arcadia_RuntimeUTF8String_create(thread, u8"bbba", sizeof(u8"bbba") - 1);
    position = Arcadia_RuntimeUTF8String_findFirstOccurrence(thread, string, 'a');
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isSizeValue(&position) && 3 == Arcadia_Value_getSizeValue(&position));
  }
}

void
findFirstOccurrenceTests3
  (
    Arcadia_Thread* thread
  )
{
  {
    Arcadia_RuntimeUTF8String* string;
    Arcadia_Value position;
    string = Arcadia_RuntimeUTF8String_create(thread, u8"ababb", sizeof(u8"ababb") - 1);
    position = Arcadia_RuntimeUTF8String_findFirstOccurrence(thread, string, 'a');
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isSizeValue(&position) && 0 == Arcadia_Value_getSizeValue(&position));
  }
  {
    Arcadia_RuntimeUTF8String* string;
    Arcadia_Value position;
    string = Arcadia_RuntimeUTF8String_create(thread, u8"babba", sizeof(u8"babba") - 1);
    position = Arcadia_RuntimeUTF8String_findFirstOccurrence(thread, string, 'a');
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isSizeValue(&position) && 1 == Arcadia_Value_getSizeValue(&position));
  }
  {
    Arcadia_RuntimeUTF8String* string;
    Arcadia_Value position;
    string = Arcadia_RuntimeUTF8String_create(thread, u8"bbaba", sizeof(u8"bbaba") - 1);
    position = Arcadia_RuntimeUTF8String_findFirstOccurrence(thread, string, 'a');
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isSizeValue(&position) && 2 == Arcadia_Value_getSizeValue(&position));
  }
  {
    Arcadia_RuntimeUTF8String* string;
    Arcadia_Value position;
    string = Arcadia_RuntimeUTF8String_create(thread, u8"bbbaa", sizeof(u8"bbbaa") - 1);
    position = Arcadia_RuntimeUTF8String_findFirstOccurrence(thread, string, 'a');
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isSizeValue(&position) && 3 == Arcadia_Value_getSizeValue(&position));
  }
}
