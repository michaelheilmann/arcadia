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

#if !defined(ARCADIA_PDLC_CONTEXT_H_INCLUDED)
#define ARCADIA_PDLC_CONTEXT_H_INCLUDED

#include "Arcadia/Languages/Include.h"
#include "Arcadia/FileSystem/Include.h"
typedef struct Arcadia_PDLC_Scanner Arcadia_PDLC_Scanner;
typedef struct Arcadia_PDLC_Parser Arcadia_PDLC_Parser;
typedef struct Arcadia_PDLC_EnterPhase Arcadia_PDLC_EnterPhase;
typedef struct Arcadia_PDLC_MemberEnterPhase Arcadia_PDLC_MemberEnterPhase;
typedef struct Arcadia_PDLC_Completer Arcadia_PDLC_Completer;
typedef struct Arcadia_PDLC_Environment Arcadia_PDLC_Environment;
typedef struct Arcadia_PDLC_Backend_Implementation Arcadia_PDLC_Backend_Implementation;

Arcadia_declareObjectType(u8"Arcadia.PDLC.Context", Arcadia_PDLC_Context,
                          Arcadia_Object);

struct Arcadia_PDLC_ContextDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_PDLC_Context {
  Arcadia_Object _parent;
  Arcadia_Languages_StringTable* stringTable;
  Arcadia_Log* log;
  Arcadia_Languages_Diagnostics* diagnostics;
  Arcadia_Languages_InputFileManager* inputFileManager;
  /// @brief The working directory.
  Arcadia_FilePath* workingDirectoryPath;
  /// @brief The list of module nodes (objects of Arcadia.PDLC.AST.ModuleNode or derived type).
  Arcadia_List* moduleNodes;
  /// @brief The root declaration scope (types and procedures, never modules).
  Arcadia_Languages_Scope* scope;
  /// @brief Independent module-name lookup; it is not a lexical parent scope.
  Arcadia_Languages_Scope* moduleScope;
  /// @brief A map from symbols to their environments.
  Arcadia_Map* environments;
  Arcadia_Map* instances;
};

Arcadia_PDLC_Context*
Arcadia_PDLC_Context_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_PDLC_CONTEXT_H_INCLUDED
