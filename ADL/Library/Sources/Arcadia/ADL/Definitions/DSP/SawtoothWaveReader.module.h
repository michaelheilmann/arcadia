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

#if !defined(ARCADIA_ADL_DEFINITIONS_DSP_SAWTOOTHWAVEREADER_MODULE_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_DSP_SAWTOOTHWAVEREADER_MODULE_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#if defined(ARCADIA_ADL_EXPORT)
  #error("this file is not supposed to be exported")
#endif
#include "Arcadia/ADL/Reader.h"
#include "Arcadia/DDLS/Include.h"

Arcadia_declareObjectType(u8"Arcadia.ADL.SawtoothWaveReader", Arcadia_ADL_SawtoothWaveReader,
                          u8"Arcadia.ADL.Reader");

struct Arcadia_ADL_SawtoothWaveReaderDispatch {
  Arcadia_ADL_ReaderDispatch _parent;
};

struct Arcadia_ADL_SawtoothWaveReader {
  Arcadia_ADL_Reader _parent;

  // The string `type` cached here.
  Arcadia_String* TYPE;
  // The string `name` cached here.
  Arcadia_String* NAME;
  // The string `DSP.SawtoothWaveGenerator` cached here.
  Arcadia_String* SCHEMANAME;
  // The string `DSP.SawtoothWaveGenerator` cached here.
  Arcadia_String* TYPENAME;
  // The string `frequency` cached here.
  Arcadia_String* FREQUENCY;
  // The validation context. Cached here.
  Arcadia_DDLS_ValidationContext* validationContext;
};

Arcadia_ADL_SawtoothWaveReader*
Arcadia_ADL_SawtoothWaveReader_create
  (
    Arcadia_Thread* thread
  );

#endif  // ARCADIA_ADL_DEFINITIONS_DSP_SAWTOOTHWAVEREADER_MODULE_H_INCLUDED
