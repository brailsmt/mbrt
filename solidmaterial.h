#ifndef SOLIDMATERIAL_H
#define SOLIDMATERIAL_H

#include "material.h"
#include "materialfactory.h"

#include <iostream>

inline Material * new_solid_material(std::map<std::string, std::string> props);

/// This is the most basic material.  It has a single set of
/// attributes that return the same value regardless of the
/// intersection point.  Currently, this is the only material
/// that can exist at the leaf nodes of a material tree.
class SolidMaterial : public Material
{
    /// Static initialization.  Registers the material with the MaterialFactory.
    class SolidMaterialStaticInit
    {
        public:
            /// Register this class with the factory.
            SolidMaterialStaticInit()
            {
                MaterialFactory::get_instance()->registerFunction("solid", sigc::ptr_fun(new_solid_material));
            }
    };

    private:
         /// Dummy variable to run static initialization
        static SolidMaterialStaticInit m_init;

    public:
        /// Factory method to create a material from a set of attributes.
        /// @param attributes A map of strings that determine the attributes of the material
        /// @return A pointer to a SolidMaterial object.
        static Material * createSolidMaterial(std::map<std::string, std::string> attributes);

    protected:
        /// This is the percentage of light that is reflected off this material.  This value is
        /// between 0 and 1, 0 means the material reflects 0% of the light, a 1 means the material
        /// reflects 100% of the light.
        double m_reflection_coefficient;

        /// This describes how much this material scatters the light off the surface.  This value is
        /// between 0 and 1, 0 means the material scatters 0% of the light, a 1 means the material
        /// scatters 100% of the light.
        double m_diffusion_factor;

        /// This number describes how reflective this material is.  Higher numbers mean shinier
        /// objects.
        double m_reflectivity;

        /// This is the color of the material.
        Color * m_color;

        /// Is this material a light source?
        bool m_is_light_source;

        /// This is how opaque the object is, or more correctly how transparent the object.  A value
        /// of 0 is fully opaque, a value of 1.0 is fully transparent.
        double m_opacity;

        /// This describes the amount of refraction occuring within the object
        double m_refraction_index; 

        /// Ensure m_opacity remains within the range of 0.0f to 1.0f.
        virtual void normalize_opacity() {
            m_opacity = m_opacity > TRANSPARENT ? TRANSPARENT
                      : m_opacity < OPAQUE      ? OPAQUE
                      :                           m_opacity;
        }

    public:
        // You can never have too many constructors.
        //
        // Oh wait, yes you can.  TODO: find out where and
        // how all these are used and get rid of the unnecessary ones.

        SolidMaterial(bool is_light)
                : m_reflection_coefficient(1.0),
                  m_diffusion_factor(0.25f),
                  m_color(new Color()),
                  m_is_light_source(is_light),
                  m_reflectivity(35.0),
                  m_refraction_index(1.35),
                  m_opacity(OPAQUE) {}

        SolidMaterial(Color * m_color, double reflect = 1.0f, double diffuse = 0.25f, bool is_light = false, double opacity = OPAQUE)
                : m_reflection_coefficient(reflect),
                  m_diffusion_factor(diffuse),
                  m_color(m_color),
                  m_is_light_source(is_light),
                  m_reflectivity(35.0),
                  m_refraction_index(1.35),
                  m_opacity(opacity) {}

        /// Fully construct a material with no defaults.
        //
        // Do not remove this constructor. Used in create... method of solidmaterial.cpp
        SolidMaterial(Color * m_color, 
                bool is_light , 
                double reflection, 
                double diffuse , 
                double reflectivity,
                double refraction,
                double opacity) : 
                  m_color(m_color),
                  m_is_light_source(is_light),
                  m_reflection_coefficient(reflection),
                  m_diffusion_factor(diffuse),
                  m_reflectivity(reflectivity),
                  m_refraction_index(refraction),
                  m_opacity(opacity) {}

        /// Copy constructor.
        SolidMaterial(const SolidMaterial &other)
                : m_reflection_coefficient(other.m_reflection_coefficient),
                  m_diffusion_factor(other.m_diffusion_factor),
                  m_color(other.m_color),
                  m_is_light_source(other.m_is_light_source),
                  m_reflectivity(other.m_reflectivity),
                  m_opacity(other.m_opacity) {}

