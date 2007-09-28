/// @file sceneparser.h
/// @author Michael Brailsford
/// @date Fri Aug 31 00:17:16 -0500 2007
/// @version         : $Revision$
/// copyright       : (c) 2007 Michael Brailsford

#include <vector>
#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>
#include <sigc++/sigc++.h>

#include "scene.h"

typedef sigc::signal<Primitive *, Scene *, xmlNode *> nodehandler_t ;

/// This class parses an XML scene definition file, and creates a Scene object based on the
/// definition.
class SceneParser : public sigc::trackable {
    private:
        /// Register the default node handlers.
        void register_default_handlers();

        /// Parse the &lt;light_sources&gt; XML node.
        ///
        /// @param root The list of nodes contained within the &lt;light_sources&gt; node.
        Primitive * parse_light_sources(Scene * scene, xmlNode * node);
        Primitive * parse_objects(Scene * scene, xmlNode * node);
        Primitive * parse_meta(Scene * scene, xmlNode * node);
        Primitive * parse_camera(Scene * scene, xmlNode * node);
        Primitive * noop(Scene * scene, xmlNode * node);

        /// Parse a set of color nodes and return them in a map<char *, Color *>.
        Primitive * parse_colors(Scene * scene, xmlNode * node);
        Primitive * parse_materials(Scene * scene, xmlNode * node); 


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

        /// Get the nodes from the root which match the xpath passed in.
        ///
        /// @param root The root node to be searched with the xpath.
        /// @param xpath The xpath to process.
        xmlXPathObjectPtr get_xpath_nodes(xmlDocPtr root, char *xpath);

        /// Parse the properties and return them as a map<string, string>.
        std::map<std::string, std::string> get_properties(xmlNode * node);

        /// Return the node content from the xmlXPathObjectPtr, for node at index idx.
        ///
        /// @return The content of the node, or NULL.
        char * get_xpath_content(const xmlXPathObjectPtr xpath, int idx = 0);

        Primitive * parse_sphere(Scene * scene, xmlNode * node);
};
