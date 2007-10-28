#include "marblematerial.h"
#include <Magick++.h>
#include <math.h>
#include <iostream>
#include "color.h"

using Magick::Color;


MarbleMaterial::StaticInit MarbleMaterial::m_init;



void MarbleMaterial::choose_materials(double noise_amount, Material ** one  , Material ** two) const
{
    *one = NULL;
    *two = NULL;

    if(noise_amount < m_break_point)
    {
        *one = m_material_one_a;
        *two = m_material_one_b;
    }
    else
    {
        *one = m_material_two_a;
        *two = m_material_two_b;
    }
}

double MarbleMaterial::interpolate_value(double noise_amount, double one, double two) const
{
    double scale = 0.0;

    if(noise_amount < m_break_point)
    {
        scale = noise_amount * m_material_one_scale;
    }
    else
    {
        scale = (noise_amount - m_break_point) * m_material_two_scale;
    }
    return one * scale + two * (1.0 - scale);
}

Color * MarbleMaterial::get_color(const Point3D& intersection_point) const 
{ 
    // !!!THREAD SAFETY ALERT!!!
    // we are using one global color here.  This
    // will absolutely break if we go multi-threaded.
    //  When that day comes, we will either need to
    // revisit this strategy or mangle the key
    // based on the thread id

    const char * noiseColorKey = "__MARBLE_WORKING_COLOR";

    Scene * scene = Scene::get_instance();
    Color * retVal = scene->get_color(noiseColorKey);
    if(retVal == NULL)
    {
        retVal = new Color();
        scene->add_color(noiseColorKey, retVal);
    }

    double noise = m_noise.get_noise(intersection_point * m_noise_scale);
    Material * one = NULL;
    Material * two = NULL;
    choose_materials(noise, &one, &two);

    // @todo TODO: This probably could be done better.

    double scale = 0.0;

    if(noise < m_break_point)
    {
        scale = noise * m_material_one_scale;
    }
    else
    {
        scale = (noise - m_break_point) * m_material_two_scale;
    }

    Color * color_one = (one->get_color(intersection_point) );
    Color * color_two = (two->get_color(intersection_point) );
    (*retVal) = (*color_one * scale) + (*color_two * (1.0-scale) );

    return retVal;

}

bool MarbleMaterial::is_light(const Point3D& intersection_point) const 
{ 
    return false; 
}

void MarbleMaterial::set_is_light(bool v) 
{ 
}

double MarbleMaterial::get_diffuse(const Point3D& intersection_point) const 
{ 
    Material * one = NULL;
    Material * two = NULL;
    double noise = m_noise.get_noise(intersection_point * m_noise_scale);
    choose_materials(noise, &one, &two);
    return interpolate_value(noise, one->get_diffuse(intersection_point), two->get_diffuse(intersection_point));
}

void MarbleMaterial::set_diffuse(double diffuse) 
{ 
}

double MarbleMaterial::get_reflection(const Point3D& intersection_point) const 
{ 
    Material * one = NULL;
    Material * two = NULL;
    double noise = m_noise.get_noise(intersection_point * m_noise_scale);
    choose_materials(noise, &one, &two);
    return interpolate_value(noise, one->get_reflection(intersection_point), two->get_reflection(intersection_point));
}

double MarbleMaterial::get_reflectivity(const Point3D& intersection_point) const 
{ 
    Material * one = NULL;
    Material * two = NULL;
    double noise = m_noise.get_noise(intersection_point* m_noise_scale);
    choose_materials(noise, &one, &two);
    return interpolate_value(noise, one->get_reflectivity(intersection_point), two->get_reflectivity(intersection_point));
}

double MarbleMaterial::get_refraction_index(const Point3D& intersection_point) const 
{ 
    // This would seem sketchy at best.  Perhaps better
    // would be to make material one the authority...
    Material * one = NULL;
    Material * two = NULL;
    double noise = m_noise.get_noise(intersection_point* m_noise_scale);
    choose_materials(noise, &one, &two);
    return interpolate_value(noise, one->get_refraction_index(intersection_point), two->get_refraction_index(intersection_point));
}

double MarbleMaterial::get_opacity(const Point3D& intersection_point) const 
{ 
    Material * one = NULL;
    Material * two = NULL;
    double noise = m_noise.get_noise(intersection_point* m_noise_scale);
    choose_materials(noise, &one, &two);
    return interpolate_value(noise, one->get_opacity(intersection_point), two->get_opacity(intersection_point));
}

void MarbleMaterial::set_opacity(double opacity) 
{
}

