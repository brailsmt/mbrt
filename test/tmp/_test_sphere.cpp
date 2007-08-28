#include "../raytrace_defs.h"
#include "../sphere.h"
#include "cxxtest/TestSuite.h"
#include <vector>
#include <string>
#include <sstream>

class SphereTest : public CxxTest::TestSuite {
    public:
        Sphere * test_sphere;
        Ray ray_hit;
        Ray ray_miss;

        void setUp();
        void tearDown();

        void test_collision_hit();
        void test_collision_miss();
};

//{{{
void SphereTest::setUp() {
    test_sphere = new Sphere(0, 0, 6, 1);
    ray_hit     = Ray   (Point3D(0, 0, 0), Point3D(0, 0, 1));
    ray_miss    = Ray   (Point3D(0, 0, 0), Point3D(0, 1, 0));
}
//}}}
//{{{
void SphereTest::tearDown() {
    delete test_sphere;
}
//}}}

//{{{
void SphereTest::test_collision_hit() {
    float t1 =  1000000000;
    TS_ASSERT(test_sphere->collides_with(ray_hit, t1));
}
//}}}
//{{{
void SphereTest::test_collision_miss() {
    float t = 0;
    TS_ASSERT(!test_sphere->collides_with(ray_miss, t));
    TS_ASSERT(!test_sphere->collides_with(ray_hit , t));
}
//}}}
