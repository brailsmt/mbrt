#include "materialfactory.h"
#include <iostream>

//#include "material.h"

using std::map;
using std::string;

//std::map<std::string, void *>  MaterialFactory::m_createFunctions;


Material * MaterialFactory::create(string type, map<string,string> attributes)
{
    if(m_createFunctions.count(type) == 0)
    {
        return NULL;
    }

    MaterialCreateFunction call = (MaterialCreateFunction) ( m_createFunctions[type]);
    
    // Guard in case someone accidently inserted a null into the map
    if(call == NULL)
    {
        return NULL;
    }
    Material * ret = call(attributes);
    return ret;
}
    

bool MaterialFactory::registerFunction(std::string type, void * createFunction)
{
    if(m_createFunctions.count(type) > 0)
    {
        std::cout << "Warning: function of type '" << type << "' already registered, ignoring subsequent attempt." << std::endl;
        return false;
    }

    std::cout << "Adding function to map..." << std::endl;
    m_createFunctions[type] = createFunction;
    std::cout << "...done (adding function to map)" << std::endl;

}

MaterialFactory * MaterialFactory::get_instance()
{
    static MaterialFactory * instance = NULL;
    if(instance == NULL)
    {
        instance = new MaterialFactory();
    }
    return instance;
}

