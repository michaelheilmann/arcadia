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

#include "Arcadia/PixelBufferIO/Linux/BMPImageWriter.h"

#include "Arcadia/PixelBufferIO/ImageWriterParameters.h"
#include "Arcadia/PixelBufferIO/Linux/Include.h"

#include "Arcadia/Media/Include.h"

#define STB_IMAGE_WRITE_STATIC static
#define STBIWDEF
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Arcadia/PixelBufferIO/Linux/_stb_image_write.h"

typedef struct WriteContext {
  Arcadia_Process* process;
  Arcadia_ByteArrayBuilder* byteArrayBuilder;
  Arcadia_Status status;
} WriteContext;

static void
writeCallback
  (
    WriteContext* writeContext,
    void* data,
    int size
  );

static void
writeCallback
  (
    WriteContext* context,
    void* data,
    int size
  )
{
  Arcadia_Process* process = context->process;
  Arcadia_Thread* thread = Arcadia_Process_getThread(process);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_ByteArrayBuilder_insertBackBytes(thread, context->byteArrayBuilder, data, size);
  }
  Arcadia_Thread_popJumpTarget(thread);
}

static void
Arcadia_Imaging_Linux_BMPImageWriter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_BMPImageWriter* self
  );

static void
Arcadia_Imaging_Linux_BMPImageWriter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_BMPImageWriterDispatch* self
  );

static void
Arcadia_Imaging_Linux_BMPImageWriter_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_BMPImageWriter* self
  );

static Arcadia_ImmutableList*
Arcadia_Imaging_Linux_BMPImageWriter_getSupportedTypesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_BMPImageWriter* self
  );

static void
Arcadia_Imaging_Linux_BMPImageWriter_writeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_BMPImageWriter* self,
    Arcadia_List* source,
    Arcadia_Imaging_ImageWriterParameters* target
  );

static void
Arcadia_Imaging_Linux_BMPImageWriter_writeBmpToByteBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_BMPImageWriter* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_ByteArrayBuilder* targetByteArrayBuilder
  );

static void
Arcadia_Imaging_Linux_BMPImageWriter_writeBmpToPathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_BMPImageWriter* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffers,
    Arcadia_String* targetPath
  );

static Arcadia_ImmutableList*
Arcadia_Imaging_Linux_BMPImageWriter_getSupportedTypesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_BMPImageWriter* self
  )
{ return self->supportedTypes; }

