// Compiled twice with Scalar, L(name), B(name) and Test(name) supplied.
// Every case is independent of any managed Arcadia.Math object instance.
#include <math.h>

static void
Test(uniformKnotsImpl)(Arcadia_Thread* thread)
{
  Scalar knots[5] = { -1, -1, -1, -1, -1 };
  Arcadia_Tests_assertTrue(thread, L(setUniformKnots)(5, knots));
  for (Arcadia_SizeValue i = 0; i < 5; ++i)
    Arcadia_Tests_assertTrue(thread, fabs((double)knots[i] - (double)i / 4.) < 0.00001);
  Arcadia_Tests_assertTrue(thread, !L(setUniformKnots)(0, knots));
  Scalar one[1] = { -1 };
  Arcadia_Tests_assertTrue(thread, L(setUniformKnots)(1, one));
  Arcadia_Tests_assertTrue(thread, one[0] == 0);
}

static void
Test(chordKnotsImpl)(Arcadia_Thread* thread)
{
  const Scalar points[][3] = { { 0, 0, 0 }, { 3, 0, 0 }, { 3, 4, 0 } };
  Scalar knots[3] = { -1, -1, -1 };
  Arcadia_Tests_assertTrue(thread, L(setChordLengthKnots)(points, 3, knots));
  Arcadia_Tests_assertTrue(thread, knots[0] == 0);
  Arcadia_Tests_assertTrue(thread, fabs((double)knots[1] - 3. / 7.) < 0.000001);
  Arcadia_Tests_assertTrue(thread, knots[2] == 1);
  const Scalar repeated[][3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 3, 0, 0 } };
  Arcadia_Tests_assertTrue(thread, L(setChordLengthKnots)(repeated, 3, knots));
  Arcadia_Tests_assertTrue(thread, knots[0] == 0 && knots[1] == (Scalar)0.5 && knots[2] == 1);
  const Scalar same[][3] = { { 5, 6, 7 }, { 5, 6, 7 }, { 5, 6, 7 } };
  Arcadia_Tests_assertTrue(thread, L(setChordLengthKnots)(same, 3, knots));
  Arcadia_Tests_assertTrue(thread, knots[0] == 0 && knots[1] == (Scalar)0.5 && knots[2] == 1);
}

static void
Test(lagrangeLineImpl)(Arcadia_Thread* thread)
{
  Scalar points[][3] = { { 0, 2, 4 }, { 4, 6, 8 } };
  Scalar knots[2] = { 0, 1 }, weights[2] = { 0, 0 };
  Scalar result[3] = { -7, -7, -7 };
  Arcadia_Tests_assertTrue(thread, L(computeWeights)(knots, 2, weights));
  Arcadia_Tests_assertTrue(thread, L(evaluate)((const Scalar (*)[3])points, knots, weights, 2, (Scalar)0.25, result));
  Arcadia_Tests_assertTrue(thread, fabs((double)result[0] - 1.) < 0.00001 && fabs((double)result[1] - 3.) < 0.00001 && fabs((double)result[2] - 5.) < 0.00001);
  Arcadia_Tests_assertTrue(thread, L(evaluate)((const Scalar (*)[3])points, knots, weights, 2, (Scalar)1.5, result));
  Arcadia_Tests_assertTrue(thread, fabs((double)result[0] - 6.) < 0.00001 && fabs((double)result[1] - 8.) < 0.00001 && fabs((double)result[2] - 10.) < 0.00001);
  // The output aliases the first control point, which is read before writing.
  Arcadia_Tests_assertTrue(thread, L(evaluate)((const Scalar (*)[3])points, knots, weights, 2, (Scalar)0.5, points[0]));
  Arcadia_Tests_assertTrue(thread, fabs((double)points[0][0] - 2.) < 0.00001 && fabs((double)points[0][1] - 4.) < 0.00001 && fabs((double)points[0][2] - 6.) < 0.00001);
}

static void
Test(lagrangePolynomialImpl)(Arcadia_Thread* thread)
{
  const Scalar points[][3] = { { 0, 0, 0 }, { 1, 1, 1 }, { 2, 4, 8 } };
  const Scalar knots[3] = { 0, 1, 2 };
  Scalar weights[3] = { 0, 0, 0 }, result[3] = { -1, -1, -1 };
  Arcadia_Tests_assertTrue(thread, L(computeWeights)(knots, 3, weights));
  Arcadia_Tests_assertTrue(thread, L(evaluate)(points, knots, weights, 3, (Scalar)0.5, result));
  Arcadia_Tests_assertTrue(thread, fabs((double)result[0] - 0.5) < 0.00001);
  Arcadia_Tests_assertTrue(thread, fabs((double)result[1] - 0.25) < 0.00001);
  Arcadia_Tests_assertTrue(thread, fabs((double)result[2] + 0.25) < 0.00001);
  // y and z follow their unique quadratic interpolation polynomials.
  Arcadia_Tests_assertTrue(thread, L(evaluate)(points, knots, weights, 3, (Scalar)1, result));
  Arcadia_Tests_assertTrue(thread, result[0] == 1 && result[1] == 1 && result[2] == 1);
}

