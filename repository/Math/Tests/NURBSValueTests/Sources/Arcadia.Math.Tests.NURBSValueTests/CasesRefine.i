// Knot-refinement tests shared by native Real32/Real64 implementations.
#define RefineCheck(x) do { if (!(x)) return EXIT_FAILURE; } while (0)

static int
Case(refineLinear)
  (
    void
  )
{
  const Scalar points[2][3] = {{0,0,0},{4,2,6}};
  const Scalar knots[4] = {0,0,1,1};
  Scalar newPoints[3][3], newKnots[5], newWeights[3];
  RefineCheck(Refine(insertKnot)(points, NULL, 2, 1, knots, 4, (Scalar)0.25,
      newPoints, newWeights, 3, newKnots, 5));
  RefineCheck(newKnots[0] == 0 && newKnots[1] == 0 &&
      newKnots[2] == (Scalar)0.25 && newKnots[3] == 1 && newKnots[4] == 1);
  RefineCheck(newPoints[1][0] == 1 && newPoints[1][1] == (Scalar)0.5);
  RefineCheck(newWeights[0] == 1 && newWeights[1] == 1 && newWeights[2] == 1);
  Scalar workspace[6], a[3], b[3];
  for (int i=0; i<=16; ++i) {
    Scalar u = (Scalar)((double)i / 16.);
    RefineCheck(N(evaluateCurve)(points, NULL, 2, 1, knots, 4, u, workspace, 6, a));
    RefineCheck(N(evaluateCurve)((const Scalar(*)[3])newPoints, newWeights,
        3, 1, newKnots, 5, u, workspace, 6, b));
    for (int k=0; k<3; ++k) RefineCheck(fabs((double)a[k]-(double)b[k]) <= Epsilon);
  }
  return EXIT_SUCCESS;
}

static int
Case(refineRational)
  (
    void
  )
{
  const Scalar points[3][3]={{1,0,0},{1,1,0},{0,1,0}};
  const Scalar weights[3]={1,(Scalar)0.7071067811865475244,1};
  const Scalar knots[6]={0,0,0,1,1,1};
  Scalar p[4][3], w[4], k[7], scratch[9], old[3], now[3];
  RefineCheck(Refine(insertKnot)(points,weights,3,2,knots,6,(Scalar)0.5,
      p,w,4,k,7));
  RefineCheck(k[3] == (Scalar)0.5);
  for (int i=0; i<=20; ++i) {
    Scalar u = (Scalar)((double)i / 20.);
    RefineCheck(N(evaluateCurve)(points,weights,3,2,knots,6,u,scratch,9,old));
    RefineCheck(N(evaluateCurve)((const Scalar(*)[3])p,w,4,2,k,7,u,scratch,9,now));
    for(int j=0;j<3;++j) RefineCheck(fabs((double)old[j]-(double)now[j]) <= 8*Epsilon);
  }
  return EXIT_SUCCESS;
}

static int
Case(refineRepeated)
  (
    void
  )
{
  const Scalar points[5][3]={{0,0,0},{1,2,0},{2,0,0},{3,2,0},{4,0,0}};
  const Scalar knots[8]={0,0,0,(Scalar)0.5,(Scalar)0.5,1,1,1};
  Scalar p[6][3],k[9],scratch[9],a[3],b[3];
  RefineCheck(Refine(insertKnot)(points,NULL,5,2,knots,8,(Scalar)0.5,
      p,NULL,6,k,9));
  RefineCheck(k[3] == (Scalar)0.5 && k[4] == (Scalar)0.5 && k[5] == (Scalar)0.5);
  for(int i=0;i<=20;++i) {
    Scalar u=(Scalar)((double)i/20.);
    RefineCheck(N(evaluateCurve)(points,NULL,5,2,knots,8,u,scratch,9,a));
    RefineCheck(N(evaluateCurve)((const Scalar(*)[3])p,NULL,6,2,k,9,u,scratch,9,b));
    for(int j=0;j<3;++j) RefineCheck(fabs((double)a[j]-(double)b[j])<=8*Epsilon);
  }
  return EXIT_SUCCESS;
}

