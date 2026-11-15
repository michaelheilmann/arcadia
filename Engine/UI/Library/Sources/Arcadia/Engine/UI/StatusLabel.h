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

#if !defined(ARCADIA_ENGINE_UI_STATUSLABEL_H_INCLUDED)
#define ARCADIA_ENGINE_UI_STATUSLABEL_H_INCLUDED
#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif
#include "Arcadia/Engine/UI/Nodes/LabelNode.h"

typedef enum Arcadia_Engine_UI_StatusSeverity {
  Arcadia_Engine_UI_StatusSeverity_Informational,
  Arcadia_Engine_UI_StatusSeverity_Success,
  Arcadia_Engine_UI_StatusSeverity_Warning,
  Arcadia_Engine_UI_StatusSeverity_Error,
} Arcadia_Engine_UI_StatusSeverity;

// Uses a normal LabelNode, never a specialized warning rendering subtype.
// Caller supplies an explicit non-color cue in message (e.g. "Warning: ...").
// A null message hides the text; later updates can restore it.
void Arcadia_Engine_UI_StatusLabel_set(Arcadia_Thread*, Arcadia_Engine_UI_LabelNode*,
                                      Arcadia_Engine_UI_StatusSeverity, Arcadia_String* message);
#endif // ARCADIA_ENGINE_UI_STATUSLABEL_H_INCLUDED
