// Shared native affine transform regression cases (Real32 and Real64).
#define AFFCHECK(c) do { if (!(c)) return EXIT_FAILURE; } while (0)
#define AFFNEAR(a,b) (fabs((double)(a)-(double)(b)) < 0.0001)

static int
Test(affineIdentityAndComposition)
  (
    void
  )
{
  AFFINE a, b, c;
  F(setIdentity)(&a);
  const Scalar translated[12] = {
    1,0,0,3, 0,1,0,-2, 0,0,1,5
  };
  AFFCHECK(F(setValues)(&b, translated));
  AFFCHECK(F(multiply)(&c, &a, &b));
  Scalar input[3] = {1,2,3};
  AFFCHECK(F(transformPoint)(&c, input, input));
  AFFCHECK(input[0]==4 && input[1]==0 && input[2]==8);
  Scalar direction[3] = {1,2,3};
  AFFCHECK(F(transformDirection)(&c, direction, direction));
  AFFCHECK(direction[0]==1 && direction[1]==2 && direction[2]==3);
  AFFCHECK(F(multiply)(&b, &b, &a));
  AFFCHECK(b.elements[0][3] == 3);
  return EXIT_SUCCESS;
}

static int
Test(affineInverseAndPlane)
  (
    void
  )
{
  const Scalar values[12] = {
    0,-2,0,3, 4,0,0,-1, 0,0,-3,7
  };
  AFFINE matrix, inverse, identity;
  AFFCHECK(F(setValues)(&matrix, values));
  AFFCHECK(F(inverse)(&inverse, &matrix));
  AFFCHECK(F(multiply)(&identity, &matrix, &inverse));
  for (int i=0;i<3;++i) for (int j=0;j<4;++j)
    AFFCHECK(AFFNEAR(identity.elements[i][j], i==j ? 1 : 0));
  PLANE plane, transformed;
  const Scalar origin[3]={0,0,2}, normal[3]={0,0,2};
  AFFCHECK(P(setFromPointNormal)(&plane, origin, normal));
  AFFCHECK(F(transformPlane)(&matrix, &plane, &transformed));
  const Scalar onOld[3]={1,1,2};
  Scalar onNew[3];
  AFFCHECK(F(transformPoint)(&matrix,onOld,onNew));
  AFFCHECK(AFFNEAR(P(signedOffset)(&transformed,onNew),0));
  // Plane transformation is valid for reflection and non-uniform scaling.
  AFFCHECK(F(transformPlane)(&inverse,&transformed,&transformed));
  AFFCHECK(AFFNEAR(transformed.normal[2],plane.normal[2]));
  AFFCHECK(AFFNEAR(transformed.offset,plane.offset));
  return EXIT_SUCCESS;
}

static int
Test(affineRayAndAABB)
  (
    void
  )
{
  const Scalar values[12] = {0,-2,0,3, 4,0,0,-1, 0,0,-3,7};
  AFFINE matrix;
  AFFCHECK(F(setValues)(&matrix,values));
  RAY ray, transformed;
  Scalar origin[3]={1,2,3},direction[3]={0,0,1};
  AFFCHECK(R(set)(&ray,origin,direction));
  AFFCHECK(F(transformRay)(&matrix,&ray,&transformed));
  AFFCHECK(transformed.origin[0]==-1 && transformed.origin[1]==3 && transformed.origin[2]==-2);
  AFFCHECK(transformed.direction[0]==0 && transformed.direction[1]==0 && transformed.direction[2]==-3);
  AABB bounds, next;
  Scalar minimum[3]={-1,-2,-3}, maximum[3]={1,2,3};
  A(setMinMax)(&bounds,minimum,maximum);
  AFFCHECK(F(transformAABB)(&matrix,&bounds,&next));
  AFFCHECK(next.minimum[0]==-1 && next.maximum[0]==7);
  AFFCHECK(next.minimum[1]==-5 && next.maximum[1]==3);
  AFFCHECK(next.minimum[2]==-2 && next.maximum[2]==16);
  AFFCHECK(F(transformAABB)(&matrix,&bounds,&bounds));
  AFFCHECK(bounds.minimum[0]==next.minimum[0]);
  A(setEmpty)(&bounds);
  AFFCHECK(F(transformAABB)(&matrix,&bounds,&next));
  AFFCHECK(next.empty);
  return EXIT_SUCCESS;
}

static int
Test(affineInvalidAndAliasing)
  (
    void
  )
{
  AFFINE matrix, unchanged;
  F(setIdentity)(&matrix);
  const Scalar nonfinite[12] = {1,0,0,0, 0,1,0,0, 0,0,(Scalar)NAN,0};
  AFFCHECK(!F(setValues)(&matrix,nonfinite));
  AFFCHECK(matrix.elements[2][2]==1);
  const Scalar singular[12] = {1,0,0,0, 0,0,0,0, 0,0,1,0};
  AFFCHECK(F(setValues)(&matrix,singular));
  F(setIdentity)(&unchanged);
  AFFCHECK(!F(inverse)(&unchanged,&matrix));
  AFFCHECK(unchanged.elements[1][1]==1);
  Scalar point[3]={1,2,3};
  AFFCHECK(!F(transformPoint)(&matrix,(const Scalar[3]){1,(Scalar)NAN,3},point));
  AFFCHECK(point[0]==1 && point[1]==2 && point[2]==3);
  PLANE plane;
  const Scalar p[3]={0,0,0},normal[3]={0,0,1};
  AFFCHECK(P(setFromPointNormal)(&plane,p,normal));
  PLANE old=plane;
  AFFCHECK(!F(transformPlane)(&matrix,&plane,&plane));
  AFFCHECK(plane.offset==old.offset && plane.normal[2]==old.normal[2]);
  return EXIT_SUCCESS;
}

#undef AFFCHECK
#undef AFFNEAR
