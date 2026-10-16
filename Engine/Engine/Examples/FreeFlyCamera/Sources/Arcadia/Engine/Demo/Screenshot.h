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

#if !defined(ARCADIA_ENGINE_DEMO_SCREENSHOT_H_INCLUDED)
#define ARCADIA_ENGINE_DEMO_SCREENSHOT_H_INCLUDED

#include "Arcadia/Engine/Include.h"

/// @brief Capture the pixels of a window and write them to a PNG file.
/// @param thread A pointer to this thread.
/// @param window A pointer to the window to capture.
/// @param index The number to embed in the file name, which is "Screenshot-<index>.png".
/// @pre
/// A successful call to Arcadia_Engine_Visuals_Window_beginRender must be in effect and its
/// matching Arcadia_Engine_Visuals_Window_endRender must not have been called yet, as the
/// capture operates on the render target which is current between those two calls.
/// @post
/// On Success:
/// - The file "Screenshot-<index>.png" was written to the working directory.
void
Arcadia_Engine_Demo_writeScreenshot
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* window,
    Arcadia_Natural32Value index
  );

#endif // ARCADIA_ENGINE_DEMO_SCREENSHOT_H_INCLUDED
