#include "materialfactory.h"
#include <iostream>

using std::map;
using std::string;

Material * MaterialFactory::create(string type, map<string,string> attributes)
{
    if(m_createFunctions.find(type) != m_createFunctions.end()) 
    {
        return m_createFunctions[type].emit(attributes);
    }

    return NULL;
}
    

bool MaterialFactory::registerFunction(std::string type, material_create_slot createFunction)
{
    if(m_createFunctions.find(type) != m_createFunctions.end())
    {
        std::cout << "Warning: function of type '" << type << "' already registered, ignoring." << std::endl;
        return false;
    }

    std::cout << "Adding material of type '" << type << "' ..." ;
    m_createFunctions[type].connect(createFunction);
    std::cout << "...done" << std::endl;

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

