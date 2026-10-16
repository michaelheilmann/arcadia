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

#if !defined(ARCADIA_VPL_TEXT_H_INCLUDED)
#define ARCADIA_VPL_TEXT_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/IR/Module.h"
#include "Arcadia/Languages/Include.h"

/// Read UTF-8 bytes, bind names, and verify the resulting canonical module.
/// sourceName is an optional diagnostic label, not a filesystem dependency.
Arcadia_VPL_IR_Module*
Arcadia_VPL_Text_read
  (
    Arcadia_Thread* thread,
    const void* bytes,
    Arcadia_SizeValue count,
    Arcadia_Languages_Diagnostics* diagnostics,
    Arcadia_String* sourceName
  );

/// Verify the current graph and return its deterministic version-1 textual representation.
Arcadia_String*
Arcadia_VPL_Text_write
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Module* module,
    Arcadia_Languages_Diagnostics* diagnostics
  );

#endif // ARCADIA_VPL_TEXT_H_INCLUDED
