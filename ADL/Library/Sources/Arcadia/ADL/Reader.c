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

#define ARCADIA_ADL_MODULE (1)
#include "Arcadia/ADL/Reader.h"

#include "Arcadia/ADL/Reader.module.h"

static void
Arcadia_ADL_Reader_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reader* self
  );

static void
Arcadia_ADL_Reader_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ReaderDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*) & Arcadia_ADL_Reader_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_Reader_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.Reader", Arcadia_ADL_Reader,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_ADL_Reader_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reader* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_Reader);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_ADL_Reader);
}

static void
Arcadia_ADL_Reader_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ReaderDispatch* self
  )
{
}

Arcadia_String*
Arcadia_ADL_Reader_getTypeName
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reader* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_ADL_Reader, getTypeName, self); }

Arcadia_ADL_Definition*
Arcadia_ADL_Reader_read
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reader* self,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_DDL_Node* input
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_ADL_Reader, read, self, context, definitions, input); }
