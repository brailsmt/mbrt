
%{
#include "marblematerial.h"
%}

%ignore make_marble_material(std::map<std::string, std::string> props);
%ignore MarbleMaterial::interpolate_value(double noise_amount, double one, double two) const;
%ignore MarbleMaterial::choose_materials(double noise_amount, Material ** one, Material ** two) const;
%ignore MarbleMaterial::get_color(const Point3D& intersesction_point) const;

%include "marblematerial.h"
