
#ifndef RAYTRACE_DEFS_H
#define RAYTRACE_DEFS_H

#include "point3d.h"
#include "color.h"
#include "scene.h"
#include <ncurses.h>
#include <time.h>

/// Typical uchar typedef.
typedef unsigned char uchar;

/// The default height in pixels for the image.
static const int HEIGHT = 512;

/// The default width in pixels for the image.
static const int WIDTH  = 512;

/// Inifinity for our purposes.
const float INF         = 1000000000.0f;

/// This determines how many iterations pass before reporting progress.
const int REPORT_FACTOR = 5000;

/// This is the square root for the the default number of subpixels.
const int SUBPIXEL_SQRT = 2;

/// The max value for a color triplet in the ppm output file.
const int PPM_MAX_VAL   = 255;

/// The max depth to which we will bounce rays.  After MAX_DEPTH reflections/refractions, stop.
const int MAX_DEPTH     = 15;

/// Defines a completely opaque substance.
const float OPAQUE      = 0.0f;

/// Defines a completely transparent substance.
const float TRANSPARENT = 1.0f;

/// The fields in this enum describe the sequence of color bytes in a color row
enum {
    RED = 0,
    GREEN,
    BLUE
};

/// This struct contains information about the rendering process.
struct raytrace_info {
    /// The number of rays traced.
    unsigned long traced_rays;

    /// The number of primary rays traced.
    unsigned long primary_rays;

    /// The number of secondary rays traced.
    unsigned long secondary_rays;

    /// The total number of primary rays.
    unsigned long total_primary_rays;

    /// The time in seconds since the Epoch that the render began.
    time_t start_time;

    /// Initialize the info struct.
    raytrace_info() :
        traced_rays(0),
        primary_rays(0),
        secondary_rays(0),
        total_primary_rays(0),
        start_time(time(NULL))
    {
    }
};

/// Trace a ray for each pixel in image.
///
/// @param data Image imformation that will be populated by tracing the rays.
/// @param eye The origin of all rays shot into the scene.
unsigned long trace_rays( Color * data, Point3D eye);

/// Trace a ray from the coordinate of the eye, through every pixel in the image.
///
/// @param pixel The pixel in the image which will be calculated by tracing the ray.
/// @param ray The ray to be traced.
/// @param depth The current depth in the recursion of rays traced.
void trace_ray( Color &pixel, const Ray &ray, int depth);

/// Generate a jitter within the range [-limit, limit).
//{{{
inline float jitter(float limit) {
    float rv = -limit;
    rv += drand48() * (2 * limit);
    return rv;
}
//}}}
#endif

// vim: ts=4 sw=4
