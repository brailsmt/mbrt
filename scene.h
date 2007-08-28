
#ifndef SCENE_H
#define SCENE_H
#include "point3d.h"

#include <vector>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/xpath.h>

class Ray;
class Primitive;
class Point3D;
class Color;
class Sphere;

/// @todo The xML parsing needs to be completely reworked.  As it is currently written, this class
/// will not scale well and will be tightly coupled with the objects in the ray tracer.  This should
/// be rewritten using function pointers/libsigc to call the correct function based on the XML tag
/// found.  This class should delegate the actual parsing of the elements to the corresponding
/// classes.

//{{{
class Scene {
    protected:
        /// This is the instance of the Scene class.
        static Scene * inst;

        std::vector<Primitive *> scene;

        /// Parse the XML document whose root is passed in.
        ///
        /// @param root The root of the document to parse.
        void parse_document(xmlDocPtr root);

        /// Get the nodes from the root which match the xpath passed in.
        ///
        /// @param root The root node to be searched with the xpath.
        /// @param xpath The xpath to process.
        xmlXPathObjectPtr get_xpath_nodes(xmlDocPtr root, char *xpath);

        /// Parse the &lt;light_sources&gt; XML node.
        ///
        /// @param root The list of nodes contained within the &lt;light_sources&gt; node.
        void parse_light_sources(xmlDocPtr root);

        /// Parse a set of color nodes and return them in a map<char *, Color *>.
        std::map<std::string, Color *> parse_colors(xmlXPathObjectPtr color_nodes);

        /// Parse the properties and return them as a map<char *, char *>.
        std::map<std::string, std::string> get_properties(struct _xmlAttr * props);

        /// Return the node content from the xmlXPathObjectPtr, for node at index idx.
        ///
        /// @return The content of the node, or NULL.
        char * get_xpath_content(const xmlXPathObjectPtr xpath, int idx = 0);

        std::vector<Sphere *> parse_spheres(xmlXPathObjectPtr sphere_nodes, std::map<std::string, Color *> colors);

        void parse_objects(xmlDocPtr root);
        void parse_meta(xmlDocPtr root);
        void parse_camera(xmlDocPtr root);


        Point3D camera;
        int pixel_width, pixel_height;
        std::string output_filename;

        /// This number is squared and determines the number of subpixels will be in the render.
        int subpixels;

        /// Loads a scene from the XML file named by scene_file_name.
        ///
        /// @param scene_file_name This is the name of the file that contains the XML specification
        ///                        of the scene to be ray traced.
        Scene(const char * scene_file_name);


    public:
        /// Returns or creates the single instance of the scene definition.
        ///
        /// @param scene_file_name The name of the file which contains the XML definition of the
        ///                        scene.  This is ignored on all but the first use of
        ///                        get_intance().
        static Scene * get_instance(const char * scene_file_name = "");

        ~Scene();

        inline std::vector<Primitive *> * get_scene() { return &scene; }

        Primitive * find_collision(const Ray &ray, float &dist) const;

        Point3D get_camera ()             const { return camera;          } 
        std::string get_output_filename() const { return output_filename; } 
        int get_viewport_pixel_width()    const { return pixel_width;     } 
        int get_viewport_pixel_height()   const { return pixel_height;    } 
        int get_subpixel_sqrt()           const { return subpixels;       }
};
//}}}

#endif
