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

#include "Arcadia/PixelBufferIO/ICOImageWriter.h"

#include "Arcadia/PixelBufferIO/ImageWriterParameters.h"
#include "Arcadia/PixelBufferIO/ImageManager.h"

static void
Arcadia_Imaging_ICOImageWriter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageWriter* self
  );

static void
Arcadia_Imaging_ICOImageWriter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageWriterDispatch* self
  );

static void
Arcadia_Imaging_ICOImageWriter_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageWriter* self
  );

static Arcadia_ImmutableList*
Arcadia_Imaging_ICOImageWriter_getSupportedTypesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageWriter* self
  );

static void
Arcadia_Imaging_ICOImageWriter_writeToPathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageWriter* self,
    Arcadia_String* extension,
    Arcadia_List* sourcePixelBuffers,
    Arcadia_String* targetPath
  );

static void
Arcadia_Imaging_ICOImageWriter_writeToByteBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageWriter* self,
    Arcadia_String* extension,
    Arcadia_List* sourcePixelBuffers,
    Arcadia_ByteArrayBuilder* targetByteBuffer
  );

static Arcadia_Imaging_ImageWriter*
Arcadia_Imaging_ICOImageWriter_getPngImageWriter
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageWriter* self
  );

static void
Arcadia_Imaging_ICOImageWriter_writeICOToPathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageWriter* self,
    Arcadia_List* sourcePixelBuffers,
    Arcadia_String* targetPath
  );

static void
Arcadia_Imaging_ICOImageWriter_writeICOToByteBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageWriter* self,
    Arcadia_List* sourcePixelBuffers,
    Arcadia_ByteArrayBuilder* targetByteBuffer
  );

static void
Arcadia_Imaging_Arcadia_Imaging_ICOImageWriter_writeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageWriter* self,
    Arcadia_List* source,
    Arcadia_Imaging_ImageWriterParameters* target
  );

static Arcadia_ImmutableList*
Arcadia_Imaging_ICOImageWriter_getSupportedTypesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageWriter* self
  )
{ return self->supportedTypes; }

static void
Arcadia_Imaging_ICOImageWriter_writeToPathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageWriter* self,
    Arcadia_String* extension,
    Arcadia_List* sourcePixelBuffers,
    Arcadia_String* targetPath
  )
{
  Arcadia_Value a = Arcadia_Value_makeObjectReferenceValue(extension);
  Arcadia_Value b[] = {
    Arcadia_Value_makeObjectReferenceValue(Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"ICO", sizeof(u8"ICO") - 1)))),
  };

  if (Arcadia_Value_isEqualTo(thread, &a, &b[0])) {
    if (Arcadia_Collection_getSize(thread, (Arcadia_Collection*)sourcePixelBuffers) < 1) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Imaging_ICOImageWriter_writeICOToPathImpl(thread, self, sourcePixelBuffers, targetPath);
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static void
Arcadia_Imaging_ICOImageWriter_writeToByteBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageWriter* self,
    Arcadia_String* extension,
    Arcadia_List* sourcePixelBuffers,
    Arcadia_ByteArrayBuilder* targetByteBuffer
  )
{
  Arcadia_Value a = Arcadia_Value_makeObjectReferenceValue(extension);
  Arcadia_Value b[] = {
    Arcadia_Value_makeObjectReferenceValue(Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"ICO", sizeof(u8"ICO") - 1)))),
  };
  if (Arcadia_Value_isEqualTo(thread, &a, &b[0])) {
    if (Arcadia_Collection_getSize(thread, (Arcadia_Collection*)sourcePixelBuffers) < 1) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Imaging_ICOImageWriter_writeICOToByteBufferImpl(thread, self, sourcePixelBuffers, targetByteBuffer);
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static Arcadia_Imaging_ImageWriter*
Arcadia_Imaging_ICOImageWriter_getPngImageWriter
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageWriter* self
  )
{
  Arcadia_Imaging_ImageManager* imageManager = Arcadia_Imaging_ImageManager_getOrCreate(thread);
  Arcadia_String* extension = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"png", sizeof(u8"png") - 1)));
  Arcadia_List* writers = Arcadia_Imaging_ImageManager_getWriters(thread, imageManager, extension);
  if (!Arcadia_Collection_getSize(thread, (Arcadia_Collection*)writers)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
    Arcadia_Thread_popJumpTarget(thread);
  }
  Arcadia_Imaging_ImageWriter* writer = (Arcadia_Imaging_ImageWriter*)Arcadia_List_getObjectReferenceValueAt(thread, writers, 0);
  return writer;
}

