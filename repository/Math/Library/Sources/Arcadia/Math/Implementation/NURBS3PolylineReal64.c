// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include "Arcadia/Math/Implementation/NURBS3PolylineReal64.h"
#include <math.h>
#include <float.h>

static Arcadia_BooleanValue
validPoints(const Arcadia_Real64Value (*points)[3], Arcadia_SizeValue count)
{
  if (!points || count < 2 || count > ((Arcadia_SizeValue)-1)/3)
    return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i=0; i<count; ++i)
    for (Arcadia_SizeValue k=0; k<3; ++k)
      if (!isfinite(points[i][k])) return Arcadia_BooleanValue_False;
  return Arcadia_BooleanValue_True;
}

static double
square3(const double x[3])
{
  return x[0]*x[0] + x[1]*x[1] + x[2]*x[2];
}

static double
scalar3(const double x[3], const double y[3])
{
  return x[0]*y[0] + x[1]*y[1] + x[2]*y[2];
}

static double
clampUnit(double x)
{
  return x < 0. ? 0. : x > 1. ? 1. : x;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3PolylineReal64_cumulativeLengths(const Arcadia_Real64Value (*points)[3],
    Arcadia_SizeValue count, Arcadia_Real64Value* distances, Arcadia_SizeValue capacity)
{
  if (!distances || capacity < count || !validPoints(points,count))
    return Arcadia_BooleanValue_False;
  // Preflight every segment to guarantee output preservation on overflow.
  double total=0.;
  for (Arcadia_SizeValue i=1; i<count; ++i) {
    double dx=(double)points[i][0]-(double)points[i-1][0];
    double dy=(double)points[i][1]-(double)points[i-1][1];
    double dz=(double)points[i][2]-(double)points[i-1][2];
    total += hypot(hypot(dx,dy),dz);
    if (!isfinite(total) || !isfinite((Arcadia_Real64Value)total))
      return Arcadia_BooleanValue_False;
  }
  // The second pass cannot fail after identical finite preflight inputs.
  distances[0]=(Arcadia_Real64Value)0;
  total=0.;
  for (Arcadia_SizeValue i=1; i<count; ++i) {
    double dx=(double)points[i][0]-(double)points[i-1][0];
    double dy=(double)points[i][1]-(double)points[i-1][1];
    double dz=(double)points[i][2]-(double)points[i-1][2];
    total += hypot(hypot(dx,dy),dz);
    distances[i]=(Arcadia_Real64Value)total;
  }
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3PolylineReal64_resampleByLength(const Arcadia_Real64Value (*points)[3],
    const Arcadia_Real64Value* distances, Arcadia_SizeValue count, Arcadia_SizeValue samples,
    Arcadia_Real64Value (*result)[3], Arcadia_SizeValue capacity)
{
  if (!distances || !result || samples < 2 || capacity < samples ||
      !validPoints(points,count) || distances[0] != (Arcadia_Real64Value)0)
    return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i=0; i<count; ++i) {
    if (!isfinite(distances[i]) || distances[i] < (Arcadia_Real64Value)0 ||
        (i && distances[i] < distances[i-1]))
      return Arcadia_BooleanValue_False;
  }
  if (!(distances[count-1] > (Arcadia_Real64Value)0)) return Arcadia_BooleanValue_False;
  // Preflight interpolations before writing any caller-owned data.
  for (int pass=0; pass<2; ++pass) {
    Arcadia_SizeValue interval=0;
    for (Arcadia_SizeValue j=0; j<samples; ++j) {
      if (j==0 || j==samples-1) {
        const Arcadia_Real64Value *p=points[j==0 ? 0 : count-1];
        if (pass) for (Arcadia_SizeValue k=0;k<3;++k) result[j][k]=p[k];
        continue;
      }
      double target=(double)distances[count-1]*((double)j/(double)(samples-1));
      if (!isfinite(target)) return Arcadia_BooleanValue_False;
      while (interval+1<count-1 && (double)distances[interval+1]<target)
        ++interval;
      Arcadia_SizeValue next=interval+1;
      double width=(double)distances[next]-(double)distances[interval];
      double t=width>0. ? clampUnit((target-(double)distances[interval])/width) : 0.;
      Arcadia_Real64Value computed[3];
      for (Arcadia_SizeValue k=0;k<3;++k) {
        double value=(double)points[interval][k] +
          ((double)points[next][k]-(double)points[interval][k])*t;
        if (!isfinite(value) || !isfinite((Arcadia_Real64Value)value))
          return Arcadia_BooleanValue_False;
        computed[k]=(Arcadia_Real64Value)value;
      }
      if (pass) for (Arcadia_SizeValue k=0;k<3;++k) result[j][k]=computed[k];
    }
  }
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3PolylineReal64_closestPoint(const Arcadia_Real64Value (*points)[3], Arcadia_SizeValue count,
    const Arcadia_Real64Value query[3], Arcadia_Real64Value result[3], Arcadia_SizeValue* segment,
    Arcadia_Real64Value* fraction, Arcadia_Real64Value* distance)
{
  if (!query || !result || !validPoints(points,count)) return Arcadia_BooleanValue_False;
  double q[3],best[3]={0.,0.,0.},bestD=HUGE_VAL,bestT=0.;
  Arcadia_SizeValue bestIndex=0;
  for (Arcadia_SizeValue k=0;k<3;++k) {
    if (!isfinite(query[k])) return Arcadia_BooleanValue_False;
    q[k]=query[k];
  }
  for (Arcadia_SizeValue i=0;i<count-1;++i) {
    double start[3],delta[3],from[3],candidate[3],diff[3];
    for (Arcadia_SizeValue k=0;k<3;++k) {
      start[k]=points[i][k];
      delta[k]=(double)points[i+1][k]-start[k];
      from[k]=q[k]-start[k];
    }
    double len2=square3(delta);
    double t=len2>0. ? clampUnit(scalar3(from,delta)/len2) : 0.;
    for (Arcadia_SizeValue k=0;k<3;++k) {
      candidate[k]=start[k]+t*delta[k];
      diff[k]=q[k]-candidate[k];
    }
    double d=square3(diff);
    if (!isfinite(d)) return Arcadia_BooleanValue_False;
    if (d<bestD) {
      bestD=d;bestT=t;bestIndex=i;
      for (Arcadia_SizeValue k=0;k<3;++k) best[k]=candidate[k];
    }
  }
  double d=sqrt(bestD);
  if (!isfinite(d) || !isfinite((Arcadia_Real64Value)d) ||
      !isfinite((Arcadia_Real64Value)bestT)) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue k=0;k<3;++k)
    if (!isfinite(best[k]) || !isfinite((Arcadia_Real64Value)best[k])) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue k=0;k<3;++k) result[k]=(Arcadia_Real64Value)best[k];
  if (segment) *segment=bestIndex;
  if (fraction) *fraction=(Arcadia_Real64Value)bestT;
  if (distance) *distance=(Arcadia_Real64Value)d;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3PolylineReal64_pickRay(const Arcadia_Real64Value (*points)[3], Arcadia_SizeValue count,
    const Arcadia_Math_Ray3Real64* ray, Arcadia_Real64Value radius,
    Arcadia_SizeValue* segment, Arcadia_Real64Value* rayT, Arcadia_Real64Value* fraction)
{
  if (!ray || !segment || !isfinite(radius) || radius < (Arcadia_Real64Value)0 ||
      !validPoints(points,count)) return Arcadia_BooleanValue_False;
  double o[3],d[3],dd,r=(double)radius;
  for (Arcadia_SizeValue k=0;k<3;++k) {
    if (!isfinite(ray->origin[k]) || !isfinite(ray->direction[k]))
      return Arcadia_BooleanValue_False;
    o[k]=ray->origin[k]; d[k]=ray->direction[k];
  }
  dd=square3(d);
  if (!(dd>0.) || !isfinite(dd)) return Arcadia_BooleanValue_False;
  double nearest=HUGE_VAL,bestRay=0.,bestFraction=0.;
  Arcadia_SizeValue bestSegment=0;
  for (Arcadia_SizeValue i=0;i<count-1;++i) {
    double edge[3],w[3];
    for (Arcadia_SizeValue k=0;k<3;++k) {
      edge[k]=(double)points[i+1][k]-(double)points[i][k];
      w[k]=o[k]-(double)points[i][k];
    }
    double ee=square3(edge),de=scalar3(d,edge),dw=scalar3(d,w),ew=scalar3(edge,w);
    if (!isfinite(ee)||!isfinite(de)||!isfinite(dw)||!isfinite(ew))
      return Arcadia_BooleanValue_False;
    // Minimize ||w+t*d-s*edge||^2 for t>=0 and 0<=s<=1.
    // Every possible constrained minimum lies at an interior stationary point
    // or on one of the three boundary edges.
    double candidates[4][2];
    Arcadia_SizeValue n=0;
    double det=dd*ee-de*de;
    if (det>0. && isfinite(det)) {
      double t=(de*ew-ee*dw)/det;
      double s=(dd*ew-de*dw)/det;
      if (t>=0. && s>=0. && s<=1.) { candidates[n][0]=t; candidates[n++][1]=s; }
    }
    candidates[n][0]=fmax(0.,-dw/dd); candidates[n++][1]=0.;
    candidates[n][0]=fmax(0.,(de-dw)/dd); candidates[n++][1]=1.;
    candidates[n][0]=0.; candidates[n++][1]=ee>0. ? clampUnit(ew/ee) : 0.;
    for (Arcadia_SizeValue j=0;j<n;++j) {
      double t=candidates[j][0],s=candidates[j][1],diff[3];
      for (Arcadia_SizeValue k=0;k<3;++k) diff[k]=w[k]+t*d[k]-s*edge[k];
      double value=square3(diff);
      if (!isfinite(value)) return Arcadia_BooleanValue_False;
      if (value<nearest) { nearest=value;bestRay=t;bestFraction=s;bestSegment=i; }
    }
  }
  if (!(nearest<=r*r) || !isfinite(bestRay) || !isfinite(bestFraction) ||
      !isfinite((Arcadia_Real64Value)bestRay) || !isfinite((Arcadia_Real64Value)bestFraction))
    return Arcadia_BooleanValue_False;
  *segment=bestSegment;
  if (rayT) *rayT=(Arcadia_Real64Value)bestRay;
  if (fraction) *fraction=(Arcadia_Real64Value)bestFraction;
  return Arcadia_BooleanValue_True;
}
