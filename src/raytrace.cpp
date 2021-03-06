/// @file raytrace.cpp
/// @author Michael Brailsford <brailsmt@yahoo.com>
/// @author Brandon Inman <brinman2002@yahoo.com>
/// @date Fri Mar 23 00:10:18 -0500 2007
/// &copy; 2007 Michael Brailsford

#include <pthread.h>
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
#include <semaphore.h>

#include "raytrace_defs.h"
#include "renderstatistics.h"
#include "renderable.h"
#include "ray.h"
#include "scene.h"
#include "color.h"
#include "matrix.h"

using std::vector;
using std::cout;
using std::endl;
using std::string;
using Magick::Image;
using Magick::ColorRGB;
using Magick::Exception;

RenderStatistics rt_info;

unsigned long trace_ray(ColorRGB &pixel, const Ray &ray, int depth);

sem_t thread_pool_semaphore;
pthread_mutex_t image_mutex = PTHREAD_MUTEX_INITIALIZER;

/// Pointer to the in-memory color data.
struct rgb * imgblob = NULL;

/// This is required to make the rendering multi-threaded.
struct thread_arg {
    const Camera * camera;
    Point3D screen_intersection;
    double dx;
    double dy;
    int y;
    int num_threads;
};

//{{{
void * shoot_ray(void * arg) {
    Scene * scene = Scene::get_instance();
    struct thread_arg * args = (struct thread_arg *)arg;
    Point3D screen_intersection = args->screen_intersection;
    const Camera * camera = args->camera;
    double dx    = args->dx;
    double dy    = args->dy;
    int y        = args->y;
    int nthreads = args->num_threads;

    int width = scene->get_viewport_pixel_width();
    //{{{
    for (int x = 0; x < width; ++x) {
        // Shoot a ray through the center of the pixel and the next pixel
        // and compare the color values, if they match, there is no need
        // to subdivide the pixel.  If they don't match, then subdivide.
        double modifier = 1.0;
        if(x == 0) {
            modifier = -1.0;
        }

        int primary_rays = 0;
        unsigned long secondary_rays = 0;
        Point3D p = Point3D(screen_intersection.x, screen_intersection.y, screen_intersection.z);
        primary_rays++;
        secondary_rays += trace_ray(imgblob[(y * width) + x], camera->ray_through(p), 0);

        struct rgb * color      = imgblob[(y * width) + x];
        struct rgb * prev_color = imgblob[(y * width) + (x - modifier)];


        //{{{
        if(!(color == prev_color)) {
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

                    // Trace the ray into the scene, recording the pixel's color value.
                    ColorRGB color;
                    primary_rays++;
                    secondary_rays += trace_ray(color, camera->ray_through(subpixel), 0);
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
            try {
                color = ColorRGB(red/colors_size, green/colors_size, blue/colors_size);
            }
            catch(Exception e) {
                color = ColorRGB(1,0,0);
                log_info("%s", e.what());
            }
        }
        //}}}

        rt_info.incrementRays(primary_rays, secondary_rays);

        //{{{
        if(nthreads > 1) {
            pthread_mutex_lock(&image_mutex);
            img->pixelColor(x, y, color);
            pthread_mutex_unlock(&image_mutex);

        }
        else {
            img->pixelColor(x, y, color);
        }
        //}}}

        screen_intersection.x += dx;

        rt_info.incrementRenderedPixels();
    }
    //}}}
    
    // Release this thread's semaphore
    sem_post(&thread_pool_semaphore);
    delete args, args = NULL;
}
//}}}

/// Trace a ray from the coordinate of the camera, through every pixel in the image.
///
/// @param pixel The pixel in the image which will be calculated by tracing the ray.
/// @param ray The ray to be traced.
/// @param depth The current depth in the recursion of rays traced.
//{{{
unsigned long trace_ray(struct rgb * pixel, const Ray &ray, int depth) {
    Renderable * primitive = NULL;
    double dist = INFINITY;
    Scene * scene = Scene::get_instance();
    int max_depth = scene->get_max_recurse_depth();
    unsigned long rays = 0;

    if (depth <= max_depth) {
        rays++;

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
                rays += trace_ray(pixel, Ray(intersection_point, reflect), depth);
                depth--;
            }
            //}}}
            // Trace the refracted ray, if the primitive is transparent.    {{{
            if (primitive->get_opacity(intersection_point) > OPAQUE) {
                depth++;
                rays += trace_ray(pixel, Ray(intersection_point, refract), depth);
                depth--;
            }
            //}}}
        }
    }

    return rays;
}
//}}}

