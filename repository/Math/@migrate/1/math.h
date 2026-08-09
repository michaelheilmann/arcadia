/// @file dx/core/math.h
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
#if !defined(DX_MATH_H_INCLUDED)
#define DX_MATH_H_INCLUDED

#include "dx/core/configuration.h"
#include "dx/core/core.h"

typedef struct DX_VEC3 DX_VEC3;
typedef struct DX_VEC4 DX_VEC4;
typedef struct DX_RGB_F32 DX_RGB_F32;
typedef struct DX_RGBA_F32 DX_RGBA_F32;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void dx_rgb_f32_lerp(DX_RGB_F32 const* a, DX_RGB_F32 const* b, dx_f32 t, DX_RGB_F32* c);
void dx_rgba_f32_lerp(DX_RGBA_F32 const* a, DX_RGBA_F32 const* b, dx_f32 t, DX_RGBA_F32* c);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static inline void dx_rgb_n8_to_rgba_f32(DX_RGB_N8 const* source, dx_f32 a, DX_RGBA_F32* target) {
  target->r = ((dx_f32)source->r) / 255.f;
  target->g = ((dx_f32)source->g) / 255.f;
  target->b = ((dx_f32)source->b) / 255.f;
  target->a = a;
}


static inline void
Core_InlineRgbaN8_to_rgba_f32(Core_InlineRgbaN8 const* source, Core_Real32 a, Core_InlineRgbaR32* target) {
  target->r = ((Core_Real32)source->r) / 255.f;
  target->g = ((Core_Real32)source->g) / 255.f;
  target->b = ((Core_Real32)source->b) / 255.f;
  target->a = ((Core_Real32)source->a) / 255.f;;
}

static inline void Core_InlineRgbaN8_to_rgba_f32(Core_InlineRgbaN8 const* source, Core_Real32 a, Core_InlineRgbaR32* target) {
  target->r = ((Core_Real32)source->r) / 255.f;
  target->g = ((Core_Real32)source->g) / 255.f;
  target->b = ((Core_Real32)source->b) / 255.f;
  target->a = ((Core_Real32)source->a) / 255.f;;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup math
/// Transform a position vector.
/// @param u Pointer to a DX_VEC3 object. The object receives the transformed position vector.
/// @param v Pointer to a DX_VEC3 object. The object represents the position to transform.
/// @param m Pointer to a DX_MAT4 object. The object represents the transformation matrix.
void dx_transform_point(DX_VEC3* u, DX_VEC3 const* v, DX_MAT4 const* m);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief An offset in 2D space.
typedef struct DX_OFFSET2_F32 {
  dx_f32 x;
  dx_f32 y;
} DX_OFFSET2_F32;

static inline void dx_offset2_f32_set(DX_OFFSET2_F32* a, dx_f32 x, dx_f32 y) {
  a->x = x;
  a->y = y;
}

/// @brief An extend in 2D space.
typedef struct DX_EXTEND2_F32 {
  dx_f32 x;
  dx_f32 y;
} DX_EXTEND2_F32;

static inline void dx_extend2_f32_set(DX_EXTEND2_F32* a, dx_f32 x, dx_f32 y) {
  a->x = x;
  a->y = y;
}

typedef struct DX_RECT2_F32 {
  /// @brief An offset in 2D.
  DX_OFFSET2_F32 offset;
  /// @brief An extend in 2D.
  DX_EXTEND2_F32 extend;
} DX_RECT2_F32;

static inline void dx_rect2_f32_set(DX_RECT2_F32* a, dx_f32 left, dx_f32 bottom, dx_f32 right, dx_f32 top) {
  dx_offset2_f32_set(&a->offset, left, bottom);
  dx_extend2_f32_set(&a->extend, right - left, top - bottom);
}

static inline void dx_rect2_f32_set2(DX_RECT2_F32* a, dx_f32 left, dx_f32 bottom, dx_f32 width, dx_f32 height) {
  dx_offset2_f32_set(&a->offset, left, bottom);
  dx_extend2_f32_set(&a->extend, width, height);
}

static inline dx_f32 dx_rect2_f32_get_left(DX_RECT2_F32 const* a) {
  if (a->extend.x < 0.f) {
    return a->offset.x + a->extend.x;
  } else {
    return a->offset.x;
  }
}

static inline dx_f32 dx_rect2_f32_get_right(DX_RECT2_F32 const* a) {
  if (a->extend.x > 0.f) {
    return a->offset.x + a->extend.x;
  } else {
    return a->offset.x;
  }
}

static inline dx_f32 dx_rect2_f32_get_bottom(DX_RECT2_F32 const* a) {
  if (a->extend.y < 0.f) {
    return a->offset.y + a->extend.y;
  } else {
    return a->offset.y;
  }
}

static inline dx_f32 dx_rect2_f32_get_top(DX_RECT2_F32 const* a) {
  if (a->extend.y > 0.f) {
    return a->offset.y + a->extend.y;
  } else {
    return a->offset.y;
  }
}

static inline void dx_rect2_f32_union(DX_RECT2_F32* a, DX_RECT2_F32 const* x, DX_RECT2_F32 const* y) {
#pragma push_macro("MIN")
#pragma push_macro("MAX")
#define MIN(x,y) (x) < (y) ? (x) : (y)
#define MAX(x,y) (x) > (y) ? (x) : (y)

  dx_f32 l = MIN(dx_rect2_f32_get_left(x), dx_rect2_f32_get_left(y));
  dx_f32 b = MIN(dx_rect2_f32_get_bottom(x), dx_rect2_f32_get_bottom(y));
  dx_f32 r = MAX(dx_rect2_f32_get_right(x), dx_rect2_f32_get_right(y));
  dx_f32 t = MAX(dx_rect2_f32_get_top(x), dx_rect2_f32_get_top(y));

  a->offset.x = l;
  a->offset.y = b;
  a->extend.x = r - l;
  a->extend.y = t - b;

#undef MAX
#undef MIN
#pragma pop_macro("MAX")
#pragma pop_macro("MIN")
}

static inline void dx_rect2_f32_translate(DX_RECT2_F32* r, DX_VEC2_F32 const* t) {
  r->offset.x += t->e[0];
  r->offset.y += t->e[1];
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if defined(DX_MATH_WITH_TESTS) && 1 == DX_MATH_WITH_TESTS
Core_Result dx_math_tests();
#endif

#endif // DX_MATH_H_INCLUDED
