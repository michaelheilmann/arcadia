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

#if !defined(ARCADIA_MILC_BACKEND_IMPLEMENTATION_H_INCLUDED)
#define ARCADIA_MILC_BACKEND_IMPLEMENTATION_H_INCLUDED

#include "Arcadia/MILC/Context.h"
#include "Arcadia/MILC/Backend/SymbolWriter.h"

/// @brief The Cxx backend.
/// @warning The context object passed to the compilation task object is modified by each invocation of Arcadia.MILC.Backend.Implemetation.execute.
/// @remarks The Cxx backend operates over the root scope. Unlike other phases, it does not iterate over ASTs.
Arcadia_declareObjectType(u8"Arcadia.MILC.Backend.Implementation", Arcadia_MILC_Backend_Implementation,
                          u8"Arcadia.Object");

struct Arcadia_MILC_Backend_ImplementationDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_MILC_Backend_Implementation {
  Arcadia_Object _parent;
  Arcadia_MILC_Context* context;
  Arcadia_MILC_Backend_SymbolWriter* classSymbolWriter;
  Arcadia_MILC_Backend_SymbolWriter* enumerationSymbolWriter;

  /// @brief Map from addresses of symbols to Arcadia.MILC.Backend.SymbolInfo or derived type instances.
  Arcadia_Map* symbolInfos;
};

Arcadia_MILC_Backend_Implementation*
Arcadia_MILC_Backend_Implementation_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  );

Arcadia_MILC_Backend_Implementation*
Arcadia_MILC_Backend_Implementation_getInstance
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  );

void
Arcadia_MILC_Backend_Implementation_run
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self
  );

Arcadia_MILC_Backend_SymbolInfo*
Arcadia_MILC_Backend_Implementation_getOrCreateInfo
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self,
    Arcadia_MILC_Symbol* symbol
  );

/// Convert a name of the form `name ('.' name)*` to a path string by replacing the periods with forward slashes.
static inline Arcadia_String*
Arcadia_MILC_Backend_Implementation_nameToPathString
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self,
    Arcadia_String* string
  )
{
  // @todo Acquire a string builder from a list of string builders in the implementation.
  // Return that string builder after use.
  Arcadia_StringBuilder* sb = Arcadia_StringBuilder_create(thread);
  Arcadia_UnicodeCodePointReader* ucpr = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, string));
  while (Arcadia_UnicodeCodePointReader_hasValue(thread, ucpr)) {
    Arcadia_Natural32Value uc = Arcadia_UnicodeCodePointReader_getValue(thread, ucpr);
    if (uc == '.') {
      uc = '/';
    }
    Arcadia_StringBuilder_insertBackCodePoint(thread, sb, uc);
    Arcadia_UnicodeCodePointReader_nextValue(thread, ucpr);
  }
  return Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(sb));
}

// Compute the CXX name `name ('_' name)*` for a given MIL name `name ('.' name)*`.
static inline Arcadia_String*
Arcadia_MILC_Backend_Implementation_computeCxxName
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self,
    Arcadia_String* string
  )
{
  Arcadia_StringBuilder* sb = Arcadia_StringBuilder_create(thread);
  Arcadia_UnicodeCodePointReader* ucpr = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, string));
  while (Arcadia_UnicodeCodePointReader_hasValue(thread, ucpr)) {
    Arcadia_Natural32Value uc = Arcadia_UnicodeCodePointReader_getValue(thread, ucpr);
    if (uc == '.') {
      uc = '_';
    }
    Arcadia_StringBuilder_insertBackCodePoint(thread, sb, uc);
    Arcadia_UnicodeCodePointReader_nextValue(thread, ucpr);
  }
  return Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(sb));
}

// Compute the CXX uppercase name `name.uppercase ('_' name.uppercase)` for a given MIL name `name ('.'name)*`.
static inline Arcadia_String*
Arcadia_MILC_Backend_Implementation_computeCxxNameUpperCase
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self,
    Arcadia_String* string
  )
{
  Arcadia_StringBuilder* sb = Arcadia_StringBuilder_create(thread);
  Arcadia_UnicodeCodePointReader* ucpr = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, string));
  while (Arcadia_UnicodeCodePointReader_hasValue(thread, ucpr)) {
    Arcadia_Natural32Value uc = Arcadia_UnicodeCodePointReader_getValue(thread, ucpr);
    if (uc == '.') {
      uc = '_';
    }
    Arcadia_StringBuilder_insertBackCodePoint(thread, sb, uc);
    Arcadia_UnicodeCodePointReader_nextValue(thread, ucpr);
  }
  Arcadia_StringBuilder_toUpperASCII(thread, sb, NULL, NULL);
  return Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(sb));
}

