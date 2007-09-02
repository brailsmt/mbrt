#include "primitive.h"
#include <vector>

using std::vector;

//{{{
double Primitive::get_diffuse() const {
    double diffuse = m_material->get_diffuse();
    return (diffuse <= 0.0f) ? 0.0f :
           (diffuse >= 1.0f) ? 1.0f :
           diffuse;
}
//}}}
//{{{
double Primitive::get_reflection() const {
    double reflection = m_material->get_reflection();
    return (reflection <= 0.0f) ? 0.0f :
           (reflection >= 1.0f) ? 1.0f :
           reflection;
}
//}}}
//{{{
double Primitive::get_reflectivity() const {
    return m_material->get_reflectivity();
}
//}}}
//{{{
double Primitive::get_specular() const {
    // Really?
    return 1.0f;
}
//}}}
//{{{
double Primitive::get_opacity() const {
    return m_material->get_opacity();
}
//}}}
//{{{
double Primitive::get_refraction_index() const {
    return m_material == NULL ? 1.0 : (m_material->get_refraction_index());
}
//}}}
//{{{
Color Primitive::get_color() const {
    return m_material == NULL ? Color() : *(m_material->get_color());
}
//}}}
//{{{
bool Primitive::is_light() const {
    return m_material == NULL ? false : m_material->is_light();
}
//}}}
//{{{
bool Primitive::set_is_light(bool v) {
    if (m_material != NULL) m_material->set_is_light(v);
}
//}}}


Color Primitive::get_color_contribution(const Point3D &intersection_point, const Ray &ray, Vector &reflect, Vector &refract) {
    // Determine the color of the pixel at the point of intersection.
    double ambient_factor = 0.2;
    Color rv = get_color() * ambient_factor;
    Scene * scene = Scene::get_instance();

    if ( is_light() == false ) {

        // now calculate all rays to light sources, and accumulate each source's contribution to
        // the pixel color.
        vector<Primitive *>::iterator iter, end = scene->get_scene()->end();
        for ( iter = scene->get_scene()->begin(); iter != end; ++iter ) {

            if ( (*iter)->is_light() == true) {
                //Sphere * light = (Sphere *)(*iter);
                Primitive * light = *iter;
                double diffusion = light->get_diffuse();

                if ( diffusion > 0.0 ) {
                    // Calculate the ray to the light source (rtl).
                    Ray rtl(intersection_point, light->get_center() - intersection_point);

                    // Calculate the normal of the surface at the point of intersection.
                    Ray normal(get_normal(intersection_point ) ); 

                    // Determine the angle, in radians, between the normal and the ray to the light source.
                    double theta = dot_product(rtl.direction(), normal.direction());

                    // Determine the vector at which the incoming ray is reflected.
                    reflect = ray.direction() - (normal.direction() * 2.0 * dot_product(ray.direction(), normal.direction()));;

                    // Determine the vector at which the incoming ray is refracted.  This is done
                    // according to Snell's Law.
                    double refraction_index = get_refraction_index();
                    double refract_coefficient = refraction_index / 1.0003; // / scene->global_refraction_index();
                    double normal_dot_neg_ray = dot_product(normal.direction(), -ray.direction());
                    double sqrt_val = 1.0 + (refract_coefficient * refract_coefficient) * (normal_dot_neg_ray * normal_dot_neg_ray);
                    if (sqrt_val >= 0.0) {
                        refract = ray.direction() * refract_coefficient + normal.direction() * ((normal_dot_neg_ray * refract_coefficient) - sqrt(sqrt_val));
                    }

                    // Determine diffuse lighting.
                    if ( theta > 0.0 ) {

                        // If this spot is in shadow, skip it.
                        if ( is_in_shadow(rtl) == true )
                            continue;

                        // Determine specular lighting.
                        double spec_theta = dot_product(reflect, rtl.direction());
                        double spec_clr = powf(spec_theta, get_reflectivity());

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
