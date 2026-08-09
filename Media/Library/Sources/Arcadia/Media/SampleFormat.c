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

#define ARCADIA_MEDIA_MODULE (1)
#include "Arcadia/Media/SampleFormat.h"

#include "Arcadia/Ring2/Include.h"

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
};

Arcadia_defineEnumerationType(u8"Arcadia.Media.SampleFormat", Arcadia_Media_SampleFormat,
                              &_typeOperations);

Arcadia_SizeValue
Arcadia_Media_SampleFormat_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_Media_SampleFormat self
  )
{
  switch (self) {
    case Arcadia_Media_SampleFormat_Natural8:
    case Arcadia_Media_SampleFormat_Integer8:
      return 1;
    case Arcadia_Media_SampleFormat_Natural16:
    case Arcadia_Media_SampleFormat_Integer16:
      return 2;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
}
