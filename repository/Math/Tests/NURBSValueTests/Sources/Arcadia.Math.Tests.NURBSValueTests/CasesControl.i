// Compiled for Real32 and Real64. The native control lattice remains u-major.
#define ControlCheck(test) do { if (!(test)) return EXIT_FAILURE; } while (0)
#if !defined(Control)
#error Control must refer to the active NURBS3ControlReal32/64 prefix
#endif

static int Case(controlBounds)(void)
{
  const Scalar pts[6][3] = {{-2,0,1},{2,3,-5},{1,1,0},{0,-4,2},{1,0,3},{3,2,0}};
  ControlAABB hull;
  ControlCheck(Control(computeAABB)(pts,2,3,&hull));
  ControlCheck(!hull.empty && hull.minimum[0]==-2 && hull.maximum[0]==3);
  ControlCheck(hull.minimum[1]==-4 && hull.maximum[1]==3);
  ControlCheck(hull.minimum[2]==-5 && hull.maximum[2]==3);
  return EXIT_SUCCESS;
}

static int Case(controlNormalize)(void)
{
  Scalar pts[2][3]={{2,4,6},{6,6,6}};
  Scalar dst[2][3]={{99,99,99},{99,99,99}};
  ControlCheck(!Control(normalize)((const Scalar(*)[3])pts,1,2,dst,1));
  ControlCheck(dst[0][0]==99);
  ControlCheck(Control(normalize)((const Scalar(*)[3])pts,1,2,dst,2));
  ControlCheck(dst[0][0]==-1 && dst[1][0]==1);
  ControlCheck(dst[0][1]==-0.5 && dst[1][1]==0.5);
  ControlCheck(dst[0][2]==0 && dst[1][2]==0);
  ControlCheck(Control(normalize)((const Scalar(*)[3])pts,1,2,pts,2));
  ControlCheck(pts[0][0]==-1 && pts[1][0]==1);
  Scalar constant[2][3]={{5,5,5},{5,5,5}};
  ControlCheck(Control(normalize)((const Scalar(*)[3])constant,1,2,constant,2));
  ControlCheck(constant[0][0]==0 && constant[1][2]==0);
  return EXIT_SUCCESS;
}

static int Case(controlTransform)(void)
{
  Scalar pts[2][3]={{1,2,3},{-1,0,1}};
  Scalar dst[2][3]={{99,99,99},{99,99,99}};
  ControlAffine matrix;
  const Scalar values[12]={2,0,0,4, 0,3,0,-5, 0,0,1,6};
  ControlCheck(ControlAffineSet(&matrix,values));
  ControlCheck(!Control(transform)((const Scalar(*)[3])pts,1,2,&matrix,dst,1));
  ControlCheck(dst[0][0]==99);
  ControlCheck(Control(transform)((const Scalar(*)[3])pts,1,2,&matrix,dst,2));
  ControlCheck(dst[0][0]==6 && dst[0][1]==1 && dst[0][2]==9);
  ControlCheck(dst[1][0]==2 && dst[1][1]==-5 && dst[1][2]==7);
  ControlCheck(Control(transform)((const Scalar(*)[3])pts,1,2,&matrix,pts,2));
  ControlCheck(pts[0][0]==6 && pts[1][1]==-5);
  return EXIT_SUCCESS;
}

static int Case(controlPick)(void)
{
  const Scalar pts[4][3]={{4,0,0},{2,0.1,0},{2,-0.1,0},{9,3,0}};
  ControlRay ray={{0,0,0},{2,0,0}};
  Arcadia_SizeValue index=99;
  Scalar t=-1;
  ControlCheck(Control(pickControl)(pts,2,2,&ray,(Scalar)0.2,&index,&t));
  ControlCheck(index==1 && fabs((double)t-1.)<=Epsilon);
  index=99;t=-1;
  ray.origin[1]=1;
  ControlCheck(!Control(pickControl)(pts,2,2,&ray,(Scalar)0.01,&index,&t));
  ControlCheck(index==99 && t==-1);
  return EXIT_SUCCESS;
}

static int Case(controlInvalid)(void)
{
  Scalar pts[2][3]={{1,2,3},{4,5,6}};
  Scalar dst[2][3]={{99,99,99},{99,99,99}};
  ControlAABB hull;
  ControlRay ray={{0,0,0},{0,0,0}};
  ControlCheck(!Control(computeAABB)((const Scalar(*)[3])pts,0,2,&hull));
  ControlCheck(!Control(normalize)((const Scalar(*)[3])pts,0,2,dst,2));
  ControlCheck(!Control(normalize)((const Scalar(*)[3])pts,(Arcadia_SizeValue)-1,2,dst,2));
  ControlCheck(!Control(transform)((const Scalar(*)[3])pts,1,2,NULL,dst,2));
  Arcadia_SizeValue index=99;
  ControlCheck(!Control(pickControl)((const Scalar(*)[3])pts,1,2,&ray,1,&index,NULL));
  ControlCheck(index==99 && dst[0][0]==99);
  pts[1][0]=(Scalar)NAN;
  ControlCheck(!Control(computeAABB)((const Scalar(*)[3])pts,1,2,&hull));
  ControlCheck(!Control(normalize)((const Scalar(*)[3])pts,1,2,dst,2));
  ControlCheck(dst[0][0]==99);
  return EXIT_SUCCESS;
}
#undef ControlCheck
