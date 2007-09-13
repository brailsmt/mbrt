#include "checkermaterial.h"

int CheckeredMaterial::choose_material(const Point3D& intersection_point) const
{
    int bit = 1;
    bit *= ( ( (int)(intersection_point.x) ) ) % 2 == 1 ? 1 : -1; 
    bit *= ( ( (int)(intersection_point.y) ) ) % 2 == 1 ? 1 : -1; 
    bit *= ( ( (int)(intersection_point.z) ) ) % 2 == 1 ? 1 : -1; 
    return bit;
}

Color * CheckeredMaterial::get_color(const Point3D& intersection_point) const 
{ 
    if(choose_material(intersection_point) > 0)
        return m_material_one->get_color(intersection_point); 
    else
        return m_material_two->get_color(intersection_point); 
}

bool CheckeredMaterial::is_light(const Point3D& intersection_point) const 
{ 
    return false; 
}
void CheckeredMaterial::set_is_light(bool v) 
{ 
    return;
    //m_is_light_source = v; 
}

double CheckeredMaterial::get_diffuse(const Point3D& intersection_point) const 
{ 
    if(choose_material(intersection_point) > 0)
        return m_material_one->get_diffuse(intersection_point); 
    else
        return m_material_two->get_diffuse(intersection_point); 
}

void CheckeredMaterial::set_diffuse(double diffuse) 
{ 
    m_material_one->set_diffuse(diffuse);
    m_material_two->set_diffuse(diffuse);

}

double CheckeredMaterial::get_reflection(const Point3D& intersection_point) const 
{ 
    if(choose_material(intersection_point) > 0)
        return m_material_one->get_reflection(intersection_point); 
    else
        return m_material_two->get_reflection(intersection_point); 
}

double CheckeredMaterial::get_reflectivity(const Point3D& intersection_point) const 
{ 
    if(choose_material(intersection_point) > 0)
        return m_material_one->get_reflectivity(intersection_point); 
    else
        return m_material_two->get_reflectivity(intersection_point); 
}

double CheckeredMaterial::get_refraction_index(const Point3D& intersection_point) const 
{ 
    if(choose_material(intersection_point) > 0)
        return m_material_one->get_refraction_index(intersection_point); 
    else
        return m_material_two->get_refraction_index(intersection_point); 
}

double CheckeredMaterial::get_opacity(const Point3D& intersection_point) const 
{ 
    if(choose_material(intersection_point) > 0)
        return m_material_one->get_opacity(intersection_point); 
    else
        return m_material_two->get_opacity(intersection_point); 
}

void CheckeredMaterial::set_opacity(double opacity) 
{
    m_material_one->set_opacity(opacity);
    m_material_two->set_opacity(opacity);
}
