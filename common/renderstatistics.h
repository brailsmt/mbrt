/* file name  : renderstatistics.h
 * authors    : Michael Brailsford
 * created    : Wed Nov 03 11:53:22 -0500 2010
 * copyright  : (c) 2010 Michael Brailsford
 */

#ifndef RENDERSTATISTICS_H
#define RENDERSTATISTICS_H

#include <pthread.h>

/// This struct contains information about the rendering process.
//{{{
class RenderStatistics {
    public:
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

        /// This the is the total number of pixels to be rendered.
        unsigned long total_pixels;

        /// The time in seconds since the Epoch that the render began.
        time_t start_time;

        /// Mutex to synchronize update operations.
        pthread_mutex_t stats_mutex;

        /// Initialize RenderStatistics.
        RenderStatistics() {
            start_time = time(NULL);
        }

        inline void setTotalPixels(int width, int height) { 
            total_pixels = width * height; 
        }

        inline void incrementRenderedPixels(int inc = 1) {
            pthread_mutex_lock(&stats_mutex);
            rendered_pixels += inc;
            pthread_mutex_unlock(&stats_mutex);
        }
        inline void incrementPrimaryRays(int inc = 1) { 
            pthread_mutex_lock(&stats_mutex);
            primary_rays += inc; 
            pthread_mutex_unlock(&stats_mutex);
        }
        inline void incrementSecondaryRays(int inc = 1) {
            pthread_mutex_lock(&stats_mutex);
            secondary_rays += inc; 
            pthread_mutex_unlock(&stats_mutex);
        }

        inline void incrementRays(int primary, int secondary) {
            pthread_mutex_lock(&stats_mutex);

            primary_rays   += primary;
            secondary_rays += secondary;

            pthread_mutex_unlock(&stats_mutex);
        }

        inline long getTracedRays()       { return primary_rays + secondary_rays; } 
        inline long getPrimaryRays()      { return primary_rays;       } 
        inline long getSecondaryRays()    { return secondary_rays;     } 
        inline long getTotalPrimaryRays() { return total_primary_rays; } 
        inline long getRenderedPixels()   { return rendered_pixels;    } 
        inline long getTotalPixels()      { return total_pixels;       } 
        inline time_t getStartTime()      { return start_time;         }
};
//}}}

#endif

