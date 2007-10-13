/* file name  : staticinit.cpp
 * author     : Michael Brailsford
 * created    : Sun Oct 07 01:35:17 -0500 2007
 * copyright  : 2007 Michael Brailsford
 */

#include "plane.h"
#include "scene.h"

// This actually makes the static init work.
Plane::StaticInit Plane::m_init;

Plane::Plane(Point3D p1, Point3D p2, Point3D p3, std::string color, std::string material_name, std::string bumpmap_name) 
    : m_point(p1)
{
    m_normal = Ray(p1, cross_product((p2 - p1), (p3 - p1)));
    m_d = dot_product(m_point, m_normal.direction());

    Scene * scene = Scene::get_instance();
    m_material = scene->get_material(material_name);

    m_bumpmap = scene->get_bumpmap(bumpmap_name);
}

bool Plane::collides_with(const Ray &ray, double &t) const {
    Point3D orig = ray.origin();
    Vector dir = ray.direction();

    double denominator = dot_product(dir, m_normal.direction());
    if(denominator > -0.0001 && denominator < 0.0001) {
        return false;
    }

    double numerator = m_d - dot_product(orig, m_normal.direction());
    if(numerator > -0.0001 && numerator < 0.0001) {
        return false;
    }

    t = numerator / denominator;
    
    return true;
}

Ray Plane::get_normal(const Point3D &p) {
    return Ray(p, m_normal.direction() );
};
