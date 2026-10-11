// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(ARCADIA_FILESYSTEM_IMPLEMENTATION_GETFILETYPE_H_INCLUDED)
#define ARCADIA_FILESYSTEM_IMPLEMENTATION_GETFILETYPE_H_INCLUDED

#if !defined(ARCADIA_FILESYSTEM_MODULE)
  #error("do not include directly, include `Arcadia/FileSystem/Include.h` instead")
#endif

#include "Arcadia/FileSystem/FileType.h"
#include "Arcadia/FileSystem/FilePath.h"
typedef struct Arcadia_DefaultFileSystem Arcadia_DefaultFileSystem;

/// @brief Get the type of a file.
/// @return The file type. #Arcadia_FileType_Unknown if the file was accessible but its not of a known type.
/// @error Arcadia_Status_FileNotFound the file was not found
/// @error Arcadia_Status_ArgumentValueInvalid @a path is a null pointer
Arcadia_FileType
Arcadia_DefaultFileSystem_getFileTypeHelper
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  );

#endif // ARCADIA_FILESYSTEM_IMPLEMENTATION_GETFILETYPE_H_INCLUDED
