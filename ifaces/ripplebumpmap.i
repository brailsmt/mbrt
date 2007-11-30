
%{
#include "ripplebumpmap.h"
%}

%ignore new_ripplebumpmap(xmlNode * node);
%ignore delete_ripplebumpmap(RippleBumpMap * bumpmap);

%ignore perturb_normal(const Ray& normal, const Point3D& intersection_point);

%include "sphere.h"
