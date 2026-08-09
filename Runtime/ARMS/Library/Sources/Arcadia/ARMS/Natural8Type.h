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

#if !defined(ARCADIA_ARMS_NATURAL8TYPE_H_INCLUDED)
#define ARCADIA_ARMS_NATURAL8TYPE_H_INCLUDED

#include "Arcadia/ARMS/Configure.h"

// uint8_t, UINT8_C, UINT8_MAX
#include <stdint.h>

// An unsigned binary integer of a width of 8 Bits.
typedef uint8_t Arcadia_ARMS_Natural8;

/// The minimum value of Arms_Natural8.
/// Guaranteed to be @a 0.
#define Arcadia_ARMS_Natural8_Minimum (UINT8_C(0))

/// The maximum value of Arms_Natural8.
#define Arcadia_ARMS_Natural8_Maximum (UINT8_MAX)

#endif // ARCADIA_ARMS_NATURAL8TYPE_H_INCLUDED
