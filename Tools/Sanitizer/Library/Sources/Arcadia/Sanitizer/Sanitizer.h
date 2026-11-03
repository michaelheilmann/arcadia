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

#if !defined(ARCADIA_SANITIZER_SANITIZER_H_INCLUDED)
#define ARCADIA_SANITIZER_SANITIZER_H_INCLUDED

#include "Arcadia/FileSystem/Include.h"
#include "Arcadia/Ring2/Include.h"

// The transform only removes trailing spaces/tabs and converts CRLF and lone
// CR to LF. No encoding conversion is performed, and an absent terminal LF
// stays absent. Output must be a different byte builder from input.
// Returns true if at least one byte differs, and records the first affected line.
Arcadia_BooleanValue
Arcadia_Sanitizer_normalize
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* input,
    Arcadia_ByteArrayBuilder* output,
    Arcadia_SizeValue* firstAffectedLine
  );

typedef struct Arcadia_Sanitizer_Statistics {
  Arcadia_SizeValue inspected;
  Arcadia_SizeValue changed;
  Arcadia_SizeValue skippedBinary;
  Arcadia_SizeValue skippedLinks;
} Arcadia_Sanitizer_Statistics;

// Traverse a file or directory. In check mode report violations but never write.
// In fix mode write only modified text files. Never follow symbolic links.
void
Arcadia_Sanitizer_scan
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* fileSystem,
    Arcadia_FilePath* path,
    Arcadia_FileHandle* output,
    Arcadia_BooleanValue fix,
    Arcadia_Sanitizer_Statistics* statistics
  );

#endif // ARCADIA_SANITIZER_SANITIZER_H_INCLUDED