static void
Arcadia_Imaging_Linux_BMPImageWriter_writeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_BMPImageWriter* self,
    Arcadia_List* source,
    Arcadia_Imaging_ImageWriterParameters* target
  )
{
  Arcadia_Value requestedExtension = Arcadia_Value_makeObjectReferenceValue(Arcadia_Imaging_ImageWriterParameters_getFormat(thread, target));
  if (!Arcadia_List_contains(thread, (Arcadia_List*)self->supportedTypes, requestedExtension)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (1 != Arcadia_Collection_getSize(thread, (Arcadia_Collection*)source)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ObjectReferenceValue sourceObject = Arcadia_List_getObjectReferenceValueAt(thread, source, 0);
  if (!Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, sourceObject), _Arcadia_Media_PixelBuffer_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Media_PixelBuffer* sourcePixelBuffer = (Arcadia_Media_PixelBuffer*)sourceObject;
  if (Arcadia_Imaging_ImageWriterParameters_hasByteBuffer(thread, target)) {
    Arcadia_Imaging_Linux_BMPImageWriter_writeBmpToByteBufferImpl(thread, self, sourcePixelBuffer, Arcadia_Imaging_ImageWriterParameters_getByteBuffer(thread, target));
  } else if (Arcadia_Imaging_ImageWriterParameters_hasPath(thread, target)) {
    Arcadia_Imaging_Linux_BMPImageWriter_writeBmpToPathImpl(thread, self, sourcePixelBuffer, Arcadia_Imaging_ImageWriterParameters_getPath(thread, target));
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static void
Arcadia_Imaging_Linux_BMPImageWriter_writeBmpToByteBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_BMPImageWriter* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_ByteArrayBuilder* targetByteArrayBuilder
  )
{
  Arcadia_Process* process = Arcadia_Thread_getProcess(thread);
  WriteContext context;
  context.status = Arcadia_Status_Success;
  context.process = process;
  context.byteArrayBuilder = targetByteArrayBuilder;

  int components;
  switch (sourcePixelBuffer->pixelFormat) {
    case Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8: {
      sourcePixelBuffer = Arcadia_Media_PixelBuffer_createClone(thread, sourcePixelBuffer);
      Arcadia_Media_PixelBuffer_setPixelFormat(thread, sourcePixelBuffer, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
      Arcadia_Media_PixelBuffer_setLinePadding(thread, sourcePixelBuffer, 0);
      components = 4;
    } break;
    case Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8: {
      sourcePixelBuffer = Arcadia_Media_PixelBuffer_createClone(thread, sourcePixelBuffer);
      Arcadia_Media_PixelBuffer_setPixelFormat(thread, sourcePixelBuffer, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
      Arcadia_Media_PixelBuffer_setLinePadding(thread, sourcePixelBuffer, 0);
      components = 4;
    } break;
    case Arcadia_Media_PixelFormat_RedGreenBlueNatural8: {
      sourcePixelBuffer = Arcadia_Media_PixelBuffer_createClone(thread, sourcePixelBuffer);
      Arcadia_Media_PixelBuffer_setLinePadding(thread, sourcePixelBuffer, 0);
      components = 3;
    } break;
    case Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8: {
      sourcePixelBuffer = Arcadia_Media_PixelBuffer_createClone(thread, sourcePixelBuffer);
      Arcadia_Media_PixelBuffer_setLinePadding(thread, sourcePixelBuffer, 0);
      components = 3;
    } break;

    case Arcadia_Media_PixelFormat_BlueGreenRedNatural8: {
      sourcePixelBuffer = Arcadia_Media_PixelBuffer_createClone(thread, sourcePixelBuffer);
      Arcadia_Media_PixelBuffer_setPixelFormat(thread, sourcePixelBuffer, Arcadia_Media_PixelFormat_RedGreenBlueNatural8);
      Arcadia_Media_PixelBuffer_setLinePadding(thread, sourcePixelBuffer, 0);
      components = 3;
    } break;
    case Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8: {
      sourcePixelBuffer = Arcadia_Media_PixelBuffer_createClone(thread, sourcePixelBuffer);
      Arcadia_Media_PixelBuffer_setPixelFormat(thread, sourcePixelBuffer, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
      Arcadia_Media_PixelBuffer_setLinePadding(thread, sourcePixelBuffer, 0);
      components = 4;
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    } break;
  };

  if (!stbi_write_bmp_to_func((stbi_write_func*)&writeCallback, &context,
                              Arcadia_Media_PixelBuffer_getWidth(thread, sourcePixelBuffer),
                              Arcadia_Media_PixelBuffer_getHeight(thread, sourcePixelBuffer),
                              components,
                              sourcePixelBuffer->bytes)) {
    context.status = Arcadia_Status_EnvironmentFailed;
  }
  if (context.status) {
    Arcadia_Thread_setStatus(thread, context.status);
    Arcadia_Thread_jump(thread);
  }
}

static void
Arcadia_Imaging_Linux_BMPImageWriter_writeBmpToPathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_BMPImageWriter* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_String *targetPath
  )
{
  Arcadia_ByteArrayBuilder* targetByteArrayBuilder = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_Imaging_Linux_BMPImageWriter_writeBmpToByteBufferImpl(thread, self, sourcePixelBuffer, targetByteArrayBuilder);
  Arcadia_FileSystem_setFileContents(thread, Arcadia_FileSystem_getOrCreate(thread),
                                     Arcadia_FilePath_parseUnix(thread, targetPath), targetByteArrayBuilder);
}

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Imaging_Linux_BMPImageWriter_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Imaging_Linux_BMPImageWriter_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Imaging_Linux_BMPImageWriter_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Imaging.Linux.BMPImageWriter", Arcadia_Imaging_Linux_BMPImageWriter,
                         u8"Arcadia.Imaging.ImageWriter", Arcadia_Imaging_ImageWriter,
                         &_typeOperations);

static void
Arcadia_Imaging_Linux_BMPImageWriter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_BMPImageWriter* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Imaging_Linux_BMPImageWriter);
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
  Arcadia_LeaveConstructor(Arcadia_Imaging_Linux_BMPImageWriter);
}

static void
Arcadia_Imaging_Linux_BMPImageWriter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_BMPImageWriterDispatch* self
  )
{
  ((Arcadia_Imaging_ImageWriterDispatch*)self)->getSupportedTypes = (Arcadia_ImmutableList*(*)(Arcadia_Thread*,Arcadia_Imaging_ImageWriter*))&Arcadia_Imaging_Linux_BMPImageWriter_getSupportedTypesImpl;
  ((Arcadia_Imaging_ImageWriterDispatch*)self)->write = (void (*)(Arcadia_Thread*, Arcadia_Imaging_ImageWriter*, Arcadia_List*, Arcadia_Imaging_ImageWriterParameters*)) & Arcadia_Imaging_Linux_BMPImageWriter_writeImpl;
}

static void
Arcadia_Imaging_Linux_BMPImageWriter_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_BMPImageWriter* self
  )
{
  if (self->supportedTypes) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->supportedTypes);
  }
}

Arcadia_Imaging_Linux_BMPImageWriter*
Arcadia_Imaging_Linux_BMPImageWriter_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Imaging_Linux_BMPImageWriter);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Imaging_Linux_BMPImageWriter);
}