static void
Arcadia_Imaging_ICOImageWriter_writeICOToPathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageWriter* self,
    Arcadia_List* sourcePixelBuffers,
    Arcadia_String* targetPath
  )
{
  Arcadia_ByteArrayBuilder* targetByteBuffer = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_Imaging_ICOImageWriter_writeICOToByteBufferImpl(thread, self, sourcePixelBuffers, targetByteBuffer);
  Arcadia_FileSystem_setFileContents(thread, Arcadia_FileSystem_getOrCreate(thread), Arcadia_FilePath_parseUnix(thread, targetPath), targetByteBuffer);
}

static void
Arcadia_Imaging_ICOImageWriter_writeICOToByteBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageWriter* self,
    Arcadia_List* sourcePixelBuffers,
    Arcadia_ByteArrayBuilder* targetByteBuffer
  )
{
  typedef struct ICONDIR {
    uint16_t reserved1;
    uint16_t type;
    uint16_t numberOfImages;
  } ICONDIR;

  typedef struct ICONDIRENTRY {
    uint8_t width;
    uint8_t height;
    uint8_t numberOfColors;
    uint8_t reserved1;
    uint16_t numberOfColorPlanes;
    uint16_t numberOfBitsPerPixel;
    uint32_t imageSize;
    uint32_t offset;
  } ICONDIRENTRY;

  ICONDIR ICOnDir = {
    .reserved1 = 0,
    .type = 1,
    .numberOfImages = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)sourcePixelBuffers)
  };
  Arcadia_List* pixelBuffers = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, targetByteBuffer, &ICOnDir, sizeof(ICONDIR));

  Arcadia_Imaging_ImageWriter* pngImageWriter =  Arcadia_Imaging_ICOImageWriter_getPngImageWriter(thread, self);
  Arcadia_String* pngExtension = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"png", sizeof(u8"png") - 1)));
  Arcadia_ByteArrayBuilder* temporary = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_Imaging_ImageWriterParameters* pngParameters = Arcadia_Imaging_ImageWriterParameters_createByteBuffer(thread, temporary, pngExtension);
  for (Arcadia_SizeValue i = 0, offset = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)sourcePixelBuffers); i < n; ++i) {
    Arcadia_Media_PixelBuffer* pixelBuffer = (Arcadia_Media_PixelBuffer*)Arcadia_List_getObjectReferenceValueAt(thread, sourcePixelBuffers, i);
    Arcadia_ByteArrayBuilder_clear(thread, temporary);
    Arcadia_Collection_clear(thread, (Arcadia_Collection*)pixelBuffers);
    Arcadia_List_insertBackObjectReferenceValue(thread, pixelBuffers, pixelBuffer);
    Arcadia_Imaging_ImageWriter_write(thread, pngImageWriter, pixelBuffers, pngParameters);
    if (Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8 != Arcadia_Media_PixelBuffer_getPixelFormat(thread, pixelBuffer)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
    size_t width = Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, pixelBuffer),
           height = Arcadia_Media_PixelBuffer_getNumberOfRows(thread, pixelBuffer);
    if (width > 256) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
    if (height > 256) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
    if (width == 256) {
      width = 0;
    }
    if (height == 256) {
      height = 256;
    }

    ICONDIRENTRY ICOnDirEntry = { .width = width,
                                  .height = height,
                                  .numberOfColors = 0,
                                  .reserved1 = 0,
                                  .numberOfColorPlanes = 1,
                                  .numberOfBitsPerPixel = 32,
                                  .imageSize = Arcadia_ByteArrayBuilder_getSize(thread, temporary),
                                  .offset = sizeof(ICONDIR) + Arcadia_Collection_getSize(thread, (Arcadia_Collection*)sourcePixelBuffers) * sizeof(ICONDIRENTRY) + offset };
    Arcadia_ByteArrayBuilder_insertBackBytes(thread, targetByteBuffer, &ICOnDirEntry, sizeof(ICONDIRENTRY));
    offset += Arcadia_ByteArrayBuilder_getSize(thread, temporary);
  }
  for (Arcadia_SizeValue i = 0, offset = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)sourcePixelBuffers); i < n; ++i) {
    Arcadia_Media_PixelBuffer* pixelBuffer = (Arcadia_Media_PixelBuffer*)Arcadia_List_getObjectReferenceValueAt(thread, sourcePixelBuffers, i);
    Arcadia_ByteArrayBuilder_clear(thread, temporary);
    Arcadia_Collection_clear(thread, (Arcadia_Collection*)pixelBuffers);
    Arcadia_List_insertBackObjectReferenceValue(thread, pixelBuffers, pixelBuffer);
    Arcadia_Imaging_ImageWriter_write(thread, pngImageWriter, pixelBuffers, pngParameters);
    Arcadia_ByteArrayBuilder_insertBackBytes(thread, targetByteBuffer, temporary->p, temporary->sz);
    offset += Arcadia_ByteArrayBuilder_getSize(thread, temporary);
  }
}

