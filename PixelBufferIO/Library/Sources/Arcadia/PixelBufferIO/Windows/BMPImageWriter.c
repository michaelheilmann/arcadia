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

#include "Arcadia/PixelBufferIO/Windows/BMPImageWriter.h"

#include "Arcadia/PixelBufferIO/Windows/MultiByteToWideCharZeroTerminated.h"

// WIC library
#pragma comment (lib, "Windowscodecs.lib")

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Arcadia/PixelBufferIO/ImageWriterParameters.h"

static Arcadia_ImmutableList*
Arcadia_Imaging_Windows_BMPImageWriter_getSupportedTypes
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_BMPImageWriter* self
  );

static void
Arcadia_Imaging_Windows_BMPImageWriter_writeToPathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_BMPImageWriter* self,
    Arcadia_String* extension,
    Arcadia_List* sourcePixelBuffers,
    Arcadia_String* targetPath
  );

static void
Arcadia_Imaging_Windows_BMPImageWriter_writeToByteBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_BMPImageWriter* self,
    Arcadia_String* extension,
    Arcadia_List* sourcePixelBuffers,
    Arcadia_ByteArrayBuilder* targetByteBuffer
  );

static void
configure
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_BMPImageWriter* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_Imaging_ImageWriterParameters* parameters
  )
{
  HRESULT hr;
  // https://learn.microsoft.com/en-us/windows/win32/wic/bmp-format-overview
  PROPBAG2 option = { 0 };
  option.pstrName = L"EnableV5Header32bppBGRA";
  VARIANT varValue;
  VariantInit(&varValue);
  varValue.vt = VT_BOOL;
  varValue.boolVal = VARIANT_TRUE;
  hr = IPropertyBag2_Write(((Arcadia_Imaging_Windows_WICImageWriterBase*)self)->pPropertyBag, 1, &option, &varValue);
  if (FAILED(hr)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
}

static void
getFormat
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_BMPImageWriter* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_Imaging_ImageWriterParameters* parameters,
    GUID const** guid
  )
{
  *guid = &GUID_ContainerFormatBmp;
}

static Arcadia_ImmutableList*
Arcadia_Imaging_Windows_BMPImageWriter_getSupportedTypes
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_BMPImageWriter* self
  )
{
  return self->supportedTypes;
}

static void
Arcadia_Imaging_Windows_BMPImageWriter_writeToPathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_BMPImageWriter* self,
    Arcadia_String* extension,
    Arcadia_List* sourcePixelBuffers,
    Arcadia_String* targetPath
  )
{
  Arcadia_Imaging_ImageWriterParameters* parameters = Arcadia_Imaging_ImageWriterParameters_createFile(thread, targetPath, extension);
  if (1 != Arcadia_Collection_getSize(thread, (Arcadia_Collection*)sourcePixelBuffers)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Media_PixelBuffer* sourcePixelBuffer =
    (Arcadia_Media_PixelBuffer*)
    Arcadia_List_getObjectReferenceValueCheckedAt
      (
        thread,
        sourcePixelBuffers,
        0,
        _Arcadia_Media_PixelBuffer_getType(thread)
      );
  Arcadia_Imaging_Windows_WICImageWriterBase_doWrite(thread, (Arcadia_Imaging_Windows_WICImageWriterBase*)self, sourcePixelBuffer, parameters);
}

static void
Arcadia_Imaging_Windows_BMPImageWriter_writeToByteBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_BMPImageWriter* self,
    Arcadia_String* extension,
    Arcadia_List* sourcePixelBuffers,
    Arcadia_ByteArrayBuilder* targetByteBuffer
  )
{
  Arcadia_Imaging_ImageWriterParameters* parameters = Arcadia_Imaging_ImageWriterParameters_createByteBuffer(thread, targetByteBuffer, extension);
  if (1 != Arcadia_Collection_getSize(thread, (Arcadia_Collection*)sourcePixelBuffers)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Media_PixelBuffer* sourcePixelBuffer =
    (Arcadia_Media_PixelBuffer*)
    Arcadia_List_getObjectReferenceValueCheckedAt
      (
        thread,
        sourcePixelBuffers,
        0,
        _Arcadia_Media_PixelBuffer_getType(thread)
      );
  Arcadia_Imaging_Windows_WICImageWriterBase_doWrite(thread, (Arcadia_Imaging_Windows_WICImageWriterBase*)self, sourcePixelBuffer, parameters);
}

static void
Arcadia_Imaging_Windows_BMPImageWriter_write
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_BMPImageWriter* self,
    Arcadia_List* source,
    Arcadia_Imaging_ImageWriterParameters* target
  )
{
  Arcadia_Value requestedExtension = Arcadia_Value_makeObjectReferenceValue(Arcadia_Imaging_ImageWriterParameters_getFormat(thread, target));
  if (!Arcadia_List_contains(thread, (Arcadia_List*)self->supportedTypes, requestedExtension)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ObjectReferenceValue sourceObject = Arcadia_List_getObjectReferenceValueAt(thread, source, 0);
  if (!Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, sourceObject), _Arcadia_Media_PixelBuffer_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Media_PixelBuffer* sourcePixelBuffer = (Arcadia_Media_PixelBuffer*)sourceObject;
  Arcadia_Imaging_Windows_WICImageWriterBase_doWrite(thread, (Arcadia_Imaging_Windows_WICImageWriterBase*)self, sourcePixelBuffer, target);
}

static void
Arcadia_Imaging_Windows_BMPImageWriter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_BMPImageWriter* self
  );

