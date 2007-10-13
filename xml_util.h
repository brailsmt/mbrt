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

struct rgb {
    int red;
    int green;
    int blue;
};

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
inline int parse_rgb(std::string rgb_triplet) {
    int rv = 0xff000000;
    if(rgb_triplet[0] == '#') {
        ((unsigned char*)rv)[0] = (unsigned char)strtol(rgb_triplet.substr(1,2).c_str(), NULL, 0);
        ((unsigned char*)rv)[1] = (unsigned char)strtol(rgb_triplet.substr(3,2).c_str(), NULL, 0);
        ((unsigned char*)rv)[2] = (unsigned char)strtol(rgb_triplet.substr(5,2).c_str(), NULL, 0);
    }
    else {
        ;//log_err("Invalid format for RGB value, expected #RRGGBB, got %s", rgb_triplet.c_str());
    }

    return rv;
}
#endif

