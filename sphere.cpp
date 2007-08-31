/// @file sphere.cpp
/// @author Michael Brailsford
/// @date Tue Mar 27 19:57:23 -0500 2007
/// &copy; 2007 Michael Brailsford

#include "sphere.h"
#include <iostream>

using std::cout;
using std::endl;

//{{{
Sphere::~Sphere() {
}
//}}}

//{{{
bool Sphere::collides_with(const Ray &ray, float &t) const {
    bool rv = false;
    Point3D v = ray.origin() - m_center;
    Vector d = ray.direction();
    float d_dot_d = dot_product(d, d);

    float b         = dot_product(v, d);
    float b_squared = b * b;
    float _ac       = (d_dot_d) * (dot_product(v, v) - m_radius*m_radius);

    if( b_squared > _ac ) {

        float tmp = sqrtf( b_squared - _ac );
        float t2 = -b + tmp;

        if( t2 > -0.00001 ) {
            float t1 = -b - tmp;
            if( t1 < t ) {
                t = t1 / d_dot_d;
                rv = true;
            }
        }
    }

    return rv;
}
//}}}
//{{{
Ray Sphere::get_normal(const Point3D &p) {
    return Ray(p, p - m_center);
}
//}}}
//{{{
Color Sphere::get_color_contribution(const Point3D &intersection_point, const Ray &ray, Vector &reflect, Vector &refract) {
    // Determine the color of the pixel at the point of intersection.
    float ambient_factor = 0.2;
    Color rv = get_color() * ambient_factor;
    Scene * scene = Scene::get_instance();

    if( is_light() == false ) {

        // now calculate all rays to light sources, and accumulate each source's contribution to
        // the pixel color.
        vector<Primitive *>::iterator iter, end = scene->get_scene()->end();
        for( iter = scene->get_scene()->begin(); iter != end; ++iter ) {

            if( (*iter)->is_light() == true) {
                Sphere * light  = (Sphere *)(*iter);
                float diffusion = light->get_diffuse();

                if( diffusion > 0.0f ) {
                    // Calculate the ray to the light source (rtl).
                    Ray rtl(intersection_point, light->get_center() - intersection_point);

                    // Calculate the normal of the surface at the point of intersection.
                    Ray normal(get_normal(intersection_point ) ); //, intersection_point - m_center ) );

                    // Determine the angle, in radians, between the normal and the ray to the light source.
                    float theta = dot_product(rtl.direction(), normal.direction());

                    // Determine the vector at which the incoming ray is reflected.
                    reflect = ray.direction() - (normal.direction() * 2.0f * dot_product(ray.direction(), normal.direction()));;

                    // Determine the vector at which the incoming ray is refracted.  This is done
                    // according to Snell's Law.
                    float refraction_index = 1.52;                         //get_refraction_index();
                    float refract_coefficient = refraction_index / 1.0003; // / scene->global_refraction_index();
                    float normal_dot_neg_ray = dot_product(normal.direction(), -ray.direction());
                    float sqrt_val =       1.0f + (refract_coefficient * refract_coefficient) * (normal_dot_neg_ray * normal_dot_neg_ray);
                    if(sqrt_val >= 0.0f) {
                        refract = ray.direction() * refract_coefficient + normal.direction() * ((normal_dot_neg_ray * refract_coefficient) - sqrt(sqrt_val));
                    }

                    // Determine diffuse lighting.
                    if( theta > 0.0f ) {

                        // If this spot is in shadow, skip it.
                        if( is_in_shadow(rtl) == true )
                            continue;

                        // Determine specular lighting.
                        float spec_theta = dot_product(reflect, rtl.direction());
                        float spec_clr   = powf(spec_theta, get_reflectivity());

                        diffusion *= theta;
                        rv += get_color() * light->get_color() * diffusion;
                        rv += light->get_color() * spec_clr * get_reflection();
                    }
                }
            }
        }
    }
    else {
        rv = get_color();
    }

    return rv;
}
//}}}
