#include "gfx.h"

void
gfx_fill_rgb
  (
    void* pixels,
    int32_t width,
    int32_t height,
    uint8_t r,
    uint8_t g,
    uint8_t b
  )
{
  if (width < 0 || width > GFX_SURFACE_WIDTH_MAX) {
    return;
  }
  if (height < 0 || height > GFX_SURFACE_HEIGHT_MAX) {
    return;
  }
  if (!pixels) {
    return;
  }
  size_t pitch = 3 * width;
  for (uint32_t y = 0; y < height; ++y) {
    for (uint32_t x = 0; x < width; ++x) {
      size_t n = 3 * x + y * pitch;
      uint8_t* p = ((uint8_t*)pixels) + n;
      p[0] = b;
      p[1] = g;
      p[2] = r;
    }
  }
}

void
gfx_fill_rgbx
  (
    void* pixels,
    int32_t width,
    int32_t height,
    uint8_t r,
    uint8_t g,
    uint8_t b
  )
{
  if (width < 0 || width > GFX_SURFACE_WIDTH_MAX) {
    return;
  }
  if (height < 0 || height > GFX_SURFACE_HEIGHT_MAX) {
    return;
  }
  if (!pixels) {
    return;
  }
  size_t pitch = 4 * width;
  for (uint32_t y = 0; y < height; ++y) {
    for (uint32_t x = 0; x < width; ++x) {
      size_t n = 4 * x + y * pitch;
      uint8_t* p = ((uint8_t*)pixels) + n;
      p[0] = b;
      p[1] = g;
      p[2] = r;
      p[3] = 0; // unused
    }
  }
}

void
gfx_fill_rect_rgb
  (
    void* pixels,
    int32_t width,
    int32_t height,
    uint8_t r,
    uint8_t g,
    uint8_t b,
    gfx_rect* rect
  )
{
  if (width < 0 || width > GFX_SURFACE_WIDTH_MAX) {
    return;
  }
  if (height < 0 || height > GFX_SURFACE_HEIGHT_MAX) {
    return;
  }
  if (!pixels) {
    return;
  }

  gfx_rect a;

  if (rect) {
    a = *rect;

    // We can stop immediatly.
    if (a.l >= width) return;
    // We can stop immediatly.
    if (a.t >= height) return;

    int32_t d;

    d = a.l + a.w;
    if (d > width) a.w -= d - width; // a.w -= d - width will always be sound
    if (!a.w) return;

    d = a.t + a.h;
    if (b > height) a.h -= d - height; // a.h -= d - height will always be sound
    if (!a.h) return;

  } else {
    a.l = a.t = 0;
    a.w = width;
    a.h = height;
  }

  size_t pitch = 4 * width;
  for (uint32_t y = a.t; y < a.t + a.h; ++y) {
    for (uint32_t x = a.l; x < a.l + a.w; ++x) {
      size_t n = 3 * x + y * pitch;
      uint8_t* p = ((uint8_t*)pixels) + n;
      p[0] = b;
      p[1] = g;
      p[2] = r;
    }
  }
}

void
gfx_fill_rect_rgbx
  (
    void* pixels,
    int32_t width,
    int32_t height,
    uint8_t r,
    uint8_t g,
    uint8_t b,
    gfx_rect* rect
  )
{
  if (width < 0 || width > GFX_SURFACE_WIDTH_MAX) {
    return;
  }
  if (height < 0 || height > GFX_SURFACE_HEIGHT_MAX) {
    return;
  }
  if (!pixels) {
    return;
  }

  gfx_rect a;

  if (rect) {
    a = *rect;

    // We can stop immediatly.
    if (a.l >= width) return;
    // We can stop immediatly.
    if (a.t >= height) return;

    int32_t d;

    d = a.l + a.w;
    if (d > width) a.w -= d - width; // a.w -= d - width will always be sound
    if (!a.w) return;

    d = a.t + a.h;
    if (b > height) a.h -= d - height; // a.h -= d - height will always be sound
    if (!a.h) return;

  } else {
    a.l = a.t = 0;
    a.w = width;
    a.h = height;
  }

  size_t pitch = 4 * width;
  for (uint32_t y = a.t; y < a.t + a.h; ++y) {
    for (uint32_t x = a.l; x < a.l + a.w; ++x) {
      size_t n = 4 * x + y * pitch;
      uint8_t* p = ((uint8_t*)pixels) + n;
      p[0] = b;
      p[1] = g;
      p[2] = r;
      p[3] = 0; // unused
    }
  }
}
