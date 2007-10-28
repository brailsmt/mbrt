/// @file raytrace.cpp
/// @author Michael Brailsford <brailsmt@yahoo.com>
/// @author Brandon Inman <brinman2002@yahoo.com>
/// @date Fri Mar 23 00:10:18 -0500 2007
/// &copy; 2007 Michael Brailsford

#include <Magick++.h>
#include <cstdlib>
#include <ctime>
#include <dlfcn.h>
#include <getopt.h>
#include <glob.h>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <signal.h>
#include <string>

#include "raytrace_defs.h"
#include "imagewriter.h"
#include "renderable.h"
#include "ray.h"
#include "scene.h"
#include "color.h"

using std::vector;
using std::cout;
using std::endl;
using std::string;
using Magick::Image;
using Magick::Color;
using Magick::ColorRGB;

struct raytrace_info rt_info;

void trace_ray(ColorRGB &pixel, const Ray &ray, int depth);

/// Trace a ray for each pixel in image.
///
/// @param data Image imformation that will be populated by tracing the rays.
/// @param eye The origin of all rays shot into the scene.
//{{{
unsigned long trace_rays(Image & img, Point3D eye) {
    //The image canvas is located around the origin of world space coordinates (0,0,0)
    double min_x, max_x;
    double min_y, max_y;
    Scene * scene = Scene::get_instance();

    max_x = 15;
    min_x = -max_x;
    max_y = 15;
    min_y = -max_y;

    // dx and dy are the amount to add to each pixel to go to the next pixel
    double dx = (max_x - min_x) / scene->get_viewport_pixel_width();
    double dy = (max_y - min_y) / scene->get_viewport_pixel_height();

    // Start out in the middle of the top-most left-most pixel.
    double start_x = min_x + (dx * 0.5);
    double start_y = min_y + (dy * 0.5);

    /// @todo  Extract the algorithm part of this function.  Then change the
    /// algorithm used based on user input/configuration.  This will allow us
    /// to compare algorithms and provide choice for users and flexibility for
    /// future algorithms.

    // This is where the ray will intersect the viewing plane, the window that the 'eye' is
    // looking out of.
    Point3D screen_intersection(start_x, start_y, 0.0);
    rt_info.rendered_pixels = 0;
    for (int y = 0; y < scene->get_viewport_pixel_height(); ++y) {
        screen_intersection.x = start_x;
        for (int x = 0; x < scene->get_viewport_pixel_width(); ++x) {
            rt_info.rendered_pixels++;

            // Shoot a ray through the center of the pixel and the next pixel
            // and compare the color values, if they match, there is no need
            // to subdivide the pixel.  If they don't match, then subdivide.
            double modifier = 1.0;
            if(x + 1 == scene->get_viewport_pixel_width()) {
                modifier = -1.0;
            }

            ColorRGB color;
            Point3D p = Point3D(screen_intersection.x, screen_intersection.y, screen_intersection.z);
            Ray ray(eye, p - eye);
            trace_ray(color, ray, 0);

            p = Point3D(screen_intersection.x + (dx * modifier), screen_intersection.y, screen_intersection.z);
            ColorRGB next_color;
            Ray next_ray(eye, p - eye);
            trace_ray(next_color, next_ray, 0);

            if(!(color == next_color)) {
                vector<ColorRGB> colors;
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
                double sdx = dx / (2 * scene->get_subpixel_sqrt());
                double sdy = dy / (2 * scene->get_subpixel_sqrt());

                // Determine the y-coordinate for the top most subpixel.
                double sy = screen_intersection.y - (dy * 0.5) + (sdy * 0.5);
                for (int i = 0; i < scene->get_subpixel_sqrt(); i++) {

                    // Determine the x-coordinate for the left most subpixel.
                    double sx = screen_intersection.x - (dx * 0.5) + (sdx * 0.5);
                    for (int j = 0; j < scene->get_subpixel_sqrt(); j++) {
                        double sy_jitter = sy + jitter(sdy);
                        double sx_jitter = sx + jitter(sdx);

                        // THIS IS NOT FOCAL LENGTH!  DON'T CHANGE BACK.
                        Point3D subpixel(sx_jitter, sy_jitter, screen_intersection.z);

                        // Calculate the ray from the eye to the point where it
                        // intersect the viewport.
                        Ray ray(eye, subpixel - eye);

                        // Trace the ray into the scene, recording the pixel's color value.
                        ColorRGB color;
                        trace_ray(color, ray, 0);
                        colors.push_back(color);

                        sx += sdx;
                    }
                    sy += sdy;
                }

                // Average all the colors to get the color value at the window.
                double red, green, blue;
                red = green = blue = 0.0;
                vector<ColorRGB>::iterator iter, end;
                for (iter = colors.begin(), end = colors.end(); iter != end; iter++) {
                    red   += iter->red();
                    green += iter->green();
                    blue  += iter->blue();
                }
                int colors_size = colors.size();
                color = ColorRGB((red / colors_size), (green / colors_size), (blue / colors_size));
            }

            img.pixelColor(x, y, color);

            screen_intersection.x += dx;
        }

        screen_intersection.y += dy;
    }

    return rt_info.traced_rays;
}
//}}}

/// Trace a ray from the coordinate of the eye, through every pixel in the image.
///
/// @param pixel The pixel in the image which will be calculated by tracing the ray.
/// @param ray The ray to be traced.
/// @param depth The current depth in the recursion of rays traced.
//{{{
void trace_ray(ColorRGB &pixel, const Ray &ray, int depth) {
    Renderable * primitive = NULL;
    double dist = INF;
    Scene * scene = Scene::get_instance();
    int max_depth = scene->get_max_recurse_depth();

    if (depth <= max_depth) {

        if ( depth == 0 ) {
            rt_info.primary_rays++;
        }

        // Track statistics.    {{{
        rt_info.traced_rays++;
        if (rt_info.traced_rays % REPORT_FACTOR == 0) {
            int x, y;
            getyx(stdscr, y, x);
            long t = (long)difftime(time(NULL), rt_info.start_time);
            mvprintw(y - 1, 0, "Elapsed time:  %02i:%02i:%02i", t / 3600, (t / 60) % 60, t % 60);
            mvprintw(y, x + 2, "%02.2f%% done", 100 * (double)(rt_info.rendered_pixels) / (double)(scene->get_viewport_pixel_width() * scene->get_viewport_pixel_height()));
            move(y, x);
            refresh();
        }
        //}}}

        if ((primitive = scene->find_collision(ray, dist)) != NULL) {
            Vector dir = ray.direction();
            Point3D intersection_point = ray.origin() + (dir * dist);
            Vector reflect;
            Vector refract;

            // Determine the main color from directly striking the object.  {{{
            pixel += primitive->get_color_contribution(intersection_point, ray, reflect, refract);
            //}}}
            // Reflect the ray, if the surface is reflective.   {{{
            double reflection_coefficient = primitive->get_reflection(intersection_point);

            //TODO we seem to be missing an important part: applying the coefficient.
            if (reflection_coefficient > 0.0) {
                depth++;
                trace_ray(pixel, Ray(intersection_point, reflect), depth);
                depth--;
            }
            //}}}
            // Trace the refracted ray, if the primitive is transparent.    {{{
            if (primitive->get_opacity(intersection_point) > OPAQUE) {
                depth++;
                trace_ray(pixel, Ray(intersection_point, refract), depth);
                depth--;
            }
            //}}}
        }
    }
}
//}}}

