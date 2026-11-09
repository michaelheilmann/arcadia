// Instantiated once for Real32 and once for Real64 from Main.c.
#define CheckDiff(test) do { if (!(test)) return EXIT_FAILURE; } while (0)
#define D(name) N(name)

static int Case(curveTangentLine)(void)
{
  const Scalar points[2][3] = {{1,2,3},{3,6,9}};
  const Scalar knots[4] = {0,0,1,1};
  Scalar scratch[8];
  Scalar tangent[3] = {0,0,0};
  for (int i=0;i<=4;++i) {
    CheckDiff(D(evaluateCurveTangent)(points,NULL,2,1,knots,4,(Scalar)i/(Scalar)4,scratch,8,tangent));
    CheckDiff(fabs((double)tangent[0] - 2.) <= Epsilon);
    CheckDiff(fabs((double)tangent[1] - 4.) <= Epsilon);
    CheckDiff(fabs((double)tangent[2] - 6.) <= Epsilon);
  }
  return EXIT_SUCCESS;
}

static int Case(curveTangentRational)(void)
{
  const Scalar points[2][3] = {{0,0,0},{2,0,0}};
  const Scalar weights[2] = {1,3};
  const Scalar knots[4] = {0,0,1,1};
  Scalar scratch[8], tangent[3];
  CheckDiff(D(evaluateCurveTangent)(points,weights,2,1,knots,4,(Scalar)0.5,scratch,8,tangent));
  CheckDiff(fabs((double)tangent[0] - 1.5) <= Epsilon);
  CheckDiff(tangent[1]==0 && tangent[2]==0);
  return EXIT_SUCCESS;
}

static int Case(curveTangentQuadratic)(void)
{
  Scalar points[3][3] = {{0,0,0},{1,2,3},{4,8,12}};
  const Scalar knots[6] = {0,0,0,1,1,1};
  Scalar scratch[12], tangent[3];
  CheckDiff(D(evaluateCurveTangent)((const Scalar(*)[3])points,NULL,3,2,knots,6,(Scalar)0.5,scratch,12,tangent));
  CheckDiff(fabs((double)tangent[0] - 4.) <= Epsilon);
  CheckDiff(fabs((double)tangent[1] - 8.) <= Epsilon);
  CheckDiff(fabs((double)tangent[2] - 12.) <= Epsilon);
  // Output aliases an input control point.
  CheckDiff(D(evaluateCurveTangent)((const Scalar(*)[3])points,NULL,3,2,knots,6,(Scalar)0.5,scratch,12,points[0]));
  CheckDiff(fabs((double)points[0][0] - 4.) <= Epsilon);
  return EXIT_SUCCESS;
}

static int Case(curveTangentDegreeZero)(void)
{
  const Scalar points[2][3] = {{5,6,7},{8,9,10}};
  const Scalar knots[3] = {0,(Scalar)0.5,1};
  Scalar scratch[4], tangent[3] = {9,9,9};
  CheckDiff(D(evaluateCurveTangent)(points,NULL,2,0,knots,3,(Scalar)0.75,scratch,4,tangent));
  CheckDiff(tangent[0]==0 && tangent[1]==0 && tangent[2]==0);
  return EXIT_SUCCESS;
}

static int Case(curveTangentRepeated)(void)
{
  const Scalar points[5][3] = {{0,0,0},{1,0,0},{2,0,0},{3,0,0},{4,0,0}};
  const Scalar knots[8] = {0,0,0,(Scalar)0.5,(Scalar)0.5,1,1,1};
  Scalar scratch[12], tangent[3];
  CheckDiff(D(evaluateCurveTangent)(points,NULL,5,2,knots,8,(Scalar)0.5,scratch,12,tangent));
  CheckDiff(isfinite(tangent[0]) && isfinite(tangent[1]) && isfinite(tangent[2]));
  CheckDiff(fabs((double)tangent[0] - 4.) <= Epsilon);
  return EXIT_SUCCESS;
}

static int Case(curveTangentInvalid)(void)
{
  const Scalar points[2][3] = {{0,0,0},{2,0,0}};
  const Scalar knots[4] = {0,0,1,1};
  const Scalar weights[2] = {1,0};
  Scalar scratch[8], tangent[3] = {7,8,9};
  CheckDiff(!D(evaluateCurveTangent)(points,NULL,2,1,knots,4,-1,scratch,8,tangent));
  CheckDiff(!D(evaluateCurveTangent)(points,NULL,2,1,knots,4,(Scalar)0.5,scratch,7,tangent));
  CheckDiff(!D(evaluateCurveTangent)(points,weights,2,1,knots,4,(Scalar)0.5,scratch,8,tangent));
  CheckDiff(tangent[0]==7 && tangent[1]==8 && tangent[2]==9);
  return EXIT_SUCCESS;
}

static int Case(surfaceDifferentialsPlane)(void)
{
  const Scalar points[4][3] = {{0,0,0},{0,2,0},{2,0,0},{2,2,0}};
  const Scalar knots[4] = {0,0,1,1};
  Scalar scratch[16], position[3], du[3], dv[3];
  for (int i=0;i<=4;++i) {
    CheckDiff(D(evaluateSurfaceDifferentials)(points,NULL,2,2,1,1,knots,4,knots,4,
      (Scalar)i/(Scalar)4,(Scalar)0.75,scratch,16,position,du,dv));
    CheckDiff(fabs((double)position[0]-(double)i/2.) <= Epsilon);
    CheckDiff(fabs((double)position[1]-1.5) <= Epsilon);
    CheckDiff(du[0]==2 && du[1]==0 && du[2]==0);
    CheckDiff(dv[0]==0 && dv[1]==2 && dv[2]==0);
  }
  return EXIT_SUCCESS;
}

