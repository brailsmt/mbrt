
#ifndef CHECKERMATERIAL_H
#define CHECKERMATERIAL_H

#include "color.h"
#include "raytrace_defs.h"
#include "material.h"
#include "materialfactory.h"

/// This class defines various properties of materials from which primitives are made.
class CheckeredMaterial : public Material{

    /// Dummy class to provide static initialization
    class CheckeredMaterialStaticInit
    {
        public:
            CheckeredMaterialStaticInit()
            {
                MaterialFactory::get_instance()->registerFunction("checkered", (void *) CheckeredMaterial::createCheckeredMaterial);
            }
    };

    protected:
    /// Dummy variable to force static initialization
        static CheckeredMaterialStaticInit m_init;

        Material * m_material_one;
        Material * m_material_two;

        int choose_material(const Point3D& intersection_point) const ;

    public:
        CheckeredMaterial(Material * one, Material * two)
        {
            m_material_one = one;
            m_material_two = two;
        }

        CheckeredMaterial(const CheckeredMaterial &other)
                :  m_material_one(other.m_material_one),
                    m_material_two(other.m_material_two) 
        {
        }


        virtual ~CheckeredMaterial() {
            //delete m_color;
        }

        virtual Color * get_color(const Point3D& intersesction_point) const ; 

        virtual bool is_light(const Point3D& intersesction_point) const ; 
        virtual void set_is_light(bool v); 

        virtual double get_diffuse(const Point3D& intersesction_point) const ; 
        virtual void set_diffuse(double diffuse); 

        virtual double get_reflection(const Point3D& intersesction_point) const; 
        virtual double get_reflectivity(const Point3D& intersesction_point) const ; 
        virtual double get_refraction_index(const Point3D& intersesction_point) const; 

        virtual double get_opacity(const Point3D& intersesction_point) const ; 
        virtual void set_opacity(double opacity) ; 

    public:
        static Material * createCheckeredMaterial(std::map<std::string, std::string>);
};
#endif
