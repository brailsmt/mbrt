#include "material.h"

Color * Material::get_color(const Point3D& intersection_point) const 
{ 
#if 0
    static Color col(1.0,1.0,1.0);
    int bit = 1;
    bit *= ( ( (int)(intersection_point.x) ) ) % 2 == 1 ? 1 : -1; 
    bit *= ( ( (int)(intersection_point.y) ) ) % 2 == 1 ? 1 : -1; 
    bit *= ( ( (int)(intersection_point.z) ) ) % 2 == 1 ? 1 : -1; 
    if(bit>0)
    {
#endif
        return m_color; 
#if 0
    }
    else
    {
        return &col;
    }
#endif

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
