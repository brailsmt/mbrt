
#ifndef RAYTRACE_DEFS_H
#define RAYTRACE_DEFS_H

#include <Magick++.h>
#include <ncurses.h>
#include <time.h>
#include <syslog.h>

#include "point3d.h"
#include "scene.h"

/// Typical uchar typedef.
typedef unsigned char uchar;

/// The default height in pixels for the image.
static const int HEIGHT = 512;

/// The default width in pixels for the image.
static const int WIDTH = 512;

/// Inifinity for our purposes.
const double INF = 1000000000.0;

/// This determines how many iterations pass before reporting progress.
const int REPORT_FACTOR = 5000;


/// This is the square root for the the default number of subpixels.
const int SUBPIXEL_SQRT = 2;

/// The max value for a color triplet in the ppm output file.
const int PPM_MAX_VAL = 255;

/// Defines a completely opaque substance.
const double OPAQUE = 0.0;

/// Defines a completely transparent substance.
const double TRANSPARENT = 1.0;

/// The fields in this enum describe the sequence of color bytes in a color row
enum {
    RED = 0,
    GREEN,
    BLUE
};

/// Macro to log with the info severity.
#define log_info(fmt, ...)    syslog(LOG_INFO, fmt,##__VA_ARGS__)

/// Macro to log with the debug severity.
#define log_debug(fmt, ...)   syslog(LOG_DEBUG, fmt,##__VA_ARGS__)

/// Macro to log with the notice severity.
#define log_notice(fmt, ...)  syslog(LOG_NOTICE, fmt,##__VA_ARGS__)

/// Macro to log with the warning severity.
#define log_warn(fmt, ...) syslog(LOG_WARNING, fmt,##__VA_ARGS__)

/// Macro to log with the err severity.
#define log_err(fmt, ...)     syslog(LOG_ERR, fmt,##__VA_ARGS__)

/// Macro to log with the crit severity.
#define log_crit(fmt, ...)    syslog(LOG_CRIT, fmt,##__VA_ARGS__)

/// Macro to log with the alert severity.
#define log_alert(fmt, ...)   syslog(LOG_ALERT, fmt,##__VA_ARGS__)

/// Macro to log with the emerg severity.
#define log_emerg(fmt, ...)   syslog(LOG_EMERG, fmt,##__VA_ARGS__)

/// This struct contains information about the rendering process.
//{{{
struct raytrace_info {
    /// The number of rays traced.
    unsigned long traced_rays;

    /// The number of primary rays traced.
    unsigned long primary_rays;

    /// The number of secondary rays traced.
    unsigned long secondary_rays;

    /// The total number of primary rays.
    unsigned long total_primary_rays;

    /// Track the number of image pixels have been rendered.
    unsigned long rendered_pixels;

    /// The time in seconds since the Epoch that the render began.
    time_t start_time;

    /// Initialize the info struct.
    raytrace_info() :
            traced_rays(0),
            primary_rays(0),
            secondary_rays(0),
            total_primary_rays(0),
            start_time(time(NULL)) {}
};
//}}}

/// Generate a jitter within the range [-limit, limit).
//{{{
inline double jitter(double limit) {
    double rv = -limit;
    rv += drand48() * (2 * limit);
    return rv;
}
//}}}

/// Provide a way to cleanly shutdown ncurses and exit the application.
//{{{
inline void exit_mbrt(int code) {
    endwin();
    log_info("******************  Aborting mbrt with status (%i)  ******************\n", code);
    closelog();
    exit(code);
}
//}}}
#endif

// vim: ts=4 sw=4
