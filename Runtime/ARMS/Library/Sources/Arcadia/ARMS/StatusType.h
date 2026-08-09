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

#if !defined(ARCADIA_ARMS_STATUSTYPE_H_INCLUDED)
#define ARCADIA_ARMS_STATUSTYPE_H_INCLUDED

#include "Arcadia/ARMS/Configure.h"

/// An enumeration of status codes.
typedef enum Arcadia_ARMS_Status {
  Arcadia_ARMS_Status_Success = 0,
  Arcadia_ARMS_Status_ArgumentValueInvalid = 1,
  Arcadia_ARMS_Status_AllocationFailed = 2,
  Arcadia_ARMS_Status_TypeExists = 3,
  Arcadia_ARMS_Status_TypeNotExists = 4,
  Arcadia_ARMS_Status_OperationInvalid = 5,
  Arcadia_ARMS_Status_EnvironmentFailed = 6,
} Arcadia_ARMS_Status;

#endif // ARCADIA_ARMS_STATUSTYPE_H_INCLUDED
