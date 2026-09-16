# Runtime/FileSystem Notes

## Module Shape

- `Runtime/FileSystem` builds the `Arcadia.FileSystem` library and currently has tests for file paths and directory iterators.
- Public API lives under `Library/Sources/Arcadia/FileSystem`, especially `FileSystem.h`, `FilePath.h`, `FileHandle.h`, and `DirectoryIterator.h`.
- Default implementations live in `Library/Sources/Arcadia/FileSystem/Implementation`.
- Platform-specific helpers live in `Library/Sources/Arcadia/FileSystem/Linux` and `Library/Sources/Arcadia/FileSystem/Windows`.
- The library depends on `Arcadia.Collections` and `Arcadia.Ring2`.
- Source and header files are explicitly registered in `Runtime/FileSystem/Library/CMakeLists.txt`; update it when adding files.

## Tests

- `Runtime/FileSystem/Tests/CMakeLists.txt` includes `FilePathTests` and `DirectoryIteratorTests`.
- Existing tests cover file path parsing/conversion and directory iteration assets.
- There is no dedicated temporary-file test directory yet; if adding tests for temporary files, register a new test subdirectory in `Runtime/FileSystem/Tests/CMakeLists.txt` and follow the local test product pattern.

## CMake Notes

- `Configure.h` is generated from `Library/Sources/Arcadia/FileSystem/Configure.h.i` into the binary `Sources` tree.
- Platform files are conditionally included by `${this}_OperatingSystem` in `Library/CMakeLists.txt`.
- Windows-only files are under `Arcadia/FileSystem/Windows`; Linux-only files are under `Arcadia/FileSystem/Linux`.

## Runtime Conventions

- Files in this module define `ARCADIA_FILESYSTEM_MODULE` before including module headers.
- Public module users include `Arcadia/FileSystem/Include.h`; many internal headers reject direct inclusion unless `ARCADIA_FILESYSTEM_MODULE` is defined.
- Errors are reported through `Arcadia_Thread_setStatus(thread, ...)` and `Arcadia_Thread_jump(thread)`, not by returning error codes.
- Use `Arcadia_JumpTarget` cleanup guards around heap allocations or OS handles when subsequent Arcadia calls may jump.
- Use existing status values consistently: allocation failures normally use `Arcadia_Status_AllocationFailed`; OS/API failures normally use `Arcadia_Status_OperationFailed` or, for environment lookup failures, `Arcadia_Status_EnvironmentFailed`.
- Preserve AGPL-3.0-or-later headers on new C, header, and CMake files.

## FileSystem Dispatch

- `FileSystem.h` defines the virtual dispatch table for filesystem operations.
- `FileSystem.c` public functions are thin virtual-call wrappers, for example `Arcadia_FileSystem_createTemporaryFile` dispatches to `createTemporaryFile`.
- `Implementation/DefaultFileSystem.c` initializes `Arcadia_DefaultFileSystem` and wires function pointers in `Arcadia_DefaultFileSystem_initializeDispatchImpl`.
- Platform helper functions must be included and assigned there when they implement a virtual operation.

## File Paths

- `Arcadia_FilePath` stores a `root`, a `relative` flag, and a list of path components.
- Use `Arcadia_FilePath_parseNative`, `Arcadia_FilePath_parseWindows`, or `Arcadia_FilePath_parseUnix` instead of constructing paths manually when converting OS paths.
- Use `Arcadia_FilePath_toNative(thread, path, Arcadia_BooleanValue_True)` when passing paths to native OS APIs that need zero-terminated strings.
- Generic paths use `/`; native paths use platform separators in `Arcadia_FilePath_toNative`.

## Platform Notes

- Windows code uses Win32 APIs and usually converts wide strings to UTF-8 with `WideCharToMultiByte` before parsing into `Arcadia_FilePath`.
- Windows filesystem handles are kept in `Arcadia_DefaultFileSystem` for standard streams and use `HANDLE` APIs.
- Linux code uses POSIX APIs such as `mkdir`, `stat`, `getpwuid_r`, `getcwd`, `open`, `read`, `write`, and `close`.
- Temporary file creation is implemented in platform stubs: Linux uses `TMPDIR` or `/tmp` with `mkstemp`; Windows uses `GetTempPathW` and `GetTempFileNameW`.
