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

#if !defined(ARCADIA_MILC_COMPILATIONFAILEDEXCEPTION_H_INCLUDED)
#define ARCADIA_MILC_COMPILATIONFAILEDEXCEPTION_H_INCLUDED

#include "Arcadia/MILC/Context.h"

/// @brief Represents a compilation task.
/// @warning The context object passed to the compilation task object is modified by each invocation of Arcadia.MILC.CompilationFailedError.execute.
Arcadia_declareObjectType(u8"Arcadia.MILC.CompilationFailedException", Arcadia_MILC_CompilationFailedException,
                          u8"Arcadia.Exception");

struct Arcadia_MILC_CompilationFailedExceptionDispatch {
  Arcadia_ExceptionDispatch _parent;
};

struct Arcadia_MILC_CompilationFailedException {
  Arcadia_Exception _parent;
};

Arcadia_MILC_CompilationFailedException*
Arcadia_MILC_CompilationFailedException_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_MILC_COMPILATIONFAILEDEXCEPTION_H_INCLUDED
