// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Arcadia/Math/Implementation/NURBS3AdaptiveReal32.h"
#include <math.h>

#define Scalar Arcadia_Real32Value
#define Eval Arcadia_Math_NURBS3Real32_evaluateCurve
#define Adaptive Arcadia_Math_NURBS3AdaptiveReal32

typedef struct AdaptiveContext {
  const Scalar (*points)[3];
  const Scalar* weights;
  Arcadia_SizeValue count, degree, knotCount;
  const Scalar* knots;
  Scalar* workspace;
  Arcadia_SizeValue workspaceElements;
  double tolerance;
  Arcadia_SizeValue maxDepth, segments, capacity;
  Scalar (*output)[3];
} AdaptiveContext;

static Arcadia_BooleanValue
sample(AdaptiveContext* ctx, Scalar parameter, Scalar result[3])
{
  if (!isfinite(parameter)) return Arcadia_BooleanValue_False;
  return Eval(ctx->points, ctx->weights, ctx->count, ctx->degree,
      ctx->knots, ctx->knotCount, parameter, ctx->workspace,
      ctx->workspaceElements, result);
}

// Depth-first traversal emits segments in increasing parameter order.
// A null output performs a complete count-only preflight traversal.
static Arcadia_BooleanValue
subdivide(AdaptiveContext* ctx, Scalar t0, const Scalar a[3],
    Scalar t1, const Scalar b[3], Arcadia_SizeValue depth)
{
  if (depth < ctx->maxDepth) {
    Scalar mid = (Scalar)(0.5 * (double)t0 + 0.5 * (double)t1);
    if (mid > t0 && mid < t1) {
      Scalar m[3];
      if (!sample(ctx, mid, m)) return Arcadia_BooleanValue_False;
      double dx=(double)m[0]-0.5*((double)a[0]+(double)b[0]);
      double dy=(double)m[1]-0.5*((double)a[1]+(double)b[1]);
      double dz=(double)m[2]-0.5*((double)a[2]+(double)b[2]);
      double error=hypot(hypot(dx,dy),dz);
      if (!isfinite(error)) return Arcadia_BooleanValue_False;
      if (error > ctx->tolerance) {
        if (!subdivide(ctx,t0,a,mid,m,depth+1) ||
            !subdivide(ctx,mid,m,t1,b,depth+1))
          return Arcadia_BooleanValue_False;
        return Arcadia_BooleanValue_True;
      }
    }
  }
  if (ctx->segments >= ctx->capacity) return Arcadia_BooleanValue_False;
  if (ctx->output) {
    for (Arcadia_SizeValue k=0;k<3;++k) {
      ctx->output[2*ctx->segments][k]=a[k];
      ctx->output[2*ctx->segments+1][k]=b[k];
    }
  }
  ++ctx->segments;
  return Arcadia_BooleanValue_True;
}

static Arcadia_BooleanValue
validate(const Scalar (*points)[3], const Scalar* weights,
    Arcadia_SizeValue count, Arcadia_SizeValue degree,
    const Scalar* knots, Arcadia_SizeValue knotCount,
    Scalar tolerance, Arcadia_SizeValue maxDepth,
    const Scalar* workspace, Arcadia_SizeValue workspaceElements)
{
  if (!points || !knots || !workspace || !count || degree >= count ||
      knotCount <= count || knotCount-count-1 != degree ||
      !isfinite(tolerance) || tolerance < (Scalar)0 || maxDepth > 20 ||
      workspaceElements/3 < degree+1 ||
      !(knots[degree] < knots[count])) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i=0;i<knotCount;++i)
    if (!isfinite(knots[i]) || (i && knots[i] < knots[i-1]))
      return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i=0;i<count;++i) {
    if (weights && (!isfinite(weights[i]) || weights[i] <= (Scalar)0))
      return Arcadia_BooleanValue_False;
    for (Arcadia_SizeValue j=0;j<3;++j)
      if (!isfinite(points[i][j])) return Arcadia_BooleanValue_False;
  }
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3AdaptiveReal32_sampleLines(
    const Scalar (*points)[3], const Scalar* weights,
    Arcadia_SizeValue count, Arcadia_SizeValue degree,
    const Scalar* knots, Arcadia_SizeValue knotCount,
    Scalar tolerance, Arcadia_SizeValue maxDepth,
    Scalar (*positions)[3], Arcadia_SizeValue vertexCapacity,
    Arcadia_SizeValue* vertexCount,
    Scalar* workspace, Arcadia_SizeValue workspaceElements)
{
  if (!positions || !vertexCount || vertexCapacity < 2 ||
      !validate(points,weights,count,degree,knots,knotCount,
        tolerance,maxDepth,workspace,workspaceElements))
    return Arcadia_BooleanValue_False;
  Scalar a[3], b[3];
  AdaptiveContext ctx={points,weights,count,degree,knotCount,knots,workspace,
      workspaceElements,0.,maxDepth,0,vertexCapacity/2,NULL};
  ctx.tolerance = (double)tolerance;
  if (!sample(&ctx,knots[degree],a) || !sample(&ctx,knots[count],b) ||
      !subdivide(&ctx,knots[degree],a,knots[count],b,0))
    return Arcadia_BooleanValue_False;
  Arcadia_SizeValue segments=ctx.segments;
  ctx.segments=0;
  ctx.output=positions;
  if (!subdivide(&ctx,knots[degree],a,knots[count],b,0))
    return Arcadia_BooleanValue_False;
  *vertexCount=2*segments;
  return Arcadia_BooleanValue_True;
}

#undef Scalar
#undef Eval
#undef Adaptive
