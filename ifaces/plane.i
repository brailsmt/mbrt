
%{
#include "plane.h"
%}

%ignore new_plane(xmlNode * node);
%ignore delete_plane(Plane * plane);
%ignore Plane::collides_with(const Ray &ray, double &t) const;
%ignore Plane::get_normal(const Point3D &p) const;

%include "plane.h"
