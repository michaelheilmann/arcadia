// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
// details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include "Arcadia/Math/Include.h"
#include "Arcadia/Testing/Include.h"
#include <stdlib.h>
#include <math.h>

#define Scalar Arcadia_Real32Value
#define N(name) Arcadia_Math_NURBS3Real32_##name
#define Case(name) real32_##name
#define GridUV Arcadia_Math_NURBS3Grid_makeUVReal32
#define Epsilon 0.00002
#include "Cases.i"
#include "CasesDifferentials.i"
#define Control(name) Arcadia_Math_NURBS3ControlReal32_##name
#define ControlAABB Arcadia_Math_AABB3Real32
#define ControlRay Arcadia_Math_Ray3Real32
#define ControlAffine Arcadia_Math_Affine3Real32
#define ControlAffineSet Arcadia_Math_Affine3Real32_setValues
#include "CasesControl.i"
#undef Control
#undef ControlAABB
#undef ControlRay
#undef ControlAffine
#undef ControlAffineSet
#include "CasesLegacySurfaces.i"
#include "CasesGrid.i"
#define Mesh(name) Arcadia_Math_NURBS3MeshReal32_##name
#include "CasesMesh.i"
#define Curve(name) Arcadia_Math_NURBS3CurveReal32_##name
#include "CasesCurveLines.i"
#define Spatial(name) Arcadia_Math_NURBS3PolylineReal32_##name
#define SpatialRay Arcadia_Math_Ray3Real32
#include "CasesPolyline.i"
#define Net(name) Arcadia_Math_NURBS3ControlNetReal32_##name
#include "CasesControlNet.i"
#define Pick(name) Arcadia_Math_NURBS3PickReal32_##name
#define PickRay Arcadia_Math_Ray3Real32
#include "CasesPick.i"
#define Adaptive(name) Arcadia_Math_NURBS3AdaptiveReal32_##name
#include "CasesAdaptive.i"
#define Refine(name) Arcadia_Math_NURBS3RefineReal32_##name
#include "CasesRefine.i"
#undef Refine
#undef Adaptive
#undef PickRay
#undef Pick
#undef Net
#undef SpatialRay
#undef Spatial
#undef Curve
#undef Mesh
#undef GridUV
#undef Scalar
#undef N
#undef Case
#undef Epsilon

