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

#if !defined(ARCADIA_RING2_COMMANDLINE_INCLUDE_H_INCLUDED)
#define ARCADIA_RING2_COMMANDLINE_INCLUDE_H_INCLUDED

#if !defined(ARCADIA_RING2_MODULE)
  #error("do not include directly, include `Arcadia/Ring2/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Ring2/Logging/Include.h"
#include "Arcadia/Ring2/Unicode/Include.h"
#include "Arcadia/Ring2/Strings/Include.h"
#include "Arcadia/Ring2/CommandLine/CommandLineArgument.h"

// A command-line argument is of the form
// @code
// <argument> : '--'<name>('='<value>)?
// <name> : a name as defined in the Common Lexical Specification at https://michaelheilmann.com/specifications/common-lexical-specification
// <value> : a string as defined in the Common Lexical Specification at https://michaelheilmann.com/specifications/common-lexical-specification
// @endcode
// @return A pointer to a Arcadia_CommandLineArgument is returned.
Arcadia_CommandLineArgument*
Arcadia_CommandLine_parseArgument
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReader* reader
  );

void
Arcadia_CommandLine_raiseRequiredArgumentMissingError
  (
    Arcadia_Thread* thread,
    Arcadia_String* key,
    Arcadia_Log* log
  );

void
Arcadia_CommandLine_raiseUnknownArgumentError
  (
    Arcadia_Thread* thread,
    Arcadia_String* key,
    Arcadia_String* value,
    Arcadia_Log* log
  );

void
Arcadia_CommandLine_raiseNoValueError
  (
    Arcadia_Thread* thread,
    Arcadia_String* key,
    Arcadia_Log* log
  );

void
Arcadia_CommandLine_raiseValueInvalidError
  (
    Arcadia_Thread* thread,
    Arcadia_String* key,
    Arcadia_String* value,
    Arcadia_Log* log
  );

void
Arcadia_CommandLine_raiseAlreadySpecifiedError
  (
    Arcadia_Thread* thread,
    Arcadia_String* key,
    Arcadia_Log* log
  );

void
Arcadia_CommandLine_invalidCommandLineArgumentError
  (
    Arcadia_Thread* thread,
    Arcadia_String* argument,
    Arcadia_Log* log
  );

#endif // ARCADIA_RING2_COMMANDLINE_INCLUDE_H_INCLUDED
