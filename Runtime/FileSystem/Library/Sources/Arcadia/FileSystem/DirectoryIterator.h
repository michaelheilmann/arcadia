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

#if !defined(ARCADIA_FILESYSTEM_DIRECTORYITERATOR_H_INCLUDED)
#define ARCADIA_FILESYSTEM_DIRECTORYITERATOR_H_INCLUDED

#if !defined(ARCADIA_FILESYSTEM_MODULE)
  #error("do not include directly, include `Arcadia/FileSystem/Include.h` instead")
#endif

#include "Arcadia/Ring2/Implementation/Configure.h"
#include "Arcadia/Ring1/Include.h"
typedef struct Arcadia_FilePath Arcadia_FilePath;

Arcadia_declareObjectType(u8"Arcadia.DirectoryIterator", Arcadia_DirectoryIterator,
                          u8"Arcadia.Object");

struct Arcadia_DirectoryIteratorDispatch {
  Arcadia_ObjectDispatch _parent;

  Arcadia_BooleanValue(*hasValue)(Arcadia_Thread* thread, Arcadia_DirectoryIterator* self);
  Arcadia_FilePath* (*getValue)(Arcadia_Thread* thread, Arcadia_DirectoryIterator* self);
  void (*nextValue)(Arcadia_Thread* thread, Arcadia_DirectoryIterator* self);
};

struct Arcadia_DirectoryIterator {
  Arcadia_Object _parent;
};

Arcadia_BooleanValue
Arcadia_DirectoryIterator_hasValue
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIterator* self
  );

Arcadia_FilePath*
Arcadia_DirectoryIterator_getValue
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIterator* self
  );

void
Arcadia_DirectoryIterator_nextValue
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIterator* self
  );

#endif // ARCADIA_FILESYSTEM_DIRECTORYITERATOR_H_INCLUDED