static void
Arcadia_Imaging_ICOImageWriter_writeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageWriter* self,
    Arcadia_List* source,
    Arcadia_Imaging_ImageWriterParameters* target
  )
{
  Arcadia_Value requestedExtension = Arcadia_Value_makeObjectReferenceValue(Arcadia_Imaging_ImageWriterParameters_getFormat(thread, target));
  if (!Arcadia_List_contains(thread, (Arcadia_List*)self->supportedTypes, requestedExtension)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (Arcadia_Imaging_ImageWriterParameters_hasByteBuffer(thread, target)) {
    Arcadia_Imaging_ICOImageWriter_writeICOToByteBufferImpl(thread, self, source, Arcadia_Imaging_ImageWriterParameters_getByteBuffer(thread, target));
  } else if (Arcadia_Imaging_ImageWriterParameters_hasPath(thread, target)) {
    Arcadia_Imaging_ICOImageWriter_writeICOToPathImpl(thread, self, source, Arcadia_Imaging_ImageWriterParameters_getPath(thread, target));
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Imaging_ICOImageWriter_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Imaging_ICOImageWriter_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Imaging_ICOImageWriter_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Imaging.ICOImageWriter", Arcadia_Imaging_ICOImageWriter,
                         u8"Arcadia.Imaging.ImageWriter", Arcadia_Imaging_ImageWriter,
                         &_typeOperations);

static void
Arcadia_Imaging_ICOImageWriter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageWriter* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Imaging_ICOImageWriter);
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
  Arcadia_List_insertBackObjectReferenceValue(thread, supportedTypes, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"ICO", sizeof(u8"ICO") - 1))));
  Arcadia_List_insertBackObjectReferenceValue(thread, supportedTypes, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"ico", sizeof(u8"ico") - 1))));
  self->supportedTypes = Arcadia_ImmutableList_create(thread, Arcadia_Value_makeObjectReferenceValue(supportedTypes));
  Arcadia_LeaveConstructor(Arcadia_Imaging_ICOImageWriter);
}

static void
Arcadia_Imaging_ICOImageWriter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageWriterDispatch* self
  )
{
  ((Arcadia_Imaging_ImageWriterDispatch*)self)->getSupportedTypes = (Arcadia_ImmutableList * (*)(Arcadia_Thread*, Arcadia_Imaging_ImageWriter*)) & Arcadia_Imaging_ICOImageWriter_getSupportedTypesImpl;
  ((Arcadia_Imaging_ImageWriterDispatch*)self)->write = (void (*)(Arcadia_Thread*, Arcadia_Imaging_ImageWriter*, Arcadia_List*, Arcadia_Imaging_ImageWriterParameters*)) & Arcadia_Imaging_ICOImageWriter_writeImpl;
}

static void
Arcadia_Imaging_ICOImageWriter_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageWriter* self
  )
{
  if (self->supportedTypes) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->supportedTypes);
  }
}

Arcadia_Imaging_ICOImageWriter*
Arcadia_Imaging_ICOImageWriter_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Imaging_ICOImageWriter);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Imaging_ICOImageWriter);
}
