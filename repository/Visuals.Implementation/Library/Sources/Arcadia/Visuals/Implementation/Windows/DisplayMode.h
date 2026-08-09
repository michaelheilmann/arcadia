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

#if !defined (ARCADIA_VISUALS_WINDOWS_DISPLAYMODE_H_INCLUDED)
#define ARCADIA_VISUALS_WINDOWS_DISPLAYMODE_H_INCLUDED

#include "Arcadia/Engine/Include.h"
typedef struct Arcadia_Engine_Visuals_Windows_DisplayDevice Arcadia_Engine_Visuals_Windows_DisplayDevice;

Arcadia_declareObjectType(u8"Arcadia.Visuals.Windows.DisplayMode", Arcadia_Engine_Visuals_Windows_DisplayMode,
                          u8"Arcadia.Engine.Visuals.DisplayMode");

struct Arcadia_Engine_Visuals_Windows_DisplayModeDispatch {
  Arcadia_Engine_Visuals_DisplayModeDispatch _parent;
};

struct Arcadia_Engine_Visuals_Windows_DisplayMode {
  Arcadia_Engine_Visuals_DisplayMode _parent;
  Arcadia_Engine_Visuals_Windows_DisplayDevice* device;
  Arcadia_Integer32Value horizontalResolution;
  Arcadia_Integer32Value verticalResolution;
  Arcadia_Integer32Value colorDepth;
  Arcadia_Integer32Value frequency;
};

Arcadia_Engine_Visuals_Windows_DisplayMode*
Arcadia_Engine_Visuals_Windows_DisplayMode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayDevice* device,
    Arcadia_Integer32Value horizontalResolution,
    Arcadia_Integer32Value verticalResolution,
    Arcadia_Integer32Value colorDepth,
    Arcadia_Integer32Value frequency
  );

#endif // ARCADIA_VISUALS_WINDOWS_DISPLAYMODE_H_INCLUDED
