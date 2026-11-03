// Adaptive line sampling for both native scalar widths.
#define AdaptiveCheck(x) do { if (!(x)) return EXIT_FAILURE; } while (0)

static int Case(adaptiveStraight)(void)
{
  const Scalar p[2][3]={{1,2,3},{5,2,3}};
  const Scalar knots[4]={0,0,1,1};
  Scalar workspace[8], vertices[32][3];
  Arcadia_SizeValue count=99;
  AdaptiveCheck(Adaptive(sampleLines)(p,NULL,2,1,knots,4,
      (Scalar)0.001,12,vertices,32,&count,workspace,8));
  AdaptiveCheck(count==2);
  AdaptiveCheck(vertices[0][0]==1 && vertices[1][0]==5);
  AdaptiveCheck(vertices[0][1]==2 && vertices[1][2]==3);
  return EXIT_SUCCESS;
}

static int Case(adaptiveCurvature)(void)
{
  const Scalar p[4][3]={{0,0,0},{0,3,0},{3,3,0},{3,0,0}};
  const Scalar knots[8]={0,0,0,0,1,1,1,1};
  Scalar workspace[16], vertices[2048][3];
  Arcadia_SizeValue count=0, coarse=0;
  AdaptiveCheck(Adaptive(sampleLines)(p,NULL,4,3,knots,8,
      (Scalar)3,12,vertices,2048,&coarse,workspace,16));
  AdaptiveCheck(coarse==2);
  AdaptiveCheck(Adaptive(sampleLines)(p,NULL,4,3,knots,8,
      (Scalar)0.05,12,vertices,2048,&count,workspace,16));
  AdaptiveCheck(count>coarse && count<=2048 && count%2==0);
  AdaptiveCheck(vertices[0][0]==0 && vertices[0][1]==0);
  AdaptiveCheck(vertices[count-1][0]==3 && vertices[count-1][1]==0);
  for (Arcadia_SizeValue i=2;i<count;i+=2)
    for (int j=0;j<3;++j) AdaptiveCheck(vertices[i-1][j]==vertices[i][j]);
  return EXIT_SUCCESS;
}

static int Case(adaptiveCapacity)(void)
{
  const Scalar p[4][3]={{0,0,0},{0,3,0},{3,3,0},{3,0,0}};
  const Scalar knots[8]={0,0,0,0,1,1,1,1};
  Scalar workspace[16], vertices[6][3];
  Arcadia_SizeValue count=79;
  vertices[0][0]=(Scalar)42;
  AdaptiveCheck(!Adaptive(sampleLines)(p,NULL,4,3,knots,8,
      (Scalar)0.001,12,vertices,6,&count,workspace,16));
  AdaptiveCheck(count==79 && vertices[0][0]==42);
  AdaptiveCheck(Adaptive(sampleLines)(p,NULL,4,3,knots,8,
      (Scalar)0.001,0,vertices,6,&count,workspace,16));
  AdaptiveCheck(count==2);
  return EXIT_SUCCESS;
}

static int Case(adaptiveInvalid)(void)
{
  const Scalar p[2][3]={{0,0,0},{2,0,0}};
  const Scalar knots[4]={0,0,1,1}, invalidKnots[4]={0,1,0,1};
  const Scalar invalidWeights[2]={1,0};
  Scalar workspace[8], vertices[4][3];
  Arcadia_SizeValue count=79;
  vertices[0][0]=(Scalar)42;
  AdaptiveCheck(!Adaptive(sampleLines)(p,invalidWeights,2,1,knots,4,
      (Scalar)0.1,8,vertices,4,&count,workspace,8));
  AdaptiveCheck(!Adaptive(sampleLines)(p,NULL,2,1,invalidKnots,4,
      (Scalar)0.1,8,vertices,4,&count,workspace,8));
  AdaptiveCheck(!Adaptive(sampleLines)(p,NULL,2,1,knots,4,
      (Scalar)-1,8,vertices,4,&count,workspace,8));
  AdaptiveCheck(!Adaptive(sampleLines)(p,NULL,2,1,knots,4,
      (Scalar)0.1,21,vertices,4,&count,workspace,8));
  AdaptiveCheck(!Adaptive(sampleLines)(p,NULL,2,1,knots,4,
      (Scalar)0.1,8,vertices,1,&count,workspace,8));
  AdaptiveCheck(count==79 && vertices[0][0]==42);
  return EXIT_SUCCESS;
}
#undef AdaptiveCheck
