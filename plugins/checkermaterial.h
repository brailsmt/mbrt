
#ifndef CHECKERMATERIAL_H
#define CHECKERMATERIAL_H

#include "color.h"
#include "raytrace_defs.h"
#include "material.h"
#include "materialfactory.h"

/// This class defines a checkered material that is a composite of
/// two other materials.
class CheckeredMaterial : public Material{

    /// Helper class to provide static initialization
    class CheckeredMaterialStaticInit
    {
        public:
            /// Register material with the factory
            CheckeredMaterialStaticInit()
            {
                MaterialFactory::get_instance()->registerFunction("checkered", (void *) CheckeredMaterial::createCheckeredMaterial);
            }
    };

    protected:
    /// Dummy variable to force static initialization
        static CheckeredMaterialStaticInit m_init;

        /// First material of composite
        Material *  m_material_one;
        
        /// Second material of composite
        Material *  m_material_two;

        /// Scaling to apply to pattern
        // TODO: deprecate this.  There are much better approaches to scaling
        // things that could be utilized
        double      m_scale;

        /// Return 0 to use second material, nonzero to use first material
        int choose_material(const Point3D& intersection_point) const ;

    public:
        /// Create a checkered material from two materials, scaled on all axis.
        /// @param one  The first material in the checker pattern
        /// @param two  The second material in the checker pattern
        /// @param scale  The factor to scale the checker pattern by
        CheckeredMaterial(Material * one, Material * two, double scale)
        {
            m_material_one = one;
            m_material_two = two;
            m_scale = scale;
        }

        /// Copy constructor
        CheckeredMaterial(const CheckeredMaterial &other)
                :  m_material_one(other.m_material_one),
                    m_material_two(other.m_material_two),
                    m_scale(other.m_scale)

        {
        }


        virtual ~CheckeredMaterial() {
            //delete m_color;
        }

        /// See material.h for a full description of this method.
        virtual Color * get_color(const Point3D& intersesction_point) const ; 

        /// Always returns false.
        virtual bool is_light(const Point3D& intersesction_point) const ; 
        
        /// This method ignores the value of v as checker materials never emit light.
        virtual void set_is_light(bool v); 

        /// See material.h for a full description of this method.
        virtual double get_diffuse(const Point3D& intersesction_point) const ; 
        
        /// This method ignores the value passed in.
        virtual void set_diffuse(double diffuse); 

        /// See material.h for a full description of this method.
        virtual double get_reflection(const Point3D& intersesction_point) const; 
        
        /// See material.h for a full description of this method.
        virtual double get_reflectivity(const Point3D& intersesction_point) const ; 
        
        /// See material.h for a full description of this method.
        virtual double get_refraction_index(const Point3D& intersesction_point) const; 

        /// See material.h for a full description of this method.
        virtual double get_opacity(const Point3D& intersesction_point) const ; 

        /// This method ignores the value passed in.
        virtual void set_opacity(double opacity) ; 

    public:
        /// Factory method to create a checkered material from two other materials.
        static Material * createCheckeredMaterial(std::map<std::string, std::string>);
};
#endif
