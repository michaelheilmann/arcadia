# Runtime/FileSystem Notes

## Module Shape

- `Runtime/FileSystem` builds the `Arcadia.FileSystem` library and has three test suites: `FilePathTests`, `DirectoryIteratorTests`, and `FileContentsTests`.
- Public API lives under `Library/Sources/Arcadia/FileSystem`, especially `FileSystem.h`, `FilePath.h`, `FileHandle.h`, and `DirectoryIterator.h`.
- Default implementations live in `Library/Sources/Arcadia/FileSystem/Implementation`.
- Platform-specific helpers live in `Library/Sources/Arcadia/FileSystem/Linux` and `Library/Sources/Arcadia/FileSystem/Windows`.
- The library depends on `Arcadia.Collections` and `Arcadia.Ring2`.
- Source and header files are explicitly registered in `Runtime/FileSystem/Library/CMakeLists.txt`; update it when adding files.

## Tests

- `Runtime/FileSystem/Tests/CMakeLists.txt` includes `FilePathTests`, `DirectoryIteratorTests`, and `FileContentsTests`.
- Existing tests cover file path parsing/conversion, directory iteration assets, and temporary-file contents/lifecycle.
- FilePathTests exposes `Normalization` and `ParentTraversal` through Arcadia.Testing; DirectoryIteratorTests exposes `Directory1` and `Directory2` over the existing asset directories. Omit arguments to run both in their original order, or use `--case NAME` to run one. Each case retains its safeExecute/process boundary; CTest discovers individual `Executable.Case` registrations through `--list`, retaining RUN_SERIAL and the suite's binary working directory. The former whole-suite selectors for these two suites are retired.
- FileContentsTests exercises the native temporary-file API and overwrite/read/delete sequence. CTest gives it a unique build-tree working directory and TMP/TEMP/TMPDIR per case/configuration/session; this audited suite opts in to parallel execution. DirectoryIteratorTests and the remaining FileSystem suites retain serial scheduling. See Runtime/Testing/Isolation.md before changing output paths or scheduling.

## CMake Notes

- `Configure.h` is generated from `Library/Sources/Arcadia/FileSystem/Configure.h.i` into the binary `Sources` tree.
- Platform files are conditionally included by `${this}_OperatingSystem` in `Library/CMakeLists.txt`.
- Windows-only files are under `Arcadia/FileSystem/Windows`; Linux-only files are under `Arcadia/FileSystem/Linux`.

## Runtime Conventions

- Files in this module define `ARCADIA_FILESYSTEM_MODULE` before including module headers.
- Public module users include `Arcadia/FileSystem/Include.h`; many internal headers reject direct inclusion unless `ARCADIA_FILESYSTEM_MODULE` is defined.
- Follow the shared error-handling and jump-target cleanup conventions in `AGENTS/CxxSourceCodeStyle.md`.
- Use existing status values consistently: allocation failures normally use `Arcadia_Status_AllocationFailed`; OS/API failures normally use `Arcadia_Status_OperationFailed` or, for environment lookup failures, `Arcadia_Status_EnvironmentFailed`.
- Preserve AGPL-3.0-or-later headers on new C, header, and CMake files as described in `AGENTS/CxxSourceCodeStyle.md`.

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
