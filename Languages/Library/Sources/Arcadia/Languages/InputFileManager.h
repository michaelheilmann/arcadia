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

#if !defined(ARCADIA_LANGUAGES_INPUTFILEMANAGER_H_INCLUDED)
#define ARCADIA_LANGUAGES_INPUTFILEMANAGER_H_INCLUDED

#include "Arcadia/FileSystem/Include.h"
#include "Arcadia/Languages/Diagnostics.h"
typedef struct Arcadia_Languages_InputFile Arcadia_Languages_InputFile;

/// Input files are accessed via this input file manager.
Arcadia_declareObjectType(u8"Arcadia.Languages.InputFileManager", Arcadia_Languages_InputFileManager,
                          u8"Arcadia.Object");
                          
struct Arcadia_Languages_InputFileManagerDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Languages_InputFileManager {
  Arcadia_Object _parent;
  Arcadia_Map* files;
  Arcadia_Languages_Diagnostics* diagnostics;
};

Arcadia_Languages_InputFileManager*
Arcadia_Languages_InputFileManager_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics
  );

/// @brief Create a pyhsical input file.
/// @param thread A pointer to this thread.
/// @param self A pointer to this input file manager.
/// @param diagnosticName The name of the input file as used in diagnostics.
/// @param path The path of the file.
/// @return The input file.
Arcadia_Languages_InputFile*
Arcadia_Languages_InputFileManager_createPhysicalInputFile
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_InputFileManager* self,
    Arcadia_String* diagnosticName,
    Arcadia_FilePath* path
  ); 

Arcadia_UnicodeCodePointReader*
Arcadia_Languages_InputFileManager_getFileReader
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_InputFileManager* self,
    Arcadia_FilePath* path
  );

#endif // ARCADIA_LANGUAGES_INPUTFILEMANAGER_H_INCLUDED
