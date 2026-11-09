// Regression cases adapted from toport/nyx/modules/m0/test/data/scene2/
// surface-nurbs-1.txt and surface-nurbs-2.txt.
// Legacy order (4,4) corresponds to Arcadia degree (3,3).
#define LegacyCheck(test) do { if (!(test)) return EXIT_FAILURE; } while (0)

static int Case(legacyNURBSFlatSurface)(void)
{
  const Scalar points[16][3] = {
    {0,0,0}, {0,1,0}, {0,2,0}, {0,3,0},
    {1,0,0}, {1,1,0}, {1,2,0}, {1,3,0},
    {2,0,0}, {2,1,0}, {2,2,0}, {2,3,0},
    {3,0,0}, {3,1,0}, {3,2,0}, {3,3,0}
  };
  const Scalar knots[8] = {0,0,0,0,1,1,1,1};
  const Scalar weights[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
  Scalar scratch[24], position[3];
  LegacyCheck(N(evaluateSurface)(points, weights, 4, 4, 3, 3,
    knots, 8, knots, 8, (Scalar)0.5, (Scalar)0.5, scratch, 24, position));
  LegacyCheck(fabs((double)position[0] - 1.5) <= Epsilon);
  LegacyCheck(fabs((double)position[1] - 1.5) <= Epsilon);
  LegacyCheck(fabs((double)position[2]) <= Epsilon);
  LegacyCheck(N(evaluateSurface)(points, weights, 4, 4, 3, 3,
    knots, 8, knots, 8, 1, 0, scratch, 24, position));
  LegacyCheck(position[0] == 3 && position[1] == 0 && position[2] == 0);
  return EXIT_SUCCESS;
}

static int Case(legacyNURBSCurvedSurface)(void)
{
  const Scalar points[16][3] = {
    {0,0,0}, {0,1,1}, {0,2,1}, {0,3,0},
    {1,0,2}, {1,1,2}, {1,2,2}, {1,3,1},
    {2,0,2}, {2,1,2}, {2,2,2}, {2,3,1},
    {3,0,0}, {3,1,1}, {3,2,1}, {3,3,0}
  };
  const Scalar knots[8] = {0,0,0,0,1,1,1,1};
  const Scalar weights[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
  Scalar scratch[32], position[3], du[3], dv[3];
  LegacyCheck(N(evaluateSurface)(points, weights, 4, 4, 3, 3,
    knots, 8, knots, 8, (Scalar)0.5, (Scalar)0.5, scratch, 32, position));
  LegacyCheck(fabs((double)position[0] - 1.5) <= Epsilon);
  LegacyCheck(fabs((double)position[1] - 1.5) <= Epsilon);
  LegacyCheck(fabs((double)position[2] - 1.59375) <= Epsilon);
  LegacyCheck(N(evaluateSurfaceDifferentials)(points, weights, 4, 4, 3, 3,
    knots, 8, knots, 8, (Scalar)0.5, (Scalar)0.5,
    scratch, 32, position, du, dv));
  LegacyCheck(fabs((double)position[2] - 1.59375) <= Epsilon);
  LegacyCheck(fabs((double)du[0] - 3.) <= Epsilon);
  LegacyCheck(fabs((double)dv[1] - 3.) <= Epsilon);
  LegacyCheck(fabs((double)du[1]) <= Epsilon);
  LegacyCheck(fabs((double)dv[0]) <= Epsilon);
  return EXIT_SUCCESS;
}

#undef LegacyCheck
