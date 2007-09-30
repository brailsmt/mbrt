#include "checkermaterial.h"
#include <iostream>

CheckeredMaterial::StaticInit CheckeredMaterial::m_init;

int CheckeredMaterial::choose_material(const Point3D& intersection_point) const
{
    int bit = ( ( ( (int)(intersection_point.x * m_scale) ) ) % 2 ) == 0 ? 1 : -1; 
    bit *= ( ( ( (int)(intersection_point.y * m_scale) ) ) % 2 ) == 0 ? 1 : -1; 
    bit *= ( ( ( (int)(intersection_point.z * m_scale) ) ) % 2 )== 0 ? 1 : -1; 
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


Material * CheckeredMaterial::createCheckeredMaterial(std::map<std::string, std::string> props)
{
    Scene * scene = Scene::get_instance();
    Material * one = scene->get_material(props["material1"]);
    Material * two = scene->get_material(props["material2"]);

    // TODO: this is done so many times it really needs a helper function
    double scale = props.count("scale") > 0 ?
        (double)strtod(props["scale"].c_str(), NULL)       :  1.0;

    return new CheckeredMaterial(one, two, scale);

}

