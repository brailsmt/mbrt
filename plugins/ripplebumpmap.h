/// @file ripplebumpmap.h
/// @author Brandon Inmane <brinman2002@yahoo.com>
/// &copy; 2007 Brandon Inman
//

#ifndef RIPPLEBUMPMAP_H
#define RIPPLEBUMPMAP_H

#include "bumpmap.h"
#include "pluginfactory.h"

class RippleBumpMap;

BumpMap * new_ripplebumpmap(xmlNode * node);
void delete_ripplebumpmap(RippleBumpMap * bumpmap);

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

