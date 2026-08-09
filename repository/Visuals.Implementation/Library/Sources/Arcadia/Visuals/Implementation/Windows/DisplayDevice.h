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

#if !defined(ARCADIA_VISUALS_WINDOWS_DISPLAYDEVICE_H_INCLUDED)
#define ARCADIA_VISUALS_WINDOWS_DISPLAYDEVICE_H_INCLUDED


#include "Arcadia/Engine/Include.h"
#if !defined(WIN32_LEAN_AND_MEAN)
  #define WIN32_LEAN_AND_MEAN
#endif
#if !defined(NOCOMM)
  #define NOCOMM
#endif
#include <Windows.h>
typedef struct Arcadia_Engine_Visuals_Windows_DisplayMode Arcadia_Engine_Visuals_Windows_DisplayMode;


Arcadia_declareObjectType(u8"Arcadia.Visuals.Windows.DisplayDevice", Arcadia_Engine_Visuals_Windows_DisplayDevice,
                          u8"Arcadia.Engine.Visuals.DisplayDevice");

struct Arcadia_Engine_Visuals_Windows_DisplayDeviceDispatch {
  Arcadia_Engine_Visuals_DisplayDeviceDispatch _parent;
};

struct Arcadia_Engine_Visuals_Windows_DisplayDevice {
  Arcadia_Engine_Visuals_DisplayDevice _parent;
  // The ID of the adapter / monitor combination.
  // Example: \\.\DISPLAY
  Arcadia_String* id;
  // The human-readable name of the monitor.
  // This is not necessarily unique.
  // Example: MAG274QRF-QD
  Arcadia_String* monitor;
  // The human-readable name of the adapter.
  // This is not necessarily unique.
  // Example: NVIDIA GeForce RTX 4070 Ti
  Arcadia_String* adapter;
  /// The handle of the monitor.
  HMONITOR hMonitor;
  /// The rectangle of the monitor in the virtual display space.
  Arcadia_Integer32Value left, top, right, bottom;

  /// The current display mode of this display device.
  Arcadia_Engine_Visuals_Windows_DisplayMode* currentDisplayMode;
  /// The list of available display modes of this display device.
  Arcadia_List* availableDisplayModes;
};

Arcadia_Engine_Visuals_Windows_DisplayDevice*
Arcadia_Engine_Visuals_Windows_DisplayDevice_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* id,
    Arcadia_String* adapter,
    Arcadia_String* monitor
  );

#endif // ARCADIA_VISUALS_WINDOWS_DISPLAYDEVICE_H_INCLUDED
