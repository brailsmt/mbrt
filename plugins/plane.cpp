/* file name  : staticinit.cpp
 * author     : Michael Brailsford
 * created    : Sun Oct 07 01:35:17 -0500 2007
 * copyright  : 2007 Michael Brailsford
 */

#include "plane.h"

// This actually makes the static init work.
Plane::StaticInit Plane::m_init;

Plane::Plane(Point3D p1, Point3D p2, Point3D p3, std::string color, std::string material_name) 
    : m_A(0.0), m_B(0.0), m_C(0.0), m_D(0.0)
{
    double m_A = p1.y * (p2.z - p3.z) + p2.y* (p3.z - p1.z) + p3.y*(p1.z - p2.z);
    double m_B = p1.z * (p2.x - p3.x) + p2.z* (p3.x - p1.x) + p3.z*(p1.x - p2.x);
    double m_C = p1.x * (p2.y - p3.y) + p2.x* (p3.y - p1.y) + p3.x*(p1.y - p2.y);
    double m_D = -(p1.x * (p2.y * p3.z - p3.y * p2.z) + p2.x * (p3.y * p1.z - p1.y * p3.z) + p3.x * (p1.y * p2.z - p2.y * p1.z));

    m_material = Scene::get_instance()->get_material(material_name);
}

bool Plane::collides_with(const Ray &ray, double &t) const {
    Point3D orig = ray.origin();
    Vector dir = ray.direction();
    double vd = (m_A * dir.x) + (m_B * dir.y) + (m_C * dir.z);
    if(vd > -0.00001 && vd < 0.00001) {
        return false;
    }

    double v0 = (m_A * orig.x) + (m_B * orig.y) + (m_C * orig.z) + m_D;
    if( (vd < 0 && v0 > 0) || (v0 < 0 && vd > 0)) {
        return false;
    }

    t = v0/vd;
    return true;
}
