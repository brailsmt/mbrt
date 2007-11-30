
%{
#include "bumpmap.h"
%}

%ignore perturb_normal(const Ray& normal, const Point3D& intersection_point);

%include "bumpmap.h"
