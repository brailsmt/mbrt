/// @file sceneparser.cpp
/// @author Michael Brailsford
/// @date Fri Aug 31 00:42:47 -0500 2007
/// &copy; 2007 Michael Brailsford

#include "sceneparser.h"
#include "renderable.h"
#include "scene.h"
#include "xml_util.h"
#include "raytrace_defs.h"

#include "pluginfactory.h"

#include <cstring>
#include <iostream>

using std::map;
using std::vector;
using std::string;

//{{{
SceneParser::SceneParser(string scene_filename)
    : m_node_handlers(),
      m_scene_filename(scene_filename)

{
    log_debug("SceneParser()\n");
    register_default_handlers();

    log_debug("%s\n", m_scene_filename.c_str());
    xmlDoc * doc = xmlReadFile(m_scene_filename.c_str(), NULL, 0);

    if ( doc == NULL ) {
        log_err("Could not read and parse %s!\n", m_scene_filename.c_str());
        exit_mbrt(EXIT_FAILURE);
    }

    xmlNode * root = xmlDocGetRootElement(doc);
    if ( root == NULL ) {
        log_err("Could not find root element in file %s!\n", m_scene_filename.c_str());
        exit_mbrt(EXIT_FAILURE);
    }
    parse(root);

    xmlFreeDoc(doc);
    xmlCleanupParser();
    log_debug("End SceneParser()\n");
}
//}}}

//{{{
void SceneParser::register_default_handlers() {
    m_node_handlers["meta"         ].connect(sigc::mem_fun(this, &SceneParser::parse_meta));
    m_node_handlers["camera"       ].connect(sigc::mem_fun(this, &SceneParser::parse_camera));
    m_node_handlers["light_sources"].connect(sigc::mem_fun(this, &SceneParser::parse_light_sources));
    m_node_handlers["objects"      ].connect(sigc::mem_fun(this, &SceneParser::parse_objects));
    m_node_handlers["materials"    ].connect(sigc::mem_fun(this, &SceneParser::parse_materials));
    m_node_handlers["bumpmaps"     ].connect(sigc::mem_fun(this, &SceneParser::parse_bumpmaps));
}
//}}}
//{{{
Renderable * SceneParser::parse_materials(Scene * scene, xmlNode * node) {
    xmlNode * child = node->children;

    // Parse all materials
    while(child != node->last) {
        if(strcmp((char *)child->name, "material") == 0) {
            map<string, string> props = get_properties(child);

            if ( props.empty() == false ) {
                Material * material = MaterialFactory::get_instance()->create(props["type"], props);
                if(material == NULL)
                {
                    log_crit("Material of type '%s' unknown!\n", props["type"].c_str());
                    exit_mbrt(EXIT_FAILURE);
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
Renderable * SceneParser::noop(Scene * scene, xmlNode * node) {
    return NULL;
}
//}}}
//{{{
Renderable * SceneParser::parse_objects(Scene * scene, xmlNode * node) {
    xmlNode * child = node->children;

    // Parse all objects.
    while(child != node->last) {
        log_debug("Renderable of type '%s'.\n", child->name);
        Renderable * prim = RenderableFactory::get_instance()->create((char *)child->name, child);
        if(prim == NULL) {
            log_warn("Renderable of type '%s' unknown...Skipping!\n", child->name);
        }
        else {
            prim->initialize(child);
            scene->add_primitive(prim);
        }

        child = child->next;
    }

    return NULL;
}
//}}}
//{{{
Renderable * SceneParser::parse_meta(Scene * scene, xmlNode * node) {
    log_debug("Entering SceneParser::parse_meta()\n");

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
        else if(strcmp((char *)child->name, "recurse") == 0) {
            scene->set_max_recurse_depth(strtol(props["max_depth"].c_str(), NULL, 0));
        }
        else if(strcmp((char *)child->name, "window") == 0) {
            scene->set_window_size(strtol(props["width"].c_str(), NULL, 0),
                    strtol(props["height"].c_str(), NULL, 0));
        }
        else if(strcmp((char *)child->name, "report") == 0) {
            scene->set_report_frequency(strtol(props["every"].c_str(), NULL, 0));
        }

        child = child->next;
    }

    log_debug("Leaving SceneParser::parse_meta()\n");
    return NULL;
}
//}}}
//{{{
Renderable * SceneParser::parse_camera(Scene * scene, xmlNode * node) {
    log_debug("Entering SceneParser::parse_camera()\n");
    map<string, string> props = get_properties(node);
    if(props.find("location") != props.end()) {
        Point3D coords(props["location"]);
        Point3D direction(props["look_at"]);

        // From the location of the camera and the look_at coordinate we can
        // determine the camera orientation.
        scene->set_camera(Camera(coords, direction));
    }
    else {
        Point3D coords((double)strtod(props["x"].c_str(), NULL),
                       (double)strtod(props["y"].c_str(), NULL),
                       (double)strtod(props["z"].c_str(), NULL));
        scene->set_camera(Camera(coords));
    }

    log_debug("Leaving SceneParser::parse_camera()\n");
    return NULL;
}
//}}}
//{{{
Renderable * SceneParser::parse_light_sources(Scene * scene, xmlNode * node) {
    log_debug("Entering SceneParser::parse_light_sources()\n");
    /// @todo This is a near duplicate of parse_objects()
    xmlNode * child = node->children;

    child = node->children;
    while(child != node->last) {
        log_debug("Renderable of type '%s'\n", child->name);
        Renderable * prim = RenderableFactory::get_instance()->create((char *)child->name, child);
        if(prim == NULL) {
            log_warn("Renderable of type '%s' unknown...Skipping!\n", child->name);
        }
        else {

            prim->initialize(child);
            prim->set_is_light(true);
            scene->add_primitive(prim);
        }

        child = child->next;
    }

    log_debug("Leaving SceneParser::parse_light_sources()\n");
    return NULL;
}
//}}}
//{{{
Renderable * SceneParser::parse_bumpmaps(Scene * scene, xmlNode * node) {
    // This function was copy/paste/modify two things
    // Sounds like a candidate for refactoring.
    
    xmlNode * child = node->children;

    // Parse all objects.
    while(child != node->last) {
        if(strcmp((char *)child->name, "bumpmap") == 0) 
        {
            map<string, string> props = get_properties(child);
            string type = props["type"];
            log_debug("BumpMap of type '%s'.\n", type.c_str());
            BumpMap * bmap = BumpMapFactory::get_instance()->create(type, child);
        if(bmap == NULL) {
                log_warn("BumpMap of type '%s' unknown...Skipping!\n", type.c_str());
        }
        else {
                scene->add_bumpmap((char *)(type.c_str()), bmap);
            }
        }

        child = child->next;
    }

    return NULL;
}
//}}}
//{{{
void SceneParser::parse(xmlNode * node) {
    log_debug("Entering SceneParser::parse()\n");

    if(strcmp((char *)node->name, "scene") == 0) {
        Scene * scene        = Scene::get_instance();
        xmlNode * child      = node->children;
       
        while(child != node->last) {
            if(m_node_handlers.find((char *)child->name) != m_node_handlers.end()) {
                m_node_handlers[(char *)child->name].emit(scene, child);
            }
            else {
                log_warn("Cannot parse <%s> nodes.  Skipping...", child->name);
            }

            child = child->next;
        }

        log_info("==================== DONE PARSING XML ==========================");
    }
    else {
        log_err("Expecting <scene> node, but got <%s>.", node->name);
    }

    log_debug("Leaving SceneParser::parse()\n");
}
//}}}

