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

    double b = 2 * dot_product(v, ray.direction());
    double b_squared = b * b;
    double _ac = 4 * (dot_product(v, v) - m_radius * m_radius);

    if ( b_squared > _ac ) {

        double discriminant = sqrtf( b_squared - _ac );
        double t2 = (-b + discriminant) * 0.5;

        if ( t2 > 0.0 ) {
            double t1 = (-b - discriminant) * 0.5;
            if ( t1 < t ) {
                t = t1;
                rv = true;
            }
        }
    }

    return rv;
}
//}}}
//{{{
Ray Sphere::get_normal(const Point3D &p) const {
    return Ray(p, p - m_center);
}
//}}}
