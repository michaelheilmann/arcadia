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

#if !defined(ARCADIA_DDLS_SCHEMA_H_INCLUDED)
#define ARCADIA_DDLS_SCHEMA_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

/// The base of all types.
Arcadia_declareObjectType(u8"Arcadia.DDLS.Type", Arcadia_DDLS_Type,
                          u8"Arcadia.Object");

struct Arcadia_DDLS_Type {
  Arcadia_Object parent;
};

/// Consists of a single element type.
Arcadia_declareObjectType(u8"Arcadia.DDLS.ListType", Arcadia_DDLS_ListType,
                          u8"Arcadia.DDLS.Type");

struct Arcadia_DDLS_ListType {
  Arcadia_DDLS_Type parent;
  Arcadia_DDLS_Type* element;
};

Arcadia_DDLS_ListType*
Arcadia_DDLS_ListType_create
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Type* element
  );

/// A map from names to types.
Arcadia_declareObjectType(u8"Arcadia.DDLS.MapType", Arcadia_DDLS_MapType,
                          u8"Arcadia.DDLS.Type");

struct Arcadia_DDLS_MapType {
  Arcadia_DDLS_Type parent;
  Arcadia_Map* entries;
};

Arcadia_DDLS_MapType*
Arcadia_DDLS_MapType_create
  (
    Arcadia_Thread* thread
  );

/// A name of a built-in scalar type.
Arcadia_declareObjectType(u8"Arcadia.DDLS.ScalarType", Arcadia_DDLS_ScalarType,
                          u8"Arcadia.DDLS.Type");

struct Arcadia_DDLS_ScalarType {
  Arcadia_DDLS_Type parent;
  Arcadia_String* name;
};

/**
 * @brief Create a scalar type.
 * @param thread This thread.
 */
Arcadia_DDLS_ScalarType*
Arcadia_DDLS_ScalarType_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_DDLS_SCHEMA_H_INCLUDED
