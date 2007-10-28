/// @file imagewriter.cpp
/// @author Michael Brailsford
/// @date Sat Aug 18 02:43:49 -0500 2007
/// &copy; 2007 Michael Brailsford

#include <Magick++.h>
#include "raytrace_defs.h"
#include "imagewriter.h"
#include "scene.h"

#include <iostream>

using std::string;
using Magick::Color;
using Magick::ColorRGB;

//{{{
PpmImageWriter::PpmImageWriter(std::string _filename, int _height, int _width)
        : m_filename(_filename),
          m_px_height(_height),
          m_px_width(_width) 
{
}
//}}}

//{{{
bool PpmImageWriter::write_image(ColorRGB * data) {
    Magick::Image img("512x512", Color("red"));

    int data_pos = 0;
    for(int i = 0; i < m_px_height; ++i) {
        for(int j = 0; j < m_px_width; ++j) {
            img.pixelColor(j, i, data[data_pos]);
            data_pos++;
        }
    }

    img.write(m_filename);
}
//}}}

