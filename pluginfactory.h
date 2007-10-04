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
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>
#include <sigc++/sigc++.h>

#include "material.h"
#include "primitive.h"

/// Signal for a function which constructs a new instance of a Material.
/// @todo  Change this to accept the XML node structure so we can keep the
/// interface for materials and primitives the same and as flexible as
/// possible.
typedef sigc::signal<Material *, std::map<std::string, std::string> > material_create_fn;

/// Signal for a function which constructs a new instance of a Primitive.
typedef sigc::signal<Primitive *, xmlNode *> primitive_create_fn;

/// This is an abstraction for material_create_fn signals to allow them to be
/// passed transparently to the register function.
typedef sigc::slot<Material *, std::map<std::string, std::string> > material_create_slot;

/// This is an abstraction for primitive_create_fn signals to allow them to be
/// passed transparently to the register function.
typedef sigc::slot<Primitive *, xmlNode *> primitive_create_slot;



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
         *
         * @deprecated
         * @todo Remove this in favor of the create() fucntion that accepts
         * the xmlNode
         */
        _ParentType * create(std::string type, std::map<std::string, std::string> attributes) {
            if(m_createFunctions.find(type) != m_createFunctions.end()) {
                _ParentType * tmp =  m_createFunctions[type].emit(attributes);
                if(!tmp)
                    log_warn("[deprecated] Create function for %s type returned NULL.", type.c_str());
                return tmp;
            }
            
            log_warn("[deprecated] Could not find signal for %s type.", type.c_str());
            return NULL;
        }

        _ParentType * create(std::string type, xmlNode * node) {
            if(m_createFunctions.find(type) != m_createFunctions.end()) {
                _ParentType * tmp =  m_createFunctions[type].emit(node);
                if(!tmp)
                    log_warn("Create function for <%s> node returned NULL.", node->name);
                return tmp;
            }

            log_warn("Could not find signal for %s type.", type.c_str());
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
            log_debug("Registering function for type <%s>.", type.c_str());
            if(m_createFunctions.find(type) != m_createFunctions.end()) {
                log_warn("Function for type <%s> already registered, ignoring.", type.c_str());
                return false;
            }

            log_debug("Adding plugin for <%s> node...", type.c_str());
            m_createFunctions[type].connect(createFunction);
            log_debug("...done");
        }

        ///Return an instance of PluginFactory.
        ///
        ///@return instance of PluginFactory
        static PluginFactory<_ParentType, _SigType, _SlotType> * get_instance() {
            static PluginFactory<_ParentType, _SigType, _SlotType> * instance;
            if(instance == NULL) {
                log_debug("Creating new factory");
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
