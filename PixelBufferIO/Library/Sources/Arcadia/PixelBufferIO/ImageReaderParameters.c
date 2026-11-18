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

#include "Arcadia/PixelBufferIO/ImageReaderParameters.h"

struct Arcadia_Imaging_ImageReaderParametersDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_Imaging_ImageReaderParameters {
  Arcadia_Object _parent;
  Arcadia_String* format;
  Arcadia_ObjectReferenceValue object;
};

static void
Arcadia_Imaging_ImageReaderParameters_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReaderParameters* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Imaging_ImageReaderParameters);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Object* input = Arcadia_ValueStack_getObjectReferenceValue(thread, 2);
  if (!input ||
      (!Arcadia_Object_isInstanceOf(thread, input, _Arcadia_String_getType(thread)) &&
       !Arcadia_Object_isInstanceOf(thread, input, _Arcadia_ByteArray_getType(thread)))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->object = input;
  self->format = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_Imaging_ImageReaderParameters);
}

static void
Arcadia_Imaging_ImageReaderParameters_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReaderParameters* self
  )
{
  if (self->format) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->format);
  }
  if (self->object) {
    Arcadia_Object_visit(thread, self->object);
  }
}

static void
Arcadia_Imaging_ImageReaderParameters_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReaderParametersDispatch* self
  )
{/*Intentionally empty.*/}

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Imaging_ImageReaderParameters_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Imaging_ImageReaderParameters_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Imaging_ImageReaderParameters_initializeDispatchImpl,
};

static const Arcadia_TypeDescriptor_Operations _typeOperations = {
  Arcadia_TypeDescriptor_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Imaging.ImageReaderParameters", Arcadia_Imaging_ImageReaderParameters,
                         u8"Arcadia.Object", Arcadia_Object, &_typeOperations);

Arcadia_Imaging_ImageReaderParameters*
Arcadia_Imaging_ImageReaderParameters_createFile
  (
    Arcadia_Thread* thread,
    Arcadia_String* path,
    Arcadia_String* format
  )
{
  _Arcadia_BeginCreate(Arcadia_Imaging_ImageReaderParameters);
  if (path) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, path);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (format) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, format);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Imaging_ImageReaderParameters);
}

Arcadia_Imaging_ImageReaderParameters*
Arcadia_Imaging_ImageReaderParameters_createByteArray
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray* bytes,
    Arcadia_String* format
  )
{
  _Arcadia_BeginCreate(Arcadia_Imaging_ImageReaderParameters);
  if (bytes) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, bytes);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (format) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, format);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Imaging_ImageReaderParameters);
}

Arcadia_BooleanValue
Arcadia_Imaging_ImageReaderParameters_hasPath
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReaderParameters* self
  )
{
  return Arcadia_Object_isInstanceOf(thread, self->object, _Arcadia_String_getType(thread));
}

Arcadia_String*
Arcadia_Imaging_ImageReaderParameters_getPath
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReaderParameters* self
  )
{
  if (!Arcadia_Imaging_ImageReaderParameters_hasPath(thread, self)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_String*)self->object;
}

Arcadia_BooleanValue
Arcadia_Imaging_ImageReaderParameters_hasByteArray
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReaderParameters* self
  )
{
  return Arcadia_Object_isInstanceOf(thread, self->object, _Arcadia_ByteArray_getType(thread));
}

Arcadia_ByteArray*
Arcadia_Imaging_ImageReaderParameters_getByteArray
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReaderParameters* self
  )
{
  if (!Arcadia_Imaging_ImageReaderParameters_hasByteArray(thread, self)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_ByteArray*)self->object;
}

Arcadia_String*
Arcadia_Imaging_ImageReaderParameters_getFormat
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReaderParameters* self
  )
{
  return self->format;
}
