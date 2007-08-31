/// @file imagewriter.h
/// @author Michael Brailsford <brailsmt@yahoo.com>
/// @date Sat Aug 18 02:33:41 -0500 2007
/// &copy; 2007 Michael Brailsford

#ifndef IMAGEWRITER_H
#define IMAGEWRITER_H

#include "scene.h"
class Color;

#include <string>
#include <map>

enum ImageTypes {
    INVALID_IMAGE_TYPE = -1,
    PPM_IMAGE = 0,

    LAST_IMAGE_TYPE
};

typedef std::map<std::string, ImageTypes> ExtensionMap;

/// Instances of the ImageWriter class takes image data and writes it to various formats.
class ImageWriter {
    public:
        /// Write the image data to file.
        ///
        /// @param data Image imformation that will be written.
        virtual void write_image (Color * data) = 0;

};

class PpmImageWriter : public ImageWriter {
    private:
        std::string filename;
        int px_height;
        int px_width;

    public:
        PpmImageWriter(std::string _filename, int _height, int _width);

        /// Write the image data to a ppm file.
        ///
        /// @param data Image imformation that will be written.
        virtual void write_image (Color * data);
};

class ImageWriterFactory {
    public:
        /// Map of extensions to the type of ImageWriter class sould be created.
        ExtensionMap extension_list;

        ImageWriterFactory() : extension_list() {
            extension_list["ppm"] = PPM_IMAGE;
        }

        /// This returns a pointer the right type of ImageWriter based on the extension of the filename passed in.
        ImageWriter * operator()(std::string filename) {
            std::string ext = filename.substr(filename.rfind("."));
            Scene * scene = Scene::get_instance();

            // Use PPM as the default.
            ImageTypes type = PPM_IMAGE;
            if (extension_list.find(ext) != extension_list.end()) {
                type = extension_list[ext];
            }

            switch (type) {
            case PPM_IMAGE:
                return new PpmImageWriter(scene->get_output_filename(), scene->get_viewport_pixel_height(), scene->get_viewport_pixel_width());
            default:
                return NULL;
            }
        }

};
#endif

