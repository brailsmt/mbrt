#include "material.h"

Color * Material::get_color(const Point3D& intersection_point) const 
{ 
        return m_color; 
}

bool Material::is_light(const Point3D& intersection_point) const 
{ 
    return m_is_light_source; 
}
void Material::set_is_light(bool v) 
{ 
    m_is_light_source = v; 
}

double Material::get_diffuse(const Point3D& intersection_point) const 
{ 
    return m_diffusion_factor; 
}

void Material::set_diffuse(double diffuse) 
{ 
    m_diffusion_factor = diffuse; 
}

double Material::get_reflection(const Point3D& intersection_point) const 
{ 
    return m_reflection_coefficient; 
}

double Material::get_reflectivity(const Point3D& intersection_point) const 
{ 
    return m_reflectivity; 
}

double Material::get_refraction_index(const Point3D& intersection_point) const 
{ 
    return m_refraction_index; 
}

double Material::get_opacity(const Point3D& intersection_point) const 
{ 
    return m_opacity; 
}

void Material::set_opacity(double opacity) 
{
    m_opacity = opacity; normalize_opacity(); 
}
