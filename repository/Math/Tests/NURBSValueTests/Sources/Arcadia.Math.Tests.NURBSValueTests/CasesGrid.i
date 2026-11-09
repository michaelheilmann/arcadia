// Companion-grid tests instantiated for both Real32 and Real64.
#define GridCheck(test) do { if (!(test)) return EXIT_FAILURE; } while (0)

static int Case(gridCounts)(void)
{
  Arcadia_SizeValue vertices = 99, triangles = 98;
  GridCheck(Arcadia_Math_NURBS3Grid_getCounts(3, 4, &vertices, &triangles));
  GridCheck(vertices == 12 && triangles == 12);
  GridCheck(Arcadia_Math_NURBS3Grid_getCounts(2, 2, &vertices, &triangles));
  GridCheck(vertices == 4 && triangles == 2);
  vertices = 99; triangles = 98;
  GridCheck(!Arcadia_Math_NURBS3Grid_getCounts(1, 4, &vertices, &triangles));
  GridCheck(!Arcadia_Math_NURBS3Grid_getCounts(4, 1, &vertices, &triangles));
  GridCheck(!Arcadia_Math_NURBS3Grid_getCounts((Arcadia_SizeValue)-1, 4, &vertices, &triangles));
  GridCheck(!Arcadia_Math_NURBS3Grid_getCounts(4, (Arcadia_SizeValue)-1, &vertices, &triangles));
  GridCheck(!Arcadia_Math_NURBS3Grid_getCounts(3, 4, &vertices, &vertices));
  GridCheck(!Arcadia_Math_NURBS3Grid_getCounts(3, 4, NULL, &triangles));
  GridCheck(!Arcadia_Math_NURBS3Grid_getCounts(3, 4, &vertices, NULL));
  GridCheck(vertices == 99 && triangles == 98);
  return EXIT_SUCCESS;
}

static int Case(gridTriangles)(void)
{
  Arcadia_SizeValue triangles[12][3];
  for (int i = 0; i < 12; ++i) for (int j = 0; j < 3; ++j) triangles[i][j] = 999;
  GridCheck(!Arcadia_Math_NURBS3Grid_makeTriangles(3, 4, triangles, 11));
  GridCheck(triangles[0][0] == 999 && triangles[11][2] == 999);
  GridCheck(Arcadia_Math_NURBS3Grid_makeTriangles(3, 4, triangles, 12));
  // First cell: vertex grid strides by samplesV=4.
  GridCheck(triangles[0][0] == 0 && triangles[0][1] == 4 && triangles[0][2] == 1);
  GridCheck(triangles[1][0] == 1 && triangles[1][1] == 4 && triangles[1][2] == 5);
  GridCheck(triangles[11][0] == 7 && triangles[11][1] == 10 && triangles[11][2] == 11);
  for (int i = 0; i < 12; ++i) for (int j = 0; j < 3; ++j)
    GridCheck(triangles[i][j] < 12);
  GridCheck(!Arcadia_Math_NURBS3Grid_makeTriangles(3, 4, NULL, 12));
  return EXIT_SUCCESS;
}

static int Case(gridUV)(void)
{
  Scalar uv[12][2];
  for (int i = 0; i < 12; ++i) uv[i][0] = uv[i][1] = (Scalar)-1;
  GridCheck(!GridUV(3, 4, uv, 11));
  GridCheck(uv[0][0] == -1 && uv[11][1] == -1);
  GridCheck(GridUV(3, 4, uv, 12));
  GridCheck(uv[0][0] == 0 && uv[0][1] == 0);
  GridCheck(uv[11][0] == 1 && uv[11][1] == 1);
  GridCheck(fabs((double)uv[6][0] - 0.5) <= Epsilon);
  GridCheck(fabs((double)uv[6][1] - 2. / 3.) <= Epsilon);
  GridCheck(!GridUV(3, 4, NULL, 12));
  return EXIT_SUCCESS;
}

static int Case(gridSampleCompatibility)(void)
{
  const Scalar points[4][3] = {{0,0,0},{0,2,0},{2,0,0},{2,2,0}};
  const Scalar knots[4] = {0,0,1,1};
  Scalar scratch[16], positions[12][3], normals[12][3], uv[12][2];
  Arcadia_SizeValue triangles[12][3];
  GridCheck(N(sampleSurface)(points, NULL, 2, 2, 1, 1,
    knots, 4, knots, 4, 3, 4, positions, normals, scratch, 16));
  GridCheck(GridUV(3, 4, uv, 12));
  GridCheck(Arcadia_Math_NURBS3Grid_makeTriangles(3, 4, triangles, 12));
  for (int i = 0; i < 12; ++i) {
    GridCheck(fabs((double)positions[i][0] - 2. * (double)uv[i][0]) <= Epsilon);
    GridCheck(fabs((double)positions[i][1] - 2. * (double)uv[i][1]) <= Epsilon);
    GridCheck(normals[i][0] == 0 && normals[i][1] == 0 && normals[i][2] == 1);
  }
  // Triangle winding agrees with cross(dP/du, dP/dv) on this plane.
  for (int i = 0; i < 12; ++i) {
    Arcadia_SizeValue a = triangles[i][0], b = triangles[i][1], c = triangles[i][2];
    double abx = (double)positions[b][0] - (double)positions[a][0];
    double aby = (double)positions[b][1] - (double)positions[a][1];
    double acx = (double)positions[c][0] - (double)positions[a][0];
    double acy = (double)positions[c][1] - (double)positions[a][1];
    GridCheck(abx * acy - aby * acx > 0.);
  }
  return EXIT_SUCCESS;
}

#undef GridCheck
