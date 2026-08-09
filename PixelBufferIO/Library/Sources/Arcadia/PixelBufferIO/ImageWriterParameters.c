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

#include "Arcadia/PixelBufferIO/ImageWriterParameters.h"

struct Arcadia_Imaging_ImageWriterParametersDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_Imaging_ImageWriterParameters {
  Arcadia_Object _parent;
  // The format string.
  Arcadia_String* format;
  // "object" either refers to a "Arcadia.ByteArrayBuilder" object or a "Arcadia.Path" object.
  Arcadia_ObjectReferenceValue object;
};

static void
Arcadia_Imaging_ImageWriterParameters_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriterParameters* self
  );

static void
Arcadia_Imaging_ImageWriterParameters_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriterParametersDispatch* self
  );

static void
Arcadia_Imaging_ImageWriterParameters_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriterParameters* self
  );

static void
Arcadia_Imaging_ImageWriterParameters_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriterParameters* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Imaging_ImageWriterParameters_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Imaging_ImageWriterParameters_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Imaging_ImageWriterParameters_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Imaging_ImageWriterParameters_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Imaging.ImageWriterParameters", Arcadia_Imaging_ImageWriterParameters,
                         u8"Arcadia.Object", Arcadia_Object, &_typeOperations);

static void
Arcadia_Imaging_ImageWriterParameters_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriterParameters* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Imaging_ImageWriterParameters);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Object* object = Arcadia_ValueStack_getObjectReferenceValue(thread, 2);
  if (Arcadia_Object_isInstanceOf(thread, object, _Arcadia_String_getType(thread))) {
    self->object = (Arcadia_ObjectReferenceValue)object;
  } else if (Arcadia_Object_isInstanceOf(thread, object, _Arcadia_ByteArrayBuilder_getType(thread))) {
    self->object = (Arcadia_ObjectReferenceValue)object;
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->format = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_Imaging_ImageWriterParameters);
}

static void
Arcadia_Imaging_ImageWriterParameters_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriterParametersDispatch* self
  )
{ }

static void
Arcadia_Imaging_ImageWriterParameters_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriterParameters* self
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
Arcadia_Imaging_ImageWriterParameters_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriterParameters* self
  )
{/*Intentionally empty.*/}

Arcadia_Imaging_ImageWriterParameters*
Arcadia_Imaging_ImageWriterParameters_createFile
  (
    Arcadia_Thread* thread,
    Arcadia_String* path,
    Arcadia_String* format
  )
{
  _Arcadia_BeginCreate(Arcadia_Imaging_ImageWriterParameters);
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
  _Arcadia_EndCreate(Arcadia_Imaging_ImageWriterParameters);
}

Arcadia_Imaging_ImageWriterParameters*
Arcadia_Imaging_ImageWriterParameters_createByteBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* byteBuffer,
    Arcadia_String* format
  )
{
  _Arcadia_BeginCreate(Arcadia_Imaging_ImageWriterParameters);
  if (byteBuffer) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, byteBuffer);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (format) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, format);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Imaging_ImageWriterParameters);
}

Arcadia_BooleanValue
Arcadia_Imaging_ImageWriterParameters_hasPath
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriterParameters* self
  )
{
  Arcadia_TypeValue type = Arcadia_Object_getType(thread, self->object);
  return Arcadia_Type_isDescendantType(thread, type, _Arcadia_String_getType(thread));
}

Arcadia_String*
Arcadia_Imaging_ImageWriterParameters_getPath
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriterParameters* self
  )
{
  if (!Arcadia_Imaging_ImageWriterParameters_hasPath(thread, self)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_String*)self->object;
}

Arcadia_BooleanValue
Arcadia_Imaging_ImageWriterParameters_hasByteBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriterParameters* self
  )
{
  Arcadia_TypeValue type = Arcadia_Object_getType(thread, self->object);
  return Arcadia_Type_isDescendantType(thread, type, _Arcadia_ByteArrayBuilder_getType(thread));
}

Arcadia_ByteArrayBuilder*
Arcadia_Imaging_ImageWriterParameters_getByteBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriterParameters* self
  )
{
  if (!Arcadia_Imaging_ImageWriterParameters_hasByteBuffer(thread, self)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_ByteArrayBuilder*)self->object;
}

Arcadia_String*
Arcadia_Imaging_ImageWriterParameters_getFormat
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriterParameters* self
  )
{
  return self->format;
}
