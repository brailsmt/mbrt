
#include "triangle.h"

Ray Triangle::get_normal(const Point3D &p) 
{
    return Ray(p, m_normal_direction);
}

bool Triangle::collides_with(const Ray &ray, double &t) const
{
#if 0
    // GetIntrPt == VecAddScalMult(dist, dir, pt)
    double dot = dot_product(m_normal_direction, ray.get_direction() );

    if( (dot < 0.0001) && (dot > 0.0001) )
        return false;

    return false;
#endif
}
