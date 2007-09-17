#include "solidmaterial.h"
#include "color.h"
#include "scene.h"
#include <string>
#include <map>

SolidMaterial::SolidMaterialStaticInit SolidMaterial::m_init;

Material * SolidMaterial::createSolidMaterial(std::map<std::string, std::string> attributes)
{
    bool isLight = attributes.count("light") > 0;
    
    Color * color = Scene::get_instance()->get_color(attributes["color"]);

    double reflection = attributes.count("reflection") > 0 ?
        (double)strtod(attributes["reflection"    ].c_str(), NULL)   : 1.0;

    double diffusion = attributes.count("diffusion") > 0 ?
        (double)strtod(attributes["diffusion"].c_str(), NULL)        :  1.0;

    double reflectivity = attributes.count("reflectivity") > 0 ?
        (double)strtod(attributes["reflectivity"].c_str(), NULL)     :  35.0;

    double refraction = attributes.count("refraction") > 0 ?
        (double)strtod(attributes["refraction"].c_str(), NULL)       :  1.0;

    double opacity = attributes.count("opacity") > 0 ?
        (double)strtod(attributes["opacity"].c_str(), NULL)          :  OPAQUE;

    std::cout << " Creating material '" << attributes["name"] << "' with:" <<std::endl;
    std::cout << "reflection:" << reflection << std::endl;
    std::cout << "diffusion:" << diffusion <<std::endl;
    std::cout << "reflectivity:" << reflectivity <<std::endl;
    std::cout << "refraction:" << refraction <<std::endl;
    std::cout << "opacity: " << opacity <<std::endl;

    return new Material( color,
                         isLight,
                         reflection,
                         diffusion,
                         reflectivity,
                         refraction,
                         opacity
                         );
}

