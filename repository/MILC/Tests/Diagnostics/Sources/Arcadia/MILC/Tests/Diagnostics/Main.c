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

#include "Arcadia/MILC/Include.h"

#include <string.h>
#include "Arcadia/Logging/Include.h"

static void
diagnostics
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Languages_InputFile* file = NULL;

  file = Arcadia_Languages_InputFile_create(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/MyConfiguration.ddl")));
  Arcadia_MILC_Diagnostics_ConfigurationFileInvalidDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, file);
  Arcadia_MILC_Diagnostics_ConfigurationFileInvalidDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Warning, file);
  Arcadia_MILC_Diagnostics_ConfigurationFileInvalidDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Information, file);

  file = Arcadia_Languages_InputFile_create(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/MyClass.mil")));
  Arcadia_MILC_Diagnostics_CyclicInheritanceDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, file, 0, Arcadia_String_createFromCxxString(thread, u8"MyClass"));
  Arcadia_MILC_Diagnostics_CyclicInheritanceDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Warning, file, 0, Arcadia_String_createFromCxxString(thread, u8"MyClass"));
  Arcadia_MILC_Diagnostics_CyclicInheritanceDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Information, file, 0, Arcadia_String_createFromCxxString(thread, u8"MyClass"));

  file = Arcadia_Languages_InputFile_create(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/MyClass.mil")));
  Arcadia_MILC_Diagnostics_FileNotFoundDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, file, Arcadia_MILC_FileType_ModuleDirectory);
  Arcadia_MILC_Diagnostics_FileNotFoundDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Warning, file,Arcadia_MILC_FileType_ModuleDirectory);
  Arcadia_MILC_Diagnostics_FileNotFoundDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Information, file, Arcadia_MILC_FileType_ModuleDirectory);

  {
    file = Arcadia_Languages_InputFile_create(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/MyEnumeration.mil")));
    Arcadia_MILC_EnumerationConstantSymbol* symbol =
      Arcadia_MILC_EnumerationConstantSymbol_create(thread, Arcadia_String_createFromCxxString(thread, u8"Clockwise"));
    Arcadia_String* value =
      Arcadia_String_createFromCxxString(thread, u8"0");
    Arcadia_MILC_Diagnostics_IntegerLiteralOutOfRangeDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, file, 0, symbol, value);
    Arcadia_MILC_Diagnostics_IntegerLiteralOutOfRangeDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Warning, file, 0, symbol, value);
    Arcadia_MILC_Diagnostics_IntegerLiteralOutOfRangeDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Information, file, 0, symbol, value);
  }

  {
    Arcadia_String* path = Arcadia_String_createFromCxxString(thread, u8"*.*");
    Arcadia_MILC_Diagnostics_InvalidPathDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, path);
    Arcadia_MILC_Diagnostics_InvalidPathDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Warning, path);
    Arcadia_MILC_Diagnostics_InvalidPathDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Information, path);
  }

  {
    file = Arcadia_Languages_InputFile_create(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/MyEnumeration.mil")));
    Arcadia_MILC_EnumerationConstantSymbol* symbol =
      Arcadia_MILC_EnumerationConstantSymbol_create(thread, Arcadia_String_createFromCxxString(thread, u8"Clockwise"));
    Arcadia_MILC_Diagnostics_MissingInitializerDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, file, 0, symbol);
    Arcadia_MILC_Diagnostics_MissingInitializerDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Warning, file, 0, symbol);
    Arcadia_MILC_Diagnostics_MissingInitializerDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Information, file, 0, symbol);
  }

  file = Arcadia_Languages_InputFile_create(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"MyModule/Module.mil")));
  Arcadia_MILC_Diagnostics_MissingModuleDefinitionDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, file);
  Arcadia_MILC_Diagnostics_MissingModuleDefinitionDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Warning, file);
  Arcadia_MILC_Diagnostics_MissingModuleDefinitionDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Information, file);

  file = Arcadia_Languages_InputFile_create(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"MyModule/Module.mil")));
  Arcadia_MILC_Diagnostics_MultipleModuleDefinitionsDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, file, Arcadia_SizeValue_Literal(0));
  Arcadia_MILC_Diagnostics_MultipleModuleDefinitionsDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Warning, file, Arcadia_SizeValue_Literal(0));
  Arcadia_MILC_Diagnostics_MultipleModuleDefinitionsDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Information, file, Arcadia_SizeValue_Literal(0));

  {
    file = Arcadia_Languages_InputFile_create(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"MyModule/MyProcedure.mil")));
    Arcadia_String* symbolName = Arcadia_String_createFromCxxString(thread, u8"MyProcedure");
    Arcadia_MILC_Diagnostics_SymbolIsAlreadyDefinedDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, file, Arcadia_SizeValue_Literal(0), symbolName);
    Arcadia_MILC_Diagnostics_SymbolIsAlreadyDefinedDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Warning, file, Arcadia_SizeValue_Literal(0), symbolName);
    Arcadia_MILC_Diagnostics_SymbolIsAlreadyDefinedDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Information, file, Arcadia_SizeValue_Literal(0), symbolName);
  }

  {
    file = Arcadia_Languages_InputFile_create(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"MyModule/MyProcedure.mil")));
    Arcadia_String* symbolName = Arcadia_String_createFromCxxString(thread, u8"MyProcedure");
    Arcadia_MILC_Diagnostics_SymbolIsNoClassDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, file, Arcadia_SizeValue_Literal(0), symbolName);
    Arcadia_MILC_Diagnostics_SymbolIsNoClassDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Warning, file, Arcadia_SizeValue_Literal(0), symbolName);
    Arcadia_MILC_Diagnostics_SymbolIsNoClassDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Information, file, Arcadia_SizeValue_Literal(0), symbolName);
  }
  {
    Arcadia_String* symbolName = Arcadia_String_createFromCxxString(thread, u8"MyClass");
    Arcadia_MILC_Diagnostics_SymbolIsNoMethodDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, file, Arcadia_SizeValue_Literal(0), symbolName);
    Arcadia_MILC_Diagnostics_SymbolIsNoMethodDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Warning, file, Arcadia_SizeValue_Literal(0), symbolName);
    Arcadia_MILC_Diagnostics_SymbolIsNoMethodDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Information, file, Arcadia_SizeValue_Literal(0), symbolName);
  }
  {
    Arcadia_String* symbolName = Arcadia_String_createFromCxxString(thread, u8"MyClass");
    Arcadia_MILC_Diagnostics_SymbolIsNotDefinedDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, file, Arcadia_SizeValue_Literal(0), symbolName);
    Arcadia_MILC_Diagnostics_SymbolIsNotDefinedDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Warning, file, Arcadia_SizeValue_Literal(0), symbolName);
    Arcadia_MILC_Diagnostics_SymbolIsNotDefinedDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Information, file, Arcadia_SizeValue_Literal(0), symbolName);
  }
}

int
main
  (
    int argc,
    char** argv
  )
{
  if (!Arcadia_Tests_safeExecute(&diagnostics)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
