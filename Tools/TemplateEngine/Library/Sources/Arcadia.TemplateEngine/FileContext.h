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

#if !defined(ARCADIA_TEMPLATEENGINE_FILECONTEXT_H_INCLUDED)
#define ARCADIA_TEMPLATEENGINE_FILECONTEXT_H_INCLUDED

#include "Arcadia.TemplateEngine/Parser/Parser.h"
#include "Arcadia.TemplateEngine/Scanner/Scanner.h"
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/FileSystem/Include.h"
typedef struct Context Context;
typedef struct Environment Environment;

// The read-evaluate procedure parses a file into a tree
// @code
// file : (text | directive)*
// @endcode
// It then iterates over the file's tree children from left to right
// text trees is directly written to an output buffer
// directive trees directives are evaluated.
Arcadia_declareObjectType(u8"Arcadia.TemplateEngine.FileContext", FileContext,
                          u8"Arcadia.Object");

struct FileContextDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct FileContext {
  Arcadia_Object _parent;

  /// The underlaying context.
  Context* context;
  /// The path to the including file.
  Arcadia_FilePath* includingFilePath;
  /// The path to the included file.
  Arcadia_FilePath* includedFilePath;

  /// The Bytes of this file.
  Arcadia_ByteArray* fileBytes;

  /// Unicode decoder for the included file.
  Arcadia_UnicodeCodePointReader* reader;
  /// The environment.
  Environment* environment;

  /// The parser.
  Arcadia_TemplateEngine_Parser* parser;
  /// The scanner.
  Arcadia_TemplateEngine_Scanner* scanner;

};

FileContext*
FileContext_create
  (
    Arcadia_Thread* thread,
    Context* context,
    Environment* environment,
    Arcadia_FilePath* includingFilePath,
    Arcadia_FilePath* includedFilePath
  );

void
FileContext_execute
  (
    Arcadia_Thread* thread,
    FileContext* context
  );

#endif // ARCADIA_TEMPLATEENGINE_FILECONTEXT_H_INCLUDED
