// Native NURBS surface mesh picking, instantiated for both precisions.
#define PickCheck(x) do { if (!(x)) return EXIT_FAILURE; } while (0)

static int
Case(pickGridCenter)
  (
    void
  )
{
  const Scalar grid[4][3] = {{0,0,0},{0,1,0},{1,0,0},{1,1,0}};
  PickRay ray = {{(Scalar).25,(Scalar).25,2},{0,0,-2}};
  Arcadia_SizeValue triangle = 99;
  Scalar t = -1, uv[2] = {-1,-1}, pos[3] = {-1,-1,-1};
  PickCheck(Pick(pickGrid)(&ray,(const Scalar(*)[3])grid,2,2,2,&triangle,&t,uv,pos));
  PickCheck(triangle == 0 && fabs((double)t - 1.) <= Epsilon);
  PickCheck(fabs((double)uv[0] - .25) <= Epsilon &&
            fabs((double)uv[1] - .25) <= Epsilon);
  PickCheck(fabs((double)pos[0] - .25) <= Epsilon &&
            fabs((double)pos[1] - .25) <= Epsilon && pos[2] == 0);
  // The back face is also pickable.
  ray.origin[2] = -2; ray.direction[2] = 2;
  PickCheck(Pick(pickGrid)(&ray,(const Scalar(*)[3])grid,2,2,2,&triangle,&t,uv,pos));
  PickCheck(triangle == 0 && t == 1);
  return EXIT_SUCCESS;
}

static int
Case(pickTrianglesNearest)
  (
    void
  )
{
  const Scalar positions[9][3] = {
    {0,0,0},{1,0,0},{0,1,0},
    {0,0,1},{1,0,1},{0,1,1},
    {0,0,1},{1,0,1},{0,1,1}
  };
  const Scalar uv[9][2] = {
    {0,0},{1,0},{0,1},{0,0},{1,0},{0,1},{0,0},{1,0},{0,1}
  };
  PickRay ray = {{(Scalar).2,(Scalar).3,3},{0,0,-2}};
  Arcadia_SizeValue triangle = 99;
  Scalar t = -1, hitUV[2], hit[3];
  PickCheck(Pick(pickTriangles)(&ray,positions,uv,9,2,&triangle,&t,hitUV,hit));
  PickCheck(triangle == 1 && t == 1); // Closest; tie chooses first.
  PickCheck(fabs((double)hitUV[0]-.2) <= Epsilon);
  PickCheck(fabs((double)hitUV[1]-.3) <= Epsilon);
  PickCheck(hit[2] == 1);
  PickCheck(!Pick(pickTriangles)(&ray,positions,uv,9,(Scalar).9,&triangle,&t,hitUV,hit));
  PickCheck(triangle == 1 && t == 1);
  PickCheck(Pick(pickTriangles)(&ray,positions,NULL,9,2,&triangle,&t,NULL,NULL));
  return EXIT_SUCCESS;
}

static int
Case(pickGridRectangular)
  (
    void
  )
{
  const Scalar grid[6][3] = {
    {0,0,0},{0,1,0},{0,2,0},
    {1,0,0},{1,1,0},{1,2,0}
  };
  PickRay ray = {{(Scalar).8,(Scalar)1.7,1},{0,0,-1}};
  Arcadia_SizeValue triangle = 99;
  Scalar uv[2];
  PickCheck(Pick(pickGrid)(&ray,(const Scalar(*)[3])grid,2,3,2,&triangle,NULL,uv,NULL));
  PickCheck(triangle == 3);
  PickCheck(fabs((double)uv[0] - .8) <= Epsilon);
  PickCheck(fabs((double)uv[1] - .85) <= Epsilon);
  return EXIT_SUCCESS;
}

static int
Case(pickGridNoHit)
  (
    void
  )
{
  const Scalar grid[4][3] = {{0,0,0},{0,1,0},{1,0,0},{1,1,0}};
  PickRay ray = {{2,2,1},{0,0,-1}};
  Arcadia_SizeValue triangle = 91;
  Scalar t = (Scalar)87, uv[2] = {12,13}, hit[3] = {21,22,23};
  PickCheck(!Pick(pickGrid)(&ray,(const Scalar(*)[3])grid,2,2,3,&triangle,&t,uv,hit));
  PickCheck(triangle == 91 && t == 87 && uv[0] == 12 && hit[0] == 21);
  ray.origin[0] = (Scalar).5; ray.origin[1] = (Scalar).5;
  ray.direction[2] = 1; // Ray travels away from surface.
  PickCheck(!Pick(pickGrid)(&ray,(const Scalar(*)[3])grid,2,2,3,&triangle,&t,uv,hit));
  PickCheck(triangle == 91 && t == 87);
  return EXIT_SUCCESS;
}

static int
Case(pickInvalid)
  (
    void
  )
{
  Scalar grid[4][3] = {{0,0,0},{0,1,0},{1,0,0},{1,1,0}};
  const Scalar uv[6][2] = {{0,0},{1,0},{0,1},{0,0},{1,0},{0,1}};
  const Scalar faces[6][3] = {
    {0,0,0},{1,0,0},{0,1,0},{1,0,0},{1,1,0},{0,1,0}
  };
  PickRay ray = {{(Scalar).5,(Scalar).5,2},{0,0,-1}};
  Arcadia_SizeValue triangle = 42;
  Scalar t = 43;
  PickCheck(!Pick(pickGrid)(&ray,(const Scalar(*)[3])grid,1,2,3,&triangle,&t,NULL,NULL));
  PickCheck(!Pick(pickGrid)(&ray,(const Scalar(*)[3])grid,(Arcadia_SizeValue)-1,2,3,&triangle,&t,NULL,NULL));
  PickCheck(!Pick(pickGrid)(&ray,(const Scalar(*)[3])grid,2,2,(Scalar)-1,&triangle,&t,NULL,NULL));
  PickCheck(!Pick(pickGrid)(&ray,(const Scalar(*)[3])grid,2,2,3,NULL,&t,NULL,NULL));
  PickCheck(!Pick(pickTriangles)(&ray,faces,uv,5,3,&triangle,&t,NULL,NULL));
  PickCheck(!Pick(pickTriangles)(&ray,faces,NULL,6,3,&triangle,&t,(Scalar[2]){0,0},NULL));
  PickCheck(triangle == 42 && t == 43);
  grid[2][0] = (Scalar)NAN;
  PickCheck(!Pick(pickGrid)(&ray,(const Scalar(*)[3])grid,2,2,3,&triangle,&t,NULL,NULL));
  grid[2][0] = 1;
  ray.direction[2] = (Scalar)NAN;
  PickCheck(!Pick(pickGrid)(&ray,(const Scalar(*)[3])grid,2,2,3,&triangle,&t,NULL,NULL));
  PickCheck(triangle == 42 && t == 43);
  return EXIT_SUCCESS;
}

static int
Case(pickDegenerate)
  (
    void
  )
{
  const Scalar grid[4][3] = {{1,1,0},{1,1,0},{1,1,0},{1,1,0}};
  PickRay ray = {{1,1,2},{0,0,-1}};
  Arcadia_SizeValue triangle = 77;
  PickCheck(!Pick(pickGrid)(&ray,(const Scalar(*)[3])grid,2,2,3,&triangle,NULL,NULL,NULL));
  PickCheck(triangle == 77);
  return EXIT_SUCCESS;
}

#undef PickCheck
