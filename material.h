#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "raytrace_defs.h"

/// This class defines various properties of materials from which primitives are made.
/// Each get_* method takes the point in which the ray intersected with the primitive. Note that
/// the material has no concept of the actual shape of the primitive, and can only make
/// a determination of each attribute based on where in space it is.  Therefore, what is rendered
/// could be considered the intersection of the infinite space in which the material exists and
/// the finite space in which the primitive exists.
///
/// Having said all that, it might make sense anyway in the future to pass in the surface normal
/// as well, or some other generic data about the primitive, to facilitate materials such as
/// texture-maps or allow for bizarre, abstract materials.

class Material {

    public:

        /// Default constructor 
        //
        //Material() {}; 

	/// Virtual destructor.
        virtual ~Material() {
        };

        /// Calculate color at given point on the material.
        /// @return Color at given point
        virtual Color * get_color(const Point3D& intersection_point) const =0;

        /// Determine if material emits light at given point.  Note: due to the
        /// rendering algoritm currently being used, varying this value based
        /// on position might have unexpected results.
        ///
        /// @return true if material emits light
        virtual bool is_light(const Point3D& intersection_point) const =0;

        /// Set whether or not object emits light.  Subclasses are not required
        /// to do anything useful with this information.
        ///
        /// @param v True if material emits light
        virtual void set_is_light(bool v) =0;

        /// Get the diffusion coefficient at the point.
        /// @param intersection_point Point in space where ray intersected an object
        /// @return the diffusion coefficient at the given point.
        virtual double get_diffuse(const Point3D& intersection_point) const =0;

        /// Set the diffusion coefficient for the object.  Subclasses are not required
        /// to do anything useful with this information.
        virtual void set_diffuse(double diffuse) =0;

        /// Get reflection coefficient at the point.
        /// @param intersection_point Point in space where ray intersected an object
        /// @return the reflection coefficient at the given point
        virtual double get_reflection(const Point3D& intersection_point) const =0;

        /// Get reflectivity at the point.
        /// @param intersection_point Point in space where ray intersected an object
        /// @return the reflectivity at the given point
        virtual double get_reflectivity(const Point3D& intersection_point) const =0;

        /// Get the index of refraction at the given point.  Note that varying the
        /// index of refraction across the material will most likely return results
        /// that are not accurate.
        /// @param intersection_point Point in space where ray intersected an object
        /// @return refraction index at the given point
        virtual double get_refraction_index(const Point3D& intersection_point) const =0;

        /// Get the opacity of this material at the point.
        /// @param intersection_point Point in space where ray intersected an object
        /// @return opacity at the given point
        virtual double get_opacity(const Point3D& intersection_point) const =0;

        /// Set opacity for the object.  Subclasses are not required to do
        /// anything useful with this information.
        ///
        /// @param opacity The suggested opacity for the material
        virtual void set_opacity(double opacity) =0;

#if 0

Stub out of specular methods

        /// Get specular coefficient.  Specular coefficient determines how much
        /// specular highlighting affects the overall color of the object.
        ///
        /// @return Specular coefficient
        virtual double get_specular_coeffiecient() = 0;

        /// Get specular power coefficient.  The power coefficient determines
        /// the rate of fall-off of the specular highlight.
        ///
        /// @return Specular power coefficient
        virtual double get_specular_power() = 0;
#endif

        
};

#endif
