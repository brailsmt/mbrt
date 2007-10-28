#include <Magick++.h>
#include "solidmaterial.h"
#include "scene.h"
#include <string>
#include <map>

using Magick::Color;

SolidMaterial::SolidMaterialStaticInit SolidMaterial::m_init;

Color * SolidMaterial::get_color(const Point3D& intersection_point) const 
{ 
        return m_color; 
}

bool SolidMaterial::is_light(const Point3D& intersection_point) const 
{ 
    return m_is_light_source; 
}
void SolidMaterial::set_is_light(bool v) 
{ 
    m_is_light_source = v; 
}

double SolidMaterial::get_diffuse(const Point3D& intersection_point) const 
{ 
    return m_diffusion_factor; 
}

void SolidMaterial::set_diffuse(double diffuse) 
{ 
    m_diffusion_factor = diffuse; 
}

double SolidMaterial::get_reflection(const Point3D& intersection_point) const 
{ 
    return m_reflection_coefficient; 
}

double SolidMaterial::get_reflectivity(const Point3D& intersection_point) const 
{ 
    return m_reflectivity; 
}

double SolidMaterial::get_refraction_index(const Point3D& intersection_point) const 
{ 
    return m_refraction_index; 
}

double SolidMaterial::get_opacity(const Point3D& intersection_point) const 
{ 
    return m_opacity; 
}

void SolidMaterial::set_opacity(double opacity) 
{
    m_opacity = opacity; normalize_opacity(); 
}
