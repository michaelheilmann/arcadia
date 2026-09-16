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

#if !defined(ARCADIA_LANGUAGES_PHYSICALINPUTFILE_H_INCLUDED)
#define ARCADIA_LANGUAGES_PHYSICALINPUTFILE_H_INCLUDED

#if !defined(ARCADIA_LANGUAGES_MODULE)
  #error("do not include directly, include `Arcadia/Languages/Include.h` instead")
#endif

#include "Arcadia/Languages/InputFile.h"
#if 0
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/FileSystem/Include.h"
#endif
#include "Arcadia/Languages/LineMap.h"


Arcadia_declareObjectType(u8"Arcadia.Languages.PhysicalInputFile", Arcadia_Languages_PhysicalInputFile,
                          u8"Arcadia.Languages.InputFile");

struct Arcadia_Languages_PhysicalInputFileDispatch {
  Arcadia_Languages_InputFileDispatch _parent;
};

struct Arcadia_Languages_PhysicalInputFile {
  Arcadia_Languages_InputFile _parent;
  /// The name of the file.
  /// Used as primary key and for diagnostics.
  Arcadia_String* name;
  /// The actual path of the file.
  Arcadia_FilePath* path;
  Arcadia_SizeValue hashValue;
  Arcadia_ByteArray* contents;
  Arcadia_Languages_LineMap* lineMap;
};

Arcadia_Languages_PhysicalInputFile*
Arcadia_Languages_PhysicalInputFile_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name,
    Arcadia_FilePath* path
  );

#endif // ARCADIA_LANGUAGES_PHYSICALINPUTFILE_H_INCLUDED
