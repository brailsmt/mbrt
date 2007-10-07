
#include "triangle.h"

Ray Triangle::get_normal(const Point3D &p) 
{
    return Ray(p, m_normal_direction);
}

bool Triangle::collides_with(const Ray &ray, double &t) const
{
    return false;
}
