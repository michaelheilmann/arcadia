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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_NETWORK_PROTOCOL_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_NETWORK_PROTOCOL_H_INCLUDED

#include "Arcadia/Ring1/Implementation/Enumeration.h"

/// An enumeration of network protocols.
Arcadia_declareEnumerationType(u8"Arcadia.Network.Protocol", Arcadia_Network_Protocol);

enum Arcadia_Network_Protocol {

  /// TCP network protocol.
  Arcadia_Network_Protocol_Tcp,

  /// UDP network protocol.
  Arcadia_Network_Protocol_Udp,

};

#endif // ARCADIA_RING1_IMPLEMENTATION_NETWORK_PROTOCOL_H_INCLUDED
