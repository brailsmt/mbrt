#include "primitive.h"

//{{{
float Primitive::get_diffuse() const {
    float diffuse = m_material->get_diffuse();
    return (diffuse <= 0.0f)? 0.0f:
           (diffuse >= 1.0f)? 1.0f:
                                 diffuse;
}
//}}}

//{{{
float Primitive::get_reflection() const {
    float reflection = m_material->get_reflection();
    return (reflection <= 0.0f)? 0.0f:
           (reflection >= 1.0f)? 1.0f:
                                 reflection;
}
//}}}
//{{{
float Primitive::get_reflectivity() const {
    return m_material->get_reflectivity();
}
//}}}

float Primitive::get_specular() const 
{ 
    // Really? 
    return 1.0f; 
}

float Primitive::get_opacity() const 
{ 
    return m_material->get_opacity(); 
}

Color Primitive::get_color() const 
{ 
    return m_material == NULL? Color(): *(m_material->get_color()); 
}

bool Primitive::is_light() const 
{ 
    return m_material == NULL? false: m_material->is_light(); 
}

bool Primitive::set_is_light(bool v) 
{ 
    if(m_material != NULL) m_material->set_is_light(v); 
}
