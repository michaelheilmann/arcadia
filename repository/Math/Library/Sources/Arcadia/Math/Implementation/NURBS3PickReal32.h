#if !defined(ARCADIA_MATH_IMPLEMENTATION_NURBS3PICKREAL32_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_NURBS3PICKREAL32_H_INCLUDED

#include "Arcadia/Math/Implementation/RayGeometry3Real32.h"
#include "Arcadia/Math/Implementation/NURBS3Grid.h"

// Pick two-sided triangles on the forward ray, selecting the smallest t.
// Equal t values prefer the lowest triangle index. This is mesh picking,
// not an analytic ray-NURBS intersection: accuracy depends on tessellation.
// A zero-distance hit is valid. maxT is a finite nonnegative ray parameter,
// not Euclidean distance; direction need not be unit length.
//
// All coordinates, UVs and ray components must be finite; direction nonzero.
// All input vertices are validated before the first hit test. On a miss,
// invalid input or an arithmetic failure, outputs are unchanged. No dynamic
// allocations, managed objects, or renderer dependencies.
// Outputs must not overlap one another or any input. Only triangleIndex is
// required; hitT, hitUV and hitPosition may be NULL. If hitUV is requested
// on the unindexed representation, sourceUV must be supplied.
//
// Unindexed triangle stream from NURBS3MeshReal32_sampleTriangles:
// vertexCount must be divisible by 3 and at least 3. UVs are interpolated
// barycentrically from the three source vertex UV coordinates.
Arcadia_BooleanValue
Arcadia_Math_NURBS3PickReal32_pickTriangles(
    const Arcadia_Math_Ray3Real32* ray,
    const Arcadia_Real32Value (*positions)[3],
    const Arcadia_Real32Value (*sourceUV)[2],
    Arcadia_SizeValue vertexCount, Arcadia_Real32Value maxT,
    Arcadia_SizeValue* triangleIndex, Arcadia_Real32Value* hitT,
    Arcadia_Real32Value hitUV[2], Arcadia_Real32Value hitPosition[3]);

// Indexed u-major grid from NURBS3Real32_sampleSurface:
// vertex(i,j) = i*samplesV + j, with both dimensions >= 2.
// Triangle order matches NURBS3Grid_makeTriangles, and UVs are implicit:
// u=i/(samplesU-1), v=j/(samplesV-1). No index or UV buffer is needed.
Arcadia_BooleanValue
Arcadia_Math_NURBS3PickReal32_pickGrid(
    const Arcadia_Math_Ray3Real32* ray,
    const Arcadia_Real32Value (*positions)[3],
    Arcadia_SizeValue samplesU, Arcadia_SizeValue samplesV,
    Arcadia_Real32Value maxT, Arcadia_SizeValue* triangleIndex,
    Arcadia_Real32Value* hitT, Arcadia_Real32Value hitUV[2],
    Arcadia_Real32Value hitPosition[3]);

#endif // ARCADIA_MATH_IMPLEMENTATION_NURBS3PICKREAL32_H_INCLUDED
