#include "noisematerial.h"
#include <math.h>
#include <iostream>

NoiseMaterial::MaterialStaticInit NoiseMaterial::m_init;

void NoiseMaterial::init_noise()
{
    int x1,y1,z1;
    for(int x = 0; x < MAX_NOISE; ++x)
    {
        for(int y = 0; y < MAX_NOISE; ++y)
        {
            for(int z = 0; z < MAX_NOISE; ++z)
            {
                m_noiseMatrix[x][y][z] = rand();
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


double NoiseMaterial::choose_material(const Point3D& intersection_point) const
{
    // Based on a Pascal interpretation of 1985 Perlin noise function,
    // which in turn was based on Alan Watt's Fundamentals of Three-
    // Dimensional Computer Graphics. 

    // TODO: refactor this.  This is ugly. Probably ugly rounding errors as well.
    // Probably lots of room for optimization.

    // Temp variables for interpolation
    int p000,p001, p010,p011, p100,p101, p110,p111;
    int p00, p01,  p10, p11;
    int p0,  p1;
    int d00, d01;
    int d10, d11;
    int d0, d1;
    int d;

    double f_x =  fabs(intersection_point.x);
    double f_y =  fabs(intersection_point.y);
    double f_z =  fabs(intersection_point.z);


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

    p0 = floor(d0 * oy) + p00;
    p1 = floor(d1 * oy) + p01;

    d = p1 - p0;

    return ((d * oz) + (p0 * 1.0)) / (RAND_MAX * 1.0);
}

Color * NoiseMaterial::get_color(const Point3D& intersection_point) const 
{ 
    // !!!THREAD SAFETY ALERT!!!
    // we are using one global color here.  This
    // will absolutely break if we go multi-threaded.
    //  When that day comes, we will either need to
    // revisit this strategy or mangle the key
    // based on the thread id

    const char * noiseColorKey = "__NOISE_WORKING_COLOR";

    Scene * scene = Scene::get_instance();
    Color * retVal = scene->get_color(noiseColorKey);
    if(retVal == NULL)
    {
        retVal = new Color();
        scene->add_color(noiseColorKey, retVal);
    }

    double coeff = choose_material(intersection_point);

    // TODO: we could really prolly just hang onto pointers
    // other than making copies.  I only did this way
    // because of misunderstanding how operators worked,
    // and haven't fixed it yet.

    Color one = *(m_material_one->get_color(intersection_point) );
    Color two = *(m_material_two->get_color(intersection_point) );
    (*retVal) = (one * coeff) + (two * (1.0-coeff) );

    return retVal;

}

bool NoiseMaterial::is_light(const Point3D& intersection_point) const 
{ 
    return false; 
}

void NoiseMaterial::set_is_light(bool v) 
{ 
}

double NoiseMaterial::get_diffuse(const Point3D& intersection_point) const 
{ 
    double coeff = choose_material(intersection_point);
    double contributionOne = m_material_one->get_diffuse(intersection_point) * coeff;
    double contributionTwo = m_material_two->get_diffuse(intersection_point) * (1.0 - coeff);
    return contributionOne + contributionTwo;
}

void NoiseMaterial::set_diffuse(double diffuse) 
{ 
}

double NoiseMaterial::get_reflection(const Point3D& intersection_point) const 
{ 
    double coeff = choose_material(intersection_point);
    double contributionOne = m_material_one->get_reflection(intersection_point) * coeff;
    double contributionTwo = m_material_two->get_reflection(intersection_point) * (1.0 - coeff);
    //std::cout << "Noise: returning reflection of " << (contributionOne + contributionTwo) << std::endl;
    return contributionOne + contributionTwo;
}

double NoiseMaterial::get_reflectivity(const Point3D& intersection_point) const 
{ 
    double coeff = choose_material(intersection_point);
    double contributionOne = m_material_one->get_reflectivity(intersection_point) * coeff;
    double contributionTwo = m_material_two->get_reflectivity(intersection_point) * (1.0 - coeff);
    //std::cout << "Noise: returning reflectivity of " << (contributionOne + contributionTwo) << std::endl;
    return contributionOne + contributionTwo;
}

double NoiseMaterial::get_refraction_index(const Point3D& intersection_point) const 
{ 
    // This would seem sketchy at best.  Perhaps better
    // would be to make material one the authority...
    double coeff = choose_material(intersection_point);
    double contributionOne = m_material_one->get_refraction_index(intersection_point) * coeff;
    double contributionTwo = m_material_two->get_refraction_index(intersection_point) * (1.0 - coeff);
    return contributionOne + contributionTwo;
}

double NoiseMaterial::get_opacity(const Point3D& intersection_point) const 
{ 
    double coeff = choose_material(intersection_point);
    double contributionOne = m_material_one->get_opacity(intersection_point) * coeff;
    double contributionTwo = m_material_two->get_opacity(intersection_point) * (1.0 - coeff);
    return contributionOne + contributionTwo;
}

void NoiseMaterial::set_opacity(double opacity) 
{
}


Material * NoiseMaterial::createNoiseMaterial(std::map<std::string, std::string> props)
{
    Scene * scene = Scene::get_instance();
    Material * one = scene->get_material(props["material1"]);
    Material * two = scene->get_material(props["material2"]);
    return new NoiseMaterial(one, two);

}

