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

#define ARCADIA_ENGINE_PRIVATE (1)
#include "Arcadia/Engine/Visuals/Diagnostics.h"

#include "Arcadia/Logging/Include.h"
#include "Arcadia/Engine/Include.h"

static inline void
Arcadia_Visuals_Diagnostics_dumpDeviceBounds
  (
    Arcadia_Thread* thread,
    Arcadia_Log* log,
    Arcadia_Engine_Visuals_DisplayDevice* device
  )
{
  Arcadia_Integer32Value left, top, right, bottom;
  Arcadia_Engine_Visuals_DisplayDevice_getBounds(thread, device, &left, &top, &right, &bottom);

  Arcadia_StringBuilder* message = Arcadia_StringBuilder_create(thread);

  Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"    bounds:");

  Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"\n     left: ");
  Arcadia_StringBuilder_insertBackString(thread, message, Arcadia_String_createFromInteger32(thread, left));

  Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"\n     top: ");
  Arcadia_StringBuilder_insertBackString(thread, message, Arcadia_String_createFromInteger32(thread, top));

  Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"\n     right: ");
  Arcadia_StringBuilder_insertBackString(thread, message, Arcadia_String_createFromInteger32(thread, right));

  Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"\n     bottom: ");
  Arcadia_StringBuilder_insertBackString(thread, message, Arcadia_String_createFromInteger32(thread, bottom));

  Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"\n");
  Arcadia_Log_information(thread, log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(message)));
}

static inline void
Arcadia_Visuals_Diagnostics_dumpModes
  (
    Arcadia_Thread* thread,
    Arcadia_Log* log,
    Arcadia_Engine_Visuals_DisplayDevice* device
  )
{
  Arcadia_StringBuilder* message = Arcadia_StringBuilder_create(thread);
  Arcadia_String* a;

  Arcadia_List* modes = Arcadia_Engine_Visuals_DisplayDevice_getAvailableDisplayModes(thread, device);

  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)modes); i < n; ++i) {
    Arcadia_Engine_Visuals_DisplayMode* mode =
      (Arcadia_Engine_Visuals_DisplayMode*)Arcadia_List_getObjectReferenceValueAt(thread, modes, i);

    Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"     ");

    Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"horizontal resolution: ");

    a = Arcadia_String_createFromInteger32(thread, Arcadia_Engine_Visuals_DisplayMode_getHorizontalResolution(thread, mode));
    Arcadia_StringBuilder_insertBackString(thread, message, a);

    Arcadia_StringBuilder_insertBackCxxString(thread, message, u8", vertical resolution: ");

    a = Arcadia_String_createFromInteger32(thread, Arcadia_Engine_Visuals_DisplayMode_getVerticalResolution(thread, mode));
    Arcadia_StringBuilder_insertBackString(thread, message, a);

    Arcadia_StringBuilder_insertBackCxxString(thread, message, u8", color depth: ");

    a = Arcadia_String_createFromInteger32(thread, Arcadia_Engine_Visuals_DisplayMode_getColorDepth(thread, mode));
    Arcadia_StringBuilder_insertBackString(thread, message, a);

    Arcadia_StringBuilder_insertBackCxxString(thread, message, u8", frequency: ");

    a = Arcadia_String_createFromInteger32(thread, Arcadia_Engine_Visuals_DisplayMode_getFrequency(thread, mode));
    Arcadia_StringBuilder_insertBackString(thread, message, a);

    Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"\n");
  }
  Arcadia_Log_information(thread, log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(message)));
}

void
Arcadia_Visuals_Diagnostics_dumpDevices
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{
  Arcadia_StringBuilder* message = Arcadia_StringBuilder_create(thread);
  Arcadia_Log* log = (Arcadia_Log*)Arcadia_ConsoleLog_create(thread);
  Arcadia_List* displayDevices = Arcadia_Engine_Visuals_BackendContext_getDisplayDevices(thread, backendContext);

  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)displayDevices); i < n; ++i) {
    Arcadia_Engine_Visuals_DisplayDevice* displayDevice =
      (Arcadia_Engine_Visuals_DisplayDevice*)Arcadia_List_getObjectReferenceValueAt(thread, displayDevices, i);

    Arcadia_String* a;

    Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"  ");

    a = Arcadia_String_createFromSize(thread, i);
    Arcadia_StringBuilder_insertBackString(thread, message, a);

    Arcadia_StringBuilder_insertBackCxxString(thread, message, u8") id: ");

    a = Arcadia_Engine_Visuals_DisplayDevice_getId(thread, displayDevice);
    Arcadia_StringBuilder_insertBackString(thread, message, a);

    Arcadia_StringBuilder_insertBackCxxString(thread, message, u8", name: ");

    a = Arcadia_Engine_Visuals_DisplayDevice_getName(thread, displayDevice);
    Arcadia_StringBuilder_insertBackString(thread, message, a);

    Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"\n");

    Arcadia_Log_information(thread, log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(message)));
    Arcadia_StringBuilder_clear(thread, message);

    Arcadia_Visuals_Diagnostics_dumpModes(thread, log, displayDevice);
    Arcadia_Visuals_Diagnostics_dumpDeviceBounds(thread, log, displayDevice);
  }
}
