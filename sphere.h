/// @file sphere.h
/// @author Michael Brailsford <brailsmt@yahoo.com>
/// @date Sun Mar 25 03:08:28 -0500 2007
/// &copy; 2007 Michael Brailsford

#ifndef SPHERE_H
#define SPHERE_H

#include "primitive.h"
#include "xml_defs.h"
#include <vector>

using std::vector;

/// A Sphere is defined by its m_center point and its radius.
class Sphere : public Primitive {
    protected:
        /// The m_center point of the sphere.
        Point3D m_center;

        /// The radius of the sphere.
        float m_radius;

    public:
        Sphere()
                : // m_material(new Material(false)),
                m_center(Point3D()),
        m_radius(1) {
            m_material = new Material(false);
        }

        /// Create a sphere with the default m_material.
        template <class _T>
        Sphere( _T x, _T y, _T z, _T r)
                :  //m_material(new Material(false)),
                m_center(x, y, z),
        m_radius((float)r) {
            m_material = new Material(false);
        }

        /// Create a sphere.
        template <class _T>
        Sphere( Material * m, _T x, _T y, _T z, _T r)
                :  //
                m_center(x, y, z),
        m_radius((float)r) {
            if (m) {
                m_material = m;
            }
            else {
                // TODO: if a null material is passed in, this
                // may be an error condition. Consider bailing
                // app.
                m_material = new Material(false);
            }

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
        virtual bool collides_with(const Ray &ray, float &t) const;
        //// @f[ a = (\overrightarrow{R_o} \cdot \overrightarrow{R_o}) @f]
        //// @f[ b = 2 (\overrightarrow{R_o} \cdot \overrightarrow{m_center}) @f]
        //// @f[ c = (\overrightarrow{m_center} \cdot \overrightarrow{m_center}) - r^2 @f]
        ////
        //// @f[ S = N \cos(\theta) - \overrightarrow{R_{in}} @f]
        //// @f[ \cos(\theta) = \overrightarrow{R_{in}} \cdot \overrightarrow{radius} @f]
        //// @f[ \overrightarrow{R_{out}} = 2N\cos(\theta) - \overrightarrow{R_{in}} = 2N(N · \overrightarrow{R_{in}}) - \overrightarrow{R_{in}}  @f]
        ////
        //// Point on the ray:
        //// @f[ R_0 = [ x_0, y_0, z_0 ] @f]
        //// @f[ R_d = [ x_d, y_d, z_d ] @f]
        //// @f[ R(t) = R_0 + \hat{R_d} * t @f]
        ////

        const Point3D get_center() const { return m_center; }

        virtual Ray get_normal(const Point3D &p) ;

        virtual Color get_color_contribution(const Point3D &itersection, const Ray &ray, Vector &reflect, Vector &refract);

};

#endif

