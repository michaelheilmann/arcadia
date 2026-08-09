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

#include "Arcadia/Visuals/Implementation/Windows/Bitmap.h"

static void
Arcadia_Engine_Visuals_Windows_Bitmap_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_Bitmap* self
  );

static void
Arcadia_Engine_Visuals_Windows_Bitmap_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_BitmapDispatch* self
  );

static void
Arcadia_Engine_Visuals_Windows_Bitmap_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_Bitmap* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_Windows_Bitmap_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_Windows_Bitmap_destruct,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_Windows_Bitmap_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.Windows.Bitmap", Arcadia_Engine_Visuals_Windows_Bitmap,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_Windows_Bitmap_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_Bitmap* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_Windows_Bitmap);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_Integer32Value width = Arcadia_ValueStack_getInteger32Value(thread, 2);
  Arcadia_Integer32Value height = Arcadia_ValueStack_getInteger32Value(thread, 1);

  self->hBitmap = NULL;
  self->hDeviceContext = NULL;

  HDC hScreenDeviceContext = GetDC(NULL);
  if (!hScreenDeviceContext) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  self->hDeviceContext = CreateCompatibleDC(hScreenDeviceContext);
  if (!self->hDeviceContext) {
    ReleaseDC(NULL, hScreenDeviceContext);
    hScreenDeviceContext = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  ReleaseDC(NULL, hScreenDeviceContext);
  hScreenDeviceContext = NULL;

  BITMAPINFO bmi;
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight = height;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 24;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biSizeImage = (bmi.bmiHeader.biWidth * bmi.bmiHeader.biHeight) * 3;

  self->hBitmap = CreateDIBSection(self->hDeviceContext, &bmi, DIB_RGB_COLORS, NULL, NULL, 0);
  if (!self->hBitmap) {
    DeleteDC(self->hDeviceContext);
    self->hDeviceContext = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  if (bmi.bmiHeader.biBitCount != 24) {
    DeleteObject(self->hBitmap);
    self->hBitmap = NULL;
    DeleteDC(self->hDeviceContext);
    self->hDeviceContext = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  self->width = width;
  self->height = height;
  DWORD lineStride = ((((bmi.bmiHeader.biWidth * bmi.bmiHeader.biBitCount) + 31) & ~31) >> 3);
  DWORD linePadding = lineStride - ((bmi.bmiHeader.biWidth * bmi.bmiHeader.biBitCount) >> 3);
  if (lineStride > INT32_MAX) {
    DeleteObject(self->hBitmap);
    self->hBitmap = NULL;
    DeleteDC(self->hDeviceContext);
    self->hDeviceContext = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  self->lineStride = (int32_t)lineStride;
  self->linePadding = (uint32_t)linePadding;
  self->numberOfBitsPerPixel = 24;
  self->pixelFormat = Arcadia_Media_PixelFormat_BlueGreenRedNatural8;

  HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
  if (!hBrush) {
    DeleteObject(self->hBitmap);
    self->hBitmap = NULL;
    DeleteDC(self->hDeviceContext);
    self->hDeviceContext = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  SelectObject(self->hDeviceContext, self->hBitmap);
  RECT fillRetc = { .left = 0, .top = 0, .right = self->width, .bottom = self->height };
  FillRect(self->hDeviceContext, &fillRetc, hBrush);
  DeleteObject(hBrush);
  hBrush = NULL;

  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_Windows_Bitmap);
}

static void
Arcadia_Engine_Visuals_Windows_Bitmap_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_BitmapDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_Windows_Bitmap_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_Bitmap* self
  )
{
  if (NULL != self->hBitmap) {
    DeleteObject(self->hBitmap);
    self->hBitmap = NULL;
  }
  if (NULL != self->hDeviceContext) {
    DeleteDC(self->hDeviceContext);
    self->hDeviceContext = NULL;
  }
}

Arcadia_Engine_Visuals_Windows_Bitmap*
Arcadia_Engine_Visuals_Windows_Bitmap_create
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_Windows_Bitmap);
  Arcadia_ValueStack_pushInteger32Value(thread, width);
  Arcadia_ValueStack_pushInteger32Value(thread, height);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_Windows_Bitmap);
}

void
Arcadia_Engine_Visuals_Windows_Bitmap_fill
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_Bitmap* self,
    Arcadia_Natural8Value r,
    Arcadia_Natural8Value g,
    Arcadia_Natural8Value b
  )
{
  HBRUSH hBrush = CreateSolidBrush(RGB(r, g, b));
  if (!hBrush) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  SelectObject(self->hDeviceContext, self->hBitmap);
  RECT fillRetc = { .left = 0, .top = 0, .right = self->width, .bottom = self->height };
  FillRect(self->hDeviceContext, &fillRetc, hBrush);
  DeleteObject(hBrush);
  hBrush = NULL;
}

Arcadia_Media_PixelBuffer*
Arcadia_Engine_Visuals_Windows_Bitmap_toPixelBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_Bitmap* self
  )
{
  DIBSECTION dibSection;
  if (!GetObject(self->hBitmap, sizeof(DIBSECTION), &dibSection)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  uint8_t* sourceBytes = dibSection.dsBm.bmBits;
  // Currently, we assume that NativeWindowsBitmap is BGR format.
  Arcadia_Media_PixelBuffer* pixelBuffer = Arcadia_Media_PixelBuffer_create(thread, 0, self->width, self->height, Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8);
  for (int32_t y = 0; y < self->height; ++y) {
    for (int32_t x = 0; x < self->width; ++x) {
      int32_t sourceOffset = self->lineStride * y + (x * self->numberOfBitsPerPixel) / 8;
      uint8_t* source = sourceBytes + sourceOffset;
      Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixelBuffer, x, y, source[2], source[1], source[0], 255);
    }
  }
  Arcadia_Media_PixelBuffer_reflectHorizontally(thread, pixelBuffer);
  return pixelBuffer;
}
