// Instantiated once for Real32 and once for Real64 from Main.c.
#define Check(test) do { if (!(test)) return EXIT_FAILURE; } while (0)

static int
Case(openUniform)
  (
    void
  )
{
  Scalar knots[7] = { (Scalar)-2, (Scalar)-2, (Scalar)-2, (Scalar)-2, (Scalar)-2, (Scalar)-2, (Scalar)-2 };
  Check(N(makeOpenUniformKnots)(4, 2, knots, 7));
  Check(knots[0] == 0 && knots[1] == 0 && knots[2] == 0);
  Check(knots[3] == (Scalar)0.5 && knots[4] == 1 && knots[5] == 1 && knots[6] == 1);
  Check(!N(makeOpenUniformKnots)(4, 4, knots, 7));
  Check(!N(makeOpenUniformKnots)(4, 2, knots, 6));
  Check(knots[0] == 0 && knots[6] == 1);
  return EXIT_SUCCESS;
}

static int
Case(curveEndpoints)
  (
    void
  )
{
  const Scalar points[2][3] = { { 0, 0, 0 }, { 2, 4, 6 } };
  Scalar knots[4];
  Scalar workspace[6];
  Scalar result[3] = { -1, -1, -1 };
  Check(N(makeOpenUniformKnots)(2, 1, knots, 4));
  Check(N(evaluateCurve)(points, NULL, 2, 1, knots, 4, (Scalar)0.25, workspace, 6, result));
  Check(fabs((double)result[0] - 0.5) <= Epsilon);
  Check(fabs((double)result[1] - 1.0) <= Epsilon);
  Check(fabs((double)result[2] - 1.5) <= Epsilon);
  Check(N(evaluateCurve)(points, NULL, 2, 1, knots, 4, 0, workspace, 6, result));
  Check(result[0] == 0 && result[1] == 0 && result[2] == 0);
  Check(N(evaluateCurve)(points, NULL, 2, 1, knots, 4, 1, workspace, 6, result));
  Check(result[0] == 2 && result[1] == 4 && result[2] == 6);
  return EXIT_SUCCESS;
}

static int
Case(curveRational)
  (
    void
  )
{
  Scalar points[2][3] = { { 0, 0, 0 }, { 2, 0, 0 } };
  const Scalar weights[2] = { 1, 3 };
  const Scalar knots[4] = { 0, 0, 1, 1 };
  Scalar workspace[6];
  Scalar result[3];
  Check(N(evaluateCurve)((const Scalar (*)[3])points, weights, 2, 1, knots, 4,
                         (Scalar)0.5, workspace, 6, result));
  Check(fabs((double)result[0] - 1.5) <= Epsilon);
  Check(result[1] == 0 && result[2] == 0);
  // Aliasing a control point is permitted; result is written after evaluation.
  Check(N(evaluateCurve)((const Scalar (*)[3])points, weights, 2, 1, knots, 4,
                         (Scalar)0.5, workspace, 6, points[0]));
  Check(fabs((double)points[0][0] - 1.5) <= Epsilon);
  return EXIT_SUCCESS;
}

static int
Case(curveRepeated)
  (
    void
  )
{
  const Scalar points[5][3] = {
    { 0, 0, 0 }, { 1, 0, 0 }, { 2, 0, 0 }, { 3, 0, 0 }, { 4, 0, 0 }
  };
  const Scalar knots[8] = { 0, 0, 0, (Scalar)0.5, (Scalar)0.5, 1, 1, 1 };
  Scalar workspace[9];
  Scalar result[3];
  Check(N(evaluateCurve)(points, NULL, 5, 2, knots, 8, (Scalar)0.5, workspace, 9, result));
  Check(fabs((double)result[0] - 2.) <= Epsilon);
  Check(N(evaluateCurve)(points, NULL, 5, 2, knots, 8, (Scalar)0.25, workspace, 9, result));
  Check(fabs((double)result[0] - 1.) <= Epsilon);
  Check(N(evaluateCurve)(points, NULL, 5, 2, knots, 8, (Scalar)0.75, workspace, 9, result));
  Check(fabs((double)result[0] - 3.) <= Epsilon);
  return EXIT_SUCCESS;
}

