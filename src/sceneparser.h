/// @file sceneparser.h
/// @author Michael Brailsford
/// @date Fri Aug 31 00:17:16 -0500 2007
/// @version         : $Revision$
/// copyright       : (c) 2007 Michael Brailsford

#include <vector>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sigc++/sigc++.h>

#include "scene.h"

typedef sigc::signal<Renderable *, Scene *, xmlNode *> nodehandler_t ;

/// This class parses an XML scene definition file, and creates a Scene object based on the
/// definition.
class SceneParser : public sigc::trackable {
    private:
        /// Register the default node handlers.
        void register_default_handlers();

        /// Parse the &lt;light_sources&gt; XML node.
        ///
        /// @param scene The scene that the object will affect.
        /// @param node The xml node that contains configuration for this element
        Renderable * parse_light_sources(Scene * scene, xmlNode * node);
        ///
        /// @param scene The scene that the object will affect.
        /// @param node The xml node that contains configuration for this element
        Renderable * parse_objects(Scene * scene, xmlNode * node);
        ///
        /// @param scene The scene that the object will affect.
        /// @param node The xml node that contains configuration for this element
        Renderable * parse_meta(Scene * scene, xmlNode * node);
        ///
        /// @param scene The scene that the object will affect.
        /// @param node The xml node that contains configuration for this element
        Renderable * parse_camera(Scene * scene, xmlNode * node);
        ///
        /// @param scene The scene that the object will affect.
        /// @param node The xml node that contains configuration for this element
        Renderable * parse_bumpmaps(Scene * scene, xmlNode * node);
        ///
        /// @param scene The scene that the object will affect.
        /// @param node The xml node that contains configuration for this element
        Renderable * noop(Scene * scene, xmlNode * node);

        /// Parse a set of color nodes and return them in a map<char *, Color *>.
        ///
        /// @param scene The scene that the object will affect.
        /// @param node The xml node that contains configuration for this element
        Renderable * parse_colors(Scene * scene, xmlNode * node);
        ///
        /// @param scene The scene that the object will affect.
        /// @param node The xml node that contains configuration for this element
        Renderable * parse_materials(Scene * scene, xmlNode * node); 


    protected:
        /// This is our map of node names to node handlers.
        std::map<std::string, nodehandler_t> m_node_handlers;

        /// This is the name of the XML file which contains the definition of
        /// the scene.
        std::string m_scene_filename;

    public:
        /// Loads a scene from the XML file named by scene_file_name.
        ///
        /// @param scene_file_name This is the name of the file that contains the XML specification
        ///                        of the scene to be ray traced.
        SceneParser(const char * scene_file_name);

        //~SceneParser();

        /// Parse the XML document whose root is passed in.
        ///
        /// @param root The root of the document to parse.
        void parse(xmlNode * root);
};
