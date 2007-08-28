#include "../raytrace_defs.cpp"
#include "../point3d.h"
#include "cxxtest/TestSuite.h"
#include <vector>
#include <string>
#include <sstream>

using namespace std;

#define MAX_COORD 10
#define THRESH 0.000001

class Point3dTest : public CxxTest::TestSuite {
    public:
        vector<Point3D> points;

        void setUp();
        void tearDown();

        void test_distance_to();
        void test_normalize();

        void test_dot_product();
        void test_operator_plus();
        void test_operator_minus();
};

//{{{
void Point3dTest::setUp() {
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
void Point3dTest::tearDown() {
}
//}}}
//{{{
void Point3dTest::test_distance_to() {
    vector<Point3D>::iterator iter, end;
    Point3D p0(0,0,0);
    int i = 0;

    for(iter = points.begin(), end = points.end(); iter != end; ++iter) {
        ostringstream msg;
        float distance = sqrtf(powf((iter->x - p0.x), 2.0f) + powf((iter->y - p0.y), 2.0f) + powf((iter->z - p0.z), 2.0f));

        float d = p0.distance_to(*iter);
        msg << "(" << iter->x << ", " << iter->y << ", " << iter->z << ")" << endl;
        msg << "(" << p0.x << ", " << p0.y << ", " << p0.z << ")" << endl;
        msg << "distance       = " << distance << endl;
        msg << "d = " << d << endl;
        TSM_ASSERT(msg.str(), d >= distance - THRESH && d <= distance + THRESH);

        //swap to ensure commutativity
        d = iter->distance_to(p0);
        TSM_ASSERT(msg.str(), d >= distance - THRESH && d <= distance + THRESH);

        //cout << msg.str() << endl;

        ++i;
        if( i%10 == 0 )
            p0 = *iter;
    }
}
//}}}
//{{{
void Point3dTest::test_normalize() {
    vector<Point3D>::iterator iter, end;
    Point3D p0(0,0,0), r(0,0,0);

    for(iter = points.begin(), end = points.end(); iter != end; ++iter) {
        r = normalize(p0, *iter);
        ostringstream msg;
        msg << "p0   = (" << p0.x    << ", " << p0.y    << ", " << p0.z    << ")" << endl;
        msg << "iter = (" << iter->x << ", " << iter->y << ", " << iter->z << ")" << endl;
        msg << "r    = (" << r.x     << ", " << r.y     << ", " << r.z     << ")" << endl;

        if( r.x > 0.0f && (r.x != 1.0f && r.y != 0.0f && r.z != 0.0f))
            TSM_ASSERT(msg.str(), r.x != p0.x && r.x != iter->x);

        if( r.y > 0.0f && (r.y != 1.0f && r.x != 0.0f && r.z != 0.0f))
            TSM_ASSERT(msg.str(), r.y != p0.y && r.y != iter->y);

        if( r.z > 0.0f && (r.z != 1.0f && r.x != 0.0f && r.y != 0.0f))
            TSM_ASSERT(msg.str(), r.z != p0.z && r.z != iter->z);
    }

}
//}}}
//{{{
void Point3dTest::test_dot_product() {
    vector<Point3D>::iterator iter, end;
    Point3D p0(0,0,0);
    
    for(iter = points.begin(), end = points.end(); iter != end; ++iter) {
        float dp = (iter->x * p0.x) + (iter->y * p0.y) + (iter->z * p0.z);
        TS_ASSERT(dp == dot_product(p0, *iter));
        p0 = *iter;
    }
}
//}}}
//{{{
void Point3dTest::test_operator_plus() {
    vector<Point3D>::iterator iter, end;
    Point3D p0(0,0,0);
    
    for(iter = points.begin(), end = points.end(); iter != end; ++iter) {
        Point3D p(p0.x + iter->x, p0.y + iter->y, p0.z + iter->z);
        Point3D result = p0 + *iter;

        TS_ASSERT(result == p);

        p0 = *iter;
    }
}
//}}}
//{{{
void Point3dTest::test_operator_minus() {
    vector<Point3D>::iterator iter, end;
    Point3D p0(0,0,0);
    
    for(iter = points.begin(), end = points.end(); iter != end; ++iter) {
        Point3D p(p0.x - iter->x, p0.y - iter->y, p0.z - iter->z);
        Point3D result = p0 - *iter;

        TS_ASSERT(result == p);

        p0 = *iter;
    }
}
//}}}
