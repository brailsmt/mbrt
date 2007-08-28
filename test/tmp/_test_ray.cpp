#include "../raytrace_defs.h"
#include "../ray.h"
#include "cxxtest/TestSuite.h"
#include <vector>
#include <string>
#include <sstream>

using namespace std;

#define MAX_COORD 10
#define THRESH 0.000001

class RayTest : public CxxTest::TestSuite {
    public:
        vector<Point3D> points;

        void setUp();
        void tearDown();

        void test_creation();
};

//{{{
void RayTest::setUp() {
    for(int x = 0; x < MAX_COORD; ++x) {
        for(int y = 0; y < MAX_COORD; ++y) {
            for(int z = 0; z < MAX_COORD; ++z) {
                points.push_back(Point3D((float)x, (float)y, (float)z));
            }
        }
    }
}
//}}}
//{{{
void RayTest::tearDown() {
    points.clear();
}
//}}}
//{{{
void RayTest::test_creation() {
    Ray r(points.front(), points.back());

    TS_ASSERT(points.front() == r.origin());
    TS_ASSERT(points.back() != r.direction());
}
//}}}
