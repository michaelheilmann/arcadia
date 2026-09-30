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

#include "Arcadia/Visuals/Implementation/OpenGL4/Capture.h"

Arcadia_Media_PixelBuffer*
Arcadia_Engine_Visuals_Implementation_OpenGL4_readPixels
  (
    Arcadia_Thread* thread,
    _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl,
    GLuint frameBufferID,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  if (width < 1 || height < 1) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }

  // The frame buffer binding in effect on entry. Restored before the error state is inspected.
  GLint previousFrameBufferID = 0;
  gl->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBufferID);
  gl->glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

  // The error state is a single global flag which is not owned by any particular call. Drain
  // it such that the inspection below is attributable to the read and does not report an
  // error left behind by unrelated code.
  while (gl->glGetError()) {/*Intentionally empty.*/}

  // "glReadPixels" with "GL_RGBA" and "GL_UNSIGNED_BYTE" stores the components of a pixel in
  // the order red, green, blue, alpha. That is the memory layout of
  // "Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8".
  // A line padding of 0 makes the line stride "width * 4" and therefore tightly packed.
  // @todo Proper encapsulation: there is no accessor for the bytes of a pixel buffer.
  Arcadia_Media_PixelBuffer* pixelBuffer = Arcadia_Media_PixelBuffer_create
    (
      thread,
      0,
      width,
      height,
      Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8
    );

  // "glReadPixels" places the row at the lower edge of the region in the first row of the
  // destination, whereas a pixel buffer stores the first row at the top. The rows are
  // reflected after the read.
  gl->glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer->bytes);

  gl->glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)previousFrameBufferID);

  if (gl->glGetError()) {
    // The pixel buffer is unreachable and hence collected. Nothing is returned to the caller.
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }

  // "Arcadia_Media_PixelBuffer_reflectHorizontally" moves the pixel at position (column, row)
  // to position (column, height - 1 - row), that is, it reflects the rows. Despite its name
  // this is the reflection required here; "Arcadia_Media_PixelBuffer_reflectVertically"
  // reflects the columns.
  // This does not raise, hence the pixel buffer stays reachable until it is returned.
  Arcadia_Media_PixelBuffer_reflectHorizontally(thread, pixelBuffer);

  return pixelBuffer;
}
