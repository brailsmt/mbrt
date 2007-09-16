#ifndef MATERIALFACTORY_H
#define MATERIALFACTORY_H

#include <string>
#include <map>

class Material;

/// Function pointer for create function within Material subclasses.
typedef Material * (*MaterialCreateFunction)(std::map<std::string,std::string>);

class MaterialFactory
{
    protected:

        static std::map<std::string, void *>  m_createFunctions;
    public:
        /** Creates a Material object based on the type and the passed in parameters
         *
         * @param type          String that the material type registered under
         * @param attributes    Attributes used to build materials
         * @return An initialized Material object
         */
       static Material * create(std::string type, std::map<std::string, std::string> attributes);

       /**
        * Registers the function to create materials of the given type.
        *
        * @param type               String that identifies the material type
        * @param createFunction     Function pointer to create the material.
        * @return                   True if successful.  Will return false if a material has previously registered
        *                           under the type name.  The first to register will always win, and a warning will
        *                           be logged that another attempt was made to register.
        */
       static bool registerFunction(std::string type, void * createFunction);
};

#endif