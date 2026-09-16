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

#if !defined(ARCADIA_MEDIA_DSP_MIX_H_INCLUDED)
#define ARCADIA_MEDIA_DSP_MIX_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/Collections/Include.h"
#include "Arcadia/Media/DSP.h"

/// @brief A DSP node summing a dynamic list of input DSP nodes.
///
/// Rendering a mix node renders each input into a temporary internal buffer and
/// sums those buffers sample-by-sample into the target. Inputs are kept reachable
/// through the mix node's visit callback.
Arcadia_declareObjectType(u8"Arcadia.Media.DSP.Mix", Arcadia_Media_DSP_Mix,
                          u8"Arcadia.Media.DSP");

struct Arcadia_Media_DSP_MixDispatch {
  Arcadia_Media_DSPDispatch parent;
};

struct Arcadia_Media_DSP_Mix {
  Arcadia_Media_DSP parent;
  Arcadia_List* inputs;
};

/// @brief Create an empty mix node.
/// @param thread A pointer to this thread.
/// @return The created mix node.
Arcadia_Media_DSP_Mix*
Arcadia_Media_DSP_Mix_create
  (
    Arcadia_Thread* thread
  );

/// @brief Add an input DSP to a mix node.
/// @param thread A pointer to this thread.
/// @param self A pointer to this mix node.
/// @param input The input DSP to render and sum when this mix node renders.
void
Arcadia_Media_DSP_Mix_addInput
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Mix* self,
    Arcadia_Media_DSP* input
  );

#endif // ARCADIA_MEDIA_DSP_MIX_H_INCLUDED
