/* file name  : xml_util.h
 * authors    : Michael Brailsford
 * created    : Sun Sep 30 02:09:55 -0500 2007
 * copyright  : (c) 2007 Michael Brailsford
 */

#ifndef XML_UTIL_H
#define XML_UTIL_H

#include <map>
#include <string>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>

typedef std::map<std::string, std::string> xml_properties;

//{{{
xml_properties get_properties(xmlNode * node) {
    xmlAttr * props = node->properties;
    xml_properties rv;

    _xmlAttr * cur = props;
    while ( cur != NULL ) {
        rv[(char *)cur->name] = (char *)cur->children->content;
        cur = cur->next;
    }

    return rv;
}
//}}}
#endif

