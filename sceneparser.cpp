/// @file sceneparser.cpp
/// @author Michael Brailsford
/// @date Fri Aug 31 00:42:47 -0500 2007
/// &copy; 2007 Michael Brailsford

#include "sceneparser.h"
#include "primitive.h"
#include "plugins/sphere.h"
#include "scene.h"

#include "materialfactory.h"

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using std::map;
using std::vector;
using std::string;

//{{{
SceneParser::SceneParser(const char * scene_filename)
    : m_node_handlers(),
      m_scene_filename(scene_filename)

{
    cout << "SceneParser()" << endl;
    register_default_handlers();

    cout << m_scene_filename << endl;
    xmlDoc * doc = xmlReadFile(m_scene_filename.c_str(), NULL, 0);

    if ( doc == NULL ) {
        cerr << "Could not read and parse " << m_scene_filename << "." << endl;
        exit(EXIT_FAILURE);
    }

    xmlNode * root = xmlDocGetRootElement(doc);
    if ( root == NULL ) {
        cerr << "Could not find root element in file " << m_scene_filename << "." << endl;
        exit(EXIT_FAILURE);
    }
    parse(root);

    xmlFreeDoc(doc);
    xmlCleanupParser();
    cout << "End SceneParser()" << endl;
}
//}}}

