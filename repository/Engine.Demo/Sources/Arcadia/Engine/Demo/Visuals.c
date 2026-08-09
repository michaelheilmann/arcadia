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

#include "Arcadia/Engine/Demo/Visuals.h"

#include "Arcadia/Engine/Demo/Configuration.h"
#include "Arcadia/Visuals/Implementation/Include.h"

#include <string.h>

static void
setDisplayMode
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* configuration,
    Arcadia_Integer32Value horizontalResolution,
    Arcadia_Integer32Value verticalResolution,
    Arcadia_Integer32Value colorDepth
  )
{
  {
    char const* path[] = {
      u8"visuals",
      u8"horizontalResolution",
    };
    Cfg2_setInteger32(thread, configuration, path, 2, horizontalResolution);
  }
  {
    char const* path[] = {
      u8"visuals",
      u8"verticalResolution",
    };
    Cfg2_setInteger32(thread, configuration, path, 2, verticalResolution);
  }
  {
    char const* path[] = {
      u8"visuals",
      u8"colorDepth",
    };
    Cfg2_setInteger32(thread, configuration, path, 2, colorDepth);
  }
}

static void
getDisplayMode
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* configuration,
    Arcadia_Integer32Value* horizontalResolution,
    Arcadia_Integer32Value* verticalResolution,
    Arcadia_Integer32Value* colorDepth
  )
{
  Arcadia_Integer32Value temporary[3];
  {
    char const* path[] = {
      u8"visuals",
      u8"horizontalResolution",
    };
    temporary[0] = Cfg2_getInteger32(thread, configuration, path, 2);
  }
  {
    char const* path[] = {
      u8"visuals",
      u8"verticalResolution",
    };
    temporary[1] = Cfg2_getInteger32(thread, configuration, path, 2);
  }
  {
    char const* path[] = {
      u8"visuals",
      u8"colorDepth",
    };
    temporary[2] = Cfg2_getInteger32(thread, configuration, path, 2);
  }
  if (horizontalResolution) *horizontalResolution = temporary[0];
  if (verticalResolution) *verticalResolution = temporary[1];
  if (colorDepth) *colorDepth = temporary[2];
}

static void
setVerticalSynchronization
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* configuration,
    Arcadia_BooleanValue value
  )
{
  char const* path[] = {
    u8"visuals",
    u8"verticalSynchronization",
  };
  Cfg2_setBoolean(thread, configuration, path, 2, value);
}

static Arcadia_BooleanValue
getVerticalSynchronization
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* configuration
  )
{
  char const* path[] = {
    u8"visuals",
    u8"verticalSynchronization",
  };
  return Cfg2_getBoolean(thread, configuration, path, 2);
}

static Arcadia_BooleanValue
findTypeByName
  (
    Arcadia_Thread* thread,
    Arcadia_Value x,
    Arcadia_Value y
  )
{
  Arcadia_Type* yy = Arcadia_Value_getTypeValue(&y);
  Arcadia_Name* b = Arcadia_Type_getName(thread, yy);
  Arcadia_String* a = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&x);
  return Arcadia_String_getNumberOfBytes(thread, a) == Arcadia_Name_getNumberOfBytes(thread, b)
      && !Arcadia_Memory_compare(thread, Arcadia_String_getBytes(thread, a), Arcadia_Name_getBytes(thread, b), Arcadia_String_getNumberOfBytes(thread, a));
}