#define Scalar Arcadia_Real64Value
#define N(name) Arcadia_Math_NURBS3Real64_##name
#define Case(name) real64_##name
#define GridUV Arcadia_Math_NURBS3Grid_makeUVReal64
#define Epsilon 0.000000000002
#include "Cases.i"
#include "CasesDifferentials.i"
#define Control(name) Arcadia_Math_NURBS3ControlReal64_##name
#define ControlAABB Arcadia_Math_AABB3Real64
#define ControlRay Arcadia_Math_Ray3Real64
#define ControlAffine Arcadia_Math_Affine3Real64
#define ControlAffineSet Arcadia_Math_Affine3Real64_setValues
#include "CasesControl.i"
#undef Control
#undef ControlAABB
#undef ControlRay
#undef ControlAffine
#undef ControlAffineSet
#include "CasesLegacySurfaces.i"
#include "CasesGrid.i"
#define Mesh(name) Arcadia_Math_NURBS3MeshReal64_##name
#include "CasesMesh.i"
#define Curve(name) Arcadia_Math_NURBS3CurveReal64_##name
#include "CasesCurveLines.i"
#define Spatial(name) Arcadia_Math_NURBS3PolylineReal64_##name
#define SpatialRay Arcadia_Math_Ray3Real64
#include "CasesPolyline.i"
#define Net(name) Arcadia_Math_NURBS3ControlNetReal64_##name
#include "CasesControlNet.i"
#define Pick(name) Arcadia_Math_NURBS3PickReal64_##name
#define PickRay Arcadia_Math_Ray3Real64
#include "CasesPick.i"
#define Adaptive(name) Arcadia_Math_NURBS3AdaptiveReal64_##name
#include "CasesAdaptive.i"
#define Refine(name) Arcadia_Math_NURBS3RefineReal64_##name
#include "CasesRefine.i"
#undef Refine
#undef Adaptive
#undef PickRay
#undef Pick
#undef Net
#undef SpatialRay
#undef Spatial
#undef Curve
#undef Mesh
#undef GridUV
#undef Scalar
#undef N
#undef Case
#undef Epsilon

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "Real32OpenUniform", &real32_openUniform },
    { "Real32CurveEndpoints", &real32_curveEndpoints },
    { "Real32CurveRational", &real32_curveRational },
    { "Real32CurveRepeated", &real32_curveRepeated },
    { "Real32CurveDegreeZero", &real32_curveDegreeZero },
    { "Real32CurveInvalid", &real32_curveInvalid },
    { "Real32SurfaceBilinear", &real32_surfaceBilinear },
    { "Real32SurfaceRectangular", &real32_surfaceRectangular },
    { "Real32SurfaceRational", &real32_surfaceRational },
    { "Real32SurfaceDegreeZero", &real32_surfaceDegreeZero },
    { "Real32SurfaceConstant", &real32_surfaceConstant },
    { "Real32SurfaceInvalid", &real32_surfaceInvalid },
    { "Real32LegacyNURBSFlatSurface", &real32_legacyNURBSFlatSurface },
    { "Real32LegacyNURBSCurvedSurface", &real32_legacyNURBSCurvedSurface },
    { "Real32NURBSGridCounts", &real32_gridCounts },
    { "Real32NURBSGridTriangles", &real32_gridTriangles },
    { "Real32NURBSGridUV", &real32_gridUV },
    { "Real32ControlBounds", &real32_controlBounds },
    { "Real32ControlNormalize", &real32_controlNormalize },
    { "Real32ControlTransform", &real32_controlTransform },
    { "Real32ControlPick", &real32_controlPick },
    { "Real32ControlInvalid", &real32_controlInvalid },
    { "Real32NURBSGridSampleCompatibility", &real32_gridSampleCompatibility },
    { "Real32NURBSMeshCounts", &real32_meshCounts },
    { "Real32NURBSMeshFlat", &real32_meshFlat },
    { "Real32NURBSMeshRational", &real32_meshRational },
    { "Real32NURBSMeshDegenerate", &real32_meshDegenerate },
    { "Real32NURBSMeshInvalid", &real32_meshInvalid },
    { "Real32CurveLineCount", &real32_curveLineCount },
    { "Real32CurveLinesLinear", &real32_curveLinesLinear },
    { "Real32CurveLinesRational", &real32_curveLinesRational },
    { "Real32CurveLinesInvalid", &real32_curveLinesInvalid },
    { "Real32CurveLength", &real32_curveLength },
    { "Real32PolylineLengths", &real32_polylineLengths },
    { "Real32PolylineResample", &real32_polylineResample },
    { "Real32PolylineResampleInvalid", &real32_polylineResampleInvalid },
    { "Real32PolylineClosest", &real32_polylineClosest },
    { "Real32PolylineRayPick", &real32_polylineRayPick },
    { "Real32PolylineRayPickInvalid", &real32_polylineRayPickInvalid },
    { "Real32ControlNetCounts", &real32_netCounts },
    { "Real32ControlNetCurve", &real32_netCurve },
    { "Real32ControlNetSurface", &real32_netSurface },
    { "Real32ControlNetDegenerate", &real32_netDegenerate },
    { "Real32ControlNetInvalid", &real32_netInvalid },
    { "Real32SurfacePickGridCenter", &real32_pickGridCenter },
    { "Real32SurfacePickTrianglesNearest", &real32_pickTrianglesNearest },
    { "Real32SurfacePickGridRectangular", &real32_pickGridRectangular },
    { "Real32SurfacePickGridNoHit", &real32_pickGridNoHit },
    { "Real32SurfacePickInvalid", &real32_pickInvalid },
    { "Real32SurfacePickDegenerate", &real32_pickDegenerate },
    { "Real32AdaptiveStraight", &real32_adaptiveStraight },
    { "Real32AdaptiveCurvature", &real32_adaptiveCurvature },
    { "Real32AdaptiveCapacity", &real32_adaptiveCapacity },
    { "Real32AdaptiveInvalid", &real32_adaptiveInvalid },
    { "Real32RefineLinear", &real32_refineLinear },
    { "Real32RefineRational", &real32_refineRational },
    { "Real32RefineRepeated", &real32_refineRepeated },
    { "Real32RefineNonUniform", &real32_refineNonUniform },
    { "Real32RefineInvalid", &real32_refineInvalid },
    { "Real32RefineDegreeZero", &real32_refineDegreeZero },
    { "Real32DifferentialsCurveTangentLine", &real32_curveTangentLine },
    { "Real32DifferentialsCurveTangentRational", &real32_curveTangentRational },
    { "Real32DifferentialsCurveTangentQuadratic", &real32_curveTangentQuadratic },
    { "Real32DifferentialsCurveTangentDegreeZero", &real32_curveTangentDegreeZero },
    { "Real32DifferentialsCurveTangentRepeated", &real32_curveTangentRepeated },
    { "Real32DifferentialsCurveTangentInvalid", &real32_curveTangentInvalid },
    { "Real32DifferentialsSurfaceDifferentialsPlane", &real32_surfaceDifferentialsPlane },
    { "Real32DifferentialsSurfaceDifferentialsRational", &real32_surfaceDifferentialsRational },
    { "Real32DifferentialsSurfaceDifferentialsDegenerate", &real32_surfaceDifferentialsDegenerate },
    { "Real32DifferentialsSurfaceDifferentialsAlias", &real32_surfaceDifferentialsAlias },
    { "Real32DifferentialsSurfaceSamplingGrid", &real32_surfaceSamplingGrid },
    { "Real32DifferentialsSurfaceSamplingDegenerate", &real32_surfaceSamplingDegenerate },
    { "Real32DifferentialsSurfaceSamplingInvalid", &real32_surfaceSamplingInvalid },
    { "Real64OpenUniform", &real64_openUniform },
    { "Real64CurveEndpoints", &real64_curveEndpoints },
    { "Real64CurveRational", &real64_curveRational },
    { "Real64CurveRepeated", &real64_curveRepeated },
    { "Real64CurveDegreeZero", &real64_curveDegreeZero },
    { "Real64CurveInvalid", &real64_curveInvalid },
    { "Real64SurfaceBilinear", &real64_surfaceBilinear },
    { "Real64SurfaceRectangular", &real64_surfaceRectangular },
    { "Real64SurfaceRational", &real64_surfaceRational },
    { "Real64SurfaceDegreeZero", &real64_surfaceDegreeZero },
    { "Real64SurfaceConstant", &real64_surfaceConstant },
    { "Real64SurfaceInvalid", &real64_surfaceInvalid },
    { "Real64LegacyNURBSFlatSurface", &real64_legacyNURBSFlatSurface },
    { "Real64LegacyNURBSCurvedSurface", &real64_legacyNURBSCurvedSurface },
    { "Real64NURBSGridCounts", &real64_gridCounts },
    { "Real64NURBSGridTriangles", &real64_gridTriangles },
    { "Real64NURBSGridUV", &real64_gridUV },
    { "Real64ControlBounds", &real64_controlBounds },
    { "Real64ControlNormalize", &real64_controlNormalize },
    { "Real64ControlTransform", &real64_controlTransform },
    { "Real64ControlPick", &real64_controlPick },
    { "Real64ControlInvalid", &real64_controlInvalid },
    { "Real64NURBSGridSampleCompatibility", &real64_gridSampleCompatibility },
    { "Real64NURBSMeshCounts", &real64_meshCounts },
    { "Real64NURBSMeshFlat", &real64_meshFlat },
    { "Real64NURBSMeshRational", &real64_meshRational },
    { "Real64NURBSMeshDegenerate", &real64_meshDegenerate },
    { "Real64NURBSMeshInvalid", &real64_meshInvalid },
    { "Real64CurveLineCount", &real64_curveLineCount },
    { "Real64CurveLinesLinear", &real64_curveLinesLinear },
    { "Real64CurveLinesRational", &real64_curveLinesRational },
    { "Real64CurveLinesInvalid", &real64_curveLinesInvalid },
    { "Real64CurveLength", &real64_curveLength },
    { "Real64PolylineLengths", &real64_polylineLengths },
    { "Real64PolylineResample", &real64_polylineResample },
    { "Real64PolylineResampleInvalid", &real64_polylineResampleInvalid },
    { "Real64PolylineClosest", &real64_polylineClosest },
    { "Real64PolylineRayPick", &real64_polylineRayPick },
    { "Real64PolylineRayPickInvalid", &real64_polylineRayPickInvalid },
    { "Real64ControlNetCounts", &real64_netCounts },
    { "Real64ControlNetCurve", &real64_netCurve },
    { "Real64ControlNetSurface", &real64_netSurface },
    { "Real64ControlNetDegenerate", &real64_netDegenerate },
    { "Real64ControlNetInvalid", &real64_netInvalid },
    { "Real64SurfacePickGridCenter", &real64_pickGridCenter },
    { "Real64SurfacePickTrianglesNearest", &real64_pickTrianglesNearest },
    { "Real64SurfacePickGridRectangular", &real64_pickGridRectangular },
    { "Real64SurfacePickGridNoHit", &real64_pickGridNoHit },
    { "Real64SurfacePickInvalid", &real64_pickInvalid },
    { "Real64SurfacePickDegenerate", &real64_pickDegenerate },
    { "Real64AdaptiveStraight", &real64_adaptiveStraight },
    { "Real64AdaptiveCurvature", &real64_adaptiveCurvature },
    { "Real64AdaptiveCapacity", &real64_adaptiveCapacity },
    { "Real64AdaptiveInvalid", &real64_adaptiveInvalid },
    { "Real64RefineLinear", &real64_refineLinear },
    { "Real64RefineRational", &real64_refineRational },
    { "Real64RefineRepeated", &real64_refineRepeated },
    { "Real64RefineNonUniform", &real64_refineNonUniform },
    { "Real64RefineInvalid", &real64_refineInvalid },
    { "Real64RefineDegreeZero", &real64_refineDegreeZero },
    { "Real64DifferentialsCurveTangentLine", &real64_curveTangentLine },
    { "Real64DifferentialsCurveTangentRational", &real64_curveTangentRational },
    { "Real64DifferentialsCurveTangentQuadratic", &real64_curveTangentQuadratic },
    { "Real64DifferentialsCurveTangentDegreeZero", &real64_curveTangentDegreeZero },
    { "Real64DifferentialsCurveTangentRepeated", &real64_curveTangentRepeated },
    { "Real64DifferentialsCurveTangentInvalid", &real64_curveTangentInvalid },
    { "Real64DifferentialsSurfaceDifferentialsPlane", &real64_surfaceDifferentialsPlane },
    { "Real64DifferentialsSurfaceDifferentialsRational", &real64_surfaceDifferentialsRational },
    { "Real64DifferentialsSurfaceDifferentialsDegenerate", &real64_surfaceDifferentialsDegenerate },
    { "Real64DifferentialsSurfaceDifferentialsAlias", &real64_surfaceDifferentialsAlias },
    { "Real64DifferentialsSurfaceSamplingGrid", &real64_surfaceSamplingGrid },
    { "Real64DifferentialsSurfaceSamplingDegenerate", &real64_surfaceSamplingDegenerate },
    { "Real64DifferentialsSurfaceSamplingInvalid", &real64_surfaceSamplingInvalid },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
