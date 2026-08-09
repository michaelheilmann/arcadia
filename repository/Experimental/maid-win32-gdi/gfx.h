#if !defined(GFX_H_INCLUDED)
#define GFX_H_INCLUDED

#include <inttypes.h>

typedef struct gfx_rect {
  int32_t l, t;
  int32_t w, h;
} gfx_rect;

/// @brief The maximum width, in pixels, of a surface.
#define GFX_SURFACE_WIDTH_MAX (INT32_MAX / 8)

/// @brief The maximum height, in pixels, of a surface.
#define GFX_SURFACE_HEIGHT_MAX (INT32_MAX / 8)

/// @brief Fill a surface.
void
gfx_fill_rgb
  (
    void* pixels,
    int32_t width,
    int32_t height,
    uint8_t r,
    uint8_t g,
    uint8_t b
  );

/// @brief Fill a surface.
void
gfx_fill_rgbx
  (
    void* pixels,
    int32_t width,
    int32_t height,
    uint8_t r,
    uint8_t g,
    uint8_t b
  );

/// @brief Fill a sub-surface of a surface.
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
  );

/// @brief Fill a sub-surface of a surface.
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
  );

#endif // GFX_H_INCLUDED
