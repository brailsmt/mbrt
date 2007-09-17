#ifndef BASICMATERIAL_H
#define BASICMATERIAL_H

#include "material.h"
#include "materialfactory.h"

#include <iostream>

class SolidMaterial : public Material
{
    class SolidMaterialStaticInit
    {
        public:
            SolidMaterialStaticInit()
            {
                MaterialFactory::get_instance()->registerFunction("solid", (void *) SolidMaterial::createSolidMaterial);
            }
    };

    private:
        /**
         * Dummy variable to run static initialization
         */
        static SolidMaterialStaticInit m_init;

    public:
        static Material * createSolidMaterial(std::map<std::string, std::string>);

};

#endif
