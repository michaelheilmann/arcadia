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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_NETWORK_SOCKETTYPE_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_NETWORK_SOCKETTYPE_H_INCLUDED

#include "Arcadia/Ring1/Implementation/Enumeration.h"

/// An enumeration of socket types.
Arcadia_declareEnumerationType(u8"Arcadia.Network.SocketType", Arcadia_Network_SocketType);

enum Arcadia_Network_SocketType {

  /// Type of a "stream" socket.
  /// Typically used with TCP.
  Arcadia_Network_SocketType_Stream,

  /// Type of a "datagram" socket.
  /// Typically used with UDP.
  Arcadia_Network_SocketType_Datagram,

};

#endif // ARCADIA_RING1_IMPLEMENTATION_NETWORK_SOCKETTYPE_H_INCLUDED
