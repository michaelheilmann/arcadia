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

#define ARCADIA_DDL_MODULE (1)
#include "Arcadia/DDL/Reader/DefaultReader.h"

#include "Arcadia/Logging/Include.h"
#include "Arcadia/DDL/Include.h"

#include "Arcadia/DDL/Nodes/Include.h"

static void
Arcadia_DDL_DefaultReader_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_DefaultReader* self
  );

static void
Arcadia_DDL_DefaultReader_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_DefaultReaderDispatch* self
  );

static void
Arcadia_DDL_DefaultReader_visit
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_DefaultReader* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDL_DefaultReader_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDL_DefaultReader_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDL_DefaultReader_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDL.DefaultReader", Arcadia_DDL_DefaultReader,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_DDL_DefaultReader_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_DefaultReader* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDL_DefaultReader);
  //
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  self->parser = Arcadia_DDL_Parser_create(thread, Arcadia_DDL_Scanner_create(thread, Arcadia_Languages_StringTable_getOrCreate(thread),
                                                                                      Arcadia_Languages_Diagnostics_create(thread, (Arcadia_Log*)Arcadia_ConsoleLog_create(thread))));
  //
  Arcadia_LeaveConstructor(Arcadia_DDL_DefaultReader);
}

static void
Arcadia_DDL_DefaultReader_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_DefaultReaderDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_DDL_DefaultReader_visit
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_DefaultReader* self
  )
{
  if (self->parser) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->parser);
  }
}

Arcadia_DDL_DefaultReader*
Arcadia_DDL_DefaultReader_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_DefaultReader);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_DDL_DefaultReader);
}

Arcadia_DDL_Node*
Arcadia_DDL_DefaultReader_run
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_DefaultReader* self,
    Arcadia_UnicodeCodePointReader* input
  )
{
  Arcadia_Languages_Parser_setInput(thread, (Arcadia_Languages_Parser*)self->parser, input);
  Arcadia_DDL_Node* node = (Arcadia_DDL_Node*)Arcadia_Value_getObjectReferenceValueChecked(thread, Arcadia_Languages_Parser_run(thread, (Arcadia_Languages_Parser*)self->parser), _Arcadia_DDL_Node_getType(thread));
  return node;
}