//{{{
void SceneParser::register_default_handlers() {
    m_node_handlers["meta"         ].connect(sigc::mem_fun(this, &SceneParser::parse_meta));
    m_node_handlers["camera"       ].connect(sigc::mem_fun(this, &SceneParser::parse_camera));
    m_node_handlers["light_sources"].connect(sigc::mem_fun(this, &SceneParser::parse_light_sources));
    m_node_handlers["objects"      ].connect(sigc::mem_fun(this, &SceneParser::parse_objects));
    m_node_handlers["sphere"       ].connect(sigc::mem_fun(this, &SceneParser::parse_sphere));
    m_node_handlers["colors"       ].connect(sigc::mem_fun(this, &SceneParser::parse_colors));
    m_node_handlers["materials"    ].connect(sigc::mem_fun(this, &SceneParser::parse_materials));
}
//}}}
//{{{
xmlXPathObjectPtr SceneParser::get_xpath_nodes(xmlDocPtr root, char *xpath) {

    xmlXPathContextPtr context;
    xmlXPathObjectPtr rv = NULL;

    context = xmlXPathNewContext(root);
    if (context == NULL) {
        cerr << "Error in xmlXPathNewContext" << endl;
        exit(EXIT_FAILURE);
    }
    else {
        rv = xmlXPathEvalExpression((xmlChar *)xpath, context);
        xmlXPathFreeContext(context);

        if (rv == NULL) {
            cerr << "Error in xmlXPathEvalExpression" << endl;
            exit(EXIT_FAILURE);
        }
        else if (xmlXPathNodeSetIsEmpty(rv->nodesetval)) {
            xmlXPathFreeObject(rv);
            cerr << "No result" << endl;
        }
    }

    return rv;
}
//}}}
//{{{
map<string, string> SceneParser::get_properties(xmlNode * node) {
    xmlAttr * props = node->properties;
    map<string, string> rv;
    _xmlAttr * cur = props;
    while ( cur != NULL ) {
        rv[(char *)cur->name] = (char *)cur->children->content;
        cur = cur->next;
    }

    return rv;
}
//}}}
//{{{
Primitive * SceneParser::parse_materials(Scene * scene, xmlNode * node) {
    xmlNode * child = node->children;

    // Parse all materials
    while(child != node->last) {
        if(strcmp((char *)child->name, "material") == 0) {
            map<string, string> props = get_properties(child);

            if ( props.empty() == false ) {
                Material * material = MaterialFactory::get_instance()->create(props["type"], props);
                if(material == NULL)
                {
                    std::cout << "ERROR: Material of type '" << props["type"] << "' unknown." << std::endl;
                    exit(1);
                }

                scene->add_material(props["name"], material);
            }
        }

        child = child->next;
    }

    return NULL;
}
//}}}
//{{{
Primitive * SceneParser::parse_colors(Scene * scene, xmlNode * node) {
    xmlNode * child = node->children;

    // Parse all colors and spheres which are the children of the node passed in.
    while(child != node->last) {
        if(strcmp((char *)child->name, "color") == 0) {
            map<string, string> props = get_properties(child);

            if ( props.empty() == false ) {
                int red   = strtol(props["red" ].c_str(), NULL, 0);
                int green = strtol(props["green"].c_str(), NULL, 0);
                int blue  = strtol(props["blue" ].c_str(), NULL, 0);

                scene->add_color(props["name"], new Color(red, green, blue));
            }
        }

        child = child->next;
    }

    return NULL;
}
//}}}
//{{{
Primitive * SceneParser::noop(Scene * scene, xmlNode * node) {
    return NULL;
}
//}}}
//{{{
Primitive * SceneParser::parse_sphere(Scene * scene, xmlNode * node) {
    cout << "Entering SceneParser::parse_sphere()" << endl;
    Sphere * rv = NULL;

    map<string, string> props = get_properties(node);

    if ( props.empty() == false ) {
        double x        = (double)strtod(props["x"        ].c_str(), NULL);
        double y        = (double)strtod(props["y"        ].c_str(), NULL);
        double z        = (double)strtod(props["z"        ].c_str(), NULL);
        double radius   = (double)strtod(props["radius"   ].c_str(), NULL);
        string color    = props["color"   ];
        string material = props["material"];

        rv = new Sphere(x, y, z, radius, color, material);
    }

    cout << "Leaving SceneParser::parse_sphere()" << endl;
    return dynamic_cast<Primitive *>(rv);
}
//}}}
//{{{
Primitive * SceneParser::parse_objects(Scene * scene, xmlNode * node) {
    cout << "Entering SceneParser::parse_objects()" << endl;
    /// @todo This is a near duplicate of parse_light_sources()
    xmlNode * child = node->children;

    child = node->children;
    while(child != node->last) {
        if(m_node_handlers.find((char *)child->name) != m_node_handlers.end()) {
            Primitive * prim = m_node_handlers[(char *)child->name].emit(scene, child);

            // Add the primitive to the scene
            scene->get_scene()->push_back(prim);
        }

        child = child->next;
    }

    cout << "Leaving SceneParser::parse_objects()" << endl;
    return NULL;
}
//}}}
//{{{
Primitive * SceneParser::parse_meta(Scene * scene, xmlNode * node) {
    cout << "Entering SceneParser::parse_meta()" << endl;
    scene->set_output_filename(m_scene_filename.replace(m_scene_filename.find(".xml"), 4, ".ppm"));

    xmlNode * child = node->children;
    while(child != node->last) {
        map<string, string> props = get_properties(child);

        if(strcmp((char *)child->name, "width") == 0) {
            scene->set_pixel_width(strtol(props["pixels"].c_str(), NULL, 0));
        }
        else if(strcmp((char *)child->name, "height") == 0) {
            scene->set_pixel_height(strtol(props["pixels"].c_str(), NULL, 0));
        }
        else if(strcmp((char *)child->name, "file") == 0) {
            scene->set_output_filename(props["name"]);
        }
        else if(strcmp((char *)child->name, "subpixels") == 0) {
            scene->set_subpixel_sqrt(strtol(props["square_of"].c_str(), NULL, 0));
        }

        child = child->next;
    }

    cout << "Leaving SceneParser::parse_meta()" << endl;
    return NULL;
}
//}}}
//{{{
Primitive * SceneParser::parse_camera(Scene * scene, xmlNode * node) {
    cout << "Entering SceneParser::parse_camera()" << endl;
    map<string, string> props = get_properties(node);
    scene->set_camera((double)strtod(props["x"].c_str(), NULL),
                      (double)strtod(props["y"].c_str(), NULL),
                      (double)strtod(props["z"].c_str(), NULL));

    cout << "Leaving SceneParser::parse_camera()" << endl;
    return NULL;
}
//}}}
//{{{
Primitive * SceneParser::parse_light_sources(Scene * scene, xmlNode * node) {
    cout << "Entering SceneParser::parse_light_sources()" << endl;
    /// @todo This is a near duplicate of parse_objects()
    xmlNode * child = node->children;

    child = node->children;
    while(child != node->last) {
        if(m_node_handlers.find((char *)child->name) != m_node_handlers.end()) {
            Primitive * prim = m_node_handlers[(char *)child->name].emit(scene, child);

            // Make the primitive a light source since it was in the <light_sources> tag.
            prim->set_is_light(true);

            // Add the primitive to the scene
            scene->get_scene()->push_back(prim);
        }

        child = child->next;
    }

    cout << "Leaving SceneParser::parse_light_sources()" << endl;
    return NULL;
}
//}}}
//{{{
void SceneParser::parse(xmlNode * node) {
    cout << "Entering SceneParser::parse()" << endl;

    if(strcmp((char *)node->name, "scene") == 0) {
        Scene * scene        = Scene::get_instance();
        xmlNode * child      = node->children;
       
        while(child != node->last) {
            if(m_node_handlers.find((char *)child->name) != m_node_handlers.end()) {
                m_node_handlers[(char *)child->name].emit(scene, child);
            }
            else {
                cout << "Cannot parse <" << child->name << "> nodes.  Skipping..." << endl;
            }

            child = child->next;
        }

        cout << "==================== DONE PARSING XML ==========================" << endl;
    }
    else {
        cout << "Expecting <scene> node, but got <" << node->name << ">." << endl;
    }

    cout << "Leaving SceneParser::parse()" << endl;
}
//}}}