static int Case(surfaceDifferentialsRational)(void)
{
  const Scalar points[4][3] = {{0,0,0},{0,2,0},{2,0,0},{2,2,0}};
  const Scalar weights[4] = {1,1,1,5};
  const Scalar knots[4] = {0,0,1,1};
  Scalar scratch[16], position[3], du[3], dv[3];
  CheckDiff(D(evaluateSurfaceDifferentials)(points,weights,2,2,1,1,knots,4,knots,4,
    (Scalar)0.5,(Scalar)0.5,scratch,16,position,du,dv));
  CheckDiff(fabs((double)position[0]-1.5) <= Epsilon);
  CheckDiff(fabs((double)position[1]-1.5) <= Epsilon);
  CheckDiff(fabs((double)du[0]-1.5) <= Epsilon && fabs((double)du[1]-0.5) <= Epsilon);
  CheckDiff(fabs((double)dv[0]-0.5) <= Epsilon && fabs((double)dv[1]-1.5) <= Epsilon);
  return EXIT_SUCCESS;
}

static int Case(surfaceDifferentialsDegenerate)(void)
{
  const Scalar points[4][3] = {{1,2,3},{1,2,3},{1,2,3},{1,2,3}};
  const Scalar knots[4] = {0,0,1,1};
  Scalar scratch[16], position[3], du[3], dv[3];
  CheckDiff(D(evaluateSurfaceDifferentials)(points,NULL,2,2,1,1,knots,4,knots,4,
    (Scalar)0.25,(Scalar)0.75,scratch,16,position,du,dv));
  CheckDiff(position[0]==1 && position[1]==2 && position[2]==3);
  CheckDiff(du[0]==0 && du[1]==0 && du[2]==0);
  CheckDiff(dv[0]==0 && dv[1]==0 && dv[2]==0);
  return EXIT_SUCCESS;
}

static int Case(surfaceDifferentialsAlias)(void)
{
  Scalar points[4][3] = {{0,0,0},{0,2,0},{2,0,0},{2,2,0}};
  const Scalar knots[4] = {0,0,1,1};
  Scalar scratch[16], du[3], dv[3];
  CheckDiff(D(evaluateSurfaceDifferentials)((const Scalar(*)[3])points,NULL,2,2,1,1,
    knots,4,knots,4,(Scalar)0.25,(Scalar)0.75,scratch,16,points[0],du,dv));
  CheckDiff(fabs((double)points[0][0]-0.5)<=Epsilon);
  CheckDiff(fabs((double)points[0][1]-1.5)<=Epsilon);
  CheckDiff(du[0]==2 && dv[1]==2);
  return EXIT_SUCCESS;
}

static int Case(surfaceSamplingGrid)(void)
{
  const Scalar points[4][3] = {{0,0,0},{0,2,0},{2,0,0},{2,2,0}};
  const Scalar knots[4] = {0,0,1,1};
  Scalar scratch[16], positions[12][3], normals[12][3];
  CheckDiff(D(sampleSurface)(points,NULL,2,2,1,1,knots,4,knots,4,
    3,4,positions,normals,scratch,16));
  for (int i=0;i<3;++i) for (int j=0;j<4;++j) {
    int idx=i*4+j;
    CheckDiff(fabs((double)positions[idx][0] - (double)i) <= Epsilon);
    CheckDiff(fabs((double)positions[idx][1] - 2.*(double)j/3.) <= Epsilon);
    CheckDiff(positions[idx][2]==0);
    CheckDiff(normals[idx][0]==0 && normals[idx][1]==0 && normals[idx][2]==1);
  }
  return EXIT_SUCCESS;
}

static int Case(surfaceSamplingDegenerate)(void)
{
  const Scalar points[4][3] = {{1,1,1},{1,1,1},{1,1,1},{1,1,1}};
  const Scalar knots[4] = {0,0,1,1};
  Scalar scratch[16], positions[4][3], normals[4][3];
  positions[0][0]=999; normals[0][0]=999;
  CheckDiff(!D(sampleSurface)(points,NULL,2,2,1,1,knots,4,knots,4,
    2,2,positions,normals,scratch,16));
  CheckDiff(positions[0][0]==999 && normals[0][0]==999);
  CheckDiff(D(sampleSurface)(points,NULL,2,2,1,1,knots,4,knots,4,
    2,2,positions,NULL,scratch,16));
  CheckDiff(positions[0][0]==1 && positions[3][2]==1);
  return EXIT_SUCCESS;
}

static int Case(surfaceSamplingInvalid)(void)
{
  const Scalar points[4][3] = {{0,0,0},{0,2,0},{2,0,0},{2,2,0}};
  const Scalar knots[4] = {0,0,1,1};
  Scalar scratch[16], positions[4][3];
  positions[0][0]=999;
  CheckDiff(!D(sampleSurface)(points,NULL,2,2,1,1,knots,4,knots,4,
    1,2,positions,NULL,scratch,16));
  CheckDiff(!D(sampleSurface)(points,NULL,2,2,1,1,knots,4,knots,4,
    2,2,positions,NULL,scratch,15));
  CheckDiff(!D(sampleSurface)(points,NULL,2,2,1,1,knots,4,knots,4,
    (Arcadia_SizeValue)-1,2,positions,NULL,scratch,16));
  CheckDiff(positions[0][0]==999);
  return EXIT_SUCCESS;
}

#undef D
#undef CheckDiff
