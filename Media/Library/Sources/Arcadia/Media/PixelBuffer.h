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

#if !defined(ARCADIA_MEDIA_PIXELBUFFER_H_INCLUDED)
#define ARCADIA_MEDIA_PIXELBUFFER_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Media/PixelFormat.h"

// https://michaelheilmann.com/Arcadia/Media/#Arcadia_Media_PixelBuffer
Arcadia_declareObjectType(u8"Arcadia.Media.PixelBuffer", Arcadia_Media_PixelBuffer,
                          u8"Arcadia.Object");

struct Arcadia_Media_PixelBufferDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_Media_PixelBuffer {
  Arcadia_Object _parent;
  /// A pointer to the Bytes of this pixel buffer.
  Arcadia_Natural8Value* bytes;
  /// The pixel format of this pixel buffer.
  Arcadia_Media_PixelFormat pixelFormat;
  /// The height, in pixels, of this pixel buffer.
  Arcadia_Integer32Value height;
  /// The width, in pixels, of this pixel buffer.
  Arcadia_Integer32Value width;
  /// The line padding of this pixel buffer.
  Arcadia_Integer32Value linePadding;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// https://michaelheilmann.com/Arcadia/Media/#Arcadia_Media_PixelBuffer_getPixelFormat
Arcadia_Media_PixelFormat
Arcadia_Media_PixelBuffer_getPixelFormat
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  );

/// @brief Get the pixel format of this pixel buffer.
/// @param self A pointer to this pixel buffer.
/// @param format The format.
/// @remarks
/// When converting from BGR or RGB to ABGR, ARGB, BGRA, or RGBA,
/// an alpha component with value 255 is added.
/// When converting from ABGR, ARGB, BGRA, or RGBA to BGR or RGB,
/// the alpha component is removed.
void
Arcadia_Media_PixelBuffer_setPixelFormat
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Natural8Value pixelFormat
  );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// https://michaelheilmann.com/Arcadia/Media/#Arcadia_Media_PixelBuffer_getLinePadding
Arcadia_Integer32Value
Arcadia_Media_PixelBuffer_getLinePadding
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  );

/// @brief Set the line padding of this pixel buffer.
/// @param self A pointer to this pixel buffer.
/// @param linePadding The line padding. Must be non-negative.
/// @warning Can result in reallocation.
/// @warning The contents of the padding Bytes are unspecified.
void
Arcadia_Media_PixelBuffer_setLinePadding
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Integer32Value linePadding
  );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Get component values of a pixel at the specified position in this pixel buffer.
/// @param self A pointer to this pixel buffer.
/// @param column The column index of the pixel.
/// Must be non-negative. Must be smaller than or equal to the number of columns.
/// @param row The row of the pixel.
/// Must be non-negative. Must be smaller than or equal to the number of rows.
/// @param r A pointer to a <code>Arcadia_Natural8Value</code> variable.
/// @param g A pointer to a <code>Arcadia_Natural8Value</code> variable.
/// @param b A pointer to a <code>Arcadia_Natural8Value</code> variable.
/// @param a A pointer to a <code>Arcadia_Natural8Value</code> variable.
/// @pre The pixel buffer must of a format ABGR, ARGB, BGR, BGRA, RGB, or RGBA.
/// The component values of the format must be an integer within the range of of [0,255].
/// @post
/// On success:
/// <code>*r</code> is assigned the red component of the pixel.
/// <code>*g</code> is assigned the green component of the pixel.
/// <code>*b</code> is assigned the blue component of the pixel.
/// <code>*a</code> is assigned the alpha component of the pixel
/// if the pixel has an alpha component.
/// Otherwise it is assigned 255.
void
Arcadia_Media_PixelBuffer_getPixelRGBA
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Integer32Value column,
    Arcadia_Integer32Value row,
    Arcadia_Natural8Value* r,
    Arcadia_Natural8Value* g,
    Arcadia_Natural8Value* b,
    Arcadia_Natural8Value* a
  );