// For modules: `<name as path>/Include.c`
// For clases, enumerations, procedures: `<name as path>.c`
static inline Arcadia_FilePath*
Arcadia_MILC_Backend_Implementation_makeCxxSourceFilePath
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* implementation,
    Arcadia_MILC_Symbol* symbol
  )
{
  if (symbol->kind == Arcadia_MILC_SymbolKind_Module) {
    Arcadia_FilePath* filePath = Arcadia_FilePath_parseGeneric(thread, Arcadia_MILC_Backend_Implementation_nameToPathString(thread, implementation, ((Arcadia_MILC_Symbol*)symbol)->name));
    Arcadia_List_insertBackObjectReferenceValue(thread, filePath->fileNames, (Arcadia_Object*)Arcadia_String_createFromCxxString(thread, u8"Include.c"));
    return filePath;
  } else {
    Arcadia_StringBuilder* stringBuilder = Arcadia_StringBuilder_create(thread);
    Arcadia_FilePath* filePath = Arcadia_FilePath_parseGeneric(thread, Arcadia_MILC_Backend_Implementation_nameToPathString(thread, implementation, ((Arcadia_MILC_Symbol*)symbol)->name));
    // Remove the file name part, append the extension to it and re-append it.
    Arcadia_String* fileName = Arcadia_List_getObjectReferenceValueAt(thread, filePath->fileNames, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)filePath->fileNames) - 1);
    Arcadia_List_removeBack(thread, filePath->fileNames, 1);
    Arcadia_StringBuilder_insertBackString(thread, stringBuilder, fileName);
    if (symbol->kind != Arcadia_MILC_SymbolKind_Enumeration) {
      // Currently a hack as we are merely able to produce proper ".c" files for enumerations.
      // All other types emit "c.g" in order to avoid overwriting the ".c" files with broken contents.
      Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ".c.g");
    } else {
      Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ".c");
    }
    Arcadia_List_insertBackObjectReferenceValue(thread, filePath->fileNames, (Arcadia_Object*)Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuilder)));
    return filePath;
  }
}

// For modules: `<name as path>/Include.h`
// For clases, enumerations, procedures: `<name as path>.h`
static inline Arcadia_FilePath*
Arcadia_MILC_Backend_Implementation_makeCxxHeaderFilePath
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* implementation,
    Arcadia_MILC_Symbol* symbol
  )
{
  if (symbol->kind == Arcadia_MILC_SymbolKind_Module) {
    Arcadia_FilePath* filePath = Arcadia_FilePath_parseGeneric(thread, Arcadia_MILC_Backend_Implementation_nameToPathString(thread, implementation, ((Arcadia_MILC_Symbol*)symbol)->name));
    Arcadia_List_insertBackObjectReferenceValue(thread, filePath->fileNames, (Arcadia_Object*)Arcadia_String_createFromCxxString(thread, u8"Include.h"));
    return filePath;
  } else {
    Arcadia_StringBuilder* stringBuilder = Arcadia_StringBuilder_create(thread);
    Arcadia_FilePath* filePath = Arcadia_FilePath_parseGeneric(thread, Arcadia_MILC_Backend_Implementation_nameToPathString(thread, implementation, ((Arcadia_MILC_Symbol*)symbol)->name));
    // Remove the file name part, append the extension to it and re-append it.
    Arcadia_String* fileName = Arcadia_List_getObjectReferenceValueAt(thread, filePath->fileNames, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)filePath->fileNames) - 1);
    Arcadia_List_removeBack(thread, filePath->fileNames, 1);
    Arcadia_StringBuilder_insertBackString(thread, stringBuilder, fileName);
    if (symbol->kind != Arcadia_MILC_SymbolKind_Enumeration) {
      // Currently a hack as we are merely able to produce proper ".h" files for enumerations.
      // All other types emit "h.g" in order to avoid overwriting the ".h" files with broken contents.
      Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ".h.g");
    } else {
      Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ".h");
    }
    Arcadia_List_insertBackObjectReferenceValue(thread, filePath->fileNames, (Arcadia_Object*)Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuilder)));
    return filePath;
  }
}

/// @error #Arcadia_Status_ArgumentValueInvalid
/// the file path is not relative, the file path is empty
Arcadia_String*
Arcadia_MILC_Backend_Implementation_pathToCxxPath
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self,
    Arcadia_FilePath* path
  );

#endif // ARCADIA_MILC_BACKEND_IMPLEMENTATION_H_INCLUDED
