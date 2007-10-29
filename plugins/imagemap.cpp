/* file name  : imagemapstaticinit.cpp
 * author     : Michael Brailsford
 * created    : Thu Oct 25 01:32:08 -0500 2007
 * copyright  : 2007 Michael Brailsford
 */

#include "imagemap.h"
#include <cmath>

using Magick::Color;
using Magick::ColorRGB;

ImageMap::StaticInit ImageMap::m_init;

//{{{
Color ImageMap::get_color(const Point3D &intersection_point, const Renderable * const obj) const {
    if(obj == NULL) {
        return Color("red");
    }
    Vector normal = obj->get_normal(intersection_point).direction();

    Vector north(0, 1, 0);
    Vector equator(-1, 0, 0);

    double phi   = acos(-(dot_product(normal, north)));
    double lattitude = phi * M_1_PI;
    double longitude;

    if(lattitude == 0.0 || lattitude == 1.0) {
        return m_image.pixelColor(0,0);
    }

    double theta = acos((dot_product(normal, equator))/sin(phi)) * (0.5 * M_1_PI);
    if(dot_product(cross_product(north, equator), normal) > 0) {
        longitude = theta;
    }
    else {
        longitude = 1 - theta;
    }

    int x, y;
    x = m_image.baseColumns() - (int)(longitude * m_image.baseColumns());
    y = (int)(lattitude * m_image.baseRows());
    if(x > m_image.baseColumns() || y > m_image.baseRows() || x < 0 || y < 0) {
        return Color("red");
    }
    else {
        return m_image.pixelColor(x, y);
    }
}
//}}}

bool ImageMap::is_light(const Point3D& intersection_point) const 
{ 
        return m_is_light_source; 
}
void ImageMap::set_is_light(bool v) 
{ 
        m_is_light_source = v; 
}

double ImageMap::get_diffuse(const Point3D& intersection_point) const 
{ 
        return m_diffusion_factor; 
}

void ImageMap::set_diffuse(double diffuse) 
{ 
        m_diffusion_factor = diffuse; 
}

double ImageMap::get_reflection(const Point3D& intersection_point) const 
{ 
        return m_reflection_coefficient; 
}

double ImageMap::get_reflectivity(const Point3D& intersection_point) const 
{ 
        return m_reflectivity; 
}

double ImageMap::get_refraction_index(const Point3D& intersection_point) const 
{ 
        return m_refraction_index; 
}

double ImageMap::get_opacity(const Point3D& intersection_point) const 
{ 
        return m_opacity; 
}

void ImageMap::set_opacity(double opacity) 
{
        m_opacity = opacity; normalize_opacity(); 
}


