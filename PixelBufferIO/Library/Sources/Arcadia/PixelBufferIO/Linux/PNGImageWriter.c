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

#include "Arcadia/PixelBufferIO/Linux/PNGImageWriter.h"

#include "Arcadia/PixelBufferIO/ImageWriterParameters.h"
#include "Arcadia/PixelBufferIO/Linux/Include.h"

#include <malloc.h> // @todo Use functionality provided by Arcadia_Process.

#include <png.h>

static void
Arcadia_Imaging_Linux_PNGImageWriter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_PNGImageWriter* self
  );

static void
Arcadia_Imaging_Linux_PNGImageWriter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_PNGImageWriterDispatch* self
  );

static void
Arcadia_Imaging_Linux_PNGImageWriter_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_PNGImageWriter* self
  );

static Arcadia_ImmutableList*
Arcadia_Imaging_Linux_PNGImageWriter_getSupportedTypesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_PNGImageWriter* self
  );

static void
Arcadia_Imaging_Linux_PNGImageWriter_writeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_PNGImageWriter* self,
    Arcadia_List* source,
    Arcadia_Imaging_ImageWriterParameters* target
  );

static void
Arcadia_Imaging_Linux_PNGImageWriter_writePngToByteBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_PNGImageWriter* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_ByteArrayBuilder* targetByteArrayBuilder
  );

static void
Arcadia_Imaging_Linux_PNGImageWriter_writePngToPathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_PNGImageWriter* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_String* targetPath
  );

typedef struct LibPngState {
  png_structp png_struct;
  png_infop png_info;
  uint8_t** row_pointers;
} LibPngState;

typedef struct LibPngWriteState {
  Arcadia_ByteArrayBuilder* byteArrayBuilder;
  Arcadia_Process* process;
} LibPngWriteState;

static void
LibPngState_destroy
  (
    Arcadia_Thread* thread,
    LibPngState* state
  )
{
  if (state->row_pointers) {
    free(state->row_pointers);
    state->row_pointers = NULL;
  }
  png_destroy_write_struct(&state->png_struct, &state->png_info);
  free(state);
}

static LibPngState*
LibPngState_create
  (
    Arcadia_Thread* thread
  )
{
  LibPngState* state = malloc(sizeof(LibPngState));
  if (!state) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  state->png_struct = NULL;
  state->png_info = NULL;
  state->row_pointers = NULL;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    state->png_struct = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!state->png_struct) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    state->png_info = png_create_info_struct(state->png_struct);
    if (!state->png_info) {
      png_destroy_write_struct(&state->png_struct, (png_infopp)NULL);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (!state->png_info) {
      png_destroy_write_struct(&state->png_struct, (png_infopp)NULL);
    }
    free(state);
    state = NULL;
    Arcadia_Thread_jump(thread);
  }
  return state;
}

static void
on_write
  (
    png_structp png_struct,
    png_bytep data,
    png_size_t length
  )
{
  LibPngWriteState* ws = (LibPngWriteState*)png_get_io_ptr(png_struct);
  Arcadia_Thread* thread = Arcadia_Process_getThread(ws->process);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_ByteArrayBuilder_insertBackBytes(thread, ws->byteArrayBuilder, data, length);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    png_error(png_struct, "error");
  }
}

static void
on_flush
  (
    png_structp png_struct
  )
{/*Intentionally empty.*/}

static Arcadia_ImmutableList*
Arcadia_Imaging_Linux_PNGImageWriter_getSupportedTypesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_PNGImageWriter* self
  )
{ return self->supportedTypes; }

