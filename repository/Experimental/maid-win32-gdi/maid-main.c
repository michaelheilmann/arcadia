#include <Windows.h>

#include "gfx.h"
#include "maid-string.h"
#include <string.h> // strcpy, strlen
#include <stdio.h> // snprintf
#include <inttypes.h> // uint64_t, UINT64_MAX, PRIu64


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



void DestroyBackBuffer(HWND hWnd);
void RecreateBackBuffer(HWND hWnd);

typedef struct Entity {
  // position
  LONG px, py;
  // speed + direction
  LONG dx, dy;
} Entity;

typedef struct World {
  LONG width, height;
} World;

struct Entity g_entity = { .px = 0, .py = 0, .dx = 0, .dy = 0 };
struct World g_world = { .width = 256, .height = 256 };

/* A "not so great" frames per second counter. */
typedef struct FPS {
  STRING* message;
  uint64_t numberOfFrames; // The number of frames accumulated.
  uint64_t numberOfMilliseconds; // The number of milliseconds accumulated.
  uint64_t lastUpdate; // The point in time of the last update.
} FPS;

int FPS_create(FPS** result) {
  if (!result) {
    return 1;
  }
  FPS* this = malloc(sizeof(FPS));
  if (!this) {
    return 1;
  }
  if (STRING_create(&this->message)) {
    free(this);
    this = NULL;
    return 1;
  }
  this->numberOfFrames = 0;
  this->numberOfMilliseconds = 0;
  this->lastUpdate = UINT64_MAX;
  *result = this;
  return 0;
}

int FPS_destroy(FPS* this) {
  if (!this) {
    return 1;
  }
  STRING_destroy(this->message);
  this->message = NULL;
  free(this);
  return 0;
}

int FPS_update(FPS* this) {
  if (!this) {
    return 1;
  }
  static_assert(MAXDWORD <= UINT64_MAX, "<environment not supported>");
  uint64_t now = (uint64_t)GetTickCount();
  this->numberOfFrames++;
  if (this->lastUpdate > now) {
    // Timer wrapped around or update was invoked for the first time.
    this->lastUpdate = now;
  }
  uint64_t delta = now - this->lastUpdate;
  this->numberOfMilliseconds += delta;
  this->lastUpdate = now;
  if (this->numberOfMilliseconds > 1000) {
    if (STRING_printf(this->message, "%"PRIu64" FPS", (uint64_t)this->numberOfFrames)) {
      return 1;
    }
    this->numberOfFrames = 0;
    this->numberOfMilliseconds -= 1000;
  }
  return 0;
}

static HDC winDC = NULL;

typedef struct BackBuffer {
  HDC hDC;
  HBITMAP hBitmap;
  void* pixels;
  uint32_t width, height;
} BackBuffer;

static BackBuffer g_backBuffer = { .hDC = NULL, .hBitmap = NULL, .pixels = NULL, .width = 0, .height = 0 };

static HBITMAP memOldMap = NULL;

static int running = 1;

static UINT_PTR g_timerID = (UINT_PTR)NULL;

static FPS *g_fps = NULL;

