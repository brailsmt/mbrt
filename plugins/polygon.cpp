/* file name  : staticinit.cpp
 * author     : Michael Brailsford
 * created    : Sun Oct 14 17:29:40 -0500 2007
 * copyright  : 2007 Michael Brailsford
 */

#include "polygon.h"
#include "scene.h"
#include <math.h>
#include <algorithm>

// This actually makes the static init work.
Polygon::StaticInit Polygon::m_init;

//{{{
Polygon::Polygon(std::string color, std::string material, std::string bumpmap)
    : m_normal(),
      m_vertices()
{
    Scene * scene = Scene::get_instance();
    m_material    = scene->get_material(material);
    m_bumpmap     = scene->get_bumpmap(bumpmap);
}
//}}}

//{{{
bool Polygon::add_vertex(Point3D vertex) {
    if(find(m_vertices.begin(), m_vertices.end(), vertex) == m_vertices.end()) {
        if(m_vertices.size() >= 3) {
            // From http://mathworld.wolfram.com/Coplanar.html:
            // Coplanarity is equivalent to the statement that the pair of lines
            // determined by the four points are not skew, and can be equivalently
            // stated in vector form as
            // (x_2-x_0).[(x_1-x_0)x(x_3-x_2)]==0.
            //
            // [where x_3 is the new vertex to be checked for coplanarity]
            if(dot_product(m_vertices[2] - m_vertices[0], cross_product((m_vertices[1] - m_vertices[0]), (vertex - m_vertices[2]))) == 0) {
                return false;
            }
            else {
                log_warn("mbrt does not support Polygons with more than 3 vertices.");
            }
        }

        m_vertices.push_back(vertex);

        // Calcluate the normal as soon as we can.
        if(m_vertices.size() == 3) {
            m_normal = Ray(m_vertices[0], cross_product((m_vertices[1] - m_vertices[0]), (m_vertices[2] - m_vertices[0])));
            m_d      = dot_product(m_vertices[0], m_normal.direction());
        }
        return true;
    }
    else {
        // The vertex already exists in the vertex list.
        return false;
    }
}
//}}}

//{{{
bool Polygon::collides_with(const Ray &ray, double &t) const {
    // Start by testing the ray to see if it intersects with plane which
    // contains the polygon.
    Point3D orig = ray.origin();
    Vector dir = ray.direction();

    double denominator = dot_product(dir, m_normal.direction());
    if(denominator > -0.0001 && denominator < 0.0001) {
        return false;
    }

    double numerator = m_d - dot_product(orig, m_normal.direction());
    if(numerator > -0.0001 && numerator < 0.0001) {
        return false;
    }

    t = numerator / denominator;
    if(t <= 0) {
        return false;
    }

    // Now we need to project the polygon onto a 2D plane to apply the Jordan
    // Curve Theorem, which is a really fancy way of saying we shoot a ray in
    // an arbitrary direction from the point of intersection and count how many
    // times that ray intersects the polygon boundary.  If the number is odd,
    // the point is inside the polygon, if the number is even then the point
    // is outside the polygon.

    // We first need to point the point where the intersects the plane.
    Point3D intersection_point = ray.origin() + (ray.direction() * t);

    // This portion of the code was lifted from
    //    http://www.softsurfer.com/Archive/algorithm_0105/algorithm_0105.htm
    // And is subject to the following copyright notice:
    // Copyright 2001, softSurfer (www.softsurfer.com)
    // This code may be freely used and modified for any purpose
    // providing that this copyright notice is included with it.
    // SoftSurfer makes no warranty for this code, and cannot be held
    // liable for any real or imagined damage resulting from its use.
    // Users of this code must verify correctness for their application.
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // THIS CODE IS ONLY VALID FOR TRIANGLES, IT DOES NOT GENERLIZE TO N SIDED
    // POLYGONS WHERE N > 3
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
    // is intersection_point inside the triangle?
    // get triangle edge vectors and plane normal
    Vector u, v, n;
    u = m_vertices[1] - m_vertices[0];   // This is an edge vector of the polygon
    v = m_vertices[2] - m_vertices[0];   // This is another edge vector of the polygon
    n = cross_product(u, v);                           // normal 

    float uu, uv, vv, wu, wv, D;
    uu = dot_product(u,u);
    uv = dot_product(u,v);
    vv = dot_product(v,v);
    Vector w = intersection_point - m_vertices[0];
    wu = dot_product(w,u);
    wv = dot_product(w,v);
    D = uv * uv - uu * vv;

    // get and test parametric coords
    float s, _t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)         // intersection_point is outside T
        return false;
    _t = (uv * wu - uu * wv) / D;
    if (_t < 0.0 || (s + _t) > 1.0) // intersection_point is outside T
        return false;

    return true;                    // intersection_point is in T

}
//}}}
// This is an incomplete general solution to determine if a point is inside an N sided polygon, where N > 3      {{{
#if 0
    // Determine the coordinate with greatest magnitude and "project" the
    // polygon onto the 2D plane by dropping that largest coord from
    // intersection_point and all the vertices.  This is odd, but correct.
    // We also transform all vertices and the intersection to the origin,
    // where the intersection point will be at the origin.
    Point3D projected_intersection(0, 0, 0);
    vector<Point3D> projection;
    vector<Point3D>::iterator iter = m_vertices.begin();
    vector<Point3D>::iterator end  = m_vertices.end();
    if( fabs(intersection_point.x) > fabs(intersection_point.y) && fabs(intersection_point.x) > fabs(intersection_point.z)) {
        for(; iter != end; ++iter) {
            projection.push_back(Point3D(iter->y - intersection_point.y , iter->z - intersection_point.z, 0));
        }
    }
    else if( fabs(intersection_point.y) > fabs(intersection_point.x) && fabs(intersection_point.y) > fabs(intersection_point.z)) {
        for(; iter != end; ++iter) {
            projection.push_back(Point3D(iter->x - intersection_point.x, iter->z - intersection_point.z, 0));
        }
    }
    else if( fabs(intersection_point.z) > fabs(intersection_point.x) && fabs(intersection_point.z) > fabs(intersection_point.y)) {
        for(; iter != end; ++iter) {
            projection.push_back(Point3D(iter->x - intersection_point.x, iter->y - intersection_point.y, 0));
        }
    }

    // Our polygon is now projected onto the XY plane, and the intersection
    // point is at (0, 0), now determine how many times the ray from the
    // origin along the +X axis intersects the polygon.  This amounts to
    // counting the number of times the line segments which make up the
    // polygon cross the X axis.
    int ncross = 0;
    vector<Point3D>::iterator iter = projection.begin();
    vector<Point3D>::iterator end  = projection.end();
    Point3D prev_vertex = *(iter++);
    for(; iter != end; ++iter) {
        // the line segment goes from prev_vertex --> *iter
        // We have to account for the following cases
        // 1)  prev_vertex's x and y coords are both < 0
        //
        // 2)  prev_vertex's x and y coords are both > 0
        // 3)  prev_vertex's x coord is < 0 and y coord > 0
        // 3)  prev_vertex's x coord is < 0 and y coord > 0
        if(prev_vertex.y > 0)

                prev_vertex = *iter;
    }

    return true;
#endif
    //}}}

//{{{
Ray Polygon::get_normal(const Point3D &p) {
    return Ray(p, m_normal.direction() );
}
//}}}
