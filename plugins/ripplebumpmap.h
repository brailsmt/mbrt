/// @file ripplebumpmap.h
/// @author Brandon Inmane <brinman2002@yahoo.com>
/// &copy; 2007 Brandon Inman
//

#ifndef RIPPLEBUMPMAP_H
#define RIPPLEBUMPMAP_H

#include "bumpmap.h"
#include "pluginfactory.h"

class RippleBumpMap;

/// Creates a rippled bumpmap
BumpMap * new_ripplebumpmap(xmlNode * node);
/// Deletes a rippled bumpmap 
void delete_ripplebumpmap(RippleBumpMap * bumpmap);

/// RippleBumpMap is a bump map that uses sin and cos.
/// On a sphere, it gives the rough appearance of a golf ball.
class RippleBumpMap : public BumpMap
{
    protected:
    /// Provides static initialization
    class StaticInit {
        public:
            /// Register material with the factory
            StaticInit() {
                BumpMapFactory::get_instance()->registerPlugin("ripple", sigc::ptr_fun(new_ripplebumpmap));
            }
    };

    /// Runs static initialization
    static StaticInit m_init;

    public:
        /// Apply bump map effect.
        virtual Ray perturb_normal(const Ray& normal, const Point3D& intersection_point) ;
};

BumpMap * new_ripplebumpmap(xmlNode * node) {
    
    // TODO attributes? scaling at least?

    BumpMap * rv = new RippleBumpMap();
    return rv;
}

void delete_ripplebumpmap(RippleBumpMap * ptr) {
    delete ptr, ptr = NULL;
}

#endif

