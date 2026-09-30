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

#if !defined(ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_CAPTURE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_CAPTURE_H_INCLUDED

#include "Arcadia/Engine/Include.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/BackendIncludes.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/Functions.h"

/// @brief Read a rectangular region of a frame buffer into a new pixel buffer.
/// @param thread A pointer to this thread.
/// @param gl A pointer to the OpenGL 4 function table.
/// @param frameBufferID The OpenGL ID of the frame buffer to read from.
/// Pass 0 to read from the default frame buffer.
/// @param width The width, in pixels, of the region to read.
/// Must be positive.
/// @param height The height, in pixels, of the region to read.
/// Must be positive.
/// @return A pointer to a new pixel buffer of #Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8
/// holding the lower left corner of the region in its upper left corner.
/// @pre An OpenGL context must be current.
/// @post
/// On Success:
/// - The frame buffer binding in effect on entry was restored.
/// @remarks
/// The frame buffer binding is restored before the error state is inspected, such that a
/// failing read does not leave an arbitrary frame buffer bound.
/// @remarks
/// No pixel pack state is touched. The default pack alignment of 4 is relied upon, which
/// makes the rows of a #Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8 region tightly
/// packed and therefore directly addressable by "glReadPixels".
Arcadia_Media_PixelBuffer*
Arcadia_Engine_Visuals_Implementation_OpenGL4_readPixels
  (
    Arcadia_Thread* thread,
    _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl,
    GLuint frameBufferID,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

#endif // ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_CAPTURE_H_INCLUDED
