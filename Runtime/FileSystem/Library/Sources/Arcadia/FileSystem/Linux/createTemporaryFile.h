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

#if !defined(ARCADIA_FILESYSTEM_LINUX_CREATETEMPORARYFILE_H_INCLUDED)
#define ARCADIA_FILESYSTEM_LINUX_CREATETEMPORARYFILE_H_INCLUDED

#if !defined(ARCADIA_FILESYSTEM_MODULE)
  #error("do not include directly, include `Arcadia/FileSystem/Include.h` instead")
#endif

#include "Arcadia/Ring2/Include.h"
typedef struct Arcadia_FilePath Arcadia_FilePath;
typedef struct Arcadia_DefaultFileSystem Arcadia_DefaultFileSystem;

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Linux || \
    Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Cygwin

/// @brief Create a temporary file and return the path to that file.
/// @param thread A pointer to this thread.
/// @param self A pointer to this default file system.
/// @return The path.
Arcadia_FilePath*
Arcadia_DefaultFileSystem_createTemporaryFile
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  );

#endif

#endif // ARCADIA_FILESYSTEM_LINUX_CREATETEMPORARYFILE_H_INCLUDED
