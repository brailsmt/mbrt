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
Color * ImageMap::get_color(const Point3D &intersection_point, const Renderable * const obj) const {
    if(obj == NULL) {
        return new Color(1, 0, 0);
    }
    Vector normal = obj->get_normal(intersection_point).direction();

    Vector north(0, 1, 0);
    Vector equator(1, 0, 0);

    double phi   = acos(-(dot_product(normal, north)));
    double longitude = phi * M_1_PI;
    double lattitude;

    if(longitude == 0.0 || longitude == 1.0) {
        Magick::ColorRGB clr = m_image.pixelColor(0,0);
        return new ColorRGB(clr.red(), clr.green(), clr.blue());
    }

    double theta = acos((dot_product(normal, equator))/sin(phi)) * (0.5 * M_1_PI);
    if(dot_product(cross_product(north, equator), normal) > 0) {
        lattitude = theta;
    }
    else {
        lattitude = 1 - theta;
    }

    int x, y;
    x = (int)(lattitude * m_image.baseColumns());
    y = (int)(longitude * m_image.baseRows());
    if(x > m_image.baseColumns() || y > m_image.baseRows() || x < 0 || y < 0) {
        return new Color(1, 0, 0);
    }
    else {
        Magick::ColorRGB clr = m_image.pixelColor(x, y);
        return new ColorRGB(clr.red(), clr.green(), clr.blue());
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


