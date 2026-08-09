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

#if !defined(ARCADIA_ARMS_SIZETYPE_H_INCLUDED)
#define ARCADIA_ARMS_SIZETYPE_H_INCLUDED

#include "Arcadia/ARMS/Configure.h"

// size_t
#include <stddef.h>
// SIZE_MAX
#include <stdint.h>

// A non-negative binary integer.
// Its width, in Bits, is at least 16.
// It is large enough to fit the result of Arms_SizeOf, Arms_AlignOf, and Arms_OffsetOf.
typedef size_t Arcadia_ARMS_Size;

/// The minimum value of Arms_Size.
/// Guaranteed to be @a 0.
#define Arcadia_ARMS_Size_Minimum ((Arcadia_ARMS_Size)0)

/// The maximum value of Arms_Size.
#define Arcadia_ARMS_Size_Maximum (SIZE_MAX)

#endif // ARCADIA_ARMS_SIZETYPE_H_INCLUDED