/// Trace a ray for each pixel in image.
///
/// @param data Image imformation that will be populated by tracing the rays.
/// @param camera The origin of all rays shot into the scene.
//{{{
void trace_rays(const Camera & camera, int num_threads) {
    Scene * scene = Scene::get_instance();

    // This defines the window that the camera is looking out of.  It is a
    // window that is fixed at 10x10 units wide.  Each unit varies its
    // dimension based on the number of pixels used to paint the image on the
    // window.
    /// @todo Make the viewport size configurable to fit the natural scale of any
    /// image the user might want to render.
    double max_x = scene->get_window_width() * 0.5;
    double min_x = -max_x;
    double max_y = (scene->get_window_height() * 0.5) * ((double)scene->get_viewport_pixel_height() / (double)scene->get_viewport_pixel_width());
    double min_y = -max_y;

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

    // This is where the ray will intersect the viewing plane, the window that the 'camera' is
    // looking out of.
    Point3D screen_intersection(start_x, start_y, 0.0);

    for (int y = 0; y < scene->get_viewport_pixel_height(); ++y) {
        screen_intersection.x = start_x;

        struct thread_arg * arg = new thread_arg;
        arg->screen_intersection = screen_intersection;
        arg->camera  = &camera;
        arg->dx      = dx;
        arg->dy      = dy;
        arg->y       = y;
        arg->num_threads = num_threads;

        if(num_threads > 1) {
            log_info("Tracing row %d", y);
            // See if there are threads to still use and block until there are.
            sem_wait(&thread_pool_semaphore);

            // need:  camera, screen_intersection, dx, dy
            pthread_t render_thread;
            pthread_create(&render_thread, NULL, shoot_ray, (void *)arg);
            pthread_detach(render_thread);
        }
        else {
            shoot_ray((void *)arg);
        }

        screen_intersection.y += dy;
    }
}
//}}}

/// Track statistics.
/// {{{
void * update_stats(void *) {
    while(1) {
        int x, y;
        getyx(stdscr, y, x);
        long t = (long)difftime(time(NULL), rt_info.getStartTime());
        mvprintw(y - 1, 0, "Elapsed time:  %02i:%02i:%02i", t / 3600, (t / 60) % 60, t % 60);
        mvprintw(y, x + 2, "%02.2f%% done", 100 * (double)rt_info.getRenderedPixels() / (double)rt_info.getTotalPixels());
        move(y, x);
        refresh();

        // sleep for a 50,000 microseconds
        usleep(50000);
    }
}
//}}}

/// Print statistics about the render.
//{{{
void print_final_stats(string fname, time_t end) {
    int hours, minutes, seconds;
    int elapsed = end - rt_info.getStartTime();
    seconds = elapsed;
    hours = seconds / 3600;
    seconds %= 3600;
    minutes = seconds / 60;
    seconds %= 60;

    unsigned long traced_rays = rt_info.getTracedRays();
    cout << endl;
    cout << endl;
    cout << "Traced " << traced_rays << " light rays into the scene!" << endl;
    cout << endl;
    cout << endl;

    printf("Rendering %s took %i seconds (%02i:%02i:%02i)\n", fname.c_str(), elapsed, hours, minutes, seconds);

    cout.setf(cout.fixed);
    cout.precision(5);
    unsigned long primary_rays = rt_info.getPrimaryRays();
    cout << "Average rays per primary ray        :  " << (double)traced_rays / (double)primary_rays << endl;
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
    int num_threads = 1;
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
        {"scene",   1, NULL, 's'},
        {"output",  1, NULL, 'o'},
        {"threads", 1, NULL, 't'},
        {0, 0, 0, 0}
    };

    log_info("Processing options...\n");
    string outfname("");
    int opt_val = -2;
    while ( (opt_val = getopt_long (argc, argv, "s:o:t:", long_options, &option_index)) != -1 ) {
        switch (opt_val) {
        case 's':
            filename = optarg;
            break;
        case 'o':
            outfname = optarg;
            break;
        case 't':
            num_threads = atoi(optarg);
            break;
        }
    }

    unsigned long traced_rays;
    int x, y;
    initscr();
    getmaxyx(stdscr, y, x);
    mvprintw(y - 1, 0, "Tracing %s...", filename.c_str());

    // Initialize the semaphore.
    log_info("Number of threads:  %d\n", num_threads);
    if(sem_init(&thread_pool_semaphore, 0, num_threads) == -1) {
        return -1;
    }

    ////////////
    // Render //
    ////////////
    log_info("Reading %s...", filename.c_str());
    // Read the scene description XML file and build the Scene object.
    Scene * scene = Scene::get_instance(filename);

    rt_info.setTotalPixels(scene->get_viewport_pixel_width(), scene->get_viewport_pixel_height());
    if(outfname == "") {
        outfname = scene->get_output_filename();
    }

    // Trace the scene.
    log_info("Rendering...\n");

    pthread_t update_ui_thread;
    pthread_create(&update_ui_thread, NULL, update_stats, NULL);
    pthread_detach(update_ui_thread);

    imgblob = new struct rgb[rt_info.getTotalPixels() * sizeof(struct rgb)];
    memset(imgblob, 0x0, rt_info.getTotalPixels() * sizeof(struct rgb));
    trace_rays(scene->get_camera(), num_threads);

    // Create the Image object.
    Image img(scene->get_viewport_pixel_width(), scene->get_viewport_pixel_height(), "RGB", StorageType::IntegerPixel, imgblob);

    // Wait until the semaphore is finished being used, indicating all threads
    // have ended.
    int sval = 0;
    for(sem_getvalue(&thread_pool_semaphore, &sval); sval < num_threads; sem_getvalue(&thread_pool_semaphore, &sval)) {
        usleep(500);
    }

    // Save the image.
    log_info("Saving image...");
    img.write(outfname);
    log_info("Done saving image...");

    delete [] imgblob;

    // Do post render stuff.
    time_t end_time = time(NULL);
    int elapsed = end_time - start_time;
    endwin();

    cout << outfname << endl;
    print_final_stats(filename, end_time);

    log_info("******************  Exiting mbrt  ******************\n");
    closelog();
    exit(EXIT_SUCCESS);
}
//}}}
