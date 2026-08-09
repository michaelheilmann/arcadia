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

#define ARCADIA_LANGUAGES_MODULE (1)
#include "Arcadia/Languages/Parser.h"

static void
Arcadia_Languages_Parser_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Parser* self
  );

static void
Arcadia_Languages_Parser_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_ParserDispatch* self
  );

static void
Arcadia_Languages_Parser_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Parser* self
  );

static void
Arcadia_Languages_Parser_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Parser* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Languages_Parser_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Languages_Parser_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Languages_Parser_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Languages_Parser_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Languages.Parser", Arcadia_Languages_Parser,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Languages_Parser_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Parser* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Languages_Parser);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Languages_Parser);
}

static void
Arcadia_Languages_Parser_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_ParserDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Languages_Parser_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Parser* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Languages_Parser_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Parser* self
  )
{/*Intentionally empty.*/}

Arcadia_Languages_Diagnostics*
Arcadia_Languages_Parser_getDiagnostics
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Parser* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Languages_Parser, getDiagnostics, self); }

Arcadia_UnicodeCodePointReader*
Arcadia_Languages_Parser_getInput
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Parser* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Languages_Parser, getInput, self); }

Arcadia_Languages_StringTable*
Arcadia_Languages_Parser_getStringTable
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Parser* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Languages_Parser, getStringTable, self); }

Arcadia_Value
Arcadia_Languages_Parser_run
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Parser* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Languages_Parser, run, self); }

void
Arcadia_Languages_Parser_setInput
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Parser* self,
    Arcadia_UnicodeCodePointReader* input
  )
{ Arcadia_VirtualCall(Arcadia_Languages_Parser, setInput, self, input); }
