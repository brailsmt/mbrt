/// @file sphere.cpp
/// @author Michael Brailsford
/// @date Tue Mar 27 19:57:23 -0500 2007
/// &copy; 2007 Michael Brailsford

#include "sphere.h"
#include <vector>

using std::vector;
using std::string;

// This actually makes the static init work.
Sphere::StaticInit Sphere::m_init;

//{{{
Sphere::~Sphere() {}
//}}}

//{{{
bool Sphere::collides_with(const Ray &ray, double &t) const {
    bool rv = false;
    Point3D v = ray.origin() - m_center;
    Vector d = ray.direction();
    double d_dot_d = dot_product(d, d);

    double b = dot_product(v, d);
    double b_squared = b * b;
    double _ac = (d_dot_d) * (dot_product(v, v) - m_radius * m_radius);

    if ( b_squared > _ac ) {

        double tmp = sqrtf( b_squared - _ac );
        double t2 = -b + tmp;

        if ( t2 > -0.00001 ) {
            double t1 = -b - tmp;
            if ( t1 < t ) {
                t = t1 / d_dot_d;
                rv = true;
            }
        }
    }

    return rv;
}
//}}}
//{{{
Ray Sphere::get_normal(const Point3D &p) {
    return Ray(p, p - m_center);
}
//}}}
