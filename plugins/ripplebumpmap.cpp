
#include "ripplebumpmap.h"

// This actually makes the static init work.
RippleBumpMap::StaticInit RippleBumpMap::m_init;

Ray RippleBumpMap::perturb_normal(const Ray& normal, const Point3D& intersection_point)
{
    Vector dir(normal.direction() );

    dir.x += sin(intersection_point.x * 10.0  ) / 10.0;
    dir.y += cos(intersection_point.y * 10.0  ) / 10.0;
    dir.z += cos(intersection_point.z * 10.0  ) / 10.0;

    return Ray(normal.origin(), dir);    
}

