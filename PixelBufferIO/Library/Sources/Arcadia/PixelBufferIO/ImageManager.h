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

#if !defined(ARCADIA_PIXELBUFFERIO_IMAGEMANAGER_H_INCLUDED)
#define ARCADIA_PIXELBUFFERIO_IMAGEMANAGER_H_INCLUDED

#include "Arcadia/Media/Include.h"

Arcadia_declareObjectType(u8"Arcadia.Imaging.ImageManager", Arcadia_Imaging_ImageManager,
                          u8"Arcadia.Object");

struct Arcadia_Imaging_ImageManagerDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_Imaging_ImageManager {
  Arcadia_Object _parent;
  Arcadia_List* writers;
};

/// @brief Get or create the image manager singleton object.
/// @param thread A pointer to this thread.
/// @return A pointer to the image manager singleton object.
Arcadia_Imaging_ImageManager*
Arcadia_Imaging_ImageManager_getOrCreate
  (
    Arcadia_Thread* thread
  );

/// @brief Get a list of image writers supporting the specified extension.
/// @param thread A pointer to this thread.
/// @param self A pointer to this image manager.
/// @return A pointer to a list of image writers.
Arcadia_List*
Arcadia_Imaging_ImageManager_getWriters
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageManager* self,
    Arcadia_String* extension
  );

#endif // ARCADIA_PIXELBUFFERIO_IMAGEMANAGER_H_INCLUDED
