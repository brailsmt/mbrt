/// @file raytrace.cpp
/// @author Michael Brailsford <brailsmt@yahoo.com>
/// @date Fri Mar 23 00:10:18 -0500 2007
/// &copy; 2007 Michael Brailsford

#include "raytrace_defs.h"
#include "scene.h"
#include "imagewriter.h"

#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <getopt.h>
#include <iostream>

using std::cerr;
using std::endl;

struct raytrace_info rt_info;

//{{{
void print_stats(char * fname, int elapsed, long primary_rays, long traced_rays) {
    int hours, minutes, seconds;
    seconds = elapsed;
    hours = seconds / 3600;
    seconds %= 3600;
    minutes = seconds / 60;
    seconds %= 60;
    cerr << endl;
    cerr << endl;
    cerr << "Traced " << rt_info.traced_rays << " light rays into the scene!" << endl;
    cerr << endl;
    cerr << endl;

    cerr << "Rendering " << fname << " took " << elapsed;
    cerr << " seconds (" << hours << ":" << minutes << ":" << seconds << ")" << endl;

    cerr.setf(cerr.fixed);
    cerr.precision(5);
    cerr << "Average rays per primary ray      :  " << (double)traced_rays / (double)primary_rays << endl;
    cerr << "Average time per " << REPORT_FACTOR << " primary rays:  " << ((double)elapsed / (double)primary_rays) * REPORT_FACTOR << "s" << endl;
    cerr << "Average time per " << REPORT_FACTOR << " rays        :  " << ((double)elapsed / (double)traced_rays ) * REPORT_FACTOR << "s" << endl;
}
//}}}

#ifndef _UNITTEST 
//{{{
int main(int argc, char ** argv) {
    initscr();

    time_t start_time = time(NULL);
    char filename[256] = "scene.xml";
    char output[256] = "scene.ppm";
    srand(time(NULL));

    extern char *optarg;
    extern int optind, opterr, optopt;
    int option_index = 0;
    static struct option long_options[] = {
                                              {"scene",  1, NULL, 's'},
                                              {"output", 1, NULL, 'o'},
                                              {0, 0, 0, 0}
                                          };

    int opt_val = -2;
    while ( (opt_val = getopt_long (argc, argv, "s:o:", long_options, &option_index)) != -1 ) {
        switch (opt_val) {
        case 's':
            strncpy(filename, optarg, strlen(optarg));
            break;
        case 'o':
            strncpy(output, optarg, strlen(optarg));
            break;
        }
    }


    // Read the scene description XML file and build the Scene object.
    Scene * scene = Scene::get_instance(filename);

    Color * data = new Color[scene->get_viewport_pixel_height() * scene->get_viewport_pixel_width()];
    rt_info.total_primary_rays = ((scene->get_subpixel_sqrt() * scene->get_subpixel_sqrt()) * scene->get_viewport_pixel_width() * scene->get_viewport_pixel_height());

    unsigned long traced_rays;
    int x, y;
    getmaxyx(stdscr, y, x);
    mvprintw(y - 1, 0, "Tracing %s...", filename);
    traced_rays = trace_rays(data, scene->get_camera());

    ImageWriterFactory factory;
    ImageWriter * writer = factory(output);
    writer->write_image(data);
    delete writer, writer = NULL;

    delete [] data;

    time_t end_time = time(NULL);
    int elapsed = end_time - start_time;
    int primary_rays = 4 * scene->get_viewport_pixel_width() * scene->get_viewport_pixel_height();
    endwin();
    print_stats(filename, elapsed, primary_rays, traced_rays);

    exit(EXIT_SUCCESS);
}
//}}}
#endif
