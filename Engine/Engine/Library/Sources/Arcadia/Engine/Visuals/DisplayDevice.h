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

#if !defined(ARCADIA_ENGINE_VISUALS_DISPLAYDEVICE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_DISPLAYDEVICE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/Visuals/DisplayDevice.h` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

typedef struct Arcadia_Engine_Visuals_DisplayDevice Arcadia_Engine_Visuals_DisplayDevice;
typedef struct Arcadia_Engine_Visuals_DisplayMode Arcadia_Engine_Visuals_DisplayMode;
typedef struct Arcadia_List Arcadia_List;

Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.DisplayDevice", Arcadia_Engine_Visuals_DisplayDevice,
                          u8"Arcadia.Object");

struct Arcadia_Engine_Visuals_DisplayDeviceDispatch {
  Arcadia_ObjectDispatch _parent;

  Arcadia_Engine_Visuals_DisplayMode* (*getCurrentDisplayMode)(Arcadia_Thread*, Arcadia_Engine_Visuals_DisplayDevice*);

  Arcadia_List* (*getAvailableDisplayModes)(Arcadia_Thread*, Arcadia_Engine_Visuals_DisplayDevice*);

  void (*getBounds)(Arcadia_Thread*, Arcadia_Engine_Visuals_DisplayDevice*, Arcadia_Integer32Value*, Arcadia_Integer32Value*, Arcadia_Integer32Value*, Arcadia_Integer32Value*);

  Arcadia_String* (*getName)(Arcadia_Thread*, Arcadia_Engine_Visuals_DisplayDevice*);

  Arcadia_String* (*getId)(Arcadia_Thread*, Arcadia_Engine_Visuals_DisplayDevice*);
};

struct Arcadia_Engine_Visuals_DisplayDevice {
  Arcadia_Object _parent;
};

Arcadia_Engine_Visuals_DisplayMode*
Arcadia_Engine_Visuals_DisplayDevice_getCurrentDisplayMode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self
  );

Arcadia_List*
Arcadia_Engine_Visuals_DisplayDevice_getAvailableDisplayModes
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self
  );

void
Arcadia_Engine_Visuals_DisplayDevice_getBounds
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self,
    Arcadia_Integer32Value* left,
    Arcadia_Integer32Value* top,
    Arcadia_Integer32Value* right,
    Arcadia_Integer32Value* bottom
  );

Arcadia_String*
Arcadia_Engine_Visuals_DisplayDevice_getName
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self
  );

Arcadia_String*
Arcadia_Engine_Visuals_DisplayDevice_getId
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self
  );

#endif // ARCADIA_ENGINE_VISUALS_DISPLAYDEVICE_H_INCLUDED
