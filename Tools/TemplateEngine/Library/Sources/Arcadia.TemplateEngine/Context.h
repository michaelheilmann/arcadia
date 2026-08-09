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

#if !defined(ARCADIA_TEMPLATEENGINE_CONTEXT_H_INCLUDED)
#define ARCADIA_TEMPLATEENGINE_CONTEXT_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/FileSystem/Include.h"
#include "Arcadia/Languages/Include.h"
typedef struct Environment Environment;
typedef struct DependenciesContext DependenciesContext;

Arcadia_declareObjectType(u8"Arcadia.TemplateEngine.Context", Context,
                          u8"Arcadia.Object");

struct ContextDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Context {
  Arcadia_Object _parent;

  Arcadia_Unicode_Encoder* target;
  Arcadia_ByteArrayBuilder* targetBuffer;

  Arcadia_Unicode_Encoder* temporary;
  Arcadia_ByteArrayBuilder* temporaryBuffer;

  /// The source file path.
  Arcadia_FilePath* sourceFilePath;
  /// The target file path.
  Arcadia_FilePath* targetFilePath;
  /// The environment file path.
  Arcadia_FilePath* environmentFilePath;

  Arcadia_FilePath* dependenciesFilePath;
  Arcadia_FilePath* logFilePath;


  Environment* environment;

  Arcadia_Stack* stack;

  /// The include graph.
  /// For example, if we start at X and X includes first A and second B.
  /// Then when processing B this list contains X, B.
  Arcadia_List* files;

  DependenciesContext* dependenciesContext;

  /// The diagnostics.
  Arcadia_Languages_Diagnostics* diagnostics;

  /// The console log.
  Arcadia_Log* consoleLog;
};

Context*
Context_create
  (
    Arcadia_Thread* thread
  );

void
Context_onRunInner
  (
    Arcadia_Thread* thread,
    Context* self,
    Arcadia_FilePath* includingFile
  );

void
Context_onRun
  (
    Arcadia_Thread* thread,
    Context* self
  );

#endif // ARCADIA_TEMPLATEENGINE_CONTEXT_H_INCLUDED
