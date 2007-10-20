/// @file concentricbumpmap.h
/// @author Brandon Inmane <brinman2002@yahoo.com>
/// &copy; 2007 Brandon Inman
//

#ifndef CONCENTRICBUMPMAP_H
#define CONCENTRICBUMPMAP_H

#include "bumpmap.h"
#include "pluginfactory.h"

class ConcentricBumpMap;

/// Creates a rippled bumpmap
BumpMap * new_concentricbumpmap(xmlNode * node);
/// Deletes a rippled bumpmap 
void delete_concentricbumpmap(ConcentricBumpMap * bumpmap);

class ConcentricBumpMap : public BumpMap
{
    protected:
    /// Provides static initialization
    class StaticInit {
        public:
            /// Register material with the factory
            StaticInit() {
                BumpMapFactory::get_instance()->registerPlugin("concentric", sigc::ptr_fun(new_concentricbumpmap));
            }
    
    };
    /// Runs static initialization
    static StaticInit m_init;

    public:
        /// Apply bump map effect.
        virtual Ray perturb_normal(const Ray& normal, const Point3D& intersection_point) ;
};

BumpMap * new_concentricbumpmap(xmlNode * node) {
    
    // TODO attributes? scaling at least?

    BumpMap * rv = new ConcentricBumpMap();
    return rv;
}

void delete_concentricbumpmap(ConcentricBumpMap * ptr) {
    delete ptr, ptr = NULL;
}

#endif

