#include "materialfactory.h"
#include <iostream>

//#include "material.h"

using std::map;
using std::string;

std::map<std::string, void *>  MaterialFactory::m_createFunctions;


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

    return call(attributes);
}
    

bool MaterialFactory::registerFunction(std::string type, void * createFunction)
{
    if(m_createFunctions.count(type) > 0)
    {
        std::cout << "Warning: function of type '" << type << "' already registered, ignoring subsequent attempt." << std::endl;
        return false;
    }

    m_createFunctions[type] = createFunction;

}
