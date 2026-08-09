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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_SIGNALS_SLOT_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_SIGNALS_SLOT_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Object.h"
#include "Arcadia/Ring1/Implementation/Objects/WeakReference.h"
#include "Arcadia/Ring1/Implementation/ForeignProcedure.h"
typedef struct Arcadia_Signal Arcadia_Signal;

Arcadia_declareObjectType(u8"Arcadia.Slot", Arcadia_Slot,
                          u8"Arcadia.Object");

struct Arcadia_SlotDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Slot {
  Arcadia_Object _parent;
  // Weak reference to the signal.
  Arcadia_WeakReference* signal;
  // Weak reference to the receiver.
  Arcadia_WeakReference* receiver;
  // The foreign procedure to invoke.
  Arcadia_ForeignProcedure* procedure;
};

// Create a connection which is associated with a signal but no node of the signal.
Arcadia_Slot*
Arcadia_Slot_create
  (
    Arcadia_Thread* thread,
    Arcadia_Signal* signal,
    Arcadia_Object* receiver,
    Arcadia_ForeignProcedure* procedure
  );

void
Arcadia_Slot_disconnect
  (
    Arcadia_Thread* thread,
    Arcadia_Slot* slot
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_SIGNALS_SLOT_H_INCLUDED
