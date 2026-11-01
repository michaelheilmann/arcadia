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

#define ARCADIA_ADL_PRIVATE (1)
#include "Arcadia/ADL/Graphs/ReferenceFields.module.h"

// Reference fields supported by source-level graph composition.
// `Graph.Output.source` is validated directly by the graph collector and is not listed here.
static const Arcadia_ADL_ReferenceFieldDescriptor _descriptors[] = {
  { "PixelBufferOperations.CheckerboardFillOperation", "firstCheckerColor", Arcadia_ADL_ReferenceFieldKind_Single, "Color" },
  { "PixelBufferOperations.CheckerboardFillOperation", "secondCheckerColor", Arcadia_ADL_ReferenceFieldKind_Single, "Color" },
  { "PixelBuffer", "operations", Arcadia_ADL_ReferenceFieldKind_List, NULL },
  { "DSP.Mix", "inputs", Arcadia_ADL_ReferenceFieldKind_List, "DSP" },
};

const Arcadia_ADL_ReferenceFieldDescriptor*
Arcadia_ADL_ReferenceFields_find
  (
    Arcadia_Thread* thread,
    Arcadia_String* typeName,
    Arcadia_String* fieldName
  )
{
  for (Arcadia_SizeValue i = 0, n = sizeof(_descriptors) / sizeof(_descriptors[0]); i < n; ++i) {
    if (Arcadia_ADL_Graphs_stringEquals(thread, typeName, _descriptors[i].typeName) &&
        Arcadia_ADL_Graphs_stringEquals(thread, fieldName, _descriptors[i].fieldName)) {
      return &_descriptors[i];
    }
  }
  return NULL;
}

Arcadia_BooleanValue
Arcadia_ADL_ReferenceFields_isPixelBufferOperationType
  (
    Arcadia_Thread* thread,
    Arcadia_String* typeName
  )
{
  if (Arcadia_ADL_Graphs_stringEquals(thread, typeName, "PixelBufferOperations.CheckerboardFillOperation") ||
      Arcadia_ADL_Graphs_stringEquals(thread, typeName, "PixelBufferOperations.FillOperation")) {
    return Arcadia_BooleanValue_True;
  }
  return Arcadia_BooleanValue_False;
}

Arcadia_BooleanValue
Arcadia_ADL_ReferenceFields_isDSPDefinitionType
  (
    Arcadia_Thread* thread,
    Arcadia_String* typeName
  )
{
  return (Arcadia_ADL_Graphs_stringEquals(thread, typeName, "DSP.SineWave") ||
          Arcadia_ADL_Graphs_stringEquals(thread, typeName, "DSP.Mix")) ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False;
}
