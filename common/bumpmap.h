#ifndef BUMPMAP_H
#define BUMPMAP_H

#include "ray.h"
/// Defines an abstract interface for bump maps to implement.

class BumpMap {
    public:
        virtual ~BumpMap() {}

        /// Perturb the normal. 
        virtual Ray perturb_normal(const Ray& normal, const Point3D& intersection_point) = 0;
};


#endif
