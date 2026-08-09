#include "dx/core/math.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// sinf, cosf, sqrtf
#include <math.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Core_InlineVector2R32_mul_vs(Core_InlineVector2R32* RETURN, Core_InlineVector2R32 const* operand1, Core_Real32 operand2) {
  RETURN->v.e[0] = operand1->v.e[0] * operand2;
  RETURN->v.e[1] = operand1->v.e[1] * operand2;
}

void Core_InlineVector2R32_mulc_vv(Core_InlineVector2R32* RETURN, Core_InlineVector2R32 const* operand1, Core_InlineVector2R32 const* operand2) {
  RETURN->v.e[0] = operand1->v.e[0] * operand2->v.e[0];
  RETURN->v.e[1] = operand1->v.e[1] * operand2->v.e[1];
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void dx_transform_point(DX_VEC3* u, DX_VEC3 const* v, DX_MAT4 const* m) {
  Core_Real32 e[3];

  e[0] = m->m.e[0][0] * v->v.e[0]
       + m->m.e[0][1] * v->v.e[1]
       + m->m.e[0][2] * v->v.e[2]
       + m->m.e[0][3] * 1.f;

  e[1] = m->m.e[1][0] * v->v.e[0]
       + m->m.e[1][1] * v->v.e[1]
       + m->m.e[1][2] * v->v.e[2]
       + m->m.e[1][3] * 1.f;

  e[2] = m->m.e[2][0] * v->v.e[0]
       + m->m.e[2][1] * v->v.e[1]
       + m->m.e[2][2] * v->v.e[2]
       + m->m.e[2][3] * 1.f;

  u->v.e[0] = e[0];
  u->v.e[1] = e[1];
  u->v.e[2] = e[2];
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if defined(DX_MATH_WITH_TESTS) && 1 == DX_MATH_WITH_TESTS
Core_Result dx_math_tests() {
  DX_VEC3 p = { 0.f, 0.f, -1.f };
  DX_VEC3 q;
  DX_MAT4 a;

#define EQ(x,y) dx_almost_equal_f32((x), (y), 2, FLT_EPSILON*2)

  // f is a counter-clockwise rotation around the y-axis by 0 degrees.
  // it follows that f((0,0,-1)) = (0,0,-1)
  dx_mat4_set_rotate_y(&a, 0.f);
  dx_transform_point(&q, &p, &a);
  if (!(EQ(q.v.e[0], 0.f) && EQ(q.v.e[1], 0.f) && EQ(q.v.e[2], -1.f))) {
    return Core_Failure;
  }

  // f is a counter-clockwise rotation around the y-axis by 90 degrees.
  // it follows that f((0,0,-1)) = (-1,0,0)
  dx_mat4_set_rotate_y(&a, 90.f);
  dx_transform_point(&q, &p, &a);
  if (!(EQ(q.v.e[0], -1.f) && EQ(q.v.e[1], 0.f) && EQ(q.v.e[2], 0.f))) {
    return Core_Failure;
  }

  // f is a counter-clockwise rotation around the y-xis by 180 degrees.
  // it follows that f((0,0,-1))= (0,0,1)
  dx_mat4_set_rotate_y(&a, 180.f);
  dx_transform_point(&q, &p, &a);
  if (!(EQ(q.v.e[0], 0.f) && EQ(q.v.e[1], 0.f) && EQ(q.v.e[2], 1.f))) {
    return Core_Failure;
  }

  // f is a counter-clockwise rotation around the y-xis by 270 degrees.
  // it follows that f((0,0,-1)) = (1,0,0)
  dx_mat4_set_rotate_y(&a, 270.f);
  dx_transform_point(&q, &p, &a);
  if (!(EQ(q.v.e[0], 1.f) && EQ(q.v.e[1], 0.f) && EQ(q.v.e[2], 0.f))) {
    return Core_Failure;
  }
#undef EQ
  return Core_Success;
}
#endif
