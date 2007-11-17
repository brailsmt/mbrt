/* file name  : polygon.h
 * authors    : Michael Brailsford
 * created    : Sun Oct 14 16:41:16 -0500 2007
 * copyright  : (c) 2007 Michael Brailsford
 */

#ifndef POLYGON_H
#define POLYGON_H

#include "raytrace_defs.h"
#include "xml_util.h"
#include "pluginfactory.h"
#include <vector>
#include "point3d.h"

class Renderable;
Renderable * new_polygon(xmlNode * node);

class Polygon : public Renderable {
    protected:
        /// Fake a static initializer.
        class StaticInit {
            public:
                /// Register material with the factory
                StaticInit() {
                    RenderableFactory::get_instance()->registerPlugin("polygon",  sigc::ptr_fun(new_polygon));
                    RenderableFactory::get_instance()->registerPlugin("triangle", sigc::ptr_fun(new_polygon));
                    RenderableFactory::get_instance()->registerPlugin("square",   sigc::ptr_fun(new_polygon));
                }
        };
        /// Force static initialization.
        static StaticInit m_init;

        /// The normal of a polygon is always the same so this stores it so we
        /// only have to calculate it once.
        Ray m_normal;
        float m_d;

        /// This is the list of vertices of this polygon.  All vertices in a
        /// polygon must be coplanar.
        std::vector<Point3D> m_vertices;

    public:
        /// Creates a new plane defined by three points in 3-space.
        Polygon(std::string color, std::string material, std::string bumpmap);

        ///// Creates a new plane defined by a point and a normal.
        //Polygon(Point3D p0, Ray normal);

        virtual ~Polygon() {}

        /// Does the ray passed in collide with this polygon?
        ///
        /// @param ray The ray which is being traced.
        /// @param t   This is z-depth at which the ray collides with the polygon.
        /// @return True if the ray collides with the polygon.
        virtual bool collides_with(const Ray &ray, double &t) const;

        /// Returns the surface normal at point p.
        virtual Ray get_normal(const Point3D &p) const;

        /// Adds the vertex to the polygon.  The vertex must be coplanar to be
        /// added.
        ///
        /// @param vertex The vertex to add to the polygon.
        /// @return True of this was a new vertex added to the polygon, false
        /// if the vertex already existed in the definition, or if the vertex
        /// is not coplanar with the other vertices.
        bool add_vertex(Point3D vertex);
};

//{{{
Renderable * new_polygon(xmlNode * node) {
    log_debug("Entering new_plane()");
    Polygon * rv = NULL;

    xml_properties props = get_properties(node);

    if ( props.empty() == false ) {
        rv = new Polygon( props["color"], 
                          props["material"],
                          props["bumpmap"]);

        // We need at least 3 vertices to define a polygon.
        int nvertices = 0;

        // Parse each vertex.
        xmlNode * child = node->children;
        while(child != node->last) {
            log_debug("Found %s.\n", child->name);

            if(strcmp((const char *)child->name, "vertex") == 0) {
                if(rv->add_vertex(parse_vertex(child))) {
                    nvertices++;
                }
            }

            child = child->next;
        }

        if(nvertices < 3) {
            log_err("Fewer than 3 vertices specified.  You must specify at least 3 vertices to define a polygon.");
            delete rv, rv = NULL;
        }
    }
    else {
        log_err("No properties specified for <polygon> tag.");
    }

    log_err("Leaving new_polygon()");
    return dynamic_cast<Renderable *>(rv);
}
//}}}
//{{{
void delete_polygon(Polygon * polygon) {
    delete polygon, polygon = NULL;
}
//}}}
#endif

