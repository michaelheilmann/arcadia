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

#if !defined(ARCADIA_FILESYSTEM_FILESYSTEM_H_INCLUDED)
#define ARCADIA_FILESYSTEM_FILESYSTEM_H_INCLUDED

#if !defined(ARCADIA_FILESYSTEM_MODULE)
  #error("do not include directly, include `Arcadia/FileSystem/Include.h` instead")
#endif

#include "Arcadia/FileSystem/DirectoryIterator.h"
#include "Arcadia/FileSystem/FileHandle.h"
#include "Arcadia/FileSystem/FilePath.h"
#include "Arcadia/FileSystem/FileType.h"

/// @code
/// class FileSystem {
///   constructor()
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.FileSystem", Arcadia_FileSystem,
                          u8"Arcadia.Object");

struct Arcadia_FileSystemDispatch {
  Arcadia_ObjectDispatch _parent;

  void
  (*createDirectoryFile)
    (
      Arcadia_Thread* thread,
      Arcadia_FileSystem* self,
      Arcadia_FilePath* path
    );

  Arcadia_DirectoryIterator*
  (*createDirectoryIterator)
    (
      Arcadia_Thread* thread,
      Arcadia_FileSystem* self,
      Arcadia_FilePath* path
    );

  Arcadia_FileHandle*
  (*createFileHandle)
    (
      Arcadia_Thread* thread,
      Arcadia_FileSystem* self
    );

  void
  (*createRegularFile)
    (
      Arcadia_Thread* thread,
      Arcadia_FileSystem* self,
      Arcadia_FilePath* path
    );

  void
  (*deleteDirectoryFile)
    (
      Arcadia_Thread* thread,
      Arcadia_FileSystem* self,
      Arcadia_FilePath* path
    );

  void
  (*deleteFile)
    (
      Arcadia_Thread* thread,
      Arcadia_FileSystem* self,
      Arcadia_FilePath* path
    );

  void
  (*deleteRegularFile)
    (
      Arcadia_Thread* thread,
      Arcadia_FileSystem* self,
      Arcadia_FilePath* path
    );

  Arcadia_BooleanValue
  (*directoryFileExists)
    (
      Arcadia_Thread* thread,
      Arcadia_FileSystem* self,
      Arcadia_FilePath* path
    );

  Arcadia_FilePath*
  (*getConfigurationDirectory)
    (
      Arcadia_Thread* thread,
      Arcadia_FileSystem* self
    );

  Arcadia_FilePath*
  (*getExecutable)
    (
      Arcadia_Thread* thread,
      Arcadia_FileSystem* self
    );

  Arcadia_ByteArrayBuilder*
  (*getFileContents)
    (
      Arcadia_Thread* thread,
      Arcadia_FileSystem* self,
      Arcadia_FilePath* path
    );

  Arcadia_FileType
  (*getFileType)
    (
      Arcadia_Thread* thread,
      Arcadia_FileSystem* self,
      Arcadia_FilePath* path
    );

  Arcadia_Natural64Value
  (*getLastWriteTime)
    (
      Arcadia_Thread* thread,
      Arcadia_FileSystem* self,
      Arcadia_FilePath* path
    );

  Arcadia_FilePath*
  (*getSaveDirectory)
    (
      Arcadia_Thread* thread,
      Arcadia_FileSystem* self
    );

  Arcadia_FilePath*
  (*getWorkingDirectory)
    (
      Arcadia_Thread* thread,
      Arcadia_FileSystem* self
    );

  Arcadia_BooleanValue
  (*regularFileExists)
    (
      Arcadia_Thread* thread,
      Arcadia_FileSystem* self,
      Arcadia_FilePath* path
    );

  void
  (*setFileContents)
    (
      Arcadia_Thread* thread,
      Arcadia_FileSystem* self,
      Arcadia_FilePath* path,
      Arcadia_ByteArrayBuilder * contents
    );

};

struct Arcadia_FileSystem {
  Arcadia_Object _parent;
};

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileSystem_createDirectoryFile
// Arcadia_Status_OperationFailed if the file exists but is not a directory file or creation failed
void
Arcadia_FileSystem_createDirectoryFile
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileSystem_createDirectoryIterator
Arcadia_DirectoryIterator*
Arcadia_FileSystem_createDirectoryIterator
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  );

Arcadia_FileHandle*
Arcadia_FileSystem_createFileHandle
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileSystem_createRegularFile
/// @brief Create a regular file.
/// @param thread A pointer to this thread.
/// @param self A pointer to this file system.
/// @param path A pointer to the path.
/// @error Arcadia_Status_NotFound
/// one or more intermediate directories do not exist; this function will only create the final directory in the path.
void
Arcadia_FileSystem_createRegularFile
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileSystem_deleteDirectoryFile
void
Arcadia_FileSystem_deleteDirectoryFile
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileSystem_deleteFile
void
Arcadia_FileSystem_deleteFile
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileSystem_deleteRegularFile
void
Arcadia_FileSystem_deleteRegularFile
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileSystem_directoryFileExists
Arcadia_BooleanValue
Arcadia_FileSystem_directoryFileExists
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  );

// https://michaelheilmann.com/Arcadia/Ring2/Arcadia_FileSystem_getConfigurationDirectory
Arcadia_FilePath*
Arcadia_FileSystem_getConfigurationDirectory
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self
  );

/// @brief Get the path of the executing executable.
/// @param thread A pointer to this thread.
/// @param self A pointer to this file system.
/// @return A pointer to the executable path.
/// @success The caller acquired a reference to the string.
Arcadia_FilePath*
Arcadia_FileSystem_getExecutable
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileSystem_getFileContents
Arcadia_ByteArrayBuilder*
Arcadia_FileSystem_getFileContents
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileSystem_getFileType
Arcadia_FileType
Arcadia_FileSystem_getFileType
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileSystem_getLastWriteTime
Arcadia_Natural64Value
Arcadia_FileSystem_getLastWriteTime
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  );

// https://michaelheilmann.com/Arcadia/Ring2/Arcadia_FileSystem_getSaveDirectory
Arcadia_FilePath*
Arcadia_FileSystem_getSaveDirectory
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/Arcadia_FileSystem_getWorkingDirectory
Arcadia_FilePath*
Arcadia_FileSystem_getWorkingDirectory
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileSystem_regularFileExists
Arcadia_BooleanValue
Arcadia_FileSystem_regularFileExists
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileSystem_setFileContents
void
Arcadia_FileSystem_setFileContents
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path,
    Arcadia_ByteArrayBuilder * contents
  );

// https://michaelheilmann.com/Arcadia/Ring2/Arcadia_FileSystem_getOrCreate
Arcadia_FileSystem*
Arcadia_FileSystem_getOrCreate
  (
    Arcadia_Thread* thread
  );

void
Arcadia_FileSystem_createDirectoryFiles
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  );

#endif // ARCADIA_FILESYSTEM_FILESYSTEM_H_INCLUDED
