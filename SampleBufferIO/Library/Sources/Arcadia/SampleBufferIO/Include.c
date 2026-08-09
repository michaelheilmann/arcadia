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

#include "Arcadia/SampleBufferIO/Include.h"

#define FLAC__NO_DLL
#include "FLAC/metadata.h"
#include "FLAC/stream_encoder.h"

void
Arcadia_SampleBufferIO_encodeFLAC
  (
    Arcadia_Thread* thread
  )
{
  // (1) Open the encoder.
  FLAC__StreamEncoder* encoder = FLAC__stream_encoder_new();
  if (!encoder) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // (2) Close the encoder.
  FLAC__stream_encoder_finish(encoder);
  encoder = NULL;
}