/// @brief Set component values of a pixel at the specified position in this pixel buffer.
/// @param self A pointer to this pixel buffer.
/// @param column The column index of the pixel.
/// Must be non-negative. Must be smaller than or equal to the number of columns.
/// @param row The row of the pixel.
/// Must be non-negative. Must be smaller than or equal to the number of rows.
/// @param r The red component value.
/// @param g The green component value.
/// @param b The blue component value.
/// @param a The alpha component value.
/// @pre The pixel buffer must of a format ABGR, ARGB, BGR, BGRA, RGB, or RGBA.
/// The component values of the format must be an integer within the range of of [0,255].
/// @post
/// On success:
/// <code>r</code> is assigned to the red component of the pixel.
/// <code>g</code> is assigned to the green component of the pixel.
/// <code>a</code> is assigned to the blue component of the pixel.
/// <code>a</code> is assigned to the alpha component of the pixel
/// if the pixel has an alpha component.
void
Arcadia_Media_PixelBuffer_setPixelRGBA
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Integer32Value column,
    Arcadia_Integer32Value row,
    Arcadia_Natural8Value r,
    Arcadia_Natural8Value g,
    Arcadia_Natural8Value b,
    Arcadia_Natural8Value a
  );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Set component values of the pixels in this pixel buffer.
/// @param self A pointer to this pixel buffer.
/// @param left The left side of the pixel rectangle to fill.
/// @param top The top side of the pixel rectangle to fill.
/// @param width The width of the pixel rectangle to fill.
/// @param height The height of the pixel rectagnel to fill.
/// @param r The red component value.
/// @param g The green component value.
/// @param b The blue component value.
/// @param a The alpha component value.
/// @error Arcadia_Status_ArgumentValueInvalid @a width and/or @a height is negative.
/// @error Arcadia_Status_ArgumentValueInvalid <code>left + width</code> and/or <code>top + height</code> would overflow.
/// @error Arcadia_Status_OperationInvalid The pixel buffer is not of the format ABGR, ARGB, BGR, BGRA, RGB, or RGBA.
/// @remarks
///
/// @post
/// On success:
/// <code>r</code> is assigned to the red component of the pixel.
/// <code>g</code> is assigned to the green component of the pixel.
/// <code>a</code> is assigned to the blue component of the pixel.
/// <code>a</code> is assigned to the alpha component of the pixel
/// if the pixel has an alpha component.
void
Arcadia_Media_PixelBuffer_fillRectangle
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value top,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height,
    Arcadia_Natural8Value r,
    Arcadia_Natural8Value g,
    Arcadia_Natural8Value b,
    Arcadia_Natural8Value a
  );

/// @brief Set component values of the pixels in this pixel buffer.
/// @param self A pointer to this pixel buffer.
/// @param r The red component value.
/// @param g The green component value.
/// @param b The blue component value.
/// @param a The alpha component value.
/// @pre The pixel buffer must of a format ABGR, ARGB, BGR, BGRA, RGB, or RGBA.
/// The component values of the format must be an integer within the range of of [0,255].
/// @post
/// On success:
/// <code>r</code> is assigned to the red component of the pixel.
/// <code>g</code> is assigned to the green component of the pixel.
/// <code>a</code> is assigned to the blue component of the pixel.
/// <code>a</code> is assigned to the alpha component of the pixel
/// if the pixel has an alpha component.
void
Arcadia_Media_PixelBuffer_fill
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Natural8Value r,
    Arcadia_Natural8Value g,
    Arcadia_Natural8Value b,
    Arcadia_Natural8Value a
  );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Swap columns.
/// @param self A pointer to this pixel buffer.
/// @param column1 The first column.
/// @param column2 The second colum.
void
Arcadia_Media_PixelBuffer_swapColumns
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Integer32Value column1,
    Arcadia_Integer32Value column2
  );

/// @brief Swap row.
/// @param self A pointer to this pixel buffer.
/// @param row1 The first row.
/// @param row2 The second row.
void
Arcadia_Media_PixelBuffer_swapRows
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Integer32Value row1,
    Arcadia_Integer32Value row2
  );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Mirror this pixel buffer vertically.
