/* file name  : plane.h
 * authors    : Michael Brailsford
 * created    : Sat Oct 06 23:23:13 -0500 2007
 * copyright  : (c) 2007 Michael Brailsford
 */

#ifndef PLANE_H
#define PLANE_H

#include "raytrace_defs.h"
#include "xml_util.h"
#include "pluginfactory.h"

class Renderable;
Renderable * new_plane(xmlNode * node);

class Plane : public Renderable {
    protected:
        /// Fake a static initializer.
        class StaticInit {
            public:
                /// Register material with the factory
                StaticInit() {
                    log_debug("here");
                    RenderableFactory::get_instance()->registerPlugin("plane", sigc::ptr_fun(new_plane));
                }
        };
        /// Force static initialization.
        static StaticInit m_init;

        /// The normal of a plane is always the same so this stores it so we
        /// only have to calculate it once.
        Ray m_normal;
        Point3D m_point;
        double m_d;

    public:
        /// Creates a new plane defined by three points in 3-space.
        Plane(Point3D p0, Point3D p1, Point3D p2, std::string color, std::string material, std::string bumpmap);

        ///// Creates a new plane defined by a point and a normal.
        //Plane(Point3D p0, Ray normal);

        virtual ~Plane() {}

        /// Does the ray passed in collide with this plane?
        ///
        /// @param ray The ray which is being traced.
        /// @param t   This is z-depth at which the ray collides with the plane.
        /// @return True if the ray collides with the plane.
        virtual bool collides_with(const Ray &ray, double &t) const;

        /// Returns the surface normal at point p.
        virtual Ray get_normal(const Point3D &p);
};

Renderable * new_plane(xmlNode * node) {
    log_debug("Entering new_plane()");
    Plane * rv = NULL;

    xml_properties props = get_properties(node);

    if ( props.empty() == false ) {
        rv = new Plane(
                        props["p0"], 
                        props["p1"],
                        props["p2"], 
                        props["color"], 
                        props["material"],
                        props["bumpmap"]);
    }
    else {
        log_err("No properties specified for <plane> tag.");
    }

    log_err("Leaving new_plane()");
    return dynamic_cast<Renderable *>(rv);
}

void delete_plane(Plane * plane) {
    delete plane, plane = NULL;
}
#endif

