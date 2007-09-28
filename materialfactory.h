/* file name       : materialfactory.h
 * contributors    : Brandon Inman
 *                   Michael Brailsford
 * header inserted : Mon Sep 24 22:31:24 -0500 2007
 * copyright       : (c) 2007 Michael Brailsford & Brandon Inman
 * version         : $Revision$
 */


#ifndef MATERIALFACTORY_H
#define MATERIALFACTORY_H

#include <string>
#include <map>
#include <sigc++/sigc++.h>

class Material;

/// Signal for function within Material subclasses which construct a new
/// instance of that Material.
typedef sigc::signal<Material *, std::map<std::string, std::string> > material_create_fn;

/// This is an abstraction for material_create_fn signals to allow them to be
/// passed transparently to the register function.
typedef sigc::slot<Material *, std::map<std::string, std::string> > material_create_slot;

/// MaterialFactory allows materials to register themselves so that the
/// parser can later create materials generically.
class MaterialFactory
{
    protected:

        /// Map of function pointers to factory methods.
        std::map<std::string, material_create_fn>  m_createFunctions;
    public:
        /** Creates a Material object based on the type and the passed in parameters
         *
         * @param type          String that the material type registered under
         * @param attributes    Attributes used to build materials
         * @return An initialized Material object
         */
       Material * create(std::string type, std::map<std::string, std::string> attributes);

       /**
        * Registers the function to create materials of the given type.
        *
        * @param type               String that identifies the material type
        * @param createFunction     Signal handler to create materials of the
        *                           given type.
        * @return                   True if successful.  Will return false if a material has previously registered
        *                           under the type name.  The first to register will always win, and a warning will
        *                           be logged that another attempt was made to register.
        */
        bool registerFunction(std::string type, material_create_slot createFunction);


        /**
         * Return an instance of MaterialFactory.
         *
         * @return instance of MaterialFactory
         */

        static MaterialFactory * get_instance();

};

#endif

/*
 * change log
 *
 * $Log$
 */
