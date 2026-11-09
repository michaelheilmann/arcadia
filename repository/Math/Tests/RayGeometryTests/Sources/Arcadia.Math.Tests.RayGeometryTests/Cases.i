// Test cases are compiled for both Real32 and Real64.
// Scalar, R, P, S, A, and Test are configured by Main.c.

#define CHECK(condition) do { if (!(condition)) return EXIT_FAILURE; } while (0)
#define NEAR(a, b) (fabs((double)((a) - (b))) <= 0.00001)

static int
Test(rayValues)(void)
{
  RAY ray;
  Scalar origin[3] = { 1, 2, 3 }, direction[3] = { 2, 0, -1 };
  Scalar zero[3] = { 0, 0, 0 };
  CHECK(R(set)(&ray, origin, direction));
  CHECK(!R(set)(&ray, origin, zero));
  CHECK(ray.direction[0] == 2 && ray.origin[2] == 3);
  Scalar point[3];
  R(pointAt)(&ray, 2, point);
  CHECK(point[0] == 5 && point[1] == 2 && point[2] == 1);
  R(pointAt)(&ray, 1, ray.origin); // Output aliases the stored origin.
  CHECK(ray.origin[0] == 3 && ray.origin[1] == 2 && ray.origin[2] == 2);
  Scalar delta[3] = { -1, 5, 2 };
  R(translate)(&ray, delta);
  CHECK(ray.origin[0] == 2 && ray.origin[1] == 7 && ray.origin[2] == 4);
  return EXIT_SUCCESS;
}

static int
Test(planeValues)(void)
{
  PLANE plane;
  Scalar p[3] = { 0, 0, 2 }, n[3] = { 0, 0, 2 };
  CHECK(P(setFromPointNormal)(&plane, p, n));
  CHECK(plane.normal[2] == 2 && plane.offset == -4);
  CHECK(P(signedOffset)(&plane, p) == 0);
  Scalar q[3] = { 0, 0, 4 };
  CHECK(P(signedOffset)(&plane, q) == 4);
  Scalar invalid[3] = { 0, 0, 0 };
  CHECK(!P(setFromPointNormal)(&plane, p, invalid));
  CHECK(plane.offset == -4);
  Scalar a[3] = { 0, 0, 0 }, b[3] = { 1, 0, 0 }, c[3] = { 0, 1, 0 };
  CHECK(P(setFromPoints)(&plane, a, b, c));
  CHECK(plane.normal[2] == 1 && plane.offset == 0);
  CHECK(!P(setFromPoints)(&plane, a, b, b));
  CHECK(plane.normal[2] == 1 && plane.offset == 0);
  return EXIT_SUCCESS;
}

static int
Test(sphereValues)(void)
{
  SPHERE a, b;
  Scalar origin[3] = { 0, 0, 0 }, offset[3] = { 3, 0, 0 }, far[3] = { 4, 0, 0 };
  CHECK(S(set)(&a, origin, 2));
  Scalar surface[3] = { 2, 0, 0 };
  CHECK(S(containsPoint)(&a, origin) && S(containsPoint)(&a, surface));
  CHECK(!S(containsPoint)(&a, offset));
  CHECK(S(set)(&b, offset, 1));
  CHECK(S(intersectsSphere)(&a, &b)); // Touching surfaces.
  CHECK(S(set)(&b, far, 1));
  CHECK(!S(intersectsSphere)(&a, &b));
  CHECK(!S(set)(&a, far, -1));
  CHECK(a.radius == 2 && a.center[0] == 0);
  CHECK(S(set)(&a, origin, 0));
  CHECK(S(containsPoint)(&a, origin));
  return EXIT_SUCCESS;
}

static int
Test(rayPlane)(void)
{
  RAY ray;
  PLANE plane;
  Scalar p[3] = { 0, 0, 0 }, n[3] = { 0, 0, 1 };
  Scalar origin[3] = { 0, 0, 5 }, direction[3] = { 0, 0, -2 };
  CHECK(P(setFromPointNormal)(&plane, p, n));
  CHECK(R(set)(&ray, origin, direction));
  Scalar t = -100;
  CHECK(R(intersectPlane)(&ray, &plane, &t));
  CHECK(NEAR(t, 2.5));
  direction[2] = 2;
  CHECK(R(set)(&ray, origin, direction));
  CHECK(!R(intersectPlane)(&ray, &plane, &t));
  CHECK(NEAR(t, 2.5));
  direction[0] = 1; direction[2] = 0;
  CHECK(R(set)(&ray, origin, direction));
  CHECK(!R(intersectPlane)(&ray, &plane, &t));
  CHECK(NEAR(t, 2.5));
  origin[2] = 0;
  CHECK(R(set)(&ray, origin, direction));
  CHECK(R(intersectPlane)(&ray, &plane, &t));
  CHECK(t == 0);
  return EXIT_SUCCESS;
}