void
Arcadia_Engine_Application_startupVisuals
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine,
    Arcadia_DDL_Node* configuration,
    Arcadia_List* windowList
  )
{
  Arcadia_Engine_Visuals_DisplayDevice* displayDevice = NULL;
  Arcadia_String* windowMode = NULL;
  Arcadia_Engine_Visuals_Window* window = NULL;
  // (1.1) Register visuals backends.
  Arcadia_Engine_Visuals_Implementation_registerBackends(thread, engine->visualsBackendTypes);
  // (1.2) Register the visuals node factory.
  Arcadia_Engine_Visuals_registerNodeFactories(thread, engine->visualsNodeFactoryTypes);
  // (2) Select visuals system.
  {
    char const* path[] = {
      u8"visuals",
      u8"backend",
    };
    Arcadia_Engine_Backend* backend = NULL;
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      Arcadia_String* backendTypeName = Cfg2_getString(thread, (Arcadia_DDL_Node*)configuration, path, 2);
      Arcadia_Value backendTypeValue = Arcadia_HashSet_findFirst(thread, (Arcadia_HashSet*)engine->visualsBackendTypes, Arcadia_Value_makeObjectReferenceValue(backendTypeName), &findTypeByName);
      if (!Arcadia_Value_isTypeValue(&backendTypeValue)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_NotFound);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_Type* backendType = Arcadia_Value_getTypeValue(&backendTypeValue);
      if (!Arcadia_Type_isDescendantType(thread, backendType, _Arcadia_Engine_Visuals_Backend_getType(thread))) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_NotFound);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_ValueStack_pushNatural8Value(thread, 0);
      backend = (Arcadia_Engine_Backend*)_Arcadia_EndCreate0(thread, backendType, Arcadia_ValueStack_getSize(thread) - 1);
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_String* backendTypeName = Arcadia_String_createFromCxxString(thread, u8"Arcadia.Visuals.Implementation.OpenGL4.Backend");
      Cfg2_setString(thread, (Arcadia_DDL_Node*)configuration, path, 2, backendTypeName);
      Arcadia_Value backendTypeValue = Arcadia_HashSet_findFirst(thread, (Arcadia_HashSet*)engine->visualsBackendTypes, Arcadia_Value_makeObjectReferenceValue(backendTypeName), &findTypeByName);
      if (!Arcadia_Value_isTypeValue(&backendTypeValue)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_NotFound);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_Type* backendType = Arcadia_Value_getTypeValue(&backendTypeValue);
      if (!Arcadia_Type_isDescendantType(thread, backendType, _Arcadia_Engine_Visuals_Backend_getType(thread))) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_NotFound);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_ValueStack_pushNatural8Value(thread, 0);
      backend = (Arcadia_Engine_Backend*)_Arcadia_EndCreate0(thread, backendType, Arcadia_ValueStack_getSize(thread) - 1);
    }
    Arcadia_Engine_Visuals_BackendContext* temporary = (Arcadia_Engine_Visuals_BackendContext*)Arcadia_Engine_Backend_createBackendContext(thread, backend);
    Arcadia_Object_lock(thread, (Arcadia_Object*)temporary);
    engine->visualsBackendContext = (Arcadia_Engine_BackendContext*)temporary;
  }
  // (3) Select display device (aka "monitor").
  {
    Arcadia_List* displayDevices = Arcadia_Engine_Visuals_BackendContext_getDisplayDevices(thread, (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext);
    Arcadia_Visuals_Diagnostics_dumpDevices(thread, (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext);
    Arcadia_Integer32Value displayDeviceIndex = 0;
    char const* path[] = {
      u8"visuals",
      u8"monitor",
    };
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      displayDeviceIndex = Cfg2_getInteger32(thread, (Arcadia_DDL_Node*)configuration, path, 2);
      if (displayDeviceIndex < 0 || displayDeviceIndex >= Arcadia_Collection_getSize(thread, (Arcadia_Collection*)displayDevices)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Cfg2_setInteger32(thread, (Arcadia_DDL_Node*)configuration, path, 2, displayDeviceIndex);
    }
    // If we don't have at least one display device, bail out.
    if (!Arcadia_Collection_getSize(thread, (Arcadia_Collection*)displayDevices)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    displayDevice = (Arcadia_Engine_Visuals_DisplayDevice*)Arcadia_List_getObjectReferenceValueAt(thread, displayDevices, displayDeviceIndex);
  }
  // (4) Get the window mode.
  {
    char const* path[] = {
      u8"visuals",
      u8"windowMode",
    };
    const char* values[] = {
      u8"windowed",
      u8"borderless fullscreen window",
      u8"fullscreen",
    };
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      windowMode = Cfg2_getString(thread, (Arcadia_DDL_Node*)configuration, path, 2);
      Arcadia_BooleanValue found = Arcadia_BooleanValue_False;
      for (Arcadia_SizeValue i = 0, n = 3; i < n; ++i) {
        Arcadia_Value a = Arcadia_Value_makeObjectReferenceValue(windowMode);
        Arcadia_Value b = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, values[i], strlen(values[i])))));
        if (Arcadia_Value_isEqualTo(thread, &a, &b)) {
          found = Arcadia_BooleanValue_True;
          break;
        }
      }
      if (!found) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      windowMode = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"windowed", sizeof(u8"windowed") - 1)));
      Cfg2_setString(thread, (Arcadia_DDL_Node*)configuration, path, 2, windowMode);
    }
  }
  // (5) Create and open a window, set its size and position according to the display device it is located on.
  {
    window = Arcadia_Engine_Visuals_BackendContext_createWindow(thread, (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext);
    Arcadia_Engine_Visuals_Window_open(thread, window);
    Arcadia_Integer32Value left, top, right, bottom;
    Arcadia_Engine_Visuals_DisplayDevice_getBounds(thread, displayDevice, &left, &top, &right, &bottom);
    Arcadia_Engine_Visuals_Window_setPosition(thread, window, left, top);
    Arcadia_Engine_Visuals_Window_setSize(thread, window, right - left, bottom - top);
    Arcadia_List_insertBackObjectReferenceValue(thread, windowList, (Arcadia_Object*)window);
  }
  // (6) Set window mode and resolution.
  {
    Arcadia_JumpTarget jumpTarget;

    // Set window mode.
    Arcadia_Value a = Arcadia_Value_makeObjectReferenceValue(windowMode);
    Arcadia_Value b;

    // Although the resolution is ignored by non-full screen window modes,
    // add a reasonable default to the configuration if no value or a broken value is in the configuration.
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      Arcadia_Integer32Value horizontalResolution, verticalResolution, colorDepth;
      getDisplayMode(thread, configuration, &horizontalResolution, &verticalResolution, &colorDepth);
      if (colorDepth < 8 || horizontalResolution < 320 || verticalResolution < 240) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
      Arcadia_Engine_Visuals_DisplayMode* displayMode = Arcadia_Engine_Visuals_DisplayDevice_getCurrentDisplayMode(thread, displayDevice);
      setDisplayMode(thread, configuration, Arcadia_Engine_Visuals_DisplayMode_getHorizontalResolution(thread, displayMode),
                                            Arcadia_Engine_Visuals_DisplayMode_getVerticalResolution(thread, displayMode),
                                            Arcadia_Engine_Visuals_DisplayMode_getColorDepth(thread, displayMode));
    }
    // Although vertical synchronization is ignored by non-full screen window moudes,
    // add a reasonable default to the configuration if no value or a broken value is in the configuration.
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      getVerticalSynchronization(thread, configuration);
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
      setVerticalSynchronization(thread, configuration, Arcadia_BooleanValue_False);
    }
    // Set the window mode and resolution.
    b = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"windowed", sizeof(u8"windowed") - 1))));
    if (Arcadia_Value_isEqualTo(thread, &a, &b)) {
      Arcadia_Engine_Visuals_Window_setFullscreen(thread, window, Arcadia_BooleanValue_False);
    }
    b = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"borderless fullscreen window", sizeof(u8"borderless fullscreen window") - 1))));
    if (Arcadia_Value_isEqualTo(thread, &a, &b)) {
      Arcadia_Engine_Visuals_Window_setFullscreen(thread, window, Arcadia_BooleanValue_True);
    }
    b = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"fullscreen", sizeof(u8"fullscreen") - 1))));
    if (Arcadia_Value_isEqualTo(thread, &a, &b)) {
      Arcadia_Engine_Visuals_Window_setFullscreen(thread, window, Arcadia_BooleanValue_True);
      Arcadia_Engine_Visuals_DisplayMode* bestDisplayMode = NULL;
      Arcadia_Integer32Value horizontalResolution, verticalResolution, colorDepth;
      getDisplayMode(thread, configuration, &horizontalResolution, &verticalResolution, &colorDepth);
      Arcadia_List* list = Arcadia_Engine_Visuals_DisplayDevice_getAvailableDisplayModes(thread, displayDevice);
      for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)list); i < n; ++i) {
        Arcadia_Engine_Visuals_DisplayMode* displayMode = (Arcadia_Engine_Visuals_DisplayMode*)Arcadia_List_getObjectReferenceValueAt(thread, list, i);
        if (horizontalResolution == Arcadia_Engine_Visuals_DisplayMode_getHorizontalResolution(thread, displayMode) &&
          verticalResolution == Arcadia_Engine_Visuals_DisplayMode_getVerticalResolution(thread, displayMode) &&
          colorDepth == Arcadia_Engine_Visuals_DisplayMode_getColorDepth(thread, displayMode)) {
          bestDisplayMode = displayMode;
          break;
        }
      }
      if (!bestDisplayMode) {
        bestDisplayMode = Arcadia_Engine_Visuals_DisplayDevice_getCurrentDisplayMode(thread, displayDevice);
        setDisplayMode(thread, configuration, Arcadia_Engine_Visuals_DisplayMode_getHorizontalResolution(thread, bestDisplayMode),
                                              Arcadia_Engine_Visuals_DisplayMode_getVerticalResolution(thread, bestDisplayMode),
                                              Arcadia_Engine_Visuals_DisplayMode_getColorDepth(thread, bestDisplayMode));
      }
      if (bestDisplayMode) {
        Arcadia_Integer32Value left, top, right, bottom;
        Arcadia_Engine_Visuals_DisplayMode_apply(thread, bestDisplayMode);
        Arcadia_Engine_Visuals_DisplayDevice_getBounds(thread, displayDevice, &left, &top, &right, &bottom);
        Arcadia_Engine_Visuals_Window_setPosition(thread, window, left, top);
        Arcadia_Engine_Visuals_Window_setSize(thread, window, right - left, bottom - top);
      }
    }
  }
  // (7) Set the window icons and the window title.
  {
    Arcadia_Engine_Visuals_Icon* icon;
    Arcadia_Media_PixelBuffer* pixelBuffer;
    Arcadia_Integer32Value width, height;
    // Set the big icon.
    Arcadia_Engine_Visuals_Window_getRequiredBigIconSize(thread, window, &width, &height);
    pixelBuffer = Arcadia_Media_PixelBuffer_create(thread, 0, width, height, Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8);
    Arcadia_Media_PixelBuffer_fill(thread, pixelBuffer, 47, 47, 47, 255);
    icon = Arcadia_Engine_Visuals_BackendContext_createIcon(thread, (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext, pixelBuffer);
    Arcadia_Engine_Visuals_Window_setBigIcon(thread, window, icon);

    // Set the small icon.
    Arcadia_Engine_Visuals_Window_getRequiredSmallIconSize(thread, window, &width, &height);
    pixelBuffer = Arcadia_Media_PixelBuffer_create(thread, 0, width, height, Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8);
    Arcadia_Media_PixelBuffer_fill(thread, pixelBuffer, 47, 47, 47, 255);
    icon = Arcadia_Engine_Visuals_BackendContext_createIcon(thread, (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext, pixelBuffer);
    Arcadia_Engine_Visuals_Window_setSmallIcon(thread, window, icon);

    // Set the title.
    Arcadia_Engine_Visuals_Window_setTitle(thread, window, Arcadia_String_createFromCxxString(thread, u8"Michael Heilmann's Liminality"));
  }
  // (8) Create the node factory.
  {
    Arcadia_Engine_NodeFactory* nodeFactory = NULL;
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      Arcadia_List* temporary = (Arcadia_List*)Arcadia_ArrayList_create(thread);
      Arcadia_Set_getAll(thread, engine->visualsNodeFactoryTypes, temporary);
      Arcadia_Value nodeFactoryTypeValue = Arcadia_List_getAt(thread, temporary, 0);
      if (!Arcadia_Value_isTypeValue(&nodeFactoryTypeValue)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_NotFound);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_Type* nodeFactoryType = Arcadia_Value_getTypeValue(&nodeFactoryTypeValue);
      if (!Arcadia_Type_isDescendantType(thread, nodeFactoryType, _Arcadia_Engine_Visuals_NodeFactory_getType(thread))) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_NotFound);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_ValueStack_pushNatural8Value(thread, 0);
      nodeFactory = (Arcadia_Engine_NodeFactory*)_Arcadia_EndCreate0(thread, nodeFactoryType, Arcadia_ValueStack_getSize(thread) - 1);
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Thread_jump(thread);
    }
    engine->visualsNodeFactory = nodeFactory;
  }
}
