#include "noise.h"
#include <syslog.h>
#include <iostream>
#if 10

void Noise::init_noise(int seed)
{
    srand(seed);
    int x1,y1,z1;
    for(int x = 0; x < MAX_NOISE; ++x)
    {
        for(int y = 0; y < MAX_NOISE; ++y)
        {
            for(int z = 0; z < MAX_NOISE; ++z)
            {

                m_noiseMatrix[x][y][z] =  rand();
                // Edges must be equal on each side for 
                // interpolation to work correctly.
                if(x == MAX_NOISE)
                    x1 = 0;
                else
                    x1 = x;

                if(y == MAX_NOISE)
                    y1 = 0;
                else
                    y1 = y;

                if(z == MAX_NOISE)
                    z1 = 0;
                else
                    z1 = z;

                m_noiseMatrix[x][y][z] = m_noiseMatrix[x1][y1][z1];
            } //for (z...
        }//for(y...
    }//for(x..
}
    
double Noise::get_noise(const Point3D& point) const
{
    // Temp variables for interpolation
    int p000,p001, p010,p011, p100,p101, p110,p111;
    int p00, p01,  p10, p11;
    int p0,  p1;
    int d00, d01;
    int d10, d11;
    int d0, d1;
    int d;

    double f_x =  fabs(point.x) ;
    double f_y =  fabs(point.y) ;
    double f_z =  fabs(point.z) ;


    // TODO: replace with "double modf(double value, int* whole)"
    //
    // Whole portion of coordinates, wraped to size of noise matrix
    int x = (int)floor(f_x) % MAX_NOISE;
    int y = (int)floor(f_y) % MAX_NOISE;
    int z = (int)floor(f_z) % MAX_NOISE;

    // Fractional part of coordinates
    double ox = f_x - floor(f_x);
    double oy = f_y - floor(f_y);
    double oz = f_z - floor(f_z);

    // Store each matrix point that surround the point
    // we are interpolating in temp vars
    // to keep calculations later from being cluttered.
    p000 = m_noiseMatrix[x   ][y   ][z   ];
    p001 = m_noiseMatrix[x   ][y   ][z+1 ];
    p010 = m_noiseMatrix[x   ][y+1 ][z   ];
    p011 = m_noiseMatrix[x   ][y+1 ][z+1 ];
    p100 = m_noiseMatrix[x+1 ][y   ][z   ];
    p101 = m_noiseMatrix[x+1 ][y   ][z+1 ];
    p110 = m_noiseMatrix[x+1 ][y+1 ][z   ];
    p111 = m_noiseMatrix[x+1 ][y+1 ][z+1 ];

    d00 = p100 - p000;
    d01 = p101 - p001;
    d10 = p110 - p010;
    d11 = p111 - p011;

    p00 = ( (int) floor(d00 * ox) ) + p000;
    p01 = ( (int) floor(d01 * ox) ) + p001;
    p10 = ( (int) floor(d10 * ox) ) + p010;
    p11 = ( (int) floor(d11 * ox) ) + p011;

    d0 = p10 - p00;
    d1 = p11 - p01;

    p0 = (int) floor(d0 * oy) + p00;
    p1 = (int) floor(d1 * oy) + p01;

    d = p1 - p0;

    return ((d * oz) + (p0 * 1.0)) / (RAND_MAX * 1.0);
}

#endif
