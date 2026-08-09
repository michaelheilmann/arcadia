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

#if !defined(ARCADIA_LANGUAGES_LEXICALERROREXCEPTION_H_INCLUDED)
#define ARCADIA_LANGUAGES_LEXICALERROREXCEPTION_H_INCLUDED

#if !defined(ARCADIA_LANGUAGES_MODULE)
  #error("do not include directly, include `Arcadia/Languages/Include.h` instead")
#endif

#include "Arcadia/Ring2/Include.h"

Arcadia_declareObjectType(u8"Arcadia.Languages.LexicalErrorException", Arcadia_Languages_LexicalErrorException,
                          u8"Arcadia.Exception");

struct Arcadia_Languages_LexicalErrorExceptionDispatch {
  Arcadia_ExceptionDispatch _parent;
};

struct Arcadia_Languages_LexicalErrorException {
  Arcadia_Exception _parent;
};

Arcadia_Languages_LexicalErrorException*
Arcadia_Languages_LexicalErrorException_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_LANGUAGES_LEXICALERROREXCEPTION_H_INCLUDED
