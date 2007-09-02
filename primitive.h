#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "raytrace_defs.h"
#include "material.h"
#include "color.h"
#include "ray.h"

/// Defines an abstract interface that all primitives must implement.
class Primitive {
    public:
        virtual ~Primitive() {}

        /// Does the ray passed in collide with this object?
        ///
        /// @param ray The ray which is being traced.
        /// @param t This is z-depth at which the ray collides with this object.
        /// @return True if the ray collides with this object.
        virtual bool collides_with(const Ray &ray, double &t) const = 0;

        /// Returns the color of the Primitive.
        virtual Color get_color() const ;

        /// Returns the diffusion factor of the primitive.
        virtual double get_diffuse() const;

        /// Returns the reflection coefficient.  This is a number between 0 and 1.
        virtual double get_reflection() const;

        /// Returns the specular coefficient.  This is a number between 0 and 1.
        virtual double get_specular() const ;

        /// Returns the shinyness factor of the object.  The higher the number the shinier.
        virtual double get_reflectivity() const ;

        /// Returns the opacity level of this object.
        virtual double get_opacity() const ;

        /// Returns the refraction index of this object.
        virtual double get_refraction_index() const ;

        /// Returns if the primitive emits light
        virtual bool is_light() const ;

        virtual bool set_is_light(bool ) ;

        /// Returns the surface normal at point p.
        virtual Ray get_normal(const Point3D &p) = 0;

        /// Determine the contribution to lighting/color of the pixel based on the incoming
        /// ray and the intersection point.
        ///
        /// @pre If this method is called the ray <b>does</b> intersect with the surface.
        ///
        /// @param intersection_point This is Point3D that describes the intersection point of ray and
        ///                           primitive.
        /// @param ray This is the incoming ray.
        /// @param scene This is the scene definition.
        /// @param reflect A reference to the reflection vector.  This is here to avoid calculating
        ///                it twice.
        /// @param refract A reference to the refraction vector.
        ///
        /// @return The color which is the result of applying diffuse and specular lighting the the
        /// primitive.
        virtual Color get_color_contribution(const Point3D &intersection_point, const Ray &ray, Vector &reflect, Vector &refract) ;

        bool is_in_shadow(const Ray &ray_to_light) {
            double dist = INF;
            Primitive * prim = NULL;
            Scene * scene = Scene::get_instance();

            prim = scene->find_collision(ray_to_light, dist);

            if ( this == prim ) {
                return false;
            }

            return (prim != NULL && !prim->is_light());
        }

        /// Return the center of the primitive.
        virtual const Point3D get_center() const
        {
            return m_center;
        }

    protected:
        Material * m_material ;

        /// "Center" of object.  Some objects do not have a true
        /// center, such as a plane, but to simplify the lighting
        /// algorithm we need to move this to Primitive.  This
        /// might also be useful in the future for centering textures
        /// or materials around.
        Point3D m_center;
};

#endif
