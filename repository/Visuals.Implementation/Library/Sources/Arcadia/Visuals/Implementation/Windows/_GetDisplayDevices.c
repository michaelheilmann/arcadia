#include "Arcadia/Visuals/Implementation/Windows/_GetDisplayDevices.h"

#include "Arcadia/Visuals/Implementation/Windows/_CharConv.h"
#include "Arcadia/Visuals/Implementation/Windows/DisplayDevice.h"

typedef struct EnumDisplayMonitorsContext {
  Arcadia_Thread* thread;
  Arcadia_List* displayDevices;
} EnumDisplayMonitorsContext;

static Arcadia_String*
Arcadia_Engine_Visuals_Windows_System_getAdapterName
  (
    Arcadia_Thread* thread,
    const CHAR* p
  )
{
  DWORD deviceIndex = 0;
  while (1) {
    DISPLAY_DEVICEA deviceInfo = { 0 };
    deviceInfo.cb = sizeof(deviceInfo);
    if (!EnumDisplayDevicesA(NULL, deviceIndex, &deviceInfo, 0)) {
      break;
    }
    deviceIndex++;
    if (!strcmp(deviceInfo.DeviceName, p)) {
      return _fromMultiByte(thread, deviceInfo.DeviceString, strlen(deviceInfo.DeviceString));
    }
  }
  return NULL;
}

static Arcadia_String*
Arcadia_Engine_Visuals_Windows_System_getMonitorName
    (
      Arcadia_Thread* thread,
      const CHAR* p
    )
{
  UINT32 pathCount, modeCount;
  HRESULT hResult = GetDisplayConfigBufferSizes(QDC_ALL_PATHS, &pathCount, &modeCount);
  if (S_OK != hResult) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  DISPLAYCONFIG_PATH_INFO* paths = malloc(sizeof(DISPLAYCONFIG_PATH_INFO) * pathCount);
  if (!paths) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  DISPLAYCONFIG_MODE_INFO* modes = malloc(sizeof(DISPLAYCONFIG_MODE_INFO) * modeCount);
  if (!modes) {
    free(paths);
    paths = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  hResult = QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &pathCount, paths, &modeCount, modes, NULL);
  if (S_OK != hResult) {
    free(modes);
    modes = NULL;
    free(paths);
    paths = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }

  wchar_t* q; size_t m;
  _multiByteToWideChar(p, strlen(p) + 1, &q, &m);

  Arcadia_String* name = NULL;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    for (UINT32 i = 0; i < pathCount; ++i) {

      DISPLAYCONFIG_SOURCE_DEVICE_NAME sourceDeviceNameInfo = { 0 };
      sourceDeviceNameInfo.header.adapterId = paths[i].targetInfo.adapterId;
      sourceDeviceNameInfo.header.id = paths[i].sourceInfo.id;
      sourceDeviceNameInfo.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_SOURCE_NAME;
      sourceDeviceNameInfo.header.size = sizeof(sourceDeviceNameInfo);
      hResult = DisplayConfigGetDeviceInfo(&sourceDeviceNameInfo.header);
      if (S_OK != hResult) {
        free(q);
        q = NULL;
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
      if (wcsncmp(sourceDeviceNameInfo.viewGdiDeviceName, q, m)) {
        continue;
      }
      DISPLAYCONFIG_TARGET_DEVICE_NAME targetDeviceName = { 0 };
      targetDeviceName.header.adapterId = paths[i].targetInfo.adapterId;
      targetDeviceName.header.id = paths[i].targetInfo.id;
      targetDeviceName.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_TARGET_NAME;
      targetDeviceName.header.size = sizeof(targetDeviceName);
      hResult = DisplayConfigGetDeviceInfo(&targetDeviceName.header);
      if (S_OK != hResult) {
        free(q);
        q = NULL;
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
      name = _fromWideChar(thread, targetDeviceName.monitorFriendlyDeviceName, wcslen(targetDeviceName.monitorFriendlyDeviceName));
      break;
    }
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    free(q);
    q = NULL;
    free(modes);
    modes = NULL;
    free(paths);
    paths = NULL;
    Arcadia_Thread_jump(thread);
  }
  free(q);
  q = NULL;
  free(modes);
  modes = NULL;
  free(paths);
  paths = NULL;
  return name;
}

static BOOL CALLBACK
enumDisplayMonitorsCallback
  (
    HMONITOR hMonitor,
    HDC hDeviceContext,
    LPRECT lpClipRect,
    LPARAM dwData
  )
{
  EnumDisplayMonitorsContext* enumDisplayMonitorsContext = (EnumDisplayMonitorsContext*)dwData;

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(enumDisplayMonitorsContext->thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {

    MONITORINFOEX monitorInfoEx = { 0 };
    monitorInfoEx.cbSize = sizeof(monitorInfoEx);
    if (!GetMonitorInfoA(hMonitor, (LPMONITORINFO)&monitorInfoEx)) {
      Arcadia_Thread_setStatus(enumDisplayMonitorsContext->thread, Arcadia_Status_EnvironmentFailed);
      return FALSE;
    }

    // (1) Get the ID.
    Arcadia_String* id = _fromMultiByte(enumDisplayMonitorsContext->thread, monitorInfoEx.szDevice, strlen(monitorInfoEx.szDevice));
    // (2) Get the monitor name.
    Arcadia_String* monitorName = Arcadia_Engine_Visuals_Windows_System_getMonitorName(enumDisplayMonitorsContext->thread, monitorInfoEx.szDevice);
    // (2) Get the adapter name
    Arcadia_String* adapterName = Arcadia_Engine_Visuals_Windows_System_getAdapterName(enumDisplayMonitorsContext->thread, monitorInfoEx.szDevice);

    Arcadia_Engine_Visuals_Windows_DisplayDevice* displayDevice = Arcadia_Engine_Visuals_Windows_DisplayDevice_create(enumDisplayMonitorsContext->thread, id, adapterName, monitorName);
    displayDevice->left = monitorInfoEx.rcMonitor.left;
    displayDevice->top = monitorInfoEx.rcMonitor.top;
    displayDevice->right = monitorInfoEx.rcMonitor.right;
    displayDevice->bottom = monitorInfoEx.rcMonitor.bottom;
    displayDevice->hMonitor = hMonitor;

    Arcadia_List_insertBackObjectReferenceValue(enumDisplayMonitorsContext->thread, enumDisplayMonitorsContext->displayDevices,
      (Arcadia_ObjectReferenceValue)displayDevice);

    Arcadia_Thread_popJumpTarget(enumDisplayMonitorsContext->thread);
  } else {
    Arcadia_Thread_popJumpTarget(enumDisplayMonitorsContext->thread);
    return FALSE;
  }
  return TRUE;
}

Arcadia_List*
_getDisplayDevices
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_List* displayDevices = (Arcadia_List*)Arcadia_ArrayList_create(thread);

  // "EnumDisplayMonitors" vs. "EnumDisplayDevices":
  // Monitors can be disabled and remapped by the control panel Display applet.
  // With "EnumDisplayDevices" you get the "raw" view, "EnumDisplayMonitors" gives you the mapped view.
  // You should only care about the mapped view provided by "EnumDisplayMonitors".

  EnumDisplayMonitorsContext enumDisplayMonitorsContext = {
    .thread = thread,
    .displayDevices = displayDevices,
  };
  Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  EnumDisplayMonitors(NULL, NULL, &enumDisplayMonitorsCallback, (LPARAM)&enumDisplayMonitorsContext);
  if (Arcadia_Thread_getStatus(thread)) {
    Arcadia_Thread_jump(thread);
  }
  return displayDevices;
}
