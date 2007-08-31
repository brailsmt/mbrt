
#include "raytrace_defs.h"
#include "primitive.h"
#include "sphere.h"
#include "scene.h"
#include <iostream>
#include <cstdlib>

using std::string;
using std::vector;
using std::map;
using std::cout;
using std::cerr;
using std::endl;

Scene * Scene::inst = NULL;

//{{{
Scene * Scene::get_instance(const char * scene_file_name) {
    if ( inst == NULL ) {
        inst = new Scene(scene_file_name);
    }

    return inst;
}
//}}}
//{{{
Scene::Scene(const char * scene_file_name)
        : camera(),
        pixel_width(WIDTH),
        pixel_height(HEIGHT),
        output_filename("out.ppm"),
subpixels(SUBPIXEL_SQRT) {
    xmlDocPtr root = xmlParseFile(scene_file_name);

    if ( root == NULL ) {
        cerr << "Could not correctly parse " << scene_file_name << endl;
        exit(EXIT_FAILURE);
    }
    else {
        parse_document(root);
    }

    xmlFreeDoc(root);
    xmlCleanupParser();
}
//}}}
//{{{
Scene::~Scene() {
    scene.clear();
}
//}}}

//{{{
Primitive * Scene::find_collision(const Ray &ray, float &dist) const {
    vector<Primitive *>::const_iterator iter, end;
    Primitive * rv = NULL;

    for (iter = scene.begin(), end = scene.end(); iter != end; ++iter) {
        float d = dist;
        if ((*iter)->collides_with(ray, d)) {
            if ( d < dist + 0.00001 && d > -0.00001) {
                rv = *iter;
                dist = d;
            }
        }
    }
    return rv;
}
//}}}
//{{{
xmlXPathObjectPtr Scene::get_xpath_nodes(xmlDocPtr root, char *xpath) {

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
void Scene::parse_light_sources(xmlDocPtr root) {
    char * colors_xpath            = "//scene/light_sources/color";
    char * sphere_xpath            = "//scene/light_sources/sphere";
    xmlXPathObjectPtr sphere_nodes = get_xpath_nodes(root, sphere_xpath);
    xmlXPathObjectPtr color_nodes  = get_xpath_nodes(root, colors_xpath);

    // Parse all colors and spheres which are the children of the <light_sources> tag.
    map<string, Color *> color_map = parse_colors(color_nodes);
    vector<Sphere *> spheres = parse_spheres(sphere_nodes, color_map);

    // Make the sphere light sources since they are in the <light_sources> tag.
    vector<Sphere *>::iterator iter, end = spheres.end();
    for (iter = spheres.begin(); iter != end; ++iter) {
        (*iter)->set_is_light(true);
    }

    // Add the spheres to the scene.
    scene.insert(scene.end(), spheres.begin(), spheres.end());
}
//}}}
//{{{
map<string, string> Scene::get_properties(struct _xmlAttr * props) {
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
map<string, Color *> Scene::parse_colors(xmlXPathObjectPtr color_nodes) {
    map<string, Color *> rv;

    xmlNodeSetPtr nodes = color_nodes->nodesetval;
    for (int x = 0; x < nodes->nodeNr; ++x) {
        xmlNodePtr node_ptr = nodes->nodeTab[x];
        map<string, string> properties = get_properties(node_ptr->properties);

        if ( properties.empty() == false ) {
            int red   = strtol(properties["red" ].c_str(), NULL, 0);
            int green = strtol(properties["green"].c_str(), NULL, 0);
            int blue  = strtol(properties["blue" ].c_str(), NULL, 0);

            rv[properties["name"]] = new Color(red, green, blue);
        }
    }

    return rv;
}
//}}}
//{{{
std::vector<Sphere *> Scene::parse_spheres(xmlXPathObjectPtr sphere_nodes, std::map<std::string, Color *> colors) {
    vector<Sphere *> rv;
    xmlNodeSetPtr nodes = sphere_nodes->nodesetval;

    for (int n = 0; n < nodes->nodeNr; ++n) {
        xmlNodePtr node_ptr = nodes->nodeTab[n];
        map<string, string> properties = get_properties(node_ptr->properties);

        if ( properties.empty() == false ) {
            float x       = (float)strtod(properties["x"       ].c_str(), NULL);
            float y       = (float)strtod(properties["y"       ].c_str(), NULL);
            float z       = (float)strtod(properties["z"       ].c_str(), NULL);
            float radius  = (float)strtod(properties["radius"  ].c_str(), NULL);
            float opacity = (float)strtod(properties["opacity" ].c_str(), NULL);

            Color * clr = NULL;
            if ( colors.find(properties["color"]) != colors.end() ) {
                clr = colors[properties["color"]];
            }
            else {
                clr = new Color();
            }

            Material * m = new Material(clr);
            m->set_opacity(opacity);
            rv.push_back(new Sphere(m, x, y, z, radius));

            clr = NULL;
        }
    }

    return rv;
}
//}}}
//{{{
void Scene::parse_objects(xmlDocPtr root) {
    char * colors_xpath = "//scene/objects/color";
    char * sphere_xpath = "//scene/objects/sphere";
    xmlXPathObjectPtr sphere_nodes = get_xpath_nodes(root, sphere_xpath);
    xmlXPathObjectPtr color_nodes  = get_xpath_nodes(root, colors_xpath);

    // Parse all colors and spheres which are the children of the <light_sources> tag.
    map<string, Color *> color_map = parse_colors(color_nodes);
    vector<Sphere *> spheres       = parse_spheres(sphere_nodes, color_map);

    // Add the spheres to the scene.
    scene.insert(scene.end(), spheres.begin(), spheres.end());
}
//}}}
//{{{
void Scene::parse_meta(xmlDocPtr root) {
    char * width_xpath     = "//scene/meta/width[@pixels]";
    char * height_xpath    = "//scene/meta/height[@pixels]";
    char * filename_xpath  = "//scene/meta/file[@name]";
    char * subpixels_xpath = "//scene/meta/subpixels[@square_of]";

    char * tmp;
    tmp = get_xpath_content(get_xpath_nodes(root, width_xpath));
    pixel_width = tmp ? strtol(tmp, NULL, 0) : pixel_width;

    tmp = get_xpath_content(get_xpath_nodes(root, height_xpath));
    pixel_height = tmp ? strtol(tmp, NULL, 0) : pixel_height;

    tmp = get_xpath_content(get_xpath_nodes(root, subpixels_xpath));
    subpixels = tmp ? strtol(tmp, NULL, 0) : subpixels;

    tmp = get_xpath_content(get_xpath_nodes(root, filename_xpath));
    output_filename = tmp ? tmp : output_filename;
}
//}}}
//{{{
char * Scene::get_xpath_content(const xmlXPathObjectPtr xpath, int idx) {
    xmlNodeSetPtr nodesetptr = xpath->nodesetval;
    if (nodesetptr == NULL || nodesetptr->nodeNr <= idx || nodesetptr->nodeTab == NULL) {
        return NULL;
    }

    xmlNodePtr nodeptr = nodesetptr->nodeTab[idx];
    if (nodeptr->properties == NULL || nodeptr->properties->children == NULL) {
        return NULL;
    }

    return (char *)xpath->nodesetval->nodeTab[idx]->properties->children->content;
}
//}}}
//{{{
void Scene::parse_camera(xmlDocPtr root) {
    char * camera_xpath = "//scene/camera";

    xmlXPathObjectPtr nodes = get_xpath_nodes(root, camera_xpath);
    map<string, string> properties = get_properties(nodes->nodesetval->nodeTab[0]->properties);
    camera.x = (float)strtod(properties["x"].c_str(), NULL);
    camera.y = (float)strtod(properties["y"].c_str(), NULL);
    camera.z = (float)strtod(properties["z"].c_str(), NULL);
}
//}}}
//{{{
void Scene::parse_document(xmlDocPtr root) {
    parse_meta(root);
    parse_camera(root);
    parse_light_sources(root);
    parse_objects(root);
    cout << "==================== DONE PARSING XML ==========================" << endl;
}
//}}}
