
#ifndef RAY_H
#define RAY_H

#include "raytrace_defs.h"

#include <iostream>
#include <sstream>

using std::cout;
using std::endl;

/// Classic ray, with an origin and direction.
//{{{
class Ray {
    protected:
        bool _is_normalized;

        Point3D _origin;
        Vector _direction;

    public:
        Ray() 
            : _origin(Point3D()),
              _direction(Vector()),
              _is_normalized(false)
        {
        }

        Ray(Point3D origin, Point3D direction) 
            : _origin(origin),
              _direction(direction),
              _is_normalized(false)
        {
            Ray::normalize();
        }

        Ray(const Ray &other) 
            : _origin(other._origin),
              _direction(other._direction),
              _is_normalized(other._is_normalized)
        {
            if (!_is_normalized) {
                Ray::normalize();
            }
        }

        ~Ray() {}

        /// Normalize the direction vector with the origin.
        inline void normalize();

        bool is_normalized() { return _is_normalized; }

        Point3D origin() const { return _origin; }
        const Vector &direction() const { return _direction; }

        std::string to_string() {
            std::ostringstream rv;
            rv << "origin = " << _origin.to_string() << ", direction = " << _direction.to_string();
            return rv.str();
        }
};
//}}}

//{{{
void Ray::normalize() {
    float norm = sqrtf(_direction.x*_direction.x + _direction.y*_direction.y + _direction.z*_direction.z);
    _direction.x /= norm;
    _direction.y /= norm;
    _direction.z /= norm;
}
//}}}
//{{{
inline bool operator==(const Ray &r1, const Ray &r2) {
    return r1.origin() == r2.origin() && r1.direction() == r2.direction();
}
//}}}

//{{{
inline bool operator!=(const Ray &r1, const Ray &r2) {
    return !(r1 == r2);
}
//}}}
#endif