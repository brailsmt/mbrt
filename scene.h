
#ifndef SCENE_H
#define SCENE_H
#include "point3d.h"

#include <map>
#include <vector>
#include <string>
#include "color.h"

class Ray;
class Primitive;
class Point3D;
class Color;
class Sphere;

/// @todo The XML parsing needs to be completely reworked.  As it is currently written, this class
/// will not scale well and will be tightly coupled with the objects in the ray tracer.  This should
/// be rewritten using function pointers/libsigc to call the correct function based on the XML tag
/// found.  This class should delegate the actual parsing of the elements to the corresponding
/// classes.

//{{{
class Scene {
    protected:
        /// This is the instance of the Scene class.
        static Scene * inst;

        /// This is the scene, which is a collection of Primitives.
        std::vector<Primitive *> scene;

        /// This is a map of all colors available for the scene.
        std::map<std::string, Color *> m_colors;

        /// This is the point which describes the camera or eye which is looking into the scene.
        Point3D camera;

        /// This is the width, in pixels, of the rendered image.
        int pixel_width;

        /// This is the height, in pixels, of the rendered image.
        int pixel_height;

        /// This is the name of the rendered image file.
        std::string output_filename;

        /// This number is squared and determines the number of subpixels to render.
        int subpixels;

        /// Loads a scene from the XML file named by scene_file_name.
        Scene();


    public:
        /// Returns or creates the single instance of the scene definition.
        ///
        /// @param scene_file_name The name of the file which contains the XML definition of the
        ///                        scene.  This is ignored on all but the first use of
        ///                        get_intance().
        static Scene * get_instance(const char * scene_file_name = "");

        /// Destructor.
        ~Scene();

        /// Return the scene's collection of Primitives.
        inline std::vector<Primitive *> * get_scene() { return &scene; }

        /// Determine which Primitive in the scene is intersected by the ray, if any.
        ///
        /// @param ray The ray that is being traced into the scene.
        /// @param dist [out] This is the distance along ray that the intersection occurs.
        ///
        /// @return The primitive intersected at distance indicated by dist.  If no Primitive is
        ///         intersected, this method returns NULL.
        Primitive * find_collision(const Ray &ray, double &dist) const;

        /// Return the point from which all rays originate.
        ///
        /// @return The point which all rays originate.
        Point3D get_camera () const { return camera; }
        void set_camera (double x, double y, double z) { camera = Point3D(x, y, z); }

        /// Return the filename for the rendered image.
        ///
        /// @return The rendered image filename.
        std::string get_output_filename() const { return output_filename; }
        void set_output_filename(std::string fname) { output_filename = fname; }

        /// Get the viewport width in pixels.
        int get_viewport_pixel_width() const { return pixel_width; }
        void set_pixel_width(int pxwidth) { pixel_width = pxwidth; }

        /// Get the viewport height in pixels.
        int get_viewport_pixel_height() const { return pixel_height; }
        void set_pixel_height(int pxheight) { pixel_height = pxheight; }

        /// Get the square root of the number of sibpixels in the image.
        int get_subpixel_sqrt() const { return subpixels; }
        void set_subpixel_sqrt(int subpx_sqrt) { subpixels = subpx_sqrt; }

        /// Return the color named by the parameter.  The color's name is
        /// defined in the scene definition file.
        Color * get_color(std::string name) { return m_colors[name]; }

        /// Add a color to the available colors.
        void add_color(std::string name, Color * color) { m_colors[name] = color; }
};
//}}}

#endif
