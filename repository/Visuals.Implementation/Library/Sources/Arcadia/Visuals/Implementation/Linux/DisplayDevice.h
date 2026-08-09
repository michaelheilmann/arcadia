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

#if !defined(ARCADIA_VISUALS_IMPLEMENTATION_LINUX_DISPLAYDEVICE_H_INCLUDED)
#define ARCADIA_VISUALS_IMPLEMENTATION_LINUX_DISPLAYDEVICE_H_INCLUDED

#include "Arcadia/Engine/Include.h"

// https://www.x.org/releases/current/doc/index.html
// https://www.x.org/releases/current/doc/randrproto/randrproto.txt
#include <X11/extensions/Xrandr.h>

typedef struct Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_BackendContext Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_BackendContext;

Arcadia_declareObjectType(u8"Arcadia.Visuals.Linux.DisplayDevice", Arcadia_Visuals_Linux_DisplayDevice,
                          u8"Arcadia.Engine.Visuals.DisplayDevice");

struct Arcadia_Visuals_Linux_DisplayDeviceDispatch {
  Arcadia_Engine_Visuals_DisplayDeviceDispatch _parent;
};

struct Arcadia_Visuals_Linux_DisplayDevice {
  Arcadia_Engine_Visuals_DisplayDevice _parent;
  // The system.
  Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_BackendContext* backendContext;
  struct {
    int left;
    int top;
    int right;
    int bottom;
  } bounds;
  // An unique ID.
  // This is usually the stringified value of output.
  Arcadia_String* id;
  // A human-readable name.
  // TODO The name is not necessarily unique, it is the device name as provided by the EDID.
  //      Make sure the name is unique programmatically.
  Arcadia_String* name;
  // The X11 ID of the output. RROutput type is an alias of XID type which is an alias of CARD32 type.
  RROutput output;
  // The X11 ID of the default mode.
  //RRMode defaultMode;
  // The X11 ID of the current mode.
  // RRMode type is an alias of XID type which is an alias of CARD32 type.
  RRMode mode;
};

Arcadia_Visuals_Linux_DisplayDevice*
Arcadia_Visuals_Linux_DisplayDevice_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_BackendContext* backendContext,
    Arcadia_String* id,
    Arcadia_String* name
  );

void
Arcadia_Visuals_Linux_DisplayDevice_updateBounds
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_Linux_DisplayDevice* self
  );

#endif // ARCADIA_VISUALS_IMPLEMENTATION_LINUX_DISPLAYDEVICE_H_INCLUDED
