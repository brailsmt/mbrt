#ifndef BASICMATERIAL_H
#define BASICMATERIAL_H

#include "material.h"
#include "materialfactory.h"

#include <iostream>

/// This is the most basic material.  It has a single set of
/// attributes that return the same value regardless of the
/// intersection point.  Currently, this is the only material
/// that can exist at the leaf nodes of a material tree.
class SolidMaterial : public Material
{
    /// Static initialization.  Registers the material with the MaterialFactory.
    class SolidMaterialStaticInit
    {
        public:
            /// Register this class with the factory.
            SolidMaterialStaticInit()
            {
                MaterialFactory::get_instance()->registerFunction("solid", (void *) SolidMaterial::createSolidMaterial);
            }
    };

    private:
         /// Dummy variable to run static initialization
        static SolidMaterialStaticInit m_init;

    public:
        /// Factory method to create a material from a set of attributes.
        /// @param attributes A map of strings that determine the attributes of the material
        /// @return A pointer to a SolidMaterial object.
        static Material * createSolidMaterial(std::map<std::string, std::string> attributes);

};

#endif
