// Native NURBS line stream tests, instantiated for both Real32 and Real64.
#define CurveCheck(x) do { if (!(x)) return EXIT_FAILURE; } while (0)

static int
Case(curveLineCount)
  (
    void
  )
{
  Arcadia_SizeValue n = 51;
  CurveCheck(Curve(getVertexCount)(2, &n) && n == 2);
  CurveCheck(Curve(getVertexCount)(5, &n) && n == 8);
  CurveCheck(!Curve(getVertexCount)(1, &n) && n == 8);
  CurveCheck(!Curve(getVertexCount)((Arcadia_SizeValue)-1, &n) && n == 8);
  CurveCheck(!Curve(getVertexCount)(2, NULL));
  return EXIT_SUCCESS;
}

static int
Case(curveLinesLinear)
  (
    void
  )
{
  const Scalar points[2][3] = {{1,2,3},{5,2,3}};
  const Scalar knots[4] = {0,0,1,1};
  Scalar workspace[8], positions[8][3], tangents[8][3];
  CurveCheck(Curve(sampleLines)(points, NULL, 2, 1, knots, 4,
      5, positions, tangents, 8, workspace, 8));
  for (int i=0;i<4;++i) {
    CurveCheck(fabs((double)positions[2*i][0] - (1. + i)) <= Epsilon);
    CurveCheck(fabs((double)positions[2*i+1][0] - (2. + i)) <= Epsilon);
    CurveCheck(positions[2*i][1] == 2 && positions[2*i+1][2] == 3);
    CurveCheck(fabs((double)tangents[2*i][0] - 4.) <= Epsilon);
    CurveCheck(fabs((double)tangents[2*i+1][0] - 4.) <= Epsilon);
  }
  CurveCheck(positions[1][0] == positions[2][0]);
  return EXIT_SUCCESS;
}

static int
Case(curveLinesRational)
  (
    void
  )
{
  const Scalar points[2][3] = {{0,0,0},{2,0,0}};
  const Scalar weights[2] = {1,3};
  const Scalar knots[4] = {0,0,1,1};
  Scalar workspace[8], positions[4][3];
  CurveCheck(Curve(sampleLines)(points, weights, 2, 1, knots, 4,
      3, positions, NULL, 4, workspace, 8));
  CurveCheck(positions[0][0] == 0 && positions[3][0] == 2);
  CurveCheck(fabs((double)positions[1][0] - 1.5) <= Epsilon);
  CurveCheck(positions[1][0] == positions[2][0]);
  return EXIT_SUCCESS;
}

static int
Case(curveLinesInvalid)
  (
    void
  )
{
  const Scalar points[2][3] = {{0,0,0},{2,0,0}};
  const Scalar knots[4] = {0,0,1,1};
  const Scalar bad[4] = {0,1,0,1};
  const Scalar invalidWeights[2] = {1,0};
  Scalar workspace[8], positions[4][3];
  positions[0][0] = 72;
  CurveCheck(!Curve(sampleLines)(points, NULL, 2, 1, knots, 4,
      3, positions, NULL, 3, workspace, 8));
  CurveCheck(!Curve(sampleLines)(points, NULL, 2, 1, bad, 4,
      3, positions, NULL, 4, workspace, 8));
  CurveCheck(!Curve(sampleLines)(points, invalidWeights, 2, 1, knots, 4,
      3, positions, NULL, 4, workspace, 8));
  CurveCheck(!Curve(sampleLines)(points, NULL, 2, 1, knots, 4,
      3, positions, NULL, 4, workspace, 5));
  CurveCheck(positions[0][0] == 72);
  return EXIT_SUCCESS;
}

static int
Case(curveLength)
  (
    void
  )
{
  const Scalar samples[3][3] = {{0,0,0},{3,4,0},{6,8,0}};
  Scalar length = -1;
  CurveCheck(Curve(approximateLength)(samples, 3, &length));
  CurveCheck(fabs((double)length - 10.) <= Epsilon);
  Scalar bad[3][3] = {{0,0,0},{3,4,0},{6,8,0}};
  bad[1][0] = (Scalar)NAN;
  CurveCheck(!Curve(approximateLength)((const Scalar(*)[3])bad, 3, &length));
  CurveCheck(length == 10);
  CurveCheck(!Curve(approximateLength)(samples, 1, &length));
  return EXIT_SUCCESS;
}
#undef CurveCheck
