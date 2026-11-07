# Legacy Math salvage completed

The historical `repository/Math/@migrate/{1,2,3}` snapshots were retired after
review against the maintained `Arcadia.Math` and `Arcadia.Media` APIs. They
were incompatible `dx/core/math.h` variants, not CMake production inputs.

| Legacy operation or type | Maintained replacement |
| --- | --- |
| `DX_VEC3`, `DX_VEC4`, vector arithmetic, cross product | `Arcadia_Math_Vector*Real32/Real64`, `crossProduct` |
| Zero-tolerant `dx_vec3_norm` | `Vector*_normalizeOrZero` |
| Componentwise `Core_InlineVector2R32_mulc_vv` | `Vector*_multiplyComponents` |
| Matrix values, affine point transform and rotation tests | `Matrix4Real32/Real64`, `transformPoint`, `MatrixTests` |
| `DX_RECT2_F32`, signed extent and union | `Rectangle2Real32/Real64` |
| `Core_InlineRgbR32` | `Arcadia_Math_RgbReal32` (native, not GC-managed) |
| `DX_RGB_N8`, byte interpolation, RGB to RGBA conversion | `Arcadia_Media_PixelRedGreenBlueNatural8` and related conversions |
| Named Capri, Malachite, Amber RGB8 declarations | `Arcadia_Media_ColorPaletteNatural8_get*` |
| Color interpolation and native RGB/RGBA operations | Math Color4 templates and native value helpers |
| Degrees to radians, scalar interpolation | Math trigonometry and Ring APIs |

The original named RGB8 declarations had no definitions in those snapshots.
The maintained palette returns explicit `(0,191,255)`, `(0,255,64)`, and
`(255,192,0)` values by copy, without reintroducing `dx`/`Core` symbols.

The old `Core.Assets.Vector3R32` and `Core.Assets.Matrix4x4R32` wrappers,
and the empty `getWorkingDirectory` stubs, were retired earlier. No old
`@migrate` translation units are required by maintained builds.

Keep new behavior in the Math and Media modules; do not restore this archive
or add legacy include paths. See the VectorTests, MatrixTests,
ColorValueTests, ColorObjectTests, RectangleTests and Media PixelBuffer tests
for executable coverage.
