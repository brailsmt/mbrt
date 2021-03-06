#include "noisematerial.h"
#include <math.h>
#include <iostream>

using Magick::Color;
using Magick::ColorRGB;

NoiseMaterial::StaticInit NoiseMaterial::m_init;

double NoiseMaterial::choose_material(const Point3D& intersection_point) const
{
    return m_noise.get_noise(intersection_point * m_scale);
}

Color NoiseMaterial::get_color(const Point3D& intersection_point) const 
{ 
    // TODO: I don't think this is needed anymore after switching to Magick++
    // colors and getting rid of the global color map in the scene.
#if 0
    // !!!THREAD SAFETY ALERT!!!
    // we are using one global color here.  This
    // will absolutely break if we go multi-threaded.
    //  When that day comes, we will either need to
    // revisit this strategy or mangle the key
    // based on the thread id

    const char * noiseColorKey = "__NOISE_WORKING_COLOR";

    Scene * scene = Scene::get_instance();
    ColorRGB retVal(noiseColorKey);
    if(!is_valid(retVal))
    {
        retVal = Color("red");
    }

#endif
    double coeff = choose_material(intersection_point);

    ColorRGB one = m_material_one->get_color(intersection_point);
    ColorRGB two = m_material_two->get_color(intersection_point);

    return (one * coeff) + (two * (1.0-coeff));

}

bool NoiseMaterial::is_light(const Point3D& intersection_point) const 
{ 
    return false; 
}

void NoiseMaterial::set_is_light(bool v) 
{ 
}

double NoiseMaterial::get_diffuse(const Point3D& intersection_point) const 
{ 
    double coeff = choose_material(intersection_point);
    double contributionOne = m_material_one->get_diffuse(intersection_point) * coeff;
    double contributionTwo = m_material_two->get_diffuse(intersection_point) * (1.0 - coeff);
    return contributionOne + contributionTwo;
}

void NoiseMaterial::set_diffuse(double diffuse) 
{ 
}

double NoiseMaterial::get_reflection(const Point3D& intersection_point) const 
{ 
    double coeff = choose_material(intersection_point);
    double contributionOne = m_material_one->get_reflection(intersection_point) * coeff;
    double contributionTwo = m_material_two->get_reflection(intersection_point) * (1.0 - coeff);
    return contributionOne + contributionTwo;
}

double NoiseMaterial::get_reflectivity(const Point3D& intersection_point) const 
{ 
    double coeff = choose_material(intersection_point);
    double contributionOne = m_material_one->get_reflectivity(intersection_point) * coeff;
    double contributionTwo = m_material_two->get_reflectivity(intersection_point) * (1.0 - coeff);
    return contributionOne + contributionTwo;
}

double NoiseMaterial::get_refraction_index(const Point3D& intersection_point) const 
{ 
    // This would seem sketchy at best.  Perhaps better
    // would be to make material one the authority...
    double coeff = choose_material(intersection_point);
    double contributionOne = m_material_one->get_refraction_index(intersection_point) * coeff;
    double contributionTwo = m_material_two->get_refraction_index(intersection_point) * (1.0 - coeff);
    return contributionOne + contributionTwo;
}

double NoiseMaterial::get_opacity(const Point3D& intersection_point) const 
{ 
    double coeff = choose_material(intersection_point);
    double contributionOne = m_material_one->get_opacity(intersection_point) * coeff;
    double contributionTwo = m_material_two->get_opacity(intersection_point) * (1.0 - coeff);
    return contributionOne + contributionTwo;
}

void NoiseMaterial::set_opacity(double opacity) 
{
}


Material * NoiseMaterial::createNoiseMaterial(std::map<std::string, std::string> props)
{
    Scene * scene = Scene::get_instance();
    Material * one = scene->get_material(props["material1"]);
    Material * two = scene->get_material(props["material2"]);
    return new NoiseMaterial(one, two);

}

