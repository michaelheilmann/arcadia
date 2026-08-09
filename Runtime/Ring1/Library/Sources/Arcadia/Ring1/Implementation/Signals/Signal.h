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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_SIGNALS_SIGNAL_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_SIGNALS_SIGNAL_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

typedef struct Arcadia_WeakReference Arcadia_WeakReference;
typedef struct Arcadia_Slot Arcadia_Slot;
#include "Arcadia/Ring1/Implementation/Object.h"
#include "Arcadia/Ring1/Implementation/ForeignProcedure.h"

Arcadia_declareObjectType(u8"Arcadia.Signal", Arcadia_Signal,
                          u8"Arcadia.Object");

struct Arcadia_SignalDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Signal {
  Arcadia_Object _parent;
  void *pimpl;
};

Arcadia_Signal*
Arcadia_Signal_create
  (
    Arcadia_Thread* thread
  );

/// @brief Connect to this signal.
/// @param thread A pointer to this thread.
/// @param self A pointer to this signal.
/// @param receiver The receiver. A weak reference to the receiver is stored.
/// @param procedure The proceduren. The procedure is invoked with the receiver as its 1st arguments, the sender as its 2nd argument and the argument as its 3rd argument.
/// Its return value is ignored.
Arcadia_Slot*
Arcadia_Signal_connect
  (
    Arcadia_Thread* thread,
    Arcadia_Signal* self,
    Arcadia_Object* receiver,
    Arcadia_ForeignProcedure* procedure
  );

// [arguments] ++ [#numberOfArguments]
// `numberOfArguments` is a Natural8 value indicating the number of arguments passed to the function.
// `arguments` is a sequence of `numberOfArguments` values.
void
Arcadia_Signal_emit
  (
    Arcadia_Thread* thread,
    Arcadia_Signal* self,
    Arcadia_Object* sender
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_SIGNALS_SIGNAL_H_INCLUDED
