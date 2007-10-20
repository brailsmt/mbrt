
#include "renderable.h"
#include "scene.h"
#include "material.h"
#include "sceneparser.h"
#include "color.h"
#include <iostream>
#include <cstdlib>

using std::string;
using std::vector;

Scene * Scene::inst = NULL;

//{{{
Scene * Scene::get_instance(const char * scene_file_name) {
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
      output_filename("scene.ppm"),
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
