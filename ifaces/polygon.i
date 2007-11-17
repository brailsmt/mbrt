
%{
#include "polygon.h"
%}

%ignore new_polygon(xmlNode * node);
%ignore delete_polygon(Polygon * sphere);
%ignore Polygon::collides_with(const Ray &ray, double &t) const;
%ignore Polygon::get_normal(const Point3D &p) const;

%include "polygon.h"
