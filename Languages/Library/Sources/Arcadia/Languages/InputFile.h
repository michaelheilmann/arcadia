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

#if !defined(ARCADIA_LANGUAGES_INPUTFILE_H_INCLUDED)
#define ARCADIA_LANGUAGES_INPUTFILE_H_INCLUDED

#if !defined(ARCADIA_LANGUAGES_MODULE)
  #error("do not include directly, include `Arcadia/Languages/Include.h` instead")
#endif

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/FileSystem/Include.h"
#include "Arcadia/Languages/LineMap.h"

Arcadia_declareObjectType(u8"Arcadia.Languages.InputFile", Arcadia_Languages_InputFile,
                          u8"Arcadia.Object");

struct Arcadia_Languages_InputFileDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Languages_InputFile {
  Arcadia_Object _parent;
  Arcadia_FilePath* path;
  Arcadia_SizeValue hashValue;
  Arcadia_ByteArray* contents;
  Arcadia_Languages_LineMap* lineMap;
};

Arcadia_Languages_InputFile*
Arcadia_Languages_InputFile_create
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* path
  );

Arcadia_FilePath*
Arcadia_Languages_InputFile_getPath
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_InputFile* self
  );


Arcadia_ByteArray*
Arcadia_Languages_InputFile_getContents
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_InputFile* self
  );

Arcadia_SizeValue
Arcadia_Languages_InputFile_getLine
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_InputFile* self,
    Arcadia_SizeValue offset
  );

#endif // ARCADIA_LANGUAGES_INPUTFILE_H_INCLUDED
