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

#include "Arcadia/PixelBufferIO/Windows/WICImageWriterBase.h"

#include "Arcadia/PixelBufferIO/Windows/MultiByteToWideCharZeroTerminated.h"

// WIC library
#pragma comment (lib, "Windowscodecs.lib")

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Arcadia/PixelBufferIO/ImageWriterParameters.h"

static HRESULT
createMemoryStream
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_WICImageWriterBase* self,
    IStream** pStream,
    size_t initialSize
  )
{
  self->hMemory = GlobalAlloc(GMEM_MOVEABLE, initialSize);
  if (!self->hMemory) {
    return E_OUTOFMEMORY;
  }
  IStream* pStream1 = NULL;
  HRESULT hr = CreateStreamOnHGlobal(self->hMemory, FALSE, &pStream1);
  if (FAILED(hr)) {
    GlobalFree(self->hMemory);
    self->hMemory = NULL;
    return hr;
  }
  *pStream = pStream1;
  return S_OK;
}

static void
startupFactory
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_WICImageWriterBase* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_Imaging_ImageWriterParameters* parameters
  )
{
  HRESULT hr;
  //
  hr = CoInitialize(NULL);
  if (FAILED(hr)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  //
  hr = CoCreateInstance(&CLSID_WICImagingFactory,
                        NULL,
                        CLSCTX_INPROC_SERVER,
                        &IID_IWICImagingFactory,
                        (LPVOID*)&self->piFactory);
  if (FAILED(hr)) {
    CoUninitialize();
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  //
  hr = IWICImagingFactory_CreateStream(self->piFactory, &self->piStream);
  if (FAILED(hr)) {
    IWICImagingFactory_Release(self->piFactory);
    self->piFactory = NULL;
    CoUninitialize();
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
}

static void
shutdownFactory
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_WICImageWriterBase* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_Imaging_ImageWriterParameters* parameters
  )
{
  IWICStream_Release(self->piStream);
  self->piStream = NULL;
  //
  IWICImagingFactory_Release(self->piFactory);
  self->piFactory = NULL;
  //
  CoUninitialize();
}

static void
startup1
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_WICImageWriterBase* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_Imaging_ImageWriterParameters* parameters
  )
{
  HRESULT hr;
  //
  if (Arcadia_Imaging_ImageWriterParameters_hasPath(thread, parameters)) {
    Arcadia_ByteArrayBuilder* b = Arcadia_ByteArrayBuilder_create(thread);
    Arcadia_ByteArrayBuilder_insertBackBytes(thread, b, Arcadia_String_getBytes(thread, Arcadia_Imaging_ImageWriterParameters_getPath(thread, parameters)),
                                                  Arcadia_String_getNumberOfBytes(thread, Arcadia_Imaging_ImageWriterParameters_getPath(thread, parameters)));
    Arcadia_ByteArrayBuilder_insertBackBytes(thread, b, u8"", 1);
    wchar_t* targetPathW = Arcadia_Windows_multiByteToWideCharZeroTerminated(b->p);
    if (!targetPathW) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    hr = IWICStream_InitializeFromFilename(self->piStream, targetPathW, GENERIC_WRITE);
    free(targetPathW);
    if (FAILED(hr)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
  } else {
    hr = createMemoryStream(thread, self, &self->piMemoryStream, 8);
    if (FAILED(hr)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    hr = IWICStream_InitializeFromIStream(self->piStream, self->piMemoryStream);
    if (FAILED(hr)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
  }
}

static void
shutdown1
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_WICImageWriterBase* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_Imaging_ImageWriterParameters* parameters
  )
{
  if (self->piMemoryStream) {
    IStream_Release(self->piMemoryStream);
    self->piMemoryStream = NULL;
  }
  if (self->hMemory) {
    GlobalFree(self->hMemory);
    self->hMemory = NULL;
  }
}

static void
startupEncoder
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_WICImageWriterBase* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_Imaging_ImageWriterParameters* parameters
  )
{
  HRESULT hr;
  //
  GUID const* pFormat = NULL;
  Arcadia_Imaging_Windows_WICImageWriterBase_getFormat(thread, self, sourcePixelBuffer, parameters, &pFormat);
  if (NULL == pFormat) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  hr = IWICImagingFactory_CreateEncoder(self->piFactory, pFormat, NULL, &self->piEncoder);
  if (FAILED(hr)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  hr = IWICBitmapEncoder_Initialize(self->piEncoder, (IStream*)self->piStream, WICBitmapEncoderNoCache);
  if (FAILED(hr)) {
    IWICBitmapEncoder_Release(self->piEncoder);
    self->piEncoder = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  //
  hr = IWICBitmapEncoder_CreateNewFrame(self->piEncoder, &self->piBitmapFrame, &self->pPropertyBag);
  if (FAILED(hr)) {
    IWICBitmapEncoder_Release(self->piEncoder);
    self->piEncoder = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      Arcadia_Imaging_Windows_WICImageWriterBase_configure(thread, self, sourcePixelBuffer, parameters);
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      IPropertyBag2_Release(self->pPropertyBag);
      self->pPropertyBag = NULL;
      IWICBitmapFrameEncode_Release(self->piBitmapFrame);
      self->piBitmapFrame = NULL;
      IWICBitmapEncoder_Release(self->piEncoder);
      self->piEncoder = NULL;
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
  }
  hr = IWICBitmapFrameEncode_Initialize(self->piBitmapFrame, self->pPropertyBag);
  if (FAILED(hr)) {
    IPropertyBag2_Release(self->pPropertyBag);
    self->pPropertyBag = NULL;
    IWICBitmapFrameEncode_Release(self->piBitmapFrame);
    self->piBitmapFrame = NULL;
    IWICBitmapEncoder_Release(self->piEncoder);
    self->piEncoder = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
}

static void
shutdownEncoder
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_WICImageWriterBase* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_Imaging_ImageWriterParameters* parameters
  )
{
  IPropertyBag2_Release(self->pPropertyBag);
  self->pPropertyBag = NULL;
  IWICBitmapFrameEncode_Release(self->piBitmapFrame);
  self->piBitmapFrame = NULL;
  IWICBitmapEncoder_Release(self->piEncoder);
  self->piEncoder = NULL;
}

static void
startup3
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_WICImageWriterBase* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_Imaging_ImageWriterParameters* parameters
  )
{
  HRESULT hr;
  //
  size_t sourceWidth = Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, sourcePixelBuffer);
  size_t sourceHeight = Arcadia_Media_PixelBuffer_getNumberOfRows(thread, sourcePixelBuffer);
  hr = IWICBitmapFrameEncode_SetSize(self->piBitmapFrame, (UINT)sourceWidth, (UINT)sourceHeight);
  if (FAILED(hr)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  //
  WICPixelFormatGUID formatGUID = GUID_WICPixelFormatUndefined;
  switch (sourcePixelBuffer->pixelFormat) {
    case Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8: {
      Arcadia_Media_PixelBuffer* pixelBuffer = Arcadia_Media_PixelBuffer_createClone(thread, sourcePixelBuffer);
      Arcadia_Media_PixelBuffer_setPixelFormat(thread, pixelBuffer, Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8);
      sourcePixelBuffer = pixelBuffer;
      formatGUID = GUID_WICPixelFormat32bppBGRA;
    } break;
    case Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8: {
      Arcadia_Media_PixelBuffer* pixelBuffer = Arcadia_Media_PixelBuffer_createClone(thread, sourcePixelBuffer);
      Arcadia_Media_PixelBuffer_setPixelFormat(thread, pixelBuffer, Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8);
      sourcePixelBuffer = pixelBuffer;
      formatGUID = GUID_WICPixelFormat32bppBGRA;
    } break;
    case Arcadia_Media_PixelFormat_BlueGreenRedNatural8: {
      formatGUID = GUID_WICPixelFormat24bppBGR;
    } break;
    case Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8: {
      formatGUID = GUID_WICPixelFormat32bppBGRA;
    } break;
    case Arcadia_Media_PixelFormat_RedGreenBlueNatural8: {
      formatGUID = GUID_WICPixelFormat24bppBGR;
    } break;
    case Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8: {
      formatGUID = GUID_WICPixelFormat32bppRGBA;
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    } break;
  };
  hr = IWICBitmapFrameEncode_SetPixelFormat(self->piBitmapFrame, &formatGUID);
  if (FAILED(hr)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  size_t sourceLineStride = Arcadia_Media_PixelBuffer_getLineStride(thread, sourcePixelBuffer);
  size_t sourceSizeInPixels = sourceLineStride * sourceHeight;
  hr = IWICBitmapFrameEncode_WritePixels(self->piBitmapFrame, (UINT)sourceHeight, (UINT)sourceLineStride, (UINT)sourceSizeInPixels, (void*)sourcePixelBuffer->bytes);
  if (FAILED(hr)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }

  hr = IWICBitmapFrameEncode_Commit(self->piBitmapFrame);
  if (FAILED(hr)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }

  hr = IWICBitmapEncoder_Commit(self->piEncoder);
  if (FAILED(hr)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  if (Arcadia_Imaging_ImageWriterParameters_hasByteBuffer(thread, parameters)) {
    STATSTG statstg;
    hr = IStream_Stat(self->piStream, &statstg, STATFLAG_NONAME);
    if (FAILED(hr)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    if (statstg.cbSize.QuadPart > SIZE_MAX) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    size_t n = (size_t)statstg.cbSize.QuadPart;
    LPVOID p = GlobalLock(self->hMemory);
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      Arcadia_ByteArrayBuilder_insertBackBytes(thread, Arcadia_Imaging_ImageWriterParameters_getByteBuffer(thread, parameters), p, n);
      GlobalUnlock(self->hMemory);
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      GlobalUnlock(self->hMemory);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Thread_jump(thread);
    }
  }
}

static void
shutdown3
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_WICImageWriterBase* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_Imaging_ImageWriterParameters* parameters
  )
{ }

typedef struct Module {
  void (*startup)(Arcadia_Thread* thread, Arcadia_Imaging_Windows_WICImageWriterBase*, Arcadia_Media_PixelBuffer* sourcePixelBuffer, Arcadia_Imaging_ImageWriterParameters* parameters);
  void (*shutdown)(Arcadia_Thread* thread, Arcadia_Imaging_Windows_WICImageWriterBase*, Arcadia_Media_PixelBuffer* sourcePixelBuffer, Arcadia_Imaging_ImageWriterParameters* parameters);
} Module;

static void
Arcadia_Imaging_Windows_WICImageWriterBase_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_WICImageWriterBase* self
  );

static void
Arcadia_Imaging_Windows_WICImageWriterBase_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_WICImageWriterBaseDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*) & Arcadia_Imaging_Windows_WICImageWriterBase_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Imaging_Windows_WICImageWriterBase_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Imaging.Windows.WICImageWriterBase", Arcadia_Imaging_Windows_WICImageWriterBase,
                         u8"Arcadia.Imaging.ImageWriter", Arcadia_Imaging_ImageWriter,
                         &_typeOperations);

static void
Arcadia_Imaging_Windows_WICImageWriterBase_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_WICImageWriterBase* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Imaging_Windows_WICImageWriterBase);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  self->piFactory = NULL;
  self->hMemory = NULL;
  self->piMemoryStream = NULL;
  self->piStream = NULL;
  self->piEncoder = NULL;
  self->piBitmapFrame = NULL;
  self->pPropertyBag = NULL;

  Arcadia_LeaveConstructor(Arcadia_Imaging_Windows_WICImageWriterBase);
}

