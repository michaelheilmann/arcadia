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

#if !defined(ARCADIA_NETWORK_EVENT_H_INCLUDED)
#define ARCADIA_NETWORK_EVENT_H_INCLUDED

#include <stddef.h>
#include <stdint.h>

typedef enum Arcadia_Network_EventType {
  Arcadia_Network_EventType_Connected = 1,
  Arcadia_Network_EventType_Data = 2,
  Arcadia_Network_EventType_Disconnected = 3,
  Arcadia_Network_EventType_Error = 4,
  Arcadia_Network_EventType_Accepted = 5
} Arcadia_Network_EventType;

typedef struct Arcadia_Network_Event Arcadia_Network_Event;

// Native-only record passed from worker(s) to sequential caller.
// Payload has ownership after pop; call Arcadia_Network_Event_dispose.
// The worker must not send a managed Arcadia object across this boundary.
struct Arcadia_Network_Event {
  uint64_t connectionId;
  Arcadia_Network_EventType type;
  int32_t errorCode;
  uint8_t* data;
  size_t dataLength;
};

void
Arcadia_Network_Event_dispose
  (
    Arcadia_Network_Event* event
  );

#endif // ARCADIA_NETWORK_EVENT_H_INCLUDED
