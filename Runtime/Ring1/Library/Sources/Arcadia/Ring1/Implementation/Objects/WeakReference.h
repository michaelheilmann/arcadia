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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_WEAKREFERENCE_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_WEAKREFERENCE_H_INCLUDED

#include "Arcadia/Ring1/Implementation/Object.h"

Arcadia_declareObjectType(u8"Arcadia.WeakReference", Arcadia_WeakReference,
                          u8"Arcadia.Object");

struct Arcadia_WeakReferenceDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_WeakReference {
  Arcadia_Object _parent;
  Arcadia_Value value;
};

Arcadia_WeakReference*
Arcadia_WeakReference_create
  (
    Arcadia_Thread* thread,
    Arcadia_Value value
  );

Arcadia_Value
Arcadia_WeakReference_getValue
  (
    Arcadia_Thread* thread,
    Arcadia_WeakReference* self
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_WEAKREFERENCE_H_INCLUDED