static void
Arcadia_Imaging_Windows_BMPImageWriter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_BMPImageWriterDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Imaging_Windows_BMPImageWriter_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Imaging_Windows_BMPImageWriter_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Imaging.Windows.BMPImageWriter", Arcadia_Imaging_Windows_BMPImageWriter,
                         u8"Arcadia.Imaging.Windows.WICImagerWriterBase", Arcadia_Imaging_Windows_WICImageWriterBase,
                         &_typeOperations);

static void
Arcadia_Imaging_Windows_BMPImageWriter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_BMPImageWriter* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Imaging_Windows_BMPImageWriter);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  self->supportedTypes = NULL;
  Arcadia_List* supportedTypes = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_List_insertBackObjectReferenceValue(thread, supportedTypes, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"bmp", sizeof(u8"bmp") - 1))));
  self->supportedTypes = Arcadia_ImmutableList_create(thread, Arcadia_Value_makeObjectReferenceValue(supportedTypes));

  Arcadia_LeaveConstructor(Arcadia_Imaging_Windows_BMPImageWriter);
}

static void
Arcadia_Imaging_Windows_BMPImageWriter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_BMPImageWriterDispatch* self
  )
{
  ((Arcadia_Imaging_ImageWriterDispatch*)self)->getSupportedTypes = (Arcadia_ImmutableList * (*)(Arcadia_Thread*, Arcadia_Imaging_ImageWriter*)) & Arcadia_Imaging_Windows_BMPImageWriter_getSupportedTypes;
  ((Arcadia_Imaging_ImageWriterDispatch*)self)->write = (void (*)(Arcadia_Thread*, Arcadia_Imaging_ImageWriter*, Arcadia_List*, Arcadia_Imaging_ImageWriterParameters*)) & Arcadia_Imaging_Windows_BMPImageWriter_write;
  ((Arcadia_Imaging_Windows_WICImageWriterBaseDispatch*)self)->getFormat = (void (*)(Arcadia_Thread*, Arcadia_Imaging_Windows_WICImageWriterBase*, Arcadia_Media_PixelBuffer*, Arcadia_Imaging_ImageWriterParameters*, GUID const**)) & getFormat;
  ((Arcadia_Imaging_Windows_WICImageWriterBaseDispatch*)self)->configure = (void (*)(Arcadia_Thread*, Arcadia_Imaging_Windows_WICImageWriterBase*, Arcadia_Media_PixelBuffer*, Arcadia_Imaging_ImageWriterParameters*)) & configure;
}

Arcadia_Imaging_Windows_BMPImageWriter*
Arcadia_Imaging_Windows_BMPImageWriter_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Imaging_Windows_BMPImageWriter);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Imaging_Windows_BMPImageWriter);
}
