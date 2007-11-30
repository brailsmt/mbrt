
%{
#include "imagemap.h"
%}

%ignore new_imagemap_material(std::map<std::string, std::string> props);
%ignore ImageMap::get_color(const Point3D& intersesction_point) const;
%ignore ImageMap::get_color(const Point3D& intersesction_point, const Renderable * const obj) const;

%include "imagemap.h"
