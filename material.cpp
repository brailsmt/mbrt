#include "material.h"

bool Material::is_light() const 
{ 
    return m_is_light_source; 
}
void Material::set_is_light(bool v) 
{ 
    m_is_light_source = v; 
}

double Material::get_diffuse() const 
{ 
    return m_diffusion_factor; 
}

void Material::set_diffuse(double diffuse) 
{ 
    m_diffusion_factor = diffuse; 
}

double Material::get_reflection() const 
{ 
    return m_reflection_coefficient; 
}

double Material::get_reflectivity() const 
{ 
    return m_reflectivity; 
}

double Material::get_refraction_index() const 
{ 
    return m_refraction_index; 
}

double Material::get_opacity() const 
{ 
    return m_opacity; 
}

void Material::set_opacity(double opacity) 
{
    m_opacity = opacity; normalize_opacity(); 
}