static int
Case(curveDegreeZero)
  (
    void
  )
{
  const Scalar points[2][3] = { { 4, 0, 0 }, { 9, 0, 0 } };
  const Scalar knots[3] = { 0, (Scalar)0.5, 1 };
  Scalar workspace[3];
  Scalar result[3];
  Check(N(evaluateCurve)(points, NULL, 2, 0, knots, 3, (Scalar)0.49, workspace, 3, result));
  Check(result[0] == 4);
  Check(N(evaluateCurve)(points, NULL, 2, 0, knots, 3, (Scalar)0.5, workspace, 3, result));
  Check(result[0] == 9);
  Check(N(evaluateCurve)(points, NULL, 2, 0, knots, 3, 1, workspace, 3, result));
  Check(result[0] == 9);
  return EXIT_SUCCESS;
}

static int
Case(curveInvalid)
  (
    void
  )
{
  const Scalar points[2][3] = { { 0, 0, 0 }, { 2, 0, 0 } };
  const Scalar knots[4] = { 0, 0, 1, 1 };
  const Scalar badKnots[4] = { 0, 1, 0, 1 };
  const Scalar zeroKnots[4] = { 0, 0, 0, 0 };
  const Scalar badWeights[2] = { 1, 0 };
  Scalar workspace[6];
  Scalar result[3] = { 9, 8, 7 };
  Check(!N(evaluateCurve)(points, NULL, 2, 1, knots, 4, 2, workspace, 6, result));
  Check(!N(evaluateCurve)(points, NULL, 2, 1, knots, 4, (Scalar)0.5, workspace, 5, result));
  Check(!N(evaluateCurve)(points, NULL, 2, 1, badKnots, 4, (Scalar)0.5, workspace, 6, result));
  Check(!N(evaluateCurve)(points, NULL, 2, 1, zeroKnots, 4, 0, workspace, 6, result));
  Check(!N(evaluateCurve)(points, badWeights, 2, 1, knots, 4, (Scalar)0.5, workspace, 6, result));
  Check(!N(evaluateCurve)(NULL, NULL, 2, 1, knots, 4, (Scalar)0.5, workspace, 6, result));
  Check(result[0] == 9 && result[1] == 8 && result[2] == 7);
  return EXIT_SUCCESS;
}

static int
Case(surfaceBilinear)
  (
    void
  )
{
  const Scalar points[4][3] = {
    { 0, 0, 0 }, { 0, 2, 0 }, { 2, 0, 0 }, { 2, 2, 0 }
  };
  const Scalar knots[4] = { 0, 0, 1, 1 };
  Scalar workspace[12];
  Scalar result[3];
  Check(N(evaluateSurface)(points, NULL, 2, 2, 1, 1, knots, 4, knots, 4,
                           (Scalar)0.25, (Scalar)0.75, workspace, 12, result));
  Check(fabs((double)result[0] - 0.5) <= Epsilon);
  Check(fabs((double)result[1] - 1.5) <= Epsilon);
  Check(result[2] == 0);
  Check(N(evaluateSurface)(points, NULL, 2, 2, 1, 1, knots, 4, knots, 4,
                           1, 1, workspace, 12, result));
  Check(result[0] == 2 && result[1] == 2);
  return EXIT_SUCCESS;
}

static int
Case(surfaceRectangular)
  (
    void
  )
{
  Scalar points[6][3] = {
    { 0, 0, 0 }, { 0, 2, 0 }, { 0, 4, 0 },
    { 2, 0, 0 }, { 2, 2, 0 }, { 2, 4, 0 }
  };
  const Scalar knotsU[4] = { 0, 0, 1, 1 };
  const Scalar knotsV[6] = { 0, 0, 0, 1, 1, 1 };
  Scalar workspace[15];
  Scalar result[3];
  Check(N(evaluateSurface)((const Scalar (*)[3])points, NULL,
                           2, 3, 1, 2, knotsU, 4, knotsV, 6,
                           (Scalar)0.25, (Scalar)0.5, workspace, 15, result));
  Check(fabs((double)result[0] - 0.5) <= Epsilon);
  Check(fabs((double)result[1] - 2.) <= Epsilon);
  Check(N(evaluateSurface)((const Scalar (*)[3])points, NULL,
                           2, 3, 1, 2, knotsU, 4, knotsV, 6,
                           (Scalar)0.25, (Scalar)0.5, workspace, 15, points[0]));
  Check(fabs((double)points[0][0] - 0.5) <= Epsilon);
  Check(fabs((double)points[0][1] - 2.) <= Epsilon);
  return EXIT_SUCCESS;
}

