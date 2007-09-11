
#include "raytrace_defs.h"
#include "primitive.h"
#include "sphere.h"
#include "scene.h"
#include "material.h"
#include "sceneparser.h"
#include <iostream>
#include <cstdlib>

using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;

Scene * Scene::inst = NULL;

//{{{
Scene * Scene::get_instance(const char * scene_file_name) {
    if ( inst == NULL ) {
        cout << "get_instance()" << endl;
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
      output_filename("out.ppm"),
      subpixels(SUBPIXEL_SQRT)
{
}
//}}}
//{{{
Scene::~Scene() {
    scene.clear();
}
//}}}

//{{{
Primitive * Scene::find_collision(const Ray &ray, double &dist) const {
    vector<Primitive *>::const_iterator iter, end;
    Primitive * rv = NULL;

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