/// Print statistics about the render.
//{{{
void print_stats(string fname, int elapsed, long primary_rays, long traced_rays) {
    int hours, minutes, seconds;
    seconds = elapsed;
    hours = seconds / 3600;
    seconds %= 3600;
    minutes = seconds / 60;
    seconds %= 60;
    cout << endl;
    cout << endl;
    cout << "Traced " << rt_info.traced_rays << " light rays into the scene!" << endl;
    cout << endl;
    cout << endl;

    printf("Rendering %s took %i seconds (%02i:%02i:%02i)\n", fname.c_str(), elapsed, hours, minutes, seconds);

    cout.setf(cout.fixed);
    cout.precision(5);
    cout << "Average rays per primary ray      :  " << (double)traced_rays / (double)primary_rays << endl;
    cout << "Average time per " << REPORT_FACTOR << " primary rays:  " << ((double)elapsed / (double)primary_rays) * REPORT_FACTOR << "s" << endl;
    cout << "Average time per " << REPORT_FACTOR << " rays        :  " << ((double)elapsed / (double)traced_rays ) * REPORT_FACTOR << "s" << endl;
}
//}}}

/// Load plugins.
//{{{
void load_plugins() {
    glob_t glob_data;
    glob("plugins/*.so", 0, NULL, &glob_data);

    char ** iter = glob_data.gl_pathv;
    for (; *iter != NULL ; ++iter) {
        // No need to keep the handle to the shared object;
        // we don't need to get any symbols from it 
        log_info("Attempting to load plugin '%s'...\n", *iter);
        printf("Attempting to load plugin '%s'...\n", *iter);
        if(dlopen(*iter, RTLD_NOW)) {
            log_info("Finished loading plugin '%s'\n", *iter);
            printf("Finished loading plugin '%s'\n", *iter);
        }
        else {
            log_crit("Failed to load plugin '%s' with error %s\n", *iter, dlerror());
            exit_mbrt(EXIT_FAILURE);
        }
    }

    globfree(&glob_data);
}
//}}}

/// Register signal handlers to ensure ncurses is shut down properly when the
/// process is terminated or crashes.
//{{{
void register_signal_handlers() {
    struct sigaction sigact;
    sigact.sa_handler = exit_mbrt;
    sigaction(15, &sigact, NULL);
    //sigaction(11, &sigact, NULL);
}
//}}}

//{{{
int main(int argc, char ** argv) {
    time_t start_time = time(NULL);
    string filename("scene.xml");
    srand(time(NULL));

    openlog("mbrt", LOG_CONS, LOG_SYSLOG);
    log_info("******************  Starting mbrt  ******************\n");

    // Do initialization stuff.
    register_signal_handlers();
    load_plugins();


    // Process command line arguments.
    extern char *optarg;
    extern int optind, opterr, optopt;
    int option_index = 0;
    static struct option long_options[] = {
        {"scene",  1, NULL, 's'},
        {"output", 1, NULL, 'o'},
        {0, 0, 0, 0}
    };

    string outfname("");
    int opt_val = -2;
    while ( (opt_val = getopt_long (argc, argv, "s:o:", long_options, &option_index)) != -1 ) {
        switch (opt_val) {
        case 's':
            filename = optarg;
            break;
        case 'o':
            outfname = optarg;
            break;
        }
    }
    unsigned long traced_rays;
    int x, y;
    initscr();
    getmaxyx(stdscr, y, x);
    mvprintw(y - 1, 0, "Tracing %s...", filename.c_str());


    ////////////
    // Render //
    ////////////
    // Read the scene description XML file and build the Scene object.
    Scene * scene = Scene::get_instance(filename);
    if(outfname == "") {
        outfname = scene->get_output_filename();
    }


    // Create the Image object.
    Image img(scene->get_geometry(), "red");

    // Trace the scene.
    traced_rays = trace_rays(img, scene->get_camera());

    // Save the image.
    img.write(outfname);


    // Do post render stuff.
    time_t end_time = time(NULL);
    int elapsed = end_time - start_time;
    int primary_rays = 4 * scene->get_viewport_pixel_width() * scene->get_viewport_pixel_height();
    endwin();
    cout << outfname << endl;
    print_stats(filename, elapsed, primary_rays, traced_rays);

    log_info("******************  Exiting mbrt  ******************\n");
    closelog();
    exit(EXIT_SUCCESS);
}
//}}}
