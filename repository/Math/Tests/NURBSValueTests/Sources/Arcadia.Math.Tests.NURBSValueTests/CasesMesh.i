// The same regression cases are instantiated with Scalar, N, Case, Epsilon.
#define MeshCheck(x) do { if (!(x)) return EXIT_FAILURE; } while (0)

static int Case(meshCounts)(void)
{
  Arcadia_SizeValue count = 99;
  MeshCheck(Mesh(getVertexCount)(2,2,&count) && count == 6);
  MeshCheck(Mesh(getVertexCount)(3,4,&count) && count == 36);
  count = 99;
  MeshCheck(!Mesh(getVertexCount)(1,4,&count) && count == 99);
  MeshCheck(!Mesh(getVertexCount)((Arcadia_SizeValue)-1,4,&count) && count == 99);
  MeshCheck(!Mesh(getVertexCount)(4,(Arcadia_SizeValue)-1,&count) && count == 99);
  MeshCheck(!Mesh(getVertexCount)(2,2,NULL));
  return EXIT_SUCCESS;
}

static int Case(meshFlat)(void)
{
  const Scalar p[4][3]={{0,0,0},{0,2,0},{2,0,0},{2,2,0}};
  const Scalar knots[4]={0,0,1,1};
  Scalar out[36][3], normals[36][3], uv[36][2], scratch[16];
  MeshCheck(Mesh(sampleTriangles)(p,NULL,2,2,1,1,knots,4,knots,4,
    3,4,out,normals,uv,36,scratch,16));
  // First triangle a,c,b: (0,0),(1,0),(0,2/3).
  MeshCheck(out[0][0] == 0 && out[0][1] == 0);
  MeshCheck(out[1][0] == 1 && out[1][1] == 0);
  MeshCheck(fabs((double)out[2][1] - 2./3.) <= Epsilon);
  MeshCheck(uv[0][0] == 0 && uv[0][1] == 0);
  MeshCheck(uv[35][0] == 1 && uv[35][1] == 1);
  for (int i=0;i<36;++i) {
    MeshCheck(normals[i][0] == 0 && normals[i][1] == 0 && normals[i][2] == 1);
    MeshCheck(fabs((double)out[i][0] - 2.*uv[i][0]) <= Epsilon);
    MeshCheck(fabs((double)out[i][1] - 2.*uv[i][1]) <= Epsilon);
  }
  for (int i=0;i<36;i+=3) {
    double abx = (double)out[i+1][0]-out[i][0];
    double aby = (double)out[i+1][1]-out[i][1];
    double acx = (double)out[i+2][0]-out[i][0];
    double acy = (double)out[i+2][1]-out[i][1];
    MeshCheck(abx*acy-aby*acx>0.);
  }
  return EXIT_SUCCESS;
}

static int Case(meshRational)(void)
{
  const Scalar p[4][3]={{0,0,0},{0,2,0},{2,0,0},{2,2,0}};
  const Scalar weights[4]={1,1,1,5};
  const Scalar knots[4]={0,0,1,1};
  Scalar out[6][3], uv[6][2], scratch[16];
  MeshCheck(Mesh(sampleTriangles)(p,weights,2,2,1,1,knots,4,knots,4,
    2,2,out,NULL,uv,6,scratch,16));
  // At the final corner (1,1), the rational surface interpolates (2,2,0).
  MeshCheck(out[5][0] == 2 && out[5][1] == 2);
  MeshCheck(out[5][2] == 0 && uv[5][0] == 1 && uv[5][1] == 1);
  return EXIT_SUCCESS;
}

static int Case(meshDegenerate)(void)
{
  const Scalar p[4][3]={{1,2,3},{1,2,3},{1,2,3},{1,2,3}};
  const Scalar knots[4]={0,0,1,1};
  Scalar out[6][3], uv[6][2], normals[6][3], scratch[16];
  out[0][0] = (Scalar)99; normals[0][0] = (Scalar)88;
  MeshCheck(!Mesh(sampleTriangles)(p,NULL,2,2,1,1,knots,4,knots,4,
    2,2,out,normals,uv,6,scratch,16));
  MeshCheck(out[0][0] == 99 && normals[0][0] == 88);
  MeshCheck(Mesh(sampleTriangles)(p,NULL,2,2,1,1,knots,4,knots,4,
    2,2,out,NULL,uv,6,scratch,16));
  MeshCheck(out[0][0] == 1 && out[5][2] == 3);
  return EXIT_SUCCESS;
}

static int Case(meshInvalid)(void)
{
  const Scalar p[4][3]={{0,0,0},{0,2,0},{2,0,0},{2,2,0}};
  const Scalar knots[4]={0,0,1,1};
  const Scalar badKnots[4]={0,1,0,1};
  const Scalar badWeights[4]={1,1,1,0};
  Scalar out[6][3], uv[6][2], scratch[16];
  out[0][0] = (Scalar)77; uv[0][0] = (Scalar)88;
  MeshCheck(!Mesh(sampleTriangles)(p,NULL,2,2,1,1,knots,4,knots,4,
    2,2,out,NULL,uv,5,scratch,16));
  MeshCheck(!Mesh(sampleTriangles)(p,NULL,2,2,1,1,knots,4,knots,4,
    2,2,out,NULL,uv,6,scratch,15));
  MeshCheck(!Mesh(sampleTriangles)(p,NULL,2,2,1,1,badKnots,4,knots,4,
    2,2,out,NULL,uv,6,scratch,16));
  MeshCheck(!Mesh(sampleTriangles)(p,badWeights,2,2,1,1,knots,4,knots,4,
    2,2,out,NULL,uv,6,scratch,16));
  MeshCheck(out[0][0] == 77 && uv[0][0] == 88);
  return EXIT_SUCCESS;
}
#undef MeshCheck
