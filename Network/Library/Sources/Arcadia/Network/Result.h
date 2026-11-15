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

#if !defined(ARCADIA_NETWORK_RESULT_H_INCLUDED)
#define ARCADIA_NETWORK_RESULT_H_INCLUDED

// Values are independent of Arcadia's managed runtime and thread status.
// API functions do not throw, call the GC, or invoke application callbacks.
typedef enum Arcadia_Network_Result {
  Arcadia_Network_Result_Success = 0,
  Arcadia_Network_Result_Ready,
  Arcadia_Network_Result_Empty,
  Arcadia_Network_Result_Full,
  Arcadia_Network_Result_InvalidArgument,
  Arcadia_Network_Result_AllocationFailed,
  Arcadia_Network_Result_Malformed,
  Arcadia_Network_Result_InvalidState,
  Arcadia_Network_Result_Closed,
  Arcadia_Network_Result_EnvironmentFailed
} Arcadia_Network_Result;

#endif // ARCADIA_NETWORK_RESULT_H_INCLUDED
