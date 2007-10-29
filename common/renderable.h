#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "raytrace_defs.h"
#include "material.h"
#include "bumpmap.h"
#include "ray.h"
#include "xml_util.h"

/// Defines an abstract interface that all primitives must implement.
class Renderable {
    public:
        virtual ~Renderable() {}

        /// Does the ray passed in collide with this object?
        ///
        /// @param ray The ray which is being traced.
        /// @param t This is z-depth at which the ray collides with this object.
        /// @return True if the ray collides with this object.
        virtual bool collides_with(const Ray &ray, double &t) const = 0;

        /// Returns the color of the Renderable.
        virtual Magick::Color get_color(const Point3D& intersection_point) const ;

        /// Returns the diffusion factor of the primitive.
        virtual double get_diffuse(const Point3D& intersection_point) const;

        /// Returns the reflection coefficient.  This is a number between 0 and 1.
        virtual double get_reflection(const Point3D& intersection_point) const;

        /// Returns the specular coefficient.  This is a number between 0 and 1.
        virtual double get_specular(const Point3D& intersection_point) const ;

        /// Returns the shinyness factor of the object.  The higher the number the shinier.
        virtual double get_reflectivity(const Point3D& intersection_point) const ;

        /// Returns the opacity level of this object.
        virtual double get_opacity(const Point3D& intersection_point) const ;

        /// Returns the refraction index of this object.
        virtual double get_refraction_index(const Point3D& intersection_point) const ;

        /// Returns if the primitive emits light
        virtual bool is_light(const Point3D& intersection_point) const ;

        virtual bool set_is_light(bool ) ;

        /// Returns the surface normal at point p. Derived classes should override
        /// this to provide the correct surface normal for the given point.
        virtual Ray get_normal(const Point3D &p) const = 0;

        /// Perform common initialization.  This method is <em>not</em>
        /// declared virtual as it should never be overridden.  
        bool initialize(xmlNode * node);


        /// Determine the contribution to lighting/color of the pixel based on the incoming
        /// ray and the intersection point.
        ///
        /// @pre If this method is called the ray <b>does</b> intersect with the surface.
        ///
        /// @param intersection_point This is Point3D that describes the intersection point of ray and
        ///                           primitive.
        /// @param ray This is the incoming ray.
        /// @param reflect A reference to the reflection vector.  This is here to avoid calculating
        ///                it twice.
        /// @param refract A reference to the refraction vector.
        ///
        /// @return The color which is the result of applying diffuse and specular lighting the the
        /// primitive.
        virtual Magick::Color get_color_contribution(const Point3D &intersection_point, const Ray &ray, Vector &reflect, Vector &refract) ;

        bool is_in_shadow(const Ray &ray_to_light, const Point3D intersection_point) {
            double dist = INF;
            Renderable * prim = NULL;
            Scene * scene = Scene::get_instance();

            prim = scene->find_collision(ray_to_light, dist);

            if ( this == prim ) {
                return false;
            }

            return (prim != NULL && !prim->is_light(intersection_point));
        }

        /// Return the center of the primitive.
        /// @return Center of the primitive
        virtual const Point3D get_center() const
        {
            return m_center;
        }

    protected:
        /// Material of the object. Materials may be simple materials,
        /// such as a solid color, or they may be complicated composites,
        /// such as checkered marble, but one primitive only considers 
        /// itself to have one material.
        Material * m_material ;

        /// "Center" of object.  Some objects do not have a true
        /// center, such as a plane, but to simplify the lighting
        /// algorithm we need to move this to Renderable.  This
        /// might also be useful in the future for centering textures
        /// or materials around.
        Point3D m_center;

        /// Bump map associated with primitive.  May be null. Bump maps
        /// are associated with the primitive and not the material as
        /// they affect the "structure" of the object as opposed to simply
        /// the coloring and shading of the object. 

        BumpMap * m_bumpmap;

        /// Material centering policy.  When set to true, materials will
        /// be centered to the center of the primitive.
        
        bool m_center_material;
    private:
        /// Returns the final surface normal at point p, after being processed 
        /// for bump-mapping. 
        Ray get_final_normal(const Point3D &p) ;
};

#endif
