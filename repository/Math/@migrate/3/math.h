/// @file dx/core/math.h
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
#if !defined(DX_MATH_H_INCLUDED)
#define DX_MATH_H_INCLUDED

#include "dx/core/configuration.h"
#include "dx/core/core.h"
#include "Core/Numerics.h"
#include "idlib-math.h"

typedef struct DX_VEC3 DX_VEC3;
typedef struct DX_VEC4 DX_VEC4;
typedef struct Core_InlineRgbR32 Core_InlineRgbR32;
typedef struct Core_InlineRgbaR32 Core_InlineRgbaR32;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static inline void dx_rgb_n8_to_rgba_f32(Core_InlineRgbN8 const* source, Core_Real32 a, Core_InlineRgbaR32* target) {
  target->r = ((Core_Real32)source->r) / 255.f;
  target->g = ((Core_Real32)source->g) / 255.f;
  target->b = ((Core_Real32)source->b) / 255.f;
  target->a = a;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup math
/// @brief Compute the product of a vector and a scalar.
/// @param RETURN A pointer to a <code>Core_InlineVector2R32</code> variable.
/// @param operand1 A pointer to a <code>Core_InlineVector2R32</code> variable.
/// The first operand aka the multiplier.
/// @param operand2 A <code>Core_Real32</code> value.
/// The second operand aka the multiplicand.
/// @remarks @a RETURN and @a operand1 all may refer to the same object.
/// @post <code>*RETURN/<code> was assigned the product.
void Core_InlineVector2R32_mul_vs(Core_InlineVector2R32* RETURN, Core_InlineVector2R32 const* operand1, Core_Real32 operand2);

/// @ingroup math
/// @brief Compute the component-wise product of two vector.
/// @param RETURN A pointer to a <code>Core_InlineVector2R32</code> variable.
/// @param operand1 A pointer to a <code>Core_InlineVector2R32</code> variable.
/// The first operand aka the multiplier.
/// @param operand1 A pointer to a <code>Core_InlineVector2R32</code> variable.
/// The second operand aka the multiplicand.
/// @remarks @a RETURN, @a operand1, and @a operand2 all may refer to the same object.
/// @post <code>*RETURN/<code> was assigned the product.
void Core_InlineVector2R32_mulc_vv(Core_InlineVector2R32* RETURN, Core_InlineVector2R32 const* operand1, Core_InlineVector2R32 const* operand2);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief An offset in 2D space.
typedef struct DX_OFFSET2_F32 {
  Core_Real32 x;
  Core_Real32 y;
} DX_OFFSET2_F32;

static inline void dx_offset2_f32_set(DX_OFFSET2_F32* a, Core_Real32 x, Core_Real32 y) {
  a->x = x;
  a->y = y;
}

/// @brief An extend in 2D space.
typedef struct DX_EXTEND2_F32 {
  Core_Real32 x;
  Core_Real32 y;
} DX_EXTEND2_F32;

static inline void dx_extend2_f32_set(DX_EXTEND2_F32* a, Core_Real32 x, Core_Real32 y) {
  a->x = x;
  a->y = y;
}

typedef struct DX_RECT2_F32 {
  /// @brief An offset in 2D.
  DX_OFFSET2_F32 offset;
  /// @brief An extend in 2D.
  DX_EXTEND2_F32 extend;
} DX_RECT2_F32;

static inline void dx_rect2_f32_set(DX_RECT2_F32* a, Core_Real32 left, Core_Real32 bottom, Core_Real32 right, Core_Real32 top) {
  dx_offset2_f32_set(&a->offset, left, bottom);
  dx_extend2_f32_set(&a->extend, right - left, top - bottom);
}

static inline void dx_rect2_f32_set2(DX_RECT2_F32* a, Core_Real32 left, Core_Real32 bottom, Core_Real32 width, Core_Real32 height) {
  dx_offset2_f32_set(&a->offset, left, bottom);
  dx_extend2_f32_set(&a->extend, width, height);
}

static inline Core_Real32 dx_rect2_f32_get_left(DX_RECT2_F32 const* a) {
  if (a->extend.x < 0.f) {
    return a->offset.x + a->extend.x;
  } else {
    return a->offset.x;
  }
}

static inline Core_Real32 dx_rect2_f32_get_right(DX_RECT2_F32 const* a) {
  if (a->extend.x > 0.f) {
    return a->offset.x + a->extend.x;
  } else {
    return a->offset.x;
  }
}

static inline Core_Real32 dx_rect2_f32_get_bottom(DX_RECT2_F32 const* a) {
  if (a->extend.y < 0.f) {
    return a->offset.y + a->extend.y;
  } else {
    return a->offset.y;
  }
}

static inline Core_Real32 dx_rect2_f32_get_top(DX_RECT2_F32 const* a) {
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

  Core_Real32 l = MIN(dx_rect2_f32_get_left(x), dx_rect2_f32_get_left(y));
  Core_Real32 b = MIN(dx_rect2_f32_get_bottom(x), dx_rect2_f32_get_bottom(y));
  Core_Real32 r = MAX(dx_rect2_f32_get_right(x), dx_rect2_f32_get_right(y));
  Core_Real32 t = MAX(dx_rect2_f32_get_top(x), dx_rect2_f32_get_top(y));

  a->offset.x = l;
  a->offset.y = b;
  a->extend.x = r - l;
  a->extend.y = t - b;

#undef MAX
#undef MIN
#pragma pop_macro("MAX")
#pragma pop_macro("MIN")
}

static inline void dx_rect2_f32_translate(DX_RECT2_F32* r, Core_InlineVector2R32 const* t) {
  r->offset.x += t->v.e[0];
  r->offset.y += t->v.e[1];
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if defined(DX_MATH_WITH_TESTS) && 1 == DX_MATH_WITH_TESTS
Core_Result dx_math_tests();
#endif

#endif // DX_MATH_H_INCLUDED
