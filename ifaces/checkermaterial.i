
%{
#include "checkermaterial.h"
%}

%ignore new_checker_material(std::map<std::string, std::string> props);
%ignore delete_checkermaterial(Material * checker_material);
%ignore CheckerMaterial::choose_material(const Point3D& intersesction_point) const;
%ignore CheckerMaterial::get_color(const Point3D& intersesction_point) const;

%include "checkermaterial.h"