static void
Arcadia_Imaging_Linux_PNGImageWriter_writeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_PNGImageWriter* self,
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
    Arcadia_Imaging_Linux_PNGImageWriter_writePngToByteBufferImpl(thread, self, sourcePixelBuffer, Arcadia_Imaging_ImageWriterParameters_getByteBuffer(thread, target));
  } else if (Arcadia_Imaging_ImageWriterParameters_hasPath(thread, target)) {
    Arcadia_Imaging_Linux_PNGImageWriter_writePngToPathImpl(thread, self, sourcePixelBuffer, Arcadia_Imaging_ImageWriterParameters_getPath(thread, target));
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static void
Arcadia_Imaging_Linux_PNGImageWriter_writePngToByteBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_PNGImageWriter* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_ByteArrayBuilder* targetByteArrayBuilder
  )
{
  LibPngState* state = NULL;
  int bitDepth;
  int colorType;

  switch (Arcadia_Media_PixelBuffer_getPixelFormat(thread, sourcePixelBuffer)) {
    case Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8:
    case Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8:
    case Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8:
    case Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8: {
      Arcadia_Media_PixelBuffer* pixelBuffer = Arcadia_Media_PixelBuffer_createClone(thread, sourcePixelBuffer);
      Arcadia_Media_PixelBuffer_setPixelFormat(thread, pixelBuffer, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
      sourcePixelBuffer = pixelBuffer;
      bitDepth = 8;
      colorType = PNG_COLOR_TYPE_RGBA;
    } break;
    case Arcadia_Media_PixelFormat_BlueGreenRedNatural8:
    case Arcadia_Media_PixelFormat_RedGreenBlueNatural8: {
      Arcadia_Media_PixelBuffer* pixelBuffer = Arcadia_Media_PixelBuffer_createClone(thread, sourcePixelBuffer);
      Arcadia_Media_PixelBuffer_setPixelFormat(thread, pixelBuffer, Arcadia_Media_PixelFormat_RedGreenBlueNatural8);
      sourcePixelBuffer = pixelBuffer;
      bitDepth = 8;
      colorType = PNG_COLOR_TYPE_RGB;
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    } break;
  };
  uint32_t width = Arcadia_Media_PixelBuffer_getWidth(thread, sourcePixelBuffer);
  uint32_t height = Arcadia_Media_PixelBuffer_getHeight(thread, sourcePixelBuffer);
  uint32_t lineStride = Arcadia_Media_PixelBuffer_getLineStride(thread, sourcePixelBuffer);
  //uint32_t pixelBytes = Arcadia_Media_PixelBuffer_getBytesPerPixel(thread, sourcePixelBuffer);
  uint8_t* pixels = sourcePixelBuffer->bytes;

  state = LibPngState_create(thread);
  if (!setjmp(png_jmpbuf(state->png_struct))) {
    state->row_pointers = malloc(sizeof(uint8_t*) * height);
    if (!state->row_pointers) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    for (size_t i = 0; i < height; ++i) {
      state->row_pointers[i] = pixels + lineStride * i;
    }
    LibPngWriteState writeState = { .byteArrayBuilder = targetByteArrayBuilder, .process = Arcadia_Thread_getProcess(thread) };
    png_set_write_fn(state->png_struct, &writeState, &on_write, &on_flush),
    png_set_IHDR
      (
        state->png_struct,
        state->png_info,
        width, height,
        bitDepth,
        colorType,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
      );
    png_write_info(state->png_struct, state->png_info);
    png_write_image(state->png_struct, state->row_pointers);
    png_write_end(state->png_struct, NULL);
  } else {
    LibPngState_destroy(thread, state);
    state = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  LibPngState_destroy(thread, state);
  state = NULL;
}

static void
Arcadia_Imaging_Linux_PNGImageWriter_writePngToPathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_PNGImageWriter* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_String* targetPath
  )
{
  LibPngState* state = NULL;
  int bitDepth;
  int colorType;

  switch (Arcadia_Media_PixelBuffer_getPixelFormat(thread, sourcePixelBuffer)) {
    case Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8:
    case Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8:
    case Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8:
    case Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8: {
      Arcadia_Media_PixelBuffer* pixelBuffer = Arcadia_Media_PixelBuffer_createClone(thread, sourcePixelBuffer);
      Arcadia_Media_PixelBuffer_setPixelFormat(thread, pixelBuffer, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
      sourcePixelBuffer = pixelBuffer;
      bitDepth = 8;
      colorType = PNG_COLOR_TYPE_RGBA;
    } break;
    case Arcadia_Media_PixelFormat_BlueGreenRedNatural8:
    case Arcadia_Media_PixelFormat_RedGreenBlueNatural8: {
      Arcadia_Media_PixelBuffer* pixelBuffer = Arcadia_Media_PixelBuffer_createClone(thread, sourcePixelBuffer);
      Arcadia_Media_PixelBuffer_setPixelFormat(thread, pixelBuffer, Arcadia_Media_PixelFormat_RedGreenBlueNatural8);
      sourcePixelBuffer = pixelBuffer;
      bitDepth = 8;
      colorType = PNG_COLOR_TYPE_RGB;
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    } break;
  };
  uint32_t width = Arcadia_Media_PixelBuffer_getWidth(thread, sourcePixelBuffer);
  uint32_t height = Arcadia_Media_PixelBuffer_getHeight(thread, sourcePixelBuffer);
  uint32_t lineStride = Arcadia_Media_PixelBuffer_getLineStride(thread, sourcePixelBuffer);
  //uint32_t pixelBytes = Arcadia_Media_PixelBuffer_getBytesPerPixel(thread, sourcePixelBuffer);
  uint8_t* pixels = sourcePixelBuffer->bytes;

  state = LibPngState_create(thread);
  FILE* fp = NULL;
  if (!setjmp(png_jmpbuf(state->png_struct))) {
    state->row_pointers = malloc(sizeof(uint8_t*) * height);
    if (!state->row_pointers) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    for (size_t i = 0; i < height; ++i) {
      state->row_pointers[i] = pixels + lineStride * i;
    }
    Arcadia_FilePath* filePath = Arcadia_FilePath_parseNative(thread, targetPath);
    Arcadia_String* filePathString = Arcadia_FilePath_toNative(thread, filePath, Arcadia_BooleanValue_True);
    fp = fopen(Arcadia_String_getBytes(thread, filePathString), "wb");
    if (!fp) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    png_init_io(state->png_struct, fp);
    png_set_IHDR
      (
        state->png_struct,
        state->png_info,
        width, height,
        bitDepth,
        colorType,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
      );
    png_write_info(state->png_struct, state->png_info);
    png_write_image(state->png_struct, state->row_pointers);
    png_write_end(state->png_struct, NULL);
  } else {
    LibPngState_destroy(thread, state);
    state = NULL;
    if (fp) {
      fclose(fp);
      fp = NULL;
    }
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  LibPngState_destroy(thread, state);
  state = NULL;
  if (fp) {
    fclose(fp);
    fp = NULL;
  }
}

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Imaging_Linux_PNGImageWriter_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Imaging_Linux_PNGImageWriter_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Imaging_Linux_PNGImageWriter_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Imaging.Linux.PNGImageWriter", Arcadia_Imaging_Linux_PNGImageWriter,
                         u8"Arcadia.Imaging.ImageWriter", Arcadia_Imaging_ImageWriter,
                         &_typeOperations);

static void
Arcadia_Imaging_Linux_PNGImageWriter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_PNGImageWriter* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Imaging_Linux_PNGImageWriter);
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
  Arcadia_List_insertBackObjectReferenceValue(thread, supportedTypes, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"png", sizeof(u8"png") - 1))));
  self->supportedTypes = Arcadia_ImmutableList_create(thread, Arcadia_Value_makeObjectReferenceValue(supportedTypes));
  Arcadia_LeaveConstructor(Arcadia_Imaging_Linux_PNGImageWriter);
}

static void
Arcadia_Imaging_Linux_PNGImageWriter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_PNGImageWriterDispatch* self
  )
{
  ((Arcadia_Imaging_ImageWriterDispatch*)self)->getSupportedTypes = (Arcadia_ImmutableList*(*)(Arcadia_Thread*,Arcadia_Imaging_ImageWriter*))&Arcadia_Imaging_Linux_PNGImageWriter_getSupportedTypesImpl;
  ((Arcadia_Imaging_ImageWriterDispatch*)self)->write = (void (*)(Arcadia_Thread*, Arcadia_Imaging_ImageWriter*, Arcadia_List*, Arcadia_Imaging_ImageWriterParameters*)) & Arcadia_Imaging_Linux_PNGImageWriter_writeImpl;
}

static void
Arcadia_Imaging_Linux_PNGImageWriter_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Linux_PNGImageWriter* self
  )
{
  if (self->supportedTypes) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->supportedTypes);
  }
}

Arcadia_Imaging_Linux_PNGImageWriter*
Arcadia_Imaging_Linux_PNGImageWriter_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Imaging_Linux_PNGImageWriter);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Imaging_Linux_PNGImageWriter);
}
