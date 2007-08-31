/// @file xml_defs.h
/// @author Michael Brailsford <brailsmt@yahoo.com>
/// @date Mon Aug 27 00:50:11 -0500 2007
/// &copy; 2007 Michael Brailsford
///
/// This file contains definitions for XML parsing of scene defintions.

#if 0
#ifndef XML_DEFS_H
#define XML_DEFS_H


#include <sigc++/sigc++.h>

/// This typedef defines the signature of functions which parse Primitives from an XPath.
typedef sigc::slot<Primitive *, xmlXPathObjectPtr> parse_handler_t;

static parse_handlers_t object_factories[] = {
        };

/// AbstractFactory class for constructing Primitives from an XML definition of the primitive.
class ObjectAbstractFactory {
        static virtual Primitive * build_from_xml(xmlXPathObjectPtr xpath) = 0;
}

#endif
#endif

