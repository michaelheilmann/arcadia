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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_SIGNALS_INTERNAL_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_SIGNALS_INTERNAL_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Thread.h"
#include "Arcadia/Ring1/Implementation/ThreadExtensions.h"
#include "Arcadia/Ring1/Implementation/Objects/WeakReference.h"
typedef struct Arcadia_Slot Arcadia_Slot;

typedef struct Nodes Nodes;

// The maximum number of slots per signal.
#define MaximumNumberOfSlots (SIZE_MAX - sizeof(Nodes) / sizeof(Arcadia_Slot*))

struct Nodes {
  // The number of times a signal may invoke itself.
  // The maximum is Arcadia_Natural32Value_Maximum.
  Arcadia_Natural32Value reentrancyCount;
  // The allocated array is the least power of two greater than or equal to size if size is greater than 8.
  // Otherwise the size of the allocated array is 8.
  Arcadia_SizeValue size;
  // An element is a pointer to a slot or a null pointer.
  Arcadia_Slot* elements[];
};

#endif // ARCADIA_RING1_IMPLEMENTATION_SIGNALS_INTERNAL_H_INCLUDED
