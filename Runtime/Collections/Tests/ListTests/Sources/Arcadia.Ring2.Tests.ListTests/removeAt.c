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

#include "Arcadia.Ring2.Tests.ListTests/removeAt.h"

void Arcadia_Collections_Tests_ListTests_removeAt(Arcadia_Thread* thread) {
  { 
    Arcadia_List* l = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    Arcadia_List_insertBackInteger32Value(thread, l, 1);
    Arcadia_List_insertBackInteger32Value(thread, l, 2);
    Arcadia_List_insertBackInteger32Value(thread, l, 3);
    Arcadia_List_removeBack(thread, l, 1);
    Arcadia_Tests_assertTrue(thread, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)l) == 2);
    Arcadia_Tests_assertTrue(thread, Arcadia_List_getInteger32ValueAt(thread, l, 0) == 1);
    Arcadia_Tests_assertTrue(thread, Arcadia_List_getInteger32ValueAt(thread, l, 1) == 2);

  }
}