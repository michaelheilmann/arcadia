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

#if !defined(ARCADIA_ENGINE_FONTCACHE_H_INCLUDED)
#define ARCADIA_ENGINE_FONTCACHE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/FontIO/AtlasBitmapFont.h"

Arcadia_declareObjectType(u8"Arcadia.Engine.FontCache", Arcadia_Engine_FontCache,
                          u8"Arcadia.Object");

struct Arcadia_Engine_FontCacheDispatch {
  Arcadia_ObjectDispatch parent;
};

Arcadia_Engine_FontCache*
Arcadia_Engine_FontCache_create
  (
    Arcadia_Thread* thread
  );

Arcadia_FontIO_AtlasBitmapFont*
Arcadia_Engine_FontCache_getOrCreate
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_FontCache* self,
    char const* fontPath,
    Arcadia_Natural32Value pixelSize
  );

#endif // ARCADIA_ENGINE_FONTCACHE_H_INCLUDED
