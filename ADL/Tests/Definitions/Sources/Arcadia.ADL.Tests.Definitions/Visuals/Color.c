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

#include "Arcadia.ADL.Tests.Definitions/Visuals/Color.h"

#include "Arcadia/ADL/Include.h"

void
Arcadia_ADL_Tests_Visuals_ColorDefinitionTest1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  static const char *DDL =
    "{\n"
    "  type : \"Color\",\n"
    "  name : \"Colors.Red\",\n"
    "  red : 255,\n"
    "  green : 0,\n"
    "  blue : 0,\n"
    "}\n"
    ;
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
}

void
Arcadia_ADL_Tests_Visuals_ColorDefinitionTest2
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  static const char* DDL =
    "{\n"
    "  type : \"Color\",\n"
    "  name : \"Colors.Green\",\n"
    "  red : 0,\n"
    "  green : 255,\n"
    "  blue : 0,\n"
    "}\n"
    ;
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
}
