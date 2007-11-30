
%{
#include "noisematerial.h"
%}

%ignore make_noise_material(std::map<std::string, std::string> props);
%ignore NoiseMaterial::createNoiseMaterial(std::map<std::string, std::string> props);
%ignore NoiseMaterial::choose_material(const Point3D& intersesction_point) const;
%ignore NoiseMaterial::get_color(const Point3D& intersesction_point) const;

%include "noisematerial.h"
