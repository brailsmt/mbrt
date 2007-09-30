/* file name       : pluginfactory.h
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
#include "material.h"
#include "primitive.h"

/// Signal for a function which constructs a new instance of a Material.
typedef sigc::signal<Material *, std::map<std::string, std::string> > material_create_fn;

/// Signal for a function which constructs a new instance of a Primitive.
typedef sigc::signal<Primitive *, std::map<std::string, std::string> > primitive_create_fn;

/// This is an abstraction for material_create_fn signals to allow them to be
/// passed transparently to the register function.
typedef sigc::slot<Material *, std::map<std::string, std::string> > material_create_slot;

/// This is an abstraction for primitive_create_fn signals to allow them to be
/// passed transparently to the register function.
typedef sigc::slot<Primitive *, std::map<std::string, std::string> > primitive_create_slot;



/// PluginFactory allows plugins to register themselves so that the
/// parser can later create materials generically.
template <class _ParentType, class _SigType, class _SlotType>
class PluginFactory {
    protected:

        /// Map of function pointers to methods for creating materials.
        std::map<std::string, _SigType>  m_createFunctions;


    public:
        /** Creates a Material object based on the type and the passed in parameters
         *
         * @param type          String that the material type registered under
         * @param attributes    Attributes used to build materials
         * @return An initialized Material object
         */
        _ParentType * create(std::string type, std::map<std::string, std::string> attributes) {
            if(m_createFunctions.find(type) != m_createFunctions.end()) {
                return m_createFunctions[type].emit(attributes);
            }

            return NULL;
        }

        /// Registers the function to create materials of the given type.
        ///
        /// @param type               String that identifies the material type
        /// @param createFunction     Signal handler to create materials of the
        ///                           given type.
        /// @return                   True if successful.  Will return false if a material has previously registered
        ///                           under the type name.  The first to register will always win, and a warning will
        ///                           be logged that another attempt was made to register.
        bool registerPlugin(std::string type, _SlotType createFunction) {
            if(m_createFunctions.find(type) != m_createFunctions.end()) {
                std::cout << "Warning: function of type '" << type << "' already registered, ignoring." << std::endl;
                return false;
            }

            std::cout << "Adding material of type '" << type << "' ..." ;
            m_createFunctions[type].connect(createFunction);
            std::cout << "...done" << std::endl;
        }

        ///Return an instance of PluginFactory.
        ///
        ///@return instance of PluginFactory
        static PluginFactory * get_instance() {
            static PluginFactory<_ParentType, _SigType, _SlotType> * instance = NULL;
            if(instance == NULL) {
                instance = new PluginFactory<_ParentType, _SigType, _SlotType>();
            }
            return instance;
        }

};

typedef PluginFactory<Material,  material_create_fn,  material_create_slot>  MaterialFactory;
typedef PluginFactory<Primitive, primitive_create_fn, primitive_create_slot> PrimitiveFactory;

#endif

/*
 * change log
 *
 * $Log$
 */