static const char* CLASSNAME = "<double buffer window class>";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

  WNDCLASSEX wc;
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.lpfnWndProc = WindowProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
  wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
  wc.hbrBackground = NULL;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = CLASSNAME;
  wc.hIconSm = NULL;

  if (!RegisterClassEx(&wc)) {
    return 0;
  }

  HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW, CLASSNAME, "Double Buffer",
    WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, NULL, hInstance, NULL);
  if (!hWnd) {
    UnregisterClass(CLASSNAME, hInstance);
    return 0;
  }

  ShowWindow(hWnd, SW_SHOWDEFAULT);
  UpdateWindow(hWnd);

  MSG msg;

  while (running) {

    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

  }
  UnregisterClass(CLASSNAME, hInstance);
  return (int)msg.lParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

  switch (uMsg) {
    case WM_CLOSE:
      running = 0;
      break;

    case WM_ERASEBKGND:
      return 1;

    case WM_DESTROY:
      if (g_fps) {
        FPS_destroy(g_fps);
        g_fps = NULL;
      }
      if (g_timerID) {
        KillTimer(hWnd, g_timerID);
        g_timerID = (UINT_PTR)NULL;
      }
      DestroyBackBuffer(hWnd);
      PostQuitMessage(0);
      return 0;

    case WM_TIMER: {
      // Move the enitity.
      if (g_entity.px - 8 < 0) {
        g_entity.dx = +1;
      }
      if (g_entity.py - 8 < 0) {
        g_entity.dy = +1;
      }
      if (g_entity.px + 8 > g_world.width) {
        g_entity.dx = -1;
      }
      if (g_entity.py + 8 > g_world.height) {
        g_entity.dy = -1;
      }
      g_entity.px += g_entity.dx;
      g_entity.py += g_entity.dy;
      // invalidate everything.
      InvalidateRect(hWnd, NULL, TRUE);
      return 0;
    } break;

    case WM_CREATE: {
      g_timerID = (UINT_PTR)1;
      if (!SetTimer(hWnd, g_timerID, 10, NULL)) {
        g_timerID = (UINT_PTR)NULL;
        return -1;
      }
      // Initialize the FPS counter text.
      if (FPS_create(&g_fps)) {
        return -1;
      }
      // Place the entity at the center of the world.
      g_entity.px = g_world.width / 2;
      g_entity.py = g_world.height / 2;
      g_entity.dx = 1;
      g_entity.dy = 1;
      // Re-create the back buffer.
      RecreateBackBuffer(hWnd);
    } break;

    case WM_SIZE: {
      // Re-create the back buffer.
      RecreateBackBuffer(hWnd);
    } break;

    case WM_PAINT: {

      PAINTSTRUCT ps;
      RECT r;

      GetClientRect(hWnd, &r);
      gfx_fill_rgbx(g_backBuffer.pixels, g_backBuffer.width, g_backBuffer.height, 0, 0, 0);

      // Always put the world's middle on the center of the screen.
      LONG canvasCX = (r.right - r.left) / 2;
      LONG canvasCY = (r.bottom - r.top) / 2;

      // The projection transformation.
      LONG tx = -g_world.width / 2 + canvasCX;
      LONG ty = -g_world.height / 2 + canvasCY;

      // Draw the world at the middle of the window.
      gfx_rect grayRect;
      grayRect.l = 0 + tx;
      grayRect.t = 0 + ty;
      grayRect.w = g_world.height;
      grayRect.h = g_world.width;
      gfx_fill_rect_rgbx(g_backBuffer.pixels, g_backBuffer.width, g_backBuffer.height, 128, 128, 128, &grayRect);

      // Draw the entity in the world.
      gfx_rect redRect;
      redRect.l = g_entity.px - 8 + tx;
      redRect.t = g_entity.py - 8 + ty;
      redRect.w = 16;
      redRect.h = 16;
      gfx_fill_rect_rgbx(g_backBuffer.pixels, g_backBuffer.width, g_backBuffer.height, 255, 0, 0, &redRect);

      // Draw the FPS.
      TextOut(g_backBuffer.hDC, 0, 0, g_fps->message->p, g_fps->message->n);

      FPS_update(g_fps);

      HDC hdc = BeginPaint(hWnd, &ps);
      BitBlt(hdc, 0, 0, r.right - r.left, r.bottom - r.top, g_backBuffer.hDC, 0, 0, SRCCOPY);
      EndPaint(hWnd, &ps);
    } break;
  }

  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void DestroyBackBuffer(HWND hWnd) {

  if (g_backBuffer.hDC != NULL) {
    SelectObject(g_backBuffer.hDC, memOldMap);

    DeleteObject(g_backBuffer.hBitmap);
    g_backBuffer.hBitmap = NULL;

    DeleteDC(g_backBuffer.hDC);
    g_backBuffer.hDC = NULL;
    
    memOldMap = NULL;
  }

  if (winDC != NULL) {
    ReleaseDC(hWnd, winDC);
    winDC = NULL;
  }

}

void RecreateBackBuffer(HWND hWnd) {

  DestroyBackBuffer(hWnd);

  RECT client;

  GetClientRect(hWnd, &client);
  winDC = GetDC(hWnd);

  long width = client.right - client.left;
  long height = client.bottom - client.top;
  g_backBuffer.hDC = CreateCompatibleDC(winDC);
#if 1
  BITMAPINFO bmi;
  memset(&bmi, 0, sizeof(BITMAPINFO));
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight = -height;  // top-down
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
  g_backBuffer.hBitmap = CreateDIBSection(winDC, &bmi, DIB_RGB_COLORS, (void**)&g_backBuffer.pixels, NULL, 0);
  if (!g_backBuffer.hBitmap) {
    return;
  }
#else
  g_backBuffer.hBitmap = CreateCompatibleBitmap(winDC, width, height);
#endif
  g_backBuffer.width = width;
  g_backBuffer.height = height;

  memOldMap = (HBITMAP)SelectObject(g_backBuffer.hDC, g_backBuffer.hBitmap);

}