        virtual ~SolidMaterial() {
        }

        /// Calculate color at given point on the material.
        /// @return Color at given point
        virtual Color * get_color(const Point3D& intersection_point) const;

        /// Determine if material emits light at given point.  Note: due to the
        /// rendering algoritm currently being used, varying this value based
        /// on position might have unexpected results.
        ///
        /// @return true if material emits light
        virtual bool is_light(const Point3D& intersection_point) const;

        /// Set whether or not object emits light.  Subclasses are not required
        /// to do anything useful with this information.
        ///
        /// @param v True if material emits light
        virtual void set_is_light(bool v);

        /// Get the diffusion coefficient at the point.
        /// @param intersection_point Point in space where ray intersected an object
        /// @return the diffusion coefficient at the given point.
        virtual double get_diffuse(const Point3D& intersection_point) const;

        /// Set the diffusion coefficient for the object.  Subclasses are not required
        /// to do anything useful with this information.
        virtual void set_diffuse(double diffuse);

        /// Get reflection coefficient at the point.
        /// @param intersection_point Point in space where ray intersected an object
        /// @return the reflection coefficient at the given point
        virtual double get_reflection(const Point3D& intersection_point) const;

        /// Get reflectivity at the point.
        /// @param intersection_point Point in space where ray intersected an object
        /// @return the reflectivity at the given point
        virtual double get_reflectivity(const Point3D& intersection_point) const;

        /// Get the index of refraction at the given point.  Note that varying the
        /// index of refraction across the material will most likely return results
        /// that are not accurate.
        /// @param intersection_point Point in space where ray intersected an object
        /// @return refraction index at the given point
        virtual double get_refraction_index(const Point3D& intersection_point) const;

        /// Get the opacity of this material at the point.
        /// @param intersection_point Point in space where ray intersected an object
        /// @return opacity at the given point
        virtual double get_opacity(const Point3D& intersection_point) const;

        /// Set opacity for the object.  Subclasses are not required to do
        /// anything useful with this information.
        ///
        /// @param opacity The suggested opacity for the material
        virtual void set_opacity(double opacity);
        
 #if 0
 Stub out specular methods.
        /// Get specular coefficient.  Specular coefficient determines how much
        /// specular highlighting affects the overall color of the object.
        ///
        /// @return Specular coefficient
        virtual double get_specular_coeffiecient() ;

        /// Get specular power coefficient.  The power coefficient determines
        /// the rate of fall-off of the specular highlight.
        ///
        /// @return Specular power coefficient
        virtual double get_specular_power() ;
#endif

};

inline Material * new_solid_material(std::map<std::string, std::string> props) {
    bool isLight = props.count("light") > 0;

    Color * color = Scene::get_instance()->get_color(props["color"]);

    double reflection = props.count("reflection") > 0 ?
        (double)strtod(props["reflection"    ].c_str(), NULL)   : 1.0;

    double diffusion = props.count("diffusion") > 0 ?
        (double)strtod(props["diffusion"].c_str(), NULL)        :  1.0;

    double reflectivity = props.count("reflectivity") > 0 ?
        (double)strtod(props["reflectivity"].c_str(), NULL)     :  35.0;

    double refraction = props.count("refraction") > 0 ?
        (double)strtod(props["refraction"].c_str(), NULL)       :  1.0;

    double opacity = props.count("opacity") > 0 ?
        (double)strtod(props["opacity"].c_str(), NULL)          :  OPAQUE;

    std::cout << " Creating material '" << props["name"] << "' with:"  << std::endl;
    std::cout << "reflection:"          << reflection    << std::endl;
    std::cout << "diffusion:"           << diffusion     << std::endl;
    std::cout << "reflectivity:"        << reflectivity  << std::endl;
    std::cout << "refraction:"          << refraction    << std::endl;
    std::cout << "opacity: "            << opacity       << std::endl;

    return new SolidMaterial( color,
            isLight,
            reflection,
            diffusion,
            reflectivity,
            refraction,
            opacity
            );
}

#endif
