/// @file dx/core/math.h
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
#if !defined(DX_MATH_H_INCLUDED)
#define DX_MATH_H_INCLUDED

#include "dx/core/configuration.h"
#include "dx/core/core.h"
#include "Core/Numerics.h"

typedef struct DX_VEC3 DX_VEC3;
typedef struct DX_VEC4 DX_VEC4;
typedef struct Core_InlineRgbR32 Core_InlineRgbR32;
typedef struct Core_InlineRgbaR32 Core_InlineRgbaR32;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Core_InlineRgbR32 {
  Core_Real32 r;
  Core_Real32 g;
  Core_Real32 b;
} Core_InlineRgbR32;

static inline void Core_InlineRgbR32_set(Core_InlineRgbR32* c, Core_Real32 r, Core_Real32 g, Core_Real32 b) {
  c->r = r;
  c->g = g;
  c->b = b;
}

static inline Core_Boolean Core_InlineRgbR32_areEqual(Core_InlineRgbR32 const* v, Core_InlineRgbR32 const* w) {
  return v->r == w->r
      && v->g == w->g
      && v->b == w->b;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static inline void dx_rgb_n8_to_rgba_f32(Core_InlineRgbN8 const* source, Core_Real32 a, Core_InlineRgbaR32* target) {
  target->r = ((Core_Real32)source->r) / 255.f;
  target->g = ((Core_Real32)source->g) / 255.f;
  target->b = ((Core_Real32)source->b) / 255.f;
  target->a = a;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// The color "Capri"  (0, 191, 255).
extern DX_RGB_N8 const dx_colors_capri;

// The color "Malachite" (0, 255, 64) from "Capri"'s tetradic palette.
extern DX_RGB_N8 const dx_colors_malachite;

// The color "Amber" (255, 192, 0) from "Capri"'s Split Complementary palette.
extern DX_RGB_N8 const dx_colors_amber;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Symbolic constant for the Core_Real32 representation of PI.
#define DX_PI_F32 3.1415926f

// @brief Convert an angle in degrees into the corresponding angle in radians.
// @param a The angle in degrees.
// @return The corresponding angle in radians.
static inline Core_Real32 dx_deg_to_rad(Core_Real32 x) {
  return (x / 180.f) * DX_PI_F32;
}

// @brief Clamp a value to the range [0,1].
// @param x The value to be clamped.
// @return x clamped.
static inline Core_Real32 dx_clamp(Core_Real32 x) {
  if (x < 0.f) return 0.f;
  else if (x > 1.f) return 1.f;
  else return x;
}

static inline void dx_lerp(Core_Real32 *result, Core_Real32 start, Core_Real32 end, Core_Real32 t) {
  t = dx_clamp(t);
  if (t == 0.f) {
    *result = start;
  } else if (t == 1.f) {
    *result = end;
  } else {
    *result = (1.f - t) * start + t * end;
  }
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

/// @ingroup math
/// @brief Compute the cross product of two vectors.
/// @param w Pointer to a DX_VEC3 object.
/// @param u Pointer to a DX_VEC3 object.
/// The object's values represent the vector, that is the 1st operand.
/// @param v Pointer to a DX_VEC3 object.
/// The object's values represent the vector, that is the 2nd operand.
/// @remarks @a w, @a u, and @a v all may refer to the same object.
/// @post <code>*w/<code> was assigned the values of the cross product vector.
void dx_vec3_cross(DX_VEC3* w, DX_VEC3 const* u, DX_VEC3 const* v) {
  Core_Real32 t[3];
  t[0] = u->e[1] * v->e[2] - u->e[2] * v->e[1];
  t[1] = u->e[2] * v->e[0] - u->e[0] * v->e[2];
  t[2] = u->e[0] * v->e[1] - u->e[1] * v->e[0];
  w->e[0] = t[0];
  w->e[1] = t[1];
  w->e[2] = t[2];
}

/// @ingroup math
/// Compute a normalized vector.
/// @param v the vector receiving the normalized vector
/// @param u the vector to normalize
/// @remarks The result is the normalized vector of the vector to be normalized if the vector to be normalized is a non-zero vector.
/// Otherwise the result is the zero vector.
/// @a v and @a u may refer to the same object.
static inline void dx_vec3_norm(DX_VEC3* u, DX_VEC3 const* v) {
  Core_Real32 sql = dx_vec3_sql(v);
  if (sql == 0.f) {
    u->e[0] = 0.f;
    u->e[1] = 0.f;
    u->e[2] = 0.f;
    return;
  }
  Core_Real32 l = sqrtf(sql);
  u->e[0] = v->e[0] / l;
  u->e[1] = v->e[1] / l;
  u->e[2] = v->e[2] / l;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct DX_VEC4 {
  Core_Real32 e[4];
} DX_VEC4;

static inline void dx_vec4_set(DX_VEC4* v, Core_Real32 x, Core_Real32 y, Core_Real32 z, Core_Real32 w) {
  v->e[0] = x;
  v->e[1] = y;
  v->e[2] = z;
  v->e[3] = w;
}

static inline Core_Boolean dx_vec4_are_equal(DX_VEC4 const* u, DX_VEC4 const* v) {
  return u->e[0] == v->e[0]
      && u->e[1] == v->e[1]
      && u->e[2] == v->e[2]
      && u->e[3] == v->e[3];
}

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
  r->offset.x += t->e[0];
  r->offset.y += t->e[1];
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if defined(DX_MATH_WITH_TESTS) && 1 == DX_MATH_WITH_TESTS
Core_Result dx_math_tests();
#endif

#endif // DX_MATH_H_INCLUDED
