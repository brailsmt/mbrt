
%{
#include "concentricbumpmap.h"
%}

%ignore new_concentricbumpmap(xmlNode * node);
%ignore delete_concentricbumpmap(ConcentricBumpMap * bumpmap);

%ignore perturb_normal(const Ray& normal, const Point3D& intersection_point);

%include "concentricbumpmap.h"
