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

#if !defined(ARCADIA_MILC_CONTEXT_H_INCLUDED)
#define ARCADIA_MILC_CONTEXT_H_INCLUDED

#include "Arcadia/Languages/Include.h"
#include "Arcadia/FileSystem/Include.h"
typedef struct Arcadia_MILC_Scanner Arcadia_MILC_Scanner;
typedef struct Arcadia_MILC_Parser Arcadia_MILC_Parser;
typedef struct Arcadia_MILC_EnterPhase Arcadia_MILC_EnterPhase;
typedef struct Arcadia_MILC_MemberEnterPhase Arcadia_MILC_MemberEnterPhase;
typedef struct Arcadia_MILC_Completer Arcadia_MILC_Completer;
typedef struct Arcadia_MILC_Environment Arcadia_MILC_Environment;
typedef struct Arcadia_MILC_Backend_Implementation Arcadia_MILC_Backend_Implementation;

Arcadia_declareObjectType(u8"Arcadia.MILC.Context", Arcadia_MILC_Context,
                          Arcadia_Object);

struct Arcadia_MILC_ContextDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_MILC_Context {
  Arcadia_Object _parent;
  Arcadia_Languages_StringTable* stringTable;
  Arcadia_Log* log;
  Arcadia_Languages_Diagnostics* diagnostics;
  Arcadia_Languages_InputFileManager* inputFileManager;
  /// @brief The working directory.
  Arcadia_FilePath* workingDirectoryPath;
  /// @brief The list of module nodes (objects of Arcadia.MILC.AST.ModuleNode or derived type).
  Arcadia_List* moduleNodes;
  /// @brief The root scope.
  Arcadia_Languages_Scope* scope;
  /// @brief A map from symbols to their environments.
  Arcadia_Map* environments;
  Arcadia_Map* instances;
};

Arcadia_MILC_Context*
Arcadia_MILC_Context_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_MILC_CONTEXT_H_INCLUDED
