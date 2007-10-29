#ifndef NOISEMATERIAL_H
#define NOISEMATERIAL_H

#include "raytrace_defs.h"
#include "material.h"
#include "pluginfactory.h"
#include "noise.h"
#include "color.h"

Material * make_noise_material(std::map<std::string, std::string> props);


/// This class defines a composite material of two materials that are combined
/// based on a noise function. The result of the noise function is used to 
/// base the return value of each attribute on a linear, weighted average of the 
/// two materials.
class NoiseMaterial : public Material{
    /// Helper class to provide static initialization
    class StaticInit
    {
        public:
            /// Perform static initialization.  Registers class with material factory.
            StaticInit()
            {
                log_info("Registering as \"noise\" and \"cloud\"");
                MaterialFactory::get_instance()->registerPlugin("noise", sigc::ptr_fun(make_noise_material));
                MaterialFactory::get_instance()->registerPlugin("cloud", sigc::ptr_fun(make_noise_material));
            }
    };

    protected:


        /// Static noise instance.
        Noise m_noise;
        
        /// Dummy variable to force static initialization
        static StaticInit m_init;

        /// First material element of composite.
        Material * m_material_one;
        /// Second material element of composite.
        Material * m_material_two;

        // Amount to scale noise by
        double m_scale;
        /// Calculate how much of first material contributes to
        /// overall material.  This is a double ranging from 0.0 to 1.0.
        /// The amount of the second material to use is (1.0 - choose_material() )
        double choose_material(const Point3D& intersection_point) const ;

    public:
        /// Create a noise-based material from two materials.
        NoiseMaterial(Material * one, Material * two, double scale = 1.0)
        {
            m_material_one = one;
            m_material_two = two;
            m_scale = scale;
        }

        /// Copy construtor
        NoiseMaterial(const NoiseMaterial &other)
                :  m_material_one(other.m_material_one),
                    m_material_two(other.m_material_two)
        {
            // TODO: this should really just be a straight copy..
        }


        virtual ~NoiseMaterial() {
            //delete m_color;
        }

        /// Get color from point.
        virtual Magick::Color get_color(const Point3D& intersesction_point) const ; 

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
        /// Factory method to create noise material.
        static Material * createNoiseMaterial(std::map<std::string, std::string>);
};

Material * make_noise_material(std::map<std::string, std::string> props) {
    Scene * scene = Scene::get_instance();
    Material * one = scene->get_material(props["material1"]);
    Material * two = scene->get_material(props["material2"]);
    double noise_scale = 1.0;
    if(props.find("noise_scale") != props.end())
    {
        noise_scale = (double)strtod(props["noise_scale"].c_str(), NULL);
    }
    return new NoiseMaterial(one, two,noise_scale);
}

#endif
