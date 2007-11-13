
#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "raytrace_defs.h"

#include <map>
#include <vector>
#include <string>
#include <Magick++.h>


/// The max depth to which we will bounce rays.  After MAX_DEPTH reflections/refractions, stop.
const int MAX_DEPTH = 15;

class Ray;
class Renderable;
class Point3D;
class Material;
class BumpMap;

//{{{
/// Scene represents everything to be rendered.  This includes light sources,
/// the camera, the objects to render, as well as a dictionary of colors and
/// materials.
class Scene {
    protected:
        /// This is the instance of the Scene class.
        static Scene * inst;

        /// This is the scene, which is a collection of Renderables.
        std::vector<Renderable *> scene;

        /// This is a map of all materials available for the scene.
        std::map<std::string, Material *> m_materials;
        
        /// This is a map of bump maps available for the scene.
        std::map<std::string, BumpMap *> m_bumpmaps;
        
        /// This is the point which describes the camera or eye which is looking into the scene.
        Camera camera;

        /// This is the width, in pixels, of the rendered image.
        int pixel_width;

        /// This is the height, in pixels, of the rendered image.
        int pixel_height;

        /// This is the name of the rendered image file.
        std::string output_filename;

        /// This number is squared and determines the number of subpixels to render.
        int subpixels;

        /// This is the maximum levels of recursion allowed for this scene.
        int max_depth;

        /// This is the width and height of the window in world space.
        std::pair<double, double> window_dims;

        /// This is frequency with which progress will be reported.
        int report_freq;

        /// Loads a scene from the XML file named by scene_file_name.
        Scene();


    public:
        /// Returns or creates the single instance of the scene definition.
        ///
        /// @param scene_file_name The name of the file which contains the XML definition of the
        ///                        scene.  This is ignored on all but the first use of
        ///                        get_intance().
        static Scene * get_instance(std::string scene_file_name = "");

        /// Destructor.
        ~Scene();

        /// Return the scene's collection of Renderables.
        inline std::vector<Renderable *> * get_scene() { return &scene; }

        /// Determine which Renderable in the scene is intersected by the ray, if any.
        ///
        /// @param ray The ray that is being traced into the scene.
        /// @param dist [out] This is the distance along ray that the intersection occurs.
        ///
        /// @return The primitive intersected at distance indicated by dist.  If no Renderable is
        ///         intersected, this method returns NULL.
        Renderable * find_collision(const Ray &ray, double &dist) const;

        /// Return the point from which all rays originate.
        ///
        /// @return The point which all rays originate.
        Camera get_camera () const { return camera; }
        void set_camera (Camera _camera) { camera = _camera; }

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

        /// Get the max recursion depth for this image.
        int get_max_recurse_depth() const { return max_depth; }
        /// ~et the max recursion depth for this image.
        void set_max_recurse_depth(int recurse_depth) { max_depth = recurse_depth < MAX_DEPTH? recurse_depth: MAX_DEPTH; }

        /// Return the color named by the parameter.  The color's name is
        /// defined in the scene definition file.
        Magick::Color get_color(std::string name) { return Magick::Color(name); }

        /// Return the material named by the parameter.  The material is
        /// defined in the scene definition file.
        Material * get_material(std::string name) { return m_materials[name]; }

        /// Add a material to the available materials.
        void add_material(std::string name, Material * material) { m_materials[name] = material; }
        
        /// Add a primitive to the scene.
        void add_primitive(Renderable * prim) { scene.push_back(prim); }
        
        /// Add a bump map to the scene.
        void add_bumpmap(std::string name, BumpMap * bumpmap) { m_bumpmaps[name] = bumpmap; }
        
        /// Return the bumpmap named by the parameter.  The bumpmap is
        /// defined in the scene definition file.
        BumpMap * get_bumpmap(std::string name) { return m_bumpmaps[name]; }

        /// Return the size of the image as an X11 geometry string.  This is
        /// needed by ImageMagick.
        std::string get_geometry();

        /// Set the size of the window through which the camera looks at the
        /// scene.
        void set_window_size(double w, double h) {
            window_dims.first  = w;
            window_dims.second = h;
        }

        /// Return the window width.
        double get_window_width() { return window_dims.first; }
        /// Return the window height.
        double get_window_height() { return window_dims.second; }

        /// Set the frequency with which progress is reported in terms of
        /// rays.
        void set_report_frequency(int freq) {
            report_freq = freq;
        }

        int get_report_frequency() { return report_freq; }

};
//}}}

#endif
