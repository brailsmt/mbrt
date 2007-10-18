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

#include "point3d.h"
#include "color.h"

typedef std::map<std::string, std::string> xml_properties;

/// Returns the properties of the XML node as an STL map<string, string>.
inline xml_properties get_properties(xmlNode * node) {
    xmlAttr * props = node->properties;
    xml_properties rv;

    _xmlAttr * cur = props;
    while ( cur != NULL ) {
        rv[(char *)cur->name] = (char *)cur->children->content;
        cur = cur->next;
    }

    return rv;
}

/// Parse RGB triplets in the form '#RRGGBB' and returns the RGB values.
inline Color parse_rgb(std::string rgb_triplet) {
    // Default to red so errors show up easily.
    int red   = 0xff;
    int green = 0x00;
    int blue  = 0x00;

    log_debug("parsing color RGB triplet %s.", rgb_triplet.c_str());
    if(rgb_triplet[0] == '#') {
        red   = strtol(rgb_triplet.substr(1,2).c_str(), NULL, 0);
        green = strtol(rgb_triplet.substr(3,2).c_str(), NULL, 0);
        blue  = strtol(rgb_triplet.substr(5,2).c_str(), NULL, 0);
    }
    if(rgb_triplet[0] == '0' && rgb_triplet[1] == 'x') {
        red   = strtol(rgb_triplet.substr(2,2).c_str(), NULL, 0);
        green = strtol(rgb_triplet.substr(4,2).c_str(), NULL, 0);
        blue  = strtol(rgb_triplet.substr(6,2).c_str(), NULL, 0);
    }
    else {
        log_err("Invalid format for RGB value, expected #RRGGBB, got %s", rgb_triplet.c_str());
    }

    return Color(red, green, blue);
}

inline Point3D parse_vertex(xmlNode * node) {
    log_debug("%s", node->name);
    xml_properties props = get_properties(node);
    return Point3D(props["at"]);
}
#endif

