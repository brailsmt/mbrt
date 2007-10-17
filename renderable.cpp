#include "renderable.h"
#include <vector>

using std::vector;

//{{{
double Renderable::get_diffuse(const Point3D& intersection_point) const {
    double diffuse = m_material->get_diffuse(intersection_point);
    return (diffuse <= 0.0) ? 0.0 :
           (diffuse >= 1.0) ? 1.0 :
           diffuse;
}
//}}}
//{{{
double Renderable::get_reflection(const Point3D& intersection_point) const {
    double reflection = m_material->get_reflection(intersection_point);
    return (reflection <= 0.0) ? 0.0 :
           (reflection >= 1.0) ? 1.0 :
           reflection;
}
//}}}
//{{{
double Renderable::get_reflectivity(const Point3D& intersection_point) const {
    return m_material->get_reflectivity(intersection_point);
}
//}}}
//{{{
double Renderable::get_specular(const Point3D& intersection_point) const {
    // Really?
    // follow up: This currently doesn't appear to be used.
    return 1.0;
}
//}}}
//{{{
double Renderable::get_opacity(const Point3D& intersection_point) const {
    return m_material->get_opacity(intersection_point);
}
//}}}
//{{{
double Renderable::get_refraction_index(const Point3D& intersection_point) const {
    return m_material == NULL ? 1.0 : (m_material->get_refraction_index(intersection_point));
}
//}}}
//{{{
Color Renderable::get_color(const Point3D& intersection_point) const {
    return m_material == NULL ? Color() : *(m_material->get_color(intersection_point));
}
//}}}
//{{{
bool Renderable::is_light(const Point3D& intersection_point) const {
    return m_material == NULL ? false : m_material->is_light(intersection_point);
}
//}}}
//{{{
bool Renderable::set_is_light(bool v) {
    if (m_material != NULL) m_material->set_is_light(v);
}
//}}}

//{{{
Ray Renderable::get_final_normal(const Point3D& p)
{
    if(m_bumpmap == NULL)
    {
        return get_normal(p);
    }
    else
    {
        return m_bumpmap->perturb_normal(get_normal(p), p);
    }
}
//}}}

Color Renderable::get_color_contribution(const Point3D &intersection_point, const Ray &ray, Vector &reflect, Vector &refract) {
    // Determine the color of the pixel at the point of intersection.
    double ambient_factor = 0.2;
    Color rv = get_color(intersection_point) * ambient_factor;
    Scene * scene = Scene::get_instance();

    if ( is_light(intersection_point) == false ) {

        // now calculate all rays to light sources, and accumulate each source's contribution to
        // the pixel color.
        vector<Renderable *>::iterator iter, end = scene->get_scene()->end();
        for ( iter = scene->get_scene()->begin(); iter != end; ++iter ) {

            if ( (*iter)->is_light(intersection_point) == true) {
                Renderable * light = *iter;

                //double diffusion = light->get_diffuse(intersection_point);
                //if ( diffusion > 0.0 ) {
                    // Calculate the ray to the light source (rtl).
                    Ray rtl(intersection_point, light->get_center() - intersection_point);

                    // Calculate the normal of the surface at the point of intersection.
                    Ray normal(get_final_normal(intersection_point) ); 

                    // Determine the angle, in radians, between the normal and the ray to the light source.
                    double theta = dot_product(rtl.direction(), normal.direction());

                    // Determine the vector at which the incoming ray is reflected.
                    reflect = ray.direction() - (normal.direction() * 2.0 * dot_product(ray.direction(), normal.direction()));;

                    // Determine the vector at which the incoming ray is refracted.  This is done
                    // according to Snell's Law.
                    
                    // TODO{: only perform calculations if object is actually refracting light
#if 0
                    double refraction_index = get_refraction_index(intersection_point);
                    double refract_coefficient = refraction_index / 1.0003; // / scene->global_refraction_index();
                    double normal_dot_neg_ray = dot_product(normal.direction(), -ray.direction());
                    double sqrt_val = 1.0 + (refract_coefficient * refract_coefficient) * (normal_dot_neg_ray * normal_dot_neg_ray);
                    if (sqrt_val >= 0.0) {
                        refract = ray.direction() * refract_coefficient + normal.direction() * ((normal_dot_neg_ray * refract_coefficient) - sqrt(sqrt_val));
                    }
                    // }TODO
#endif

                    // Determine diffuse lighting.
                    if ( theta > 0.0 ) {

                        // If this spot is in shadow, skip it.
                        if ( is_in_shadow(rtl, intersection_point) == true )
                            continue;
                        double specular_color = 0.0;
                        double reflectivity = get_reflectivity(intersection_point) ;
                            // Determine specular lighting.
                        
                        if(reflectivity > 0.001)
                        {
                            double spec_theta = dot_product(reflect, rtl.direction());
                            if(spec_theta > 0.0001)
                                specular_color = powf(spec_theta, reflectivity);
                            else
                                specular_color = 0.0;
                        }

                        //diffusion *= theta;
                        double diffusion = get_diffuse(intersection_point) * theta;
                        if(diffusion > 1.0)
                        {
                            log_warn("Diffusion %d", diffusion);
                        }

                        rv += get_color(intersection_point) * light->get_color(intersection_point) * diffusion;
                        // I don't think this does what you think it does.  It uses the reflection coefficient
                        // as the specular coefficient.  These are not the same thing.
                        rv += light->get_color(intersection_point) * specular_color * get_reflection(intersection_point);
                    }
                //}
            }
        }
    }
    else {
        rv = get_color(intersection_point);
    }

    return rv;
}

bool Renderable::initialize(xmlNode * node)
{
    log_debug("Initializing primitive...");
    xml_properties props = get_properties(node);
    Scene * scene = Scene::get_instance();

    m_material = scene->get_material( props["material"] );
    if(!m_material)
    {
        log_err("Material of type '%s' not found.", props["material"].c_str() );
    }
    else
    {
        log_debug("Added material '%s'", props["material"].c_str() );
    }

    if(props.find("bumpmap") != props.end())
    {
        m_bumpmap = scene->get_bumpmap( props["bumpmap"] );
    }
    else
    {
        m_bumpmap = NULL;
    }


    return true;  
}

