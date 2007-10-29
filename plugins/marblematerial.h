#ifndef MARBLEMATERIAL_H
#define MARBLEMATERIAL_H

#include "raytrace_defs.h"
#include "material.h"
#include "pluginfactory.h"
#include "noise.h"


/// Create marble material
Material * make_marble_material(std::map<std::string, std::string> props);


/// This class defines a more configurable marble texture.  Technically, this is
/// still a cloud texture.  
///
/// This material supports the following properties:
///
///    material1a  (Required)   First component of first marble material
///    material1b  (Required)   Second component of first marble material
///    material2a  (Required)   First component of second marble material
///    material2b  (Required)   Second component of second marble material
///    break_point (Required)   Noise value where material breaks from first marble material into second
///    noise_scale (Required)   How much to scale noise by.  Larger values give grainier marbles.
///
class MarbleMaterial : public Material{
    /// Helper class to provide static initialization
    class StaticInit
    {
        public:
            /// Perform static initialization.  Registers class with material factory.
            StaticInit()
            {
                log_info("Registering as \"marble\" ");
                MaterialFactory::get_instance()->registerPlugin("marble", sigc::ptr_fun(make_marble_material));
            }
    };

    protected:


        /// Static noise instance.
        Noise m_noise;
        
        /// Dummy variable to force static initialization
        static StaticInit m_init;

        /// First material element of composite.
        Material * m_material_one_a;
        /// Second material element of composite.
        Material * m_material_one_b;

        /// Third material element of composite.
        Material * m_material_two_a;
        /// Fourth material element of composite.
        Material * m_material_two_b;
        
        /// Point in noise calculation where material
        /// 'breaks' into second set of materials
        double m_break_point;
        
        /// Amount to scale first material set by. 
        double m_material_one_scale; 
        
        /// Amount to scale second material set by. 
        double m_material_two_scale; 

        /// Amount to scale noise by. Larger values create
        /// much grainier marbles.
        double m_noise_scale;

    public:
        /// Create a noise-based material from two materials.
        MarbleMaterial(
            Material * one_a, 
            Material * one_b, 
            Material * two_a, 
            Material * two_b,
            double break_point,
            double noise_scale) :
                m_material_one_a(one_a),
                m_material_one_b(one_b),
                m_material_two_a(two_a),
                m_material_two_b(two_b),
                m_break_point(break_point),
                m_noise_scale(noise_scale)
        {
            m_material_one_scale = 1.0 / m_break_point ;
            m_material_two_scale = 1.0 / (1-m_break_point);
        }

        virtual ~MarbleMaterial() {
        }

        /// Perform interpolation of two values based on the noise value.
        double interpolate_value(double noise_amount, double one, double two) const;
        
        /// Choose which set of materials to use for a give noise value 
        void choose_materials(double noise_amount, Material ** one  , Material ** two) const;
        
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

        /// Get reflectivity for the given point. 
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

};

Material * make_marble_material(std::map<std::string, std::string> props) {
    Scene * scene = Scene::get_instance();
    Material * one = scene->get_material(props["material1a"]);
    Material * two = scene->get_material(props["material1b"]);
    Material * three = scene->get_material(props["material2a"]);
    Material * four = scene->get_material(props["material2b"]);
    double break_point = (double)strtod(props["break_point"].c_str(), NULL);
    double noise_scale = (double)strtod(props["noise_scale"].c_str(), NULL);
    return new MarbleMaterial(one, two, three, four, break_point, noise_scale);
}

#endif
