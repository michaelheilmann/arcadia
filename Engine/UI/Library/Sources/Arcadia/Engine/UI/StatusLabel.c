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

#define ARCADIA_ENGINE_UI_PRIVATE (1)
#include "Arcadia/Engine/UI/StatusLabel.h"
#include "Arcadia/Engine/UI/Include.h"

void
Arcadia_Engine_UI_StatusLabel_set
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LabelNode* label,
    Arcadia_Engine_UI_StatusSeverity severity,
    Arcadia_String* message
  )
{
  if (!label || severity > Arcadia_Engine_UI_StatusSeverity_Error || severity < Arcadia_Engine_UI_StatusSeverity_Informational) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (severity == Arcadia_Engine_UI_StatusSeverity_Error) {
    label->red = 255; label->green = 130; label->blue = 130;
  } else if (severity == Arcadia_Engine_UI_StatusSeverity_Warning) {
    label->red = 255; label->green = 210; label->blue = 130;
  } else if (severity == Arcadia_Engine_UI_StatusSeverity_Success) {
    label->red = 165; label->green = 230; label->blue = 175;
  } else {
    label->red = 220; label->green = 228; label->blue = 240;
  }
  label->alpha = message ? 255 : 0;
  Arcadia_String* display = message ? message : Arcadia_String_createFromCxxString(thread, u8"");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)display);
  Arcadia_Engine_UI_LabelNode_setText(thread, label, display);
  Arcadia_ValueStack_popValues(thread, 1);
}
