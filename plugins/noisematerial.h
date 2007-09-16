#ifndef NOISEMATERIAL_H
#define NOISEMATERIAL_H

#include "color.h"
#include "raytrace_defs.h"
#include "material.h"
#include "materialfactory.h"

/// This class defines a composite material of two materials that are combined
/// based on a noise function.
class NoiseMaterial : public Material{
    /// Dummy class to provide static initialization
    class MaterialStaticInit
    {
        public:
            MaterialStaticInit()
            {
                MaterialFactory::get_instance()->registerFunction("noise", (void *) NoiseMaterial::createNoiseMaterial);
            }
    };
    protected:
    /// Dummy variable to force static initialization
        static MaterialStaticInit m_init;

        Material * m_material_one;
        Material * m_material_two;

        double choose_material(const Point3D& intersection_point) const ;

    public:
        /// Create a noise-based material from two materials.
        NoiseMaterial(Material * one, Material * two)
        {
            m_material_one = one;
            m_material_two = two;
        }

        /// Copy construtor
        NoiseMaterial(const NoiseMaterial &other)
                :  m_material_one(other.m_material_one),
                    m_material_two(other.m_material_two) 
        {
        }


        virtual ~NoiseMaterial() {
            //delete m_color;
        }

        /// Get color from point.
        virtual Color * get_color(const Point3D& intersesction_point) const ; 

        /// Return if material emits light.  Always returns false.
        virtual bool is_light(const Point3D& intersesction_point) const ; 

        /// No operation.  Value of v is ignored.
        virtual void set_is_light(bool v); 

        /// Get the diffuse coefficient of the material at the given point
        virtual double get_diffuse(const Point3D& intersesction_point) const ; 

        /// No operation.  Value of diffuse is ignored.
        virtual void set_diffuse(double diffuse); 

        /// Get reflection coefficient for the given point.
        virtual double get_reflection(const Point3D& intersesction_point) const; 

        /// Get reflectivity (?) for the given point.  TODO: figure out how reflection and reflectivity are different
        virtual double get_reflectivity(const Point3D& intersesction_point) const ; 

        /// Get the refraction index at the point.  This may produce some bizarre images
        /// if the refraction indexes vary among the materials.  It will certainly be
        /// incorrect for a noise material as varying refraction within the object will
        /// not be honored.  Only the location on the surface will be considered.
        virtual double get_refraction_index(const Point3D& intersesction_point) const; 

        /// Get opacity of object at the point. 
        virtual double get_opacity(const Point3D& intersesction_point) const ; 

        /// No operation. See other set methods.
        virtual void set_opacity(double opacity) ; 

    public:
        static Material * createNoiseMaterial(std::map<std::string, std::string>);
};

#endif
