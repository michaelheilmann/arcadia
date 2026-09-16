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

#if !defined(ARCADIA_MEDIA_SAMPLEFORMAT_H_INCLUDED)
#define ARCADIA_MEDIA_SAMPLEFORMAT_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"

// https://michaelheilmann.com/Arcadia/Media/#Arcadia_Media_SampleFormat
Arcadia_declareEnumerationType(u8"Arcadia.Media.SampleFormat", Arcadia_Media_SampleFormat);

enum Arcadia_Media_SampleFormat {

  Arcadia_Media_SampleFormat_Integer16,

  Arcadia_Media_SampleFormat_Integer8,

  Arcadia_Media_SampleFormat_Natural16,

  Arcadia_Media_SampleFormat_Natural8,

  // TODO: Add to documentation.
  Arcadia_Media_SampleFormat_Real32,

};

// https://michaelheilmann.com/Arcadia/Media/#Arcadia_Media_SampleFormat_getNumberOfBytes
Arcadia_SizeValue
Arcadia_Media_SampleFormat_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_Media_SampleFormat self
  );

#endif // ARCADIA_MEDIA_SAMPLEFORMAT_H_INCLUDED