static int
Test(raySphere)(void)
{
  RAY ray;
  SPHERE sphere;
  Scalar center[3] = { 0, 0, 0 }, origin[3] = { -5, 0, 0 };
  Scalar direction[3] = { 2, 0, 0 };
  CHECK(S(set)(&sphere, center, 2));
  CHECK(R(set)(&ray, origin, direction));
  Scalar t = -100;
  CHECK(R(intersectSphere)(&ray, &sphere, &t) && NEAR(t, 1.5));
  origin[1] = 2; direction[0] = 1;
  CHECK(R(set)(&ray, origin, direction));
  CHECK(R(intersectSphere)(&ray, &sphere, &t) && NEAR(t, 5)); // Tangent.
  origin[1] = 3;
  CHECK(R(set)(&ray, origin, direction));
  CHECK(!R(intersectSphere)(&ray, &sphere, &t) && NEAR(t, 5));
  origin[0] = 5; origin[1] = 0;
  CHECK(R(set)(&ray, origin, direction));
  CHECK(!R(intersectSphere)(&ray, &sphere, &t) && NEAR(t, 5));
  origin[0] = 0;
  CHECK(R(set)(&ray, origin, direction));
  CHECK(R(intersectSphere)(&ray, &sphere, &t) && t == 0); // Inside.
  return EXIT_SUCCESS;
}

static int
Test(rayAABB)(void)
{
  RAY ray;
  AABB bounds;
  A(setEmpty)(&bounds);
  Scalar origin[3] = { -5, 0, 0 }, direction[3] = { 2, 0, 0 };
  CHECK(R(set)(&ray, origin, direction));
  Scalar a = -100, b = -200;
  CHECK(!R(intersectAABB)(&ray, &bounds, &a, &b));
  CHECK(a == -100 && b == -200);
  Scalar min[3] = { -1, -1, -1 }, max[3] = { 1, 1, 1 };
  A(setMinMax)(&bounds, min, max);
  CHECK(R(intersectAABB)(&ray, &bounds, &a, &b));
  CHECK(a == 2 && b == 3);
  origin[1] = 2;
  CHECK(R(set)(&ray, origin, direction));
  CHECK(!R(intersectAABB)(&ray, &bounds, &a, &b));
  CHECK(a == 2 && b == 3);
  origin[0] = 0; origin[1] = 0; direction[0] = 1;
  CHECK(R(set)(&ray, origin, direction));
  CHECK(R(intersectAABB)(&ray, &bounds, &a, &b));
  CHECK(a == 0 && b == 1); // Inside.
  origin[0] = -5; origin[1] = 1; origin[2] = 1;
  CHECK(R(set)(&ray, origin, direction));
  CHECK(R(intersectAABB)(&ray, &bounds, &a, &b));
  CHECK(a == 4 && b == 6); // Boundary hit.
  origin[0] = 5;
  CHECK(R(set)(&ray, origin, direction));
  CHECK(!R(intersectAABB)(&ray, &bounds, &a, &b));
  CHECK(a == 4 && b == 6);
  return EXIT_SUCCESS;
}

static int
Test(rayTriangle)(void)
{
  RAY ray;
  Scalar a[3] = { 0, 0, 0 }, b[3] = { 1, 0, 0 }, c[3] = { 0, 1, 0 };
  Scalar origin[3] = { 0.25, 0.25, 2 }, direction[3] = { 0, 0, -1 };
  CHECK(R(set)(&ray, origin, direction));
  Scalar t = -10, u = -20, v = -30;
  CHECK(R(intersectTriangle)(&ray, a, b, c, &t, &u, &v));
  CHECK(t == 2 && u == 0.25 && v == 0.25);
  origin[0] = 0; origin[1] = 0;
  CHECK(R(set)(&ray, origin, direction));
  CHECK(R(intersectTriangle)(&ray, a, b, c, &t, &u, &v));
  CHECK(t == 2 && u == 0 && v == 0); // Vertex boundary.
  origin[2] = -2; direction[2] = 1;
  CHECK(R(set)(&ray, origin, direction));
  CHECK(R(intersectTriangle)(&ray, a, b, c, &t, &u, &v)); // Back face.
  CHECK(t == 2 && u == 0 && v == 0);
  origin[2] = 2;
  direction[2] = 1;
  CHECK(R(set)(&ray, origin, direction));
  CHECK(!R(intersectTriangle)(&ray, a, b, c, &t, &u, &v));
  CHECK(t == 2 && u == 0 && v == 0);
  direction[0] = 1; direction[2] = 0;
  CHECK(R(set)(&ray, origin, direction));
  CHECK(!R(intersectTriangle)(&ray, a, b, c, &t, &u, &v));
  CHECK(!R(intersectTriangle)(&ray, a, b, b, &t, &u, &v));
  CHECK(t == 2 && u == 0 && v == 0);
  return EXIT_SUCCESS;
}

