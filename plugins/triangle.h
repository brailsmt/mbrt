#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "primitive.h"
#include "pluginfactory.h"
#include "solidmaterial.h"
#include "scene.h"
#include "xml_util.h"
#include <string>

class Triangle;

Primitive * new_triangle(xmlNode * node);
void delete_triangle(Triangle * triangle);

/// A Triangle is defined by its m_center point and its radius.
class Triangle : public Primitive {

    protected:

    /// Fake a static initializer.
    class StaticInit {
        public:
            /// Register material with the factory
            StaticInit() {
                PrimitiveFactory::get_instance()->registerPlugin("triangle", sigc::ptr_fun(new_triangle));
            }
    };
    /// Force static initialization.
    static StaticInit m_init;

    protected:

        /// One point on the triangle
        Point3D m_location;
        
        ///  One vector of the triangle
        Vector m_vector_one;
        
        ///  Other vector of the triangle
        Vector m_vector_two;

        /// Normal direction for the triangle.  Triangles, being flat, 
        /// have the a surface normal pointing in the same direction
        /// at all points along the primtive.
        Vector m_normal_direction;

        /// Material of this primitive.  Overrides any color value.
        std::string m_material_name;
        
        /// Color of this primitive.  Overridden by material, if it exists.
        std::string m_color_name;
        
        /// Bump map associated with this primitive. Most often, will be blank.
        std::string m_bumpmap_name;

        /// Dot product of normal and location of the prim.  Intermediate value
        /// in intersection calculation, stored as it never changes.
        double m_normal_location_dot ;

    public:

        /// Create a triangle with the default m_material.
        Triangle( 
            const Point3D& location,
            const Vector& vec1,
            const Vector& vec2,
            std::string color_name = "", 
            std::string material_name = "",
            std::string bumpmap_name = "") 
            : 
              m_location(location),  
              m_vector_one(vec1),  
              m_vector_two(vec2),  
              m_normal_direction( cross_product( vec1, vec2) ),
              m_material_name(material_name),
              m_color_name(color_name),
              m_bumpmap_name(bumpmap_name)
        {
            // TODO some of this initialization will be common across
            // all prims. Consider refactor to base class.
            log_debug("Creating a Triangle, color = %s, material = %s, bumpmap = %s",
                                        m_color_name.c_str(), m_material_name.c_str(), m_bumpmap_name.c_str());

            m_normal_location_dot = dot_product(m_normal_direction, m_location);

            Scene * scene = Scene::get_instance();
            m_material = scene->get_material(material_name);
            
            // TODO: should we really handle as a default, or should we trap this as an error?
            if(m_material == NULL)
            {
                Color * color = scene->get_color(color_name);
                color = color ? color : new Color(1.0,0.5,0.5);
                m_material = new SolidMaterial(color ,false);
            }

            m_bumpmap = scene->get_bumpmap(bumpmap_name);
        }

        /// Clean up the triangle's resources.
        virtual ~Triangle() {};

        /// Determine if a ray intersects this object.
        /// @param ray The ray we are testing to determine collision.
        /// @param t The z-depth where the ray collides with this triangle.
        /// @return True if the ray collides with this triangle.
        virtual bool collides_with(const Ray &ray, double &t) const;

        /// Return the surface normal of the triangle.  A triangle's surface normal is the vector
        /// from the center of the triangle to the intersection point, translated to the intersection
        /// point and normalized.
        virtual Ray get_normal(const Point3D &p) ;
};

Primitive * new_triangle(xmlNode * node) {
    log_debug("Entering new_triangle()");
    Triangle * rv = NULL;

    xml_properties props = get_properties(node);

    if ( props.empty() == false ) {
        double x1        = (double)strtod(props["x"].c_str(), NULL);
        double y1        = (double)strtod(props["y"].c_str(), NULL);
        double z1        = (double)strtod(props["z"].c_str(), NULL);
        double vx1        = (double)strtod(props["vector1_x"].c_str(), NULL);
        double vy1        = (double)strtod(props["vector1_y"].c_str(), NULL);
        double vz1        = (double)strtod(props["vector1_z"].c_str(), NULL);
        double vx2        = (double)strtod(props["vector2_x"].c_str(), NULL);
        double vy2        = (double)strtod(props["vector2_y"].c_str(), NULL);
        double vz2        = (double)strtod(props["vector2_z"].c_str(), NULL);
        std::string color    = props["color"   ];
        std::string material = props["material"];
        std::string bumpmap  = props["bumpmap"];

        rv = new Triangle(
            Point3D(x1, y1, z1), 
            Vector(vx1, vy1, vz1), 
            Vector(vx2, vy2, vz2), 
            color, 
            material, 
            bumpmap);
    }
    else {
        log_err("No properties specified for <triangle> tag.");
    }

    log_err("Leaving new_triangle()");
    return dynamic_cast<Primitive *>(rv);
}

void delete_triangle(Triangle * triangle) {
    delete triangle, triangle = NULL;
}


#endif
