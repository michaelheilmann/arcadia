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

#if !defined(ARCADIA_DDLS_IMPLEMENTATION_DIAGNOSTICS_H_INCLUDED)
#define ARCADIA_DDLS_IMPLEMENTATION_DIAGNOSTICS_H_INCLUDED

#include "Arcadia/DDLS/Nodes/Include.h"
#include "Arcadia/DDL/Include.h"
#include "Arcadia/Languages/Include.h"

// Used for building diagnostic messages.
Arcadia_declareObjectType(u8"Arcadia.DDLS.Diagnostics", Arcadia_DDLS_Diagnostics,
                          u8"Arcadia.Object");

struct Arcadia_DDLS_DiagnosticsDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_DDLS_Diagnostics {
  Arcadia_Object parent;
  Arcadia_String* BOOLEAN;
  Arcadia_String* LIST;
  Arcadia_String* MAP;
  Arcadia_String* NUMBER;
  Arcadia_String* SCHEMA;
  Arcadia_String* SCHEMAREFERENCE;
  Arcadia_String* STRING;
  Arcadia_String* VOID;
  Arcadia_StringBuilder* stringBuffer;
  Arcadia_Languages_StringTable* stringTable;
};

Arcadia_DDLS_Diagnostics*
Arcadia_DDLS_Diagnostics_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTable* stringTable
  );

void
Arcadia_DDLS_Diagnostics_unexpectedTypeError
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Diagnostics* self,
    Arcadia_DDLS_Node* ddlsNode,
    Arcadia_DDL_Node* node
  );

void
Arcadia_DDLS_Diagnostics_mapEntryAlreadyDefined
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Diagnostics* self,
    Arcadia_String* name
  );

void
Arcadia_DDLS_Diagnostics_mapEntryExistsError
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Diagnostics* self,
    Arcadia_String* name
  );

void
Arcadia_DDLS_Diagnostics_mapEntryNotExistsError
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Diagnostics* self,
    Arcadia_String* name
  );

void
Arcadia_DDLS_Diagnostics_unresolvedSchemaReferenceError
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Diagnostics* self,
    Arcadia_String* name
  );


#endif // ARCADIA_DDLS_IMPLEMENTATION_DIAGNOSTICS_H_INCLUDED
