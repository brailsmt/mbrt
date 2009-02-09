
%{
#include "renderable.h"
#include "sphere.h"
%}

%ignore new_sphere(xmlNode * node);
%ignore delete_sphere(Sphere * sphere);
%ignore Sphere::collides_with(const Ray &ray, double &t) const;
%ignore Sphere::get_normal(const Point3D &p) const;

%include "renderable.h"
%include "sphere.h"