static int
Case(surfaceRational)
  (
    void
  )
{
  const Scalar points[4][3] = {
    { 0, 0, 0 }, { 0, 2, 0 }, { 2, 0, 0 }, { 2, 2, 0 }
  };
  const Scalar weights[4] = { 1, 1, 1, 5 };
  const Scalar knots[4] = { 0, 0, 1, 1 };
  Scalar workspace[12];
  Scalar result[3];
  Check(N(evaluateSurface)(points, weights, 2, 2, 1, 1, knots, 4, knots, 4,
                           (Scalar)0.5, (Scalar)0.5, workspace, 12, result));
  Check(fabs((double)result[0] - 1.5) <= Epsilon);
  Check(fabs((double)result[1] - 1.5) <= Epsilon);
  return EXIT_SUCCESS;
}

static int
Case(surfaceDegreeZero)
  (
    void
  )
{
  const Scalar points[4][3] = {
    { 0, 0, 0 }, { 0, 2, 0 }, { 2, 0, 0 }, { 2, 2, 0 }
  };
  const Scalar knots[3] = { 0, (Scalar)0.5, 1 };
  Scalar workspace[6];
  Scalar result[3];
  Check(N(evaluateSurface)(points, NULL, 2, 2, 0, 0, knots, 3, knots, 3,
                           (Scalar)0.8, (Scalar)0.3, workspace, 6, result));
  Check(result[0] == 2 && result[1] == 0);
  Check(N(evaluateSurface)(points, NULL, 2, 2, 0, 0, knots, 3, knots, 3,
                           1, 1, workspace, 6, result));
  Check(result[0] == 2 && result[1] == 2);
  return EXIT_SUCCESS;
}

static int
Case(surfaceConstant)
  (
    void
  )
{
  const Scalar points[6][3] = {
    { 7, -2, 5 }, { 7, -2, 5 }, { 7, -2, 5 },
    { 7, -2, 5 }, { 7, -2, 5 }, { 7, -2, 5 }
  };
  const Scalar weights[6] = { 1, 4, 2, 3, 5, 6 };
  const Scalar knotsU[4] = { 0, 0, 1, 1 };
  const Scalar knotsV[6] = { 0, 0, 0, 1, 1, 1 };
  Scalar workspace[15];
  Scalar result[3];
  Check(N(evaluateSurface)(points, weights, 2, 3, 1, 2,
                           knotsU, 4, knotsV, 6, (Scalar)0.35, (Scalar)0.6,
                           workspace, 15, result));
  Check(fabs((double)result[0] - 7.) <= Epsilon);
  Check(fabs((double)result[1] + 2.) <= Epsilon);
  Check(fabs((double)result[2] - 5.) <= Epsilon);
  return EXIT_SUCCESS;
}

static int
Case(surfaceInvalid)
  (
    void
  )
{
  const Scalar points[4][3] = {
    { 0, 0, 0 }, { 0, 2, 0 }, { 2, 0, 0 }, { 2, 2, 0 }
  };
  const Scalar knots[4] = { 0, 0, 1, 1 };
  const Scalar badKnots[4] = { 0, 1, 0, 1 };
  const Scalar badWeights[4] = { 1, 1, 1, -1 };
  Scalar workspace[12];
  Scalar result[3] = { 9, 8, 7 };
  Check(!N(evaluateSurface)(points, NULL, 2, 2, 1, 1, knots, 4, knots, 4,
                            (Scalar)0.5, (Scalar)0.5, workspace, 11, result));
  Check(!N(evaluateSurface)(points, NULL, 2, 2, 1, 1, knots, 4, badKnots, 4,
                            (Scalar)0.5, (Scalar)0.5, workspace, 12, result));
  Check(!N(evaluateSurface)(points, badWeights, 2, 2, 1, 1, knots, 4, knots, 4,
                            (Scalar)0.5, (Scalar)0.5, workspace, 12, result));
  Check(!N(evaluateSurface)(points, NULL, 2, 2, 1, 1, knots, 4, knots, 4,
                            (Scalar)-1, (Scalar)0.5, workspace, 12, result));
  Check(!N(evaluateSurface)(points, NULL, 0, 2, 1, 1, knots, 4, knots, 4,
                            (Scalar)0.5, (Scalar)0.5, workspace, 12, result));
  Check(result[0] == 9 && result[1] == 8 && result[2] == 7);
  return EXIT_SUCCESS;
}

#undef Check
