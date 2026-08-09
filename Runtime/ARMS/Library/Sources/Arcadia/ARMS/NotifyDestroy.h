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

#if !defined(ARCADIA_ARMS_NOTIFYDESTROY_H_INCLUDED)
#define ARCADIA_ARMS_NOTIFYDESTROY_H_INCLUDED

#include "Arcadia/ARMS/Configure.h"
#include "Arcadia/ARMS/StatusType.h"

#if defined(Arcadia_ARMS_Configuration_WithNotifyDestroy) && 1 ==  Arcadia_ARMS_Configuration_WithNotifyDestroy

typedef void (Arcadia_ARMS_NotifyDestroyCallback)(void* argument1, void* argument2);

Arcadia_ARMS_Status
Arcadia_ARMS_removeNotifyDestroyAll
  (
    void* observed
  );

Arcadia_ARMS_Status
Arcadia_ARMS_addNotifyDestroy
  (
    void* observed,
    void* argument1,
    void* argument2,
    Arcadia_ARMS_NotifyDestroyCallback* callback
  );

Arcadia_ARMS_Status
Arcadia_ARMS_removeNotifyDestroy
  (
    void* observed,
    void* argument1,
    void* argument2,
    Arcadia_ARMS_NotifyDestroyCallback* callback
  );

Arcadia_ARMS_Status
Arcadia_ARMS_removeNotifyDestroyAll
  (
    void* observed
  );

#endif // Arcadia_ARMS_Configuration_WithNotifyDestroy

#endif // ARCADIA_ARMS_NOTIFYDESTROY_H_INCLUDED
