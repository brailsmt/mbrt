/// @file sphere.h
/// @author Michael Brailsford <brailsmt@yahoo.com>
/// @date Sun Mar 25 03:08:28 -0500 2007
/// &copy; 2007 Michael Brailsford

#ifndef SPHERE_H
#define SPHERE_H

#include "primitive.h"
#include "scene.h"
#include "xml_util.h"
#include <string>

#include "solidmaterial.h"

class Sphere;

Primitive * new_sphere(xmlNode * node);
void delete_sphere(Sphere * sphere);

/// A Sphere is defined by its m_center point and its radius.
class Sphere : public Primitive {

    protected:
        /// Fake a static initializer.
        class StaticInit {
            public:
                /// Register material with the factory
                StaticInit() {
                    log_debug("Registering 'sphere'");
                    PrimitiveFactory::get_instance()->registerPlugin("sphere", sigc::ptr_fun(new_sphere));
                    log_debug("Sphere registered.");
                }
        };
        /// Force static initialization.
        static StaticInit m_init;

        /// The radius of the sphere.
        double m_radius;

    public:
        /// Create a default sphere with a radius of 1.
        /*
        Sphere() : m_radius(1) {
            m_material = new SolidMaterial(false);
        }
        */

        /// Create a sphere with the default m_material.
        //template <class _T>
        Sphere( 
            Point3D center,
            double r)
            : m_radius(r)
        {
            log_debug("Creating a Sphere at (%0.2f, %0.2f, %0.2f) with a radius of %0.2f", center.x, center.y, center.z, r );

            m_center = center;

        }

        /// Clean up the sphere's resources.
        virtual ~Sphere();

        /// Determine if a ray intersects this object.
        ///
        /// Any point along a ray: @f[ p = u + dt @f]
        ///   Where @f$p@f$ is the point along the ray, @f$u@f$ is the ray's origin, @f$d@f$ is the
        ///   ray's direction and @f$t@f$ is how far down the ray the point is.
        ///
        /// The equation of a sphere is: @f[ |I - C|^2 = r^2 @f]
        /// Where @f$I@f$ is any point on the sphere's surface, @f$C@f$ is the sphere's m_center, and
        /// @f$r@f$ is the sphere's radius.
        ///
        /// We need to find all points where the ray intersects the sphere.  That is we need to find
        /// all places where the ray is on the surface of the sphere, so we substitute @f$p@f$ for
        /// @f$I@f$, yielding:
        ///
        /// @f[ |(u + dt) - C|^2 = r^2  @f]
        ///
        /// We can safely drop the absolute value, since it will be squared anyway.  Simplifying, we
        /// arrive at:
        ///
        /// - @f$ ((u - C) + dt)^2 = r^2  @f$
        /// - Where @f$ V = u - C @f$, yields:
        /// - @f$ (V + dt)^2 = r^2 @f$
        ///
        /// Further simplifying:
        /// - @f$ (V + dt)^2 - r^2 = 0 @f$
        /// - @f$ V^2 + Vdt + Vdt + d^2t^2 - r^2 = 0 @f$
        /// - @f$ V^2 + 2Vdt + d^2t^2 - r^2 = 0 @f$
        ///
        /// Arranging in a form more suitable for the quadratic equation:
        /// - @f$ d^2t^2 + 2Vdt + (V^2 - r^2) = 0 @f$
        ///
        /// Solving for @f$t@f$ using the quadratic equation @f$ t = \frac{-B \pm \sqrt{ B^2 - 4AC}}{2A} @f$:
        /// - @f$ A = d^2 @f$
        /// - @f$ B = 2Vd @f$
        /// - @f$ C = V^2 - r^2 @f$
        ///
        /// It should be noted @f$d^2@f$, @f$2Vd@f$, and @f$V^2@f$ are all dot products, which
        /// yield a single scalar value.  Accordingly:
        /// - @f$ A = d \cdot d @f$
        /// - @f$ B = 2V \cdot d @f$
        /// - @f$ C = V \cdot V - r^2 @f$
        ///
        /// Finally, The equation for @f$t@f$ is: @f[ t = \frac{-(2(V \cdot d)) \pm \sqrt{ (2(V \cdot d))^2 - 4(d \cdot d)(V \cdot V - r^2) }}{2(d \cdot d)} @f]
        ///
        /// Which can be further simplified to: @f[ t = \frac{-((V \cdot d)) \pm \sqrt{ ((V \cdot d))^2 - (d \cdot d)(V \cdot V - r^2) }}{(d \cdot d)} @f]
        ///
        /// We want to keep performance high, so we can take some shortcuts.  For example, we can
        /// disregard all non-real numbers.  This means that we can stop processing the equation
        /// when @f$2(V \cdot d)^2@f$ is less than @f$4(d \cdot d)(V \cdot V - r^2)@f$, since the
        /// square root of a negative number is not a real number.
        ///
        /// @param ray The ray we are testing to determine collision.
        /// @param t The z-depth where the ray collides with this sphere.
        /// @return True if the ray collides with this sphere.
        virtual bool collides_with(const Ray &ray, double &t) const;

        /// Return the surface normal of the sphere.  A sphere's surface normal is the vector
        /// from the center of the sphere to the intersection point, translated to the intersection
        /// point and normalized.
        virtual Ray get_normal(const Point3D &p) ;
};

Primitive * new_sphere(xmlNode * node) {
    log_debug("Entering new_sphere()");
    Sphere * rv = NULL;

    xml_properties props = get_properties(node);

    if ( props.empty() == false ) {
        double radius   = (double)strtod(props["radius"   ].c_str(), NULL);

        if(props.find("center") != props.end()) {
            rv = new Sphere(Point3D(props["center"]), radius); 
        }
        else {
            double x = (double)strtod(props["x"].c_str(), NULL);
            double y = (double)strtod(props["y"].c_str(), NULL);
            double z = (double)strtod(props["z"].c_str(), NULL);
            rv = new Sphere(Point3D(x, y, z), radius);
        }
    }
    else {
        log_err("No properties specified for <sphere> tag.");
    }

    log_debug("Leaving new_sphere()");
    return rv;
    //return dynamic_cast<Primitive *>(rv);
}

void delete_sphere(Sphere * sphere) {
    delete sphere, sphere = NULL;
}

#endif

