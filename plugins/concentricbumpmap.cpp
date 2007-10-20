
#include "concentricbumpmap.h"


// This actually makes the static init work.
ConcentricBumpMap::StaticInit ConcentricBumpMap::m_init;

Ray ConcentricBumpMap::perturb_normal(const Ray& normal, const Point3D& intersection_point)
{
    static Point3D zero;

    double len = zero.distance_to(intersection_point);
    if(len == 0.0) 
    {
        len = 0.00001;
    }

    Vector dir(normal.direction() );
    double offset = sin(len * 2.0  ) / (len/2.5);

    dir.x += offset;
    dir.y += offset;
    // dir.z += 0.0;

    return Ray(normal.origin(), dir);    
}

