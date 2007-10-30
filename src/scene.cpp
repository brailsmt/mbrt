
#include "renderable.h"
#include "scene.h"
#include "material.h"
#include "sceneparser.h"

#include <iostream>
#include <cstdlib>
#include <sstream>

using std::string;
using std::vector;
using std::string;
using std::ostringstream;

Scene * Scene::inst = NULL;

//{{{
Scene * Scene::get_instance(string scene_file_name) {
    if ( inst == NULL ) {
        log_debug("get_instance()\n");
        inst = new Scene();
        SceneParser sp(scene_file_name);
    }

    return inst;
}
//}}}
//{{{
Scene::Scene() 
    : scene(),
      camera(),
      pixel_width(WIDTH),
      pixel_height(HEIGHT),
      output_filename("scene.png"),
      subpixels(SUBPIXEL_SQRT),
      max_depth(MAX_DEPTH)
{
}
//}}}
//{{{
Scene::~Scene() {
    scene.clear();
}
//}}}

//{{{
Renderable * Scene::find_collision(const Ray &ray, double &dist) const {
    vector<Renderable *>::const_iterator iter, end;
    Renderable * rv = NULL;

    for (iter = scene.begin(), end = scene.end(); iter != end; ++iter) {
        double d = dist;
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
std::string Scene::get_geometry() {
    ostringstream str;
    str << pixel_width << "x" << pixel_height;
    return str.str();
}
//}}}
