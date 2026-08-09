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

#if !defined (ARCADIA_VISUALS_IMPLEMENTATION_LINUX_DISPLAYMODE_H_INCLUDED)
#define ARCADIA_VISUALS_IMPLEMENTATION_LINUX_DISPLAYMODE_H_INCLUDED

#include "Arcadia/Engine/Include.h"
typedef struct Arcadia_Visuals_Linux_DisplayDevice Arcadia_Visuals_Linux_DisplayDevice;

// https://www.x.org/releases/current/doc/index.html
// https://www.x.org/releases/current/doc/randrproto/randrproto.txt
#include <X11/extensions/Xrandr.h>

Arcadia_declareObjectType(u8"Arcadia.Visuals.Linux.DisplayMode", Arcadia_Visuals_Linux_DisplayMode,
                          u8"Arcadia.Engine.Visuals.DisplayMode");

struct Arcadia_Visuals_Linux_DisplayModeDispatch {
  Arcadia_Engine_Visuals_DisplayModeDispatch _parent;
};

struct Arcadia_Visuals_Linux_DisplayMode {
  Arcadia_Engine_Visuals_DisplayMode _parent;
  Arcadia_Visuals_Linux_DisplayDevice* device;
  RRMode modeId;
  Arcadia_Integer32Value horizontalResolution;
  Arcadia_Integer32Value verticalResolution;
  Arcadia_Integer32Value colorDepth;
  Arcadia_Integer32Value frequency;
};

Arcadia_Visuals_Linux_DisplayMode*
Arcadia_Visuals_Linux_DisplayMode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_Linux_DisplayDevice* device,
    Arcadia_Integer32Value horizontalResolution,
    Arcadia_Integer32Value verticalResolution,
    Arcadia_Integer32Value colorDepth,
    Arcadia_Integer32Value frequency
  );

#endif // ARCADIA_VISUALS_IMPLEMENTATION_LINUX_DISPLAYMODE_H_INCLUDED