static void
Arcadia_Imaging_Windows_WICImageWriterBase_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_WICImageWriterBaseDispatch* self
  )
{ }

void
Arcadia_Imaging_Windows_WICImageWriterBase_doWrite
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_WICImageWriterBase* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_Imaging_ImageWriterParameters* parameters
  )
{
  const Module modules[] = {
   { &startupFactory, &shutdownFactory },
   { &startup1, &shutdown1 },
   { &startupEncoder, &shutdownEncoder },
   { &startup3, &shutdown3 },
  };
  const size_t numberOfModules = sizeof(modules) / sizeof(Module);
  size_t currentModule = 0;

  while (currentModule < numberOfModules) {
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      modules[currentModule++].startup(thread, self, sourcePixelBuffer, parameters);
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      while (currentModule > 0) {
        modules[--currentModule].shutdown(thread, self, sourcePixelBuffer, parameters);
      }
      Arcadia_Thread_jump(thread);
    }
  }

  while (currentModule > 0) {
    modules[--currentModule].shutdown(thread, self, sourcePixelBuffer, parameters);
  }
}

void
Arcadia_Imaging_Windows_WICImageWriterBase_getFormat
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_WICImageWriterBase* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_Imaging_ImageWriterParameters* parameters,
    GUID const** guid
  )
{ Arcadia_VirtualCall(Arcadia_Imaging_Windows_WICImageWriterBase, getFormat, self, sourcePixelBuffer, parameters, guid); }

void
Arcadia_Imaging_Windows_WICImageWriterBase_configure
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_WICImageWriterBase* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_Imaging_ImageWriterParameters* parameters
  )
{ Arcadia_VirtualCall(Arcadia_Imaging_Windows_WICImageWriterBase, configure, self, sourcePixelBuffer, parameters); }
