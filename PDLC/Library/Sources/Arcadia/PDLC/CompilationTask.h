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

#if !defined(ARCADIA_PDLC_COMPILATIONTASK_H_INCLUDED)
#define ARCADIA_PDLC_COMPILATIONTASK_H_INCLUDED

#include "Arcadia/PDLC/Context.h"

/// @brief Represents a compilation task.
/// @warning The context object passed to the compilation task object is modified by each invocation of Arcadia.PDLC.CompilationTask.execute.
Arcadia_declareObjectType(u8"Arcadia.PDLC.CompilationTask", Arcadia_PDLC_CompilationTask,
                          u8"Arcadia.Object");

struct Arcadia_PDLC_CompilationTaskDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_PDLC_CompilationTask {
  Arcadia_Object _parent;
  /// @brief The context.
  Arcadia_PDLC_Context* context;
  /// @brief The log.
  Arcadia_Log* log;
};

Arcadia_PDLC_CompilationTask*
Arcadia_PDLC_CompilationTask_create
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  );

void
Arcadia_PDLC_CompilationTask_run
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_CompilationTask* self,
    Arcadia_FilePath* workingDirectoryPath,
    Arcadia_List* moduleDirectoryPaths
  );

#endif // ARCADIA_PDLC_COMPILATIONTASK_H_INCLUDED