static void
Test(lagrangeInvalidImpl)(Arcadia_Thread* thread)
{
  const Scalar knots[3] = { 0, 1, 1 };
  Scalar weights[3] = { 23, 23, 23 };
  Arcadia_Tests_assertTrue(thread, !L(computeWeights)(knots, 3, weights));
  Arcadia_Tests_assertTrue(thread, weights[0] == 23 && weights[1] == 23 && weights[2] == 23);
  const Scalar points[][3] = { { 0, 0, 0 } };
  Scalar result[3] = { 99, 98, 97 };
  Arcadia_Tests_assertTrue(thread, !L(evaluate)(points, knots, weights, 0, (Scalar)0.5, result));
  Arcadia_Tests_assertTrue(thread, result[0] == 99 && result[1] == 98 && result[2] == 97);
  Scalar zeros[1] = { 0 };
  Arcadia_Tests_assertTrue(thread, !L(evaluate)(points, knots, zeros, 1, (Scalar)0.5, result));
  Arcadia_Tests_assertTrue(thread, result[0] == 99 && result[1] == 98 && result[2] == 97);
}

static void
Test(bezierCubicImpl)(Arcadia_Thread* thread)
{
  const Scalar points[][3] = { { 0, 0, 0 }, { 1, 3, 0 }, { 2, 3, 0 }, { 3, 0, 0 } };
  Scalar workspace[12], result[3] = { -1, -1, -1 };
  Arcadia_Tests_assertTrue(thread, B(evaluate)(points, 4, (Scalar)0.5, workspace, 12, result));
  Arcadia_Tests_assertTrue(thread, fabs((double)result[0] - 1.5) < 0.00001);
  Arcadia_Tests_assertTrue(thread, fabs((double)result[1] - 2.25) < 0.00001);
  Arcadia_Tests_assertTrue(thread, result[2] == 0);
  Arcadia_Tests_assertTrue(thread, B(evaluateDerivative)(points, 4, (Scalar)0.5, workspace, 12, result));
  Arcadia_Tests_assertTrue(thread, result[0] == 3 && result[1] == 0 && result[2] == 0);
  Arcadia_Tests_assertTrue(thread, B(evaluateDerivative)(points, 4, (Scalar)0, workspace, 12, result));
  Arcadia_Tests_assertTrue(thread, result[0] == 3 && result[1] == 9 && result[2] == 0);
  Arcadia_Tests_assertTrue(thread, B(evaluateDerivative)(points, 4, (Scalar)1, workspace, 12, result));
  Arcadia_Tests_assertTrue(thread, result[0] == 3 && result[1] == -9 && result[2] == 0);
}

static void
Test(bezierDegenerateImpl)(Arcadia_Thread* thread)
{
  Scalar points[][3] = { { 2, 4, 6 }, { 6, 8, 10 } };
  Scalar workspace[6], result[3] = { 101, 102, 103 };
  Arcadia_Tests_assertTrue(thread, B(evaluate)((const Scalar (*)[3])points, 2, (Scalar)1.5, workspace, 6, result));
  Arcadia_Tests_assertTrue(thread, result[0] == 8 && result[1] == 10 && result[2] == 12);
  Arcadia_Tests_assertTrue(thread, B(evaluateDerivative)((const Scalar (*)[3])points, 2, (Scalar)0.5, workspace, 6, result));
  Arcadia_Tests_assertTrue(thread, result[0] == 4 && result[1] == 4 && result[2] == 4);
  Arcadia_Tests_assertTrue(thread, B(evaluate)((const Scalar (*)[3])points, 2, (Scalar)0.5, workspace, 6, points[1]));
  Arcadia_Tests_assertTrue(thread, points[1][0] == 4 && points[1][1] == 6 && points[1][2] == 8);
  Arcadia_Tests_assertTrue(thread, B(evaluate)((const Scalar (*)[3])points, 1, (Scalar)0.5, workspace, 6, result));
  Arcadia_Tests_assertTrue(thread, result[0] == 2 && result[1] == 4 && result[2] == 6);
  Arcadia_Tests_assertTrue(thread, B(evaluateDerivative)((const Scalar (*)[3])points, 1, (Scalar)0.5, workspace, 6, result));
  Arcadia_Tests_assertTrue(thread, result[0] == 0 && result[1] == 0 && result[2] == 0);
}

static void
Test(bezierInvalidImpl)(Arcadia_Thread* thread)
{
  const Scalar points[][3] = { { 0, 0, 0 }, { 1, 1, 1 } };
  Scalar workspace[6], result[3] = { 30, 40, 50 };
  Arcadia_Tests_assertTrue(thread, !B(evaluate)(points, 0, (Scalar)0.5, workspace, 6, result));
  Arcadia_Tests_assertTrue(thread, !B(evaluate)(points, 2, (Scalar)0.5, workspace, 5, result));
  Arcadia_Tests_assertTrue(thread, !B(evaluateDerivative)(points, 2, (Scalar)0.5, workspace, 5, result));
  Arcadia_Tests_assertTrue(thread, result[0] == 30 && result[1] == 40 && result[2] == 50);
}

#define CURVE_CASE(name) \
static int Test(name)(void) { return Arcadia_Tests_safeExecute(&Test(name##Impl)) ? EXIT_SUCCESS : EXIT_FAILURE; }
CURVE_CASE(uniformKnots)
CURVE_CASE(chordKnots)
CURVE_CASE(lagrangeLine)
CURVE_CASE(lagrangePolynomial)
CURVE_CASE(lagrangeInvalid)
CURVE_CASE(bezierCubic)
CURVE_CASE(bezierDegenerate)
CURVE_CASE(bezierInvalid)
#undef CURVE_CASE
