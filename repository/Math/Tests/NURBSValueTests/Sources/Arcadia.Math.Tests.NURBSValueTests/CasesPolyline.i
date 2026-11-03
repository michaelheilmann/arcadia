// Polyline spatial tests included twice, with Real32 / Real64 substitutions.
#define SpatialCheck(x) do { if (!(x)) return EXIT_FAILURE; } while (0)

static int Case(polylineLengths)(void)
{
  const Scalar points[4][3]={{0,0,0},{3,4,0},{3,4,0},{6,8,0}};
  Scalar lengths[4]={-1,-1,-1,-1};
  SpatialCheck(Spatial(cumulativeLengths)(points,4,lengths,4));
  SpatialCheck(lengths[0]==0 && lengths[1]==5 && lengths[2]==5 && lengths[3]==10);
  lengths[0]=-1;
  SpatialCheck(!Spatial(cumulativeLengths)(points,4,lengths,3));
  SpatialCheck(lengths[0]==-1);
  Scalar bad[2][3]={{0,0,0},{1,0,0}};
  bad[1][0]=(Scalar)NAN;
  SpatialCheck(!Spatial(cumulativeLengths)((const Scalar(*)[3])bad,2,lengths,4));
  return EXIT_SUCCESS;
}

static int Case(polylineResample)(void)
{
  const Scalar points[4][3]={{0,0,0},{3,4,0},{3,4,0},{6,8,0}};
  const Scalar lengths[4]={0,5,5,10};
  Scalar out[5][3];
  SpatialCheck(Spatial(resampleByLength)(points,lengths,4,5,out,5));
  SpatialCheck(out[0][0]==0 && out[4][0]==6);
  SpatialCheck(fabs((double)out[1][0]-1.5)<=Epsilon);
  SpatialCheck(fabs((double)out[2][1]-4.)<=Epsilon);
  SpatialCheck(fabs((double)out[3][0]-4.5)<=Epsilon);
  out[0][0]=-13;
  SpatialCheck(!Spatial(resampleByLength)(points,lengths,4,5,out,4));
  SpatialCheck(out[0][0]==-13);
  return EXIT_SUCCESS;
}

static int Case(polylineResampleInvalid)(void)
{
  const Scalar points[3][3]={{0,0,0},{1,0,0},{2,0,0}};
  const Scalar bad[3]={0,2,1};
  const Scalar zero[3]={0,0,0};
  Scalar out[3][3];out[0][0]=99;
  SpatialCheck(!Spatial(resampleByLength)(points,bad,3,3,out,3));
  SpatialCheck(!Spatial(resampleByLength)(points,zero,3,3,out,3));
  SpatialCheck(!Spatial(resampleByLength)(points,bad,3,1,out,3));
  SpatialCheck(out[0][0]==99);
  return EXIT_SUCCESS;
}

static int Case(polylineClosest)(void)
{
  const Scalar points[4][3]={{0,0,0},{2,0,0},{2,0,0},{2,2,0}};
  Scalar query[3]={1,1,0},out[3],fraction=-1,distance=-1;
  Arcadia_SizeValue index=99;
  SpatialCheck(Spatial(closestPoint)(points,4,query,out,&index,&fraction,&distance));
  SpatialCheck(index==0 && fraction==(Scalar)0.5 && distance==1);
  SpatialCheck(out[0]==1 && out[1]==0 && out[2]==0);
  Scalar q2[3]={4,2,0};
  SpatialCheck(Spatial(closestPoint)(points,4,q2,out,&index,&fraction,&distance));
  SpatialCheck(index==2 && fraction==1 && out[0]==2 && out[1]==2 && distance==2);
  q2[0]=(Scalar)NAN;
  out[0]=17;
  SpatialCheck(!Spatial(closestPoint)(points,4,q2,out,&index,&fraction,&distance));
  SpatialCheck(out[0]==17 && index==2);
  return EXIT_SUCCESS;
}

static int Case(polylineRayPick)(void)
{
  const Scalar points[4][3]={{0,0,0},{2,0,0},{2,2,0},{4,2,0}};
  SpatialRay ray;
  ray.origin[0]=1;ray.origin[1]=0;ray.origin[2]=5;
  ray.direction[0]=0;ray.direction[1]=0;ray.direction[2]=-2;
  Arcadia_SizeValue segment=99;
  Scalar t=-1,fraction=-1;
  SpatialCheck(Spatial(pickRay)(points,4,&ray,0,&segment,&t,&fraction));
  SpatialCheck(segment==0 && fabs((double)t-2.5)<=Epsilon && fraction==(Scalar)0.5);
  ray.origin[0]=4;ray.origin[1]=2;
  SpatialCheck(Spatial(pickRay)(points,4,&ray,0,&segment,&t,&fraction));
  SpatialCheck(segment==2 && fraction==1);
  ray.origin[1]=5;
  SpatialCheck(!Spatial(pickRay)(points,4,&ray,0.1f,&segment,&t,&fraction));
  SpatialCheck(segment==2);
  return EXIT_SUCCESS;
}

static int Case(polylineRayPickInvalid)(void)
{
  const Scalar points[2][3]={{0,0,0},{1,0,0}};
  SpatialRay ray;
  ray.origin[0]=0;ray.origin[1]=0;ray.origin[2]=1;
  ray.direction[0]=0;ray.direction[1]=0;ray.direction[2]=-1;
  Arcadia_SizeValue segment=44;
  Scalar t=99;
  SpatialCheck(!Spatial(pickRay)(points,2,&ray,-1,&segment,&t,NULL));
  SpatialCheck(segment==44 && t==99);
  ray.direction[2]=0;
  SpatialCheck(!Spatial(pickRay)(points,2,&ray,1,&segment,&t,NULL));
  SpatialCheck(segment==44 && t==99);
  return EXIT_SUCCESS;
}
#undef SpatialCheck
