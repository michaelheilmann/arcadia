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

#if !defined(ARCADIA_ARMS_REFERENCECOUNTER_H_INCLUDED)
#define ARCADIA_ARMS_REFERENCECOUNTER_H_INCLUDED

#include "Arcadia/ARMS/Configure.h"
#include "Arcadia/ARMS/SizeType.h"
#include "Arcadia/ARMS/StatusType.h"

/* The type of a reference counter. */
typedef Arcadia_ARMS_Size Arcadia_ARMS_ReferenceCounter;

/* The minimum value of a reference counter. */
#define Arcadia_ARMS_ReferenceCounter_Minimum (Arcadia_ARMS_Size_Minimum)

/* The maximum value of a reference counter. */
#define Arcadia_ARMS_ReferenceCounter_Maximum (Arcadia_ARMS_Size_Maximum)

#endif // ARCADIA_ARMS_REFERENCECOUNTER_H_INCLUDED