static int
Test(optionalOutputs)(void)
{
  RAY ray;
  PLANE plane;
  SPHERE sphere;
  AABB bounds;
  Scalar center[3] = { 0, 0, 0 }, origin[3] = { -2, 0, 0 };
  Scalar direction[3] = { 1, 0, 0 }, normal[3] = { 1, 0, 0 };
  Scalar a[3] = { 0, -1, -1 }, b[3] = { 0, 1, -1 }, c[3] = { 0, 0, 1 };
  CHECK(R(set)(&ray, origin, direction));
  CHECK(S(set)(&sphere, center, 1));
  CHECK(P(setFromPointNormal)(&plane, center, normal));
  Scalar min[3] = { -1, -1, -1 }, max[3] = { 1, 1, 1 };
  A(setMinMax)(&bounds, min, max);
  CHECK(R(intersectPlane)(&ray, &plane, NULL));
  CHECK(R(intersectSphere)(&ray, &sphere, NULL));
  CHECK(R(intersectAABB)(&ray, &bounds, NULL, NULL));
  CHECK(R(intersectTriangle)(&ray, a, b, c, NULL, NULL, NULL));
  return EXIT_SUCCESS;
}


// Nyx M0_Plane_Distance and M0_Plane_GetPoint adapted to existing native planes.
static int
Test(planeMetricAndProjection)(void)
{
  PLANE plane;
  const Scalar point[3] = { 0, 0, 2 };
  const Scalar normal[3] = { 0, 0, 2 };
  const Scalar probe[3] = { 4, 8, 7 };
  CHECK(P(setFromPointNormal)(&plane, point, normal));
  Scalar distance = (Scalar)-99;
  CHECK(P(signedDistance)(&plane, probe, &distance));
  CHECK(NEAR(distance, 5));
  Scalar onPlane[3] = { 99, 99, 99 };
  CHECK(P(pointOnPlane)(&plane, onPlane));
  CHECK(NEAR(onPlane[0], 0) && NEAR(onPlane[1], 0) && NEAR(onPlane[2], 2));
  Scalar projection[3] = { 0, 0, 0 };
  CHECK(P(projectPoint)(&plane, probe, projection));
  CHECK(NEAR(projection[0], 4) && NEAR(projection[1], 8) && NEAR(projection[2], 2));
  CHECK(P(projectPoint)(&plane, projection, projection)); // Safe aliasing.
  CHECK(NEAR(projection[2], 2));
  CHECK(P(signedDistance)(&plane, projection, &distance));
  CHECK(NEAR(distance, 0));
  return EXIT_SUCCESS;
}

static int
Test(planeTranslateChecked)(void)
{
  PLANE plane;
  const Scalar point[3] = { 0, 0, 2 };
  const Scalar normal[3] = { 0, 0, 2 };
  CHECK(P(setFromPointNormal)(&plane, point, normal));
  const Scalar movement[3] = { 1, 3, -5 };
  CHECK(P(translateChecked)(&plane, movement));
  CHECK(NEAR(plane.offset, 6));
  Scalar onPlane[3];
  CHECK(P(pointOnPlane)(&plane, onPlane));
  CHECK(NEAR(onPlane[2], -3));
  const Scalar reversal[3] = { -1, -3, 5 };
  CHECK(P(translateChecked)(&plane, reversal));
  CHECK(NEAR(plane.offset, -4));
  return EXIT_SUCCESS;
}

static int
Test(planeMetricInvalid)(void)
{
  PLANE plane;
  const Scalar point[3] = { 0, 0, 2 };
  const Scalar normal[3] = { 0, 0, 2 };
  CHECK(P(setFromPointNormal)(&plane, point, normal));
  Scalar output[3] = { 11, 12, 13 }, distance = 7;
  const Scalar invalid[3] = { 0, 0, (Scalar)NAN };
  CHECK(!P(projectPoint)(&plane, invalid, output));
  CHECK(!P(signedDistance)(&plane, invalid, &distance));
  CHECK(!P(translateChecked)(&plane, invalid));
  CHECK(output[0] == 11 && output[1] == 12 && output[2] == 13);
  CHECK(distance == 7 && plane.offset == -4);
  plane.normal[0] = 0; plane.normal[1] = 0; plane.normal[2] = 0;
  CHECK(!P(pointOnPlane)(&plane, output));
  CHECK(!P(projectPoint)(&plane, point, output));
  CHECK(!P(signedDistance)(&plane, point, &distance));
  CHECK(output[0] == 11 && output[1] == 12 && output[2] == 13);
  CHECK(distance == 7);
  return EXIT_SUCCESS;
}

#undef CHECK
#undef NEAR
