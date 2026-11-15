// Tests instantiated for Real32 and Real64 by NURBSValueTests/Main.c.
#define NetCheck(value) do { if (!(value)) return EXIT_FAILURE; } while (0)

static int
Case(netCounts)
  (
    void
  )
{
  Arcadia_SizeValue lines = 71, vertices = 72;
  NetCheck(Net(countCurve)(4, &lines, &vertices) && lines == 3 && vertices == 6);
  NetCheck(Net(countSurface)(2, 3, &lines, &vertices) && lines == 7 && vertices == 14);
  NetCheck(Net(countSurface)(3, 2, &lines, &vertices) && lines == 7 && vertices == 14);
  NetCheck(!Net(countSurface)(1, 3, &lines, &vertices));
  NetCheck(!Net(countSurface)((Arcadia_SizeValue)-1, 3, &lines, &vertices));
  NetCheck(!Net(countCurve)((Arcadia_SizeValue)-1, &lines, &vertices));
  NetCheck(!Net(countCurve)(4, &lines, &lines));
  NetCheck(!Net(countSurface)(2, 3, &lines, NULL));
  NetCheck(lines == 7 && vertices == 14);
  return EXIT_SUCCESS;
}

static int
Case(netCurve)
  (
    void
  )
{
  const Scalar points[3][3] = {{0,0,0},{1,2,3},{2,4,6}};
  Scalar output[4][3];
  Arcadia_SizeValue pairs[2][2];
  NetCheck(Net(emitCurve)(points, 3, output, 4, pairs, 2));
  NetCheck(output[0][0] == 0 && output[1][0] == 1);
  NetCheck(output[2][0] == 1 && output[3][0] == 2);
  NetCheck(output[3][1] == 4 && output[3][2] == 6);
  NetCheck(pairs[0][0] == 0 && pairs[0][1] == 1);
  NetCheck(pairs[1][0] == 1 && pairs[1][1] == 2);
  NetCheck(Net(emitCurve)(points, 3, output, 4, NULL, 0));
  return EXIT_SUCCESS;
}

static int
Case(netSurface)
  (
    void
  )
{
  const Scalar points[6][3] = {
    {0,0,0},{0,1,0},{0,2,0},
    {1,0,0},{1,1,0},{1,2,0}
  };
  Scalar output[14][3]; Arcadia_SizeValue pairs[7][2];
  NetCheck(Net(emitSurface)(points, 2, 3, output, 14, pairs, 7));
  // First u-edges: (0,3), (1,4), (2,5); then v-edges.
  NetCheck(pairs[0][0] == 0 && pairs[0][1] == 3);
  NetCheck(pairs[2][0] == 2 && pairs[2][1] == 5);
  NetCheck(pairs[3][0] == 0 && pairs[3][1] == 1);
  NetCheck(pairs[6][0] == 4 && pairs[6][1] == 5);
  for (Arcadia_SizeValue i = 0; i < 7; ++i)
    for (Arcadia_SizeValue k = 0; k < 3; ++k) {
      NetCheck(output[2*i][k] == points[pairs[i][0]][k]);
      NetCheck(output[2*i+1][k] == points[pairs[i][1]][k]);
    }
  NetCheck(Net(emitSurface)(points, 2, 3, output, 14, NULL, 0));
  return EXIT_SUCCESS;
}

static int
Case(netDegenerate)
  (
    void
  )
{
  const Scalar points[4][3] = {{2,2,2},{2,2,2},{2,2,2},{2,2,2}};
  Scalar out[8][3]; Arcadia_SizeValue pairs[4][2];
  NetCheck(Net(emitSurface)(points, 2, 2, out, 8, pairs, 4));
  for (Arcadia_SizeValue i = 0; i < 8; ++i)
    NetCheck(out[i][0] == 2 && out[i][1] == 2 && out[i][2] == 2);
  NetCheck(pairs[0][0] == 0 && pairs[0][1] == 2);
  NetCheck(pairs[3][0] == 2 && pairs[3][1] == 3);
  return EXIT_SUCCESS;
}

static int
Case(netInvalid)
  (
    void
  )
{
  Scalar points[4][3] = {{0,0,0},{1,0,0},{0,1,0},{1,1,0}};
  Scalar out[8][3] = {{0}}; Arcadia_SizeValue pairs[4][2] = {{0}};
  out[0][0] = (Scalar)77; pairs[0][0] = 88;
  NetCheck(!Net(emitSurface)((const Scalar(*)[3])points, 2, 2, out, 7, pairs, 4));
  NetCheck(!Net(emitSurface)((const Scalar(*)[3])points, 2, 2, out, 8, pairs, 3));
  NetCheck(!Net(emitSurface)(NULL, 2, 2, out, 8, pairs, 4));
  NetCheck(!Net(emitCurve)((const Scalar(*)[3])points, 4, out, 5, pairs, 3));
  points[2][0] = (Scalar)NAN;
  NetCheck(!Net(emitSurface)((const Scalar(*)[3])points, 2, 2, out, 8, pairs, 4));
  NetCheck(out[0][0] == 77 && pairs[0][0] == 88);
  return EXIT_SUCCESS;
}
#undef NetCheck
