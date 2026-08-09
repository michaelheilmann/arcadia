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

#if !defined(ARCADIA_TEMPLATEENGINE_DEPENDENCIESCONTEXT_H_INCLUDED)
#define ARCADIA_TEMPLATEENGINE_DEPENDENCIESCONTEXT_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/FileSystem/Include.h"

Arcadia_declareObjectType(u8"Arcadia.TemplateEngine.DependenciesContext", DependenciesContext,
                          u8"Arcadia.Object");

struct DependenciesContextDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct DependenciesContext {
  Arcadia_Object _parent;
  // The path of the dependencies file.
  Arcadia_FilePath* dependenciesFilePath;
  // Map from absolute file name strings to a set list of absolute file name strings
  Arcadia_Map* dependencies;
};

DependenciesContext*
DependenciesContext_create
  (
    Arcadia_Thread* thread
  );

void
DependenciesContext_write
  (
    Arcadia_Thread* thread,
    DependenciesContext* self
  );

/* Invoke this if a source file was discovered. This may be invoked multiple times for the same file. */
void
DependencyContext_onFile
  (
    Arcadia_Thread* thread,
    DependenciesContext* self,
    Arcadia_FilePath* path
  );

/* Invoke this if a dependency froom a file "from" to a file "to" was discovered. This may be invoked multiple times for the same combination of files. */
void
DependencyContext_onDependency
  (
    Arcadia_Thread* thread,
    DependenciesContext* self,
    Arcadia_FilePath* from,
    Arcadia_FilePath* to
  );

#endif // ARCADIA_TEMPLATEENGINE_DEPENDENCIESCONTEXT_H_INCLUDED
