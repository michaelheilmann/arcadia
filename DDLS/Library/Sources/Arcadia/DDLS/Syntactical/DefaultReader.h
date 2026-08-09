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

#if !defined(ARCADIA_DDLS_READER_DEFAULTREADER_H_INCLUDED)
#define ARCADIA_DDLS_READER_DEFAULTREADER_H_INCLUDED

#include "Arcadia/DDLS/Nodes/Include.h"
#include "Arcadia/DDL/Include.h"

Arcadia_declareObjectType(u8"Arcadia.DDLS.DefaultReader", Arcadia_DDLS_DefaultReader,
                          u8"Arcadia.Object");

struct Arcadia_DDLS_DefaultReaderDispatch {
  Arcadia_ObjectDispatch parent;
  Arcadia_DDLS_Node* (*run)(Arcadia_Thread* thread, Arcadia_DDLS_DefaultReader*, Arcadia_ByteArray*);
};

struct Arcadia_DDLS_DefaultReader {
  Arcadia_Object parent;
  Arcadia_Value BOOLEAN;
  Arcadia_Value NUMBER;
  Arcadia_Value STRING;
  Arcadia_Value VOID;

  Arcadia_DDL_Parser* parser;
};

Arcadia_DDLS_DefaultReader*
Arcadia_DDLS_DefaultReader_create
  (
    Arcadia_Thread* thread
  );

Arcadia_DDLS_Node*
Arcadia_DDLS_DefaultReader_run
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self,
    Arcadia_ByteArray* input
  );

#endif // ARCADIA_DDLS_READER_DEFAULTREADER_H_INCLUDED
