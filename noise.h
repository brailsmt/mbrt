#ifndef NOISE_H
#define NOISE_H

#include "point3d.h"

/// Number of points along each axis in the noise
/// matrix.  Increasing this value will reduce the
/// number of times the noise has to wrap around.
const int MAX_NOISE = 20;

class Noise
{
    public:

        /// Create noise with default seed.
        Noise() { init_noise(0); } 

        /// Create noise with specified seed.
        Noise(int seed) { init_noise(seed); } 
        
        /// Calculate noise value at given point in space.
        ///
        ///Based on a Pascal interpretation of 1985 Perlin noise function,
        /// which in turn was based on Alan Watt's Fundamentals of Three-
        /// Dimensional Computer Graphics. 

        double get_noise(const Point3D& point) const ;
    protected:
        /// Initialize noise matrix
        void init_noise(int seed);

        /// Noise matrix. Hold random values.  Exact noise
        /// value is interpolated from the values in this matrix.
        int m_noiseMatrix [MAX_NOISE][MAX_NOISE][MAX_NOISE];

};

#endif