static int
Case(refineNonUniform)
  (
    void
  )
{
  const Scalar points[5][3]={{0,0,0},{1,1,0},{2,-1,0},{3,1,0},{4,0,0}};
  const Scalar knots[8]={0,0,0,(Scalar)0.2,(Scalar)0.7,1,1,1};
  Scalar p[6][3],k[9],scratch[9],a[3],b[3];
  RefineCheck(Refine(insertKnot)(points,NULL,5,2,knots,8,(Scalar)0.45,
      p,NULL,6,k,9));
  for(int i=0;i<=20;++i) {
    Scalar u=(Scalar)((double)i/20.);
    RefineCheck(N(evaluateCurve)(points,NULL,5,2,knots,8,u,scratch,9,a));
    RefineCheck(N(evaluateCurve)((const Scalar(*)[3])p,NULL,6,2,k,9,u,scratch,9,b));
    for(int j=0;j<3;++j) RefineCheck(fabs((double)a[j]-(double)b[j])<=8*Epsilon);
  }
  return EXIT_SUCCESS;
}

static int
Case(refineInvalid)
  (
    void
  )
{
  const Scalar points[3][3]={{0,0,0},{1,1,0},{2,0,0}};
  const Scalar knots[6]={0,0,0,1,1,1};
  const Scalar badKnots[6]={0,0,1,0,1,1};
  const Scalar badWeights[3]={1,0,1};
  Scalar p[4][3], w[4], k[7];
  p[0][0]=(Scalar)77; k[0]=(Scalar)88; w[0]=(Scalar)99;
  RefineCheck(!Refine(insertKnot)(points,NULL,3,2,knots,6,0,p,w,4,k,7));
  RefineCheck(!Refine(insertKnot)(points,NULL,3,2,knots,6,1,p,w,4,k,7));
  RefineCheck(!Refine(insertKnot)(points,NULL,3,2,knots,6,(Scalar)0.5,p,w,3,k,7));
  RefineCheck(!Refine(insertKnot)(points,NULL,3,2,knots,6,(Scalar)0.5,p,w,4,k,6));
  RefineCheck(!Refine(insertKnot)(points,NULL,3,2,badKnots,6,(Scalar)0.5,p,w,4,k,7));
  RefineCheck(!Refine(insertKnot)(points,badWeights,3,2,knots,6,(Scalar)0.5,p,w,4,k,7));
  RefineCheck(!Refine(insertKnot)(points,(const Scalar[]){1,1,1},3,2,
      knots,6,(Scalar)0.5,p,NULL,4,k,7));
  RefineCheck(!Refine(insertKnot)(points,NULL,3,2,knots,6,(Scalar)NAN,p,w,4,k,7));
  RefineCheck(p[0][0] == 77 && k[0] == 88 && w[0] == 99);
  return EXIT_SUCCESS;
}

static int
Case(refineDegreeZero)
  (
    void
  )
{
  const Scalar points[2][3]={{4,0,0},{9,0,0}};
  const Scalar knots[3]={0,(Scalar)0.5,1};
  Scalar p[3][3],k[4],scratch[3],a[3],b[3];
  RefineCheck(Refine(insertKnot)(points,NULL,2,0,knots,3,(Scalar)0.25,
      p,NULL,3,k,4));
  RefineCheck(k[1] == (Scalar)0.25 && k[2] == (Scalar)0.5);
  for(int i=0;i<=20;++i) {
    Scalar u=(Scalar)((double)i/20.);
    RefineCheck(N(evaluateCurve)(points,NULL,2,0,knots,3,u,scratch,3,a));
    RefineCheck(N(evaluateCurve)((const Scalar(*)[3])p,NULL,3,0,k,4,u,scratch,3,b));
    RefineCheck(a[0] == b[0]);
  }
  return EXIT_SUCCESS;
}
#undef RefineCheck
