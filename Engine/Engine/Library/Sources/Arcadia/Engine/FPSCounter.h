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

#if !defined(ARCADIA_ENGINE_FPSCOUNTER_H_INCLUDED)
#define ARCADIA_ENGINE_FPSCOUNTER_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring2/Include.h"

#define Arcadia_Engine_FPSCounter_MaximumTextLength (9)

typedef struct Arcadia_Engine_FPSCounter {
  Arcadia_Real64Value accumulatedTicks;
  Arcadia_Real64Value numberOfFrames;
  Arcadia_Integer32Value* samples;
  Arcadia_SizeValue sampleCount;
  Arcadia_SizeValue sampleIndex;
  Arcadia_Integer32Value currentFps;
} Arcadia_Engine_FPSCounter;

void
Arcadia_Engine_FPSCounter_initialize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_FPSCounter* self
  );

void
Arcadia_Engine_FPSCounter_uninitialize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_FPSCounter* self
  );

void
Arcadia_Engine_FPSCounter_update
  (
    Arcadia_Engine_FPSCounter* self,
    Arcadia_Real64Value tick
  );

Arcadia_SizeValue
Arcadia_Engine_FPSCounter_format
  (
    Arcadia_Engine_FPSCounter const* self,
    char* buffer
  );

char const*
Arcadia_Engine_FPSCounter_getGlyphSource
  (
    void
  );

#endif // ARCADIA_ENGINE_FPSCOUNTER_H_INCLUDED