/// @param self A pointer to this pixel buffer.
/// @post The pixel buffer was reflected horizontally.
/// That is, it was reflected by the axis starting at (width / 2, -infinity) to (width / 2, +infinity).
/// such that the pixel at position (column, row) is now at position (width - 1 - column, row).
void
Arcadia_Media_PixelBuffer_reflectVertically
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  );

/// @brief Reflect this pixel buffer horizontally.
/// @param self A pointer to this pixel buffer.
/// @post The pixel buffer was reflected horizontally.
/// That is, it was reflected by the axis starting at (-infinity, height / 2) to (+infinity, height / 2).
/// such that the pixel at position (column, row) is now at position (column, height - 1 - row).
void
Arcadia_Media_PixelBuffer_reflectHorizontally
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// https://michaelheilmann.com/Arcadia/Media/#Arcadia_Media_PixelBuffer_getNumberOfColumns
Arcadia_Integer32Value
Arcadia_Media_PixelBuffer_getNumberOfColumns
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  );

/// @brief Set the number of columns of this pixel buffer.
/// @param self A pointer to this pixel buffer.
/// @param numberOfColumns The new number of columns of this pixel buffer.
/// @remarks Columns are added to /remove from the right side.
void
Arcadia_Media_PixelBuffer_setNumberOfColumns
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Integer32Value numberOfColumns
  );

// https://michaelheilmann.com/Arcadia/Media/#Arcadia_Media_PixelBuffer_getNumberOfRows
Arcadia_Integer32Value
Arcadia_Media_PixelBuffer_getNumberOfRows
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  );

/// @brief Set the number of rows of this pixel buffer.
/// @param self A pointer to this pixel buffer.
/// @param numberOfRows The new number of rows of this pixel buffer.
/// @remarks Rows are added to/remove from the bottom side.
void
Arcadia_Media_PixelBuffer_setNumberOfRows
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Integer32Value numberOfRows
  );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// https://michaelheilmann.com/Arcadia/Media/#Arcadia_Media_PixelBuffer_getLineStride
Arcadia_Integer32Value
Arcadia_Media_PixelBuffer_getLineStride
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Get the Bytes per pixel of this pixel buffer.
/// @param self A pointer to this pixel buffer.
/// @return The line stride of this pixel buffer.
Arcadia_Integer32Value
Arcadia_Media_PixelBuffer_getBytesPerPixel
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// https://michaelheilmann.com/Arcadia/Media/#Arcadia_Media_PixelBuffer_create
Arcadia_Media_PixelBuffer*
Arcadia_Media_PixelBuffer_create
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value linePadding,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height,
    Arcadia_Natural8Value pixelFormat
  );

Arcadia_Media_PixelBuffer*
Arcadia_Media_PixelBuffer_createClone
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* other
  );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// https://michaelheilmann.com/Arcadia/Media/#Arcadia_Media_PixelBuffer_getWidth
Arcadia_Integer32Value
Arcadia_Media_PixelBuffer_getWidth
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  );

/// @brief Set the width of this pixel buffer.
/// @param self A pointer to this pixel buffer.
/// @param width The new width of this pixel buffer.
/// @remarks Columns are added to /remove from the right side.
void
Arcadia_Media_PixelBuffer_setWidth
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Integer32Value width
  );

// https://michaelheilmann.com/Arcadia/Media/#Arcadia_Media_PixelBuffer_getHeight
Arcadia_Integer32Value
Arcadia_Media_PixelBuffer_getHeight
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  );

/// @brief Set the height of this pixel buffer.
/// @param self A pointer to this pixel buffer.
/// @param height The new height of this pixel buffer.
/// @remarks Rows are added to/remove from the bottom side.
void
Arcadia_Media_PixelBuffer_setHeight
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Integer32Value height
  );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Swap the contents of this pixel buffer with the contents of another pixel buffer.
/// @param thread A pointer to this thread.
/// @param self A pointer to this pixel buffer.
/// @param other A pointer to the other pixel buffer.
void
Arcadia_Media_PixelBuffer_swap
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Media_PixelBuffer* other
  );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void
Arcadia_Media_PixelBuffer_assign
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Media_PixelBuffer* other
  );

#endif // ARCADIA_MEDIA_PIXELBUFFER_H_INCLUDED
