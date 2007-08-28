
#include "raytrace_defs.h"
#include "primitive.h"
#include "ray.h"
#include <iostream>
#include "sphere.h"
#include "scene.h"
#include <ncurses.h>

using std::vector;
using std::cerr;
using std::cout;
using std::endl;

extern struct raytrace_info rt_info;

//{{{
void trace_ray(Color &pixel, const Ray &ray, int depth) {
    Primitive * primitive = NULL;
    float dist = INF;
    Scene * scene = Scene::get_instance();

    if(depth <= MAX_DEPTH) {

        if( depth == 0 ) {
            rt_info.primary_rays++;
        }

        // Track statistics.    {{{
        rt_info.traced_rays++;
        if (rt_info.traced_rays % REPORT_FACTOR == 0) {
            int x, y;
            getyx(stdscr, y, x);
            long t = (long)difftime(time(NULL), rt_info.start_time);
            mvprintw(y-1, 0, "Elapsed time:  %02i:%02i:%02i", t/3600, (t/60)%60, t%60);
            mvprintw(y, x+2, "%02.2f%% done", 100 * (float)rt_info.primary_rays / (float)rt_info.total_primary_rays);
            move(y, x);
            refresh();
        }
        //}}}

        if((primitive = scene->find_collision(ray, dist)) != NULL) {
            Vector dir = ray.direction();
            Point3D intersection_point = ray.origin() + (dir * dist);
            Vector reflect;
            Vector refract;

            // Determine the main color from the directly striking the object.  {{{
            pixel += primitive->get_color_contribution(intersection_point, ray, reflect, refract);
            //}}}
            // Reflect the ray, if the surface is reflective.   {{{
            float reflection_coefficient = primitive->get_reflection();
            if(reflection_coefficient > 0.0f) {
                depth++;
                trace_ray(pixel, Ray(intersection_point, reflect), depth);
                depth--;
            }
            //}}}
            // Trace the refracted ray, if the primitive is transparent.    {{{
            if(primitive->get_opacity() > OPAQUE) {
                depth++;
                trace_ray(pixel, Ray(intersection_point, refract), depth);
                depth--;
            }
            //}}}
        }
    }
}
//}}}
//{{{
unsigned long trace_rays(Color * data, Point3D eye) {
    //The image canvas is located around the origin of world space coordinates (0,0,0)
    float min_x, max_x;
    float min_y, max_y;
    Scene * scene = Scene::get_instance();

    max_x = 15;
    min_x = -max_x;
    max_y = 15;
    min_y = -max_y;

    // dx and dy are the amount to add to each pixel to go to the next pixel
    float dx = (max_x - min_x) / scene->get_viewport_pixel_width();
    float dy = (max_y - min_y) / scene->get_viewport_pixel_height();

    // Start out in the middle of the top-most left-most pixel.
    float start_x = min_x + (dx * 0.5);
    float start_y = min_y + (dy * 0.5);

    // This is where the ray will intersect the viewing plane, the window that the 'eye' is
    // looking out of.
    Point3D screen_intersection(start_x, start_y, 0.0f);
    for (int y = 0; y < scene->get_viewport_pixel_height(); ++y) {
        screen_intersection.x = start_x;
        for (int x = 0; x < scene->get_viewport_pixel_width(); ++x) {
            // Implement stochastic anti-aliasing.  Which is just a fancy way
            // to say break up the pixel into subpixels, determine the center
            // point of the subpixel, add a random offset and shoot a ray
            // through it.

            // dx and dy are the distance from the center of this pixel to the center of the next
            // pixel.  dx/2 and dy/2 are the distance from the center of this pixel to the boundary
            // of this pixel.  So the pixel is bounded by (x - dx/2, y - dy/2) and (x + dx/2, y + dy/2).
            //
            // To break this pixel further into subpixels, start at (x-dx/2, y - dy/2), or the top
            // left corner of the pixel.  To this add dx/2 divided by the number of x subpixels, and
            // dy/2 divided by the number of y subpixels.  This determines the center of the top
            // most subpixel.  From that point, add dx/number of x subpixels to reach each
            // successive pixel.
            
            // Determine the subpixel dx and dy values, these are used to
            // shift the screen intersection within each pixel
            float sdx = dx / (2 * scene->get_subpixel_sqrt());
            float sdy = dy / (2 * scene->get_subpixel_sqrt());

            vector<Color> colors;
            // Determine the y-coordinate for the top most subpixel.
            float sy = screen_intersection.y - (dy * 0.5) + (sdy * 0.5);
            for(int i = 0; i < scene->get_subpixel_sqrt(); i++) {

                // Determine the x-coordinate for the left most subpixel.
                float sx = screen_intersection.x - (dx * 0.5) + (sdx * 0.5);
                for(int j = 0; j < scene->get_subpixel_sqrt(); j++) {
                    float sy_jitter = sy + jitter(sdy);
                    float sx_jitter = sx + jitter(sdx);

                    Point3D subpixel(sx_jitter, sy_jitter, screen_intersection.z);
                    Ray ray(eye, subpixel - eye);

                    Color color;
                    trace_ray(color, ray, 0);
                    colors.push_back(color);

                    sx += sdx;
                }
                sy += sdy;
            }

            // Average all the colors to get the color value at the window.
            float red, green, blue;
            red = green = blue = 0.0f;
            vector<Color>::iterator iter, end;
            for(iter = colors.begin(), end = colors.end(); iter != end; iter++) {
                red   += iter->red;
                green += iter->green;
                blue  += iter->blue;
            }
            data[(y*scene->get_viewport_pixel_width()) + x] = Color((red/colors.size()), (green/colors.size()), (blue/colors.size()));

            screen_intersection.x += dx;
        }

        screen_intersection.y += dy;
    }

    cout << endl;
    return rt_info.traced_rays;
}
//}}}


// vim: ts=4 sw=4
