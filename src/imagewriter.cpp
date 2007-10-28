/// @file imagewriter.cpp
/// @author Michael Brailsford
/// @date Sat Aug 18 02:43:49 -0500 2007
/// &copy; 2007 Michael Brailsford

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
    bool rv = false;
    int x, y;
    getyx(stdscr, y, x);

    FILE * _file = fopen(m_filename.c_str(), "w+");
    if ( _file == NULL ) {
        mvprintw(y, 0, "Could not open %s!  Aborting save!", m_filename.c_str());
    }
    else {
        int header_length = 32;
        char header[header_length];
        memset(header, 0x0, header_length);
        snprintf(header, header_length, "P6 %i %i %i ", m_px_width, m_px_height, PPM_MAX_VAL);

        // Write the ppm header.
        fwrite(header, sizeof(char), strlen(header), _file);
        // Write the data.
        for ( int i = 0; i < (m_px_width * m_px_height); ++i) {
            uchar rgbdata[] = {
                (int)(data[i].red()) * PPM_MAX_VAL,
                (int)(data[i].green()) * PPM_MAX_VAL,
                (int)(data[i].blue()) * PPM_MAX_VAL
            };
            fwrite(rgbdata, sizeof(rgbdata), 1, _file);
        }

        fclose(_file);

        mvprintw(y, 0, "\n");
        mvprintw(y, 0, "Saved rendered image to %s", m_filename.c_str());
        rv = true;
    }
    refresh();

    return rv;
}
//}}}

