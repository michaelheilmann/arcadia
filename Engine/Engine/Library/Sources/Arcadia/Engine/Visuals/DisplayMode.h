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

#if !defined (ARCADIA_ENGINE_VISUALS_DISPLAYMODE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_DISPLAYMODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"

// Information on a display mode available on a display device.
// See Arcadia.Engine.Visuals.DisplayDevice for more information.
Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.DisplayMode", Arcadia_Engine_Visuals_DisplayMode,
                          u8"Arcadia.Object");

struct Arcadia_Engine_Visuals_DisplayModeDispatch {
  Arcadia_ObjectDispatch _parent;

  Arcadia_Integer32Value (*getHorizontalResolution)(Arcadia_Thread* thread, Arcadia_Engine_Visuals_DisplayMode* self);
  Arcadia_Integer32Value (*getVerticalResolution)(Arcadia_Thread* thread, Arcadia_Engine_Visuals_DisplayMode* self);
  Arcadia_Integer32Value (*getColorDepth)(Arcadia_Thread* thread, Arcadia_Engine_Visuals_DisplayMode* self);
  Arcadia_Integer32Value (*getFrequency)(Arcadia_Thread* thread, Arcadia_Engine_Visuals_DisplayMode* self);
  void (*apply)(Arcadia_Thread* thread, Arcadia_Engine_Visuals_DisplayMode* self);
};

struct Arcadia_Engine_Visuals_DisplayMode {
  Arcadia_Object _parent;
};

Arcadia_Integer32Value
Arcadia_Engine_Visuals_DisplayMode_getHorizontalResolution
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayMode* self
  );

Arcadia_Integer32Value
Arcadia_Engine_Visuals_DisplayMode_getVerticalResolution
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayMode* self
  );

Arcadia_Integer32Value
Arcadia_Engine_Visuals_DisplayMode_getColorDepth
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayMode* self
  );

Arcadia_Integer32Value
Arcadia_Engine_Visuals_DisplayMode_getFrequency
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayMode* self
  );

void
Arcadia_Engine_Visuals_DisplayMode_apply
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayMode* self
  );

#endif // ARCADIA_ENGINE_VISUALS_DISPLAYMODE_H_INCLUDED
