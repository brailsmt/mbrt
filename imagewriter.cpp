/// @file imagewriter.cpp
/// @author Michael Brailsford
/// @date Sat Aug 18 02:43:49 -0500 2007
/// &copy; 2007 Michael Brailsford

#include "raytrace_defs.h"
#include "imagewriter.h"
#include "color.h"
#include "scene.h"

#include <iostream>

using std::string;
using std::cout;
using std::endl;

//{{{
PpmImageWriter::PpmImageWriter(std::string _filename, int _height, int _width)
  : filename(_filename),
    px_height(_height),
    px_width(_width)
{
}
//}}}

//{{{
void PpmImageWriter::write_image(Color * data) {
    FILE * _file = fopen(filename.c_str(), "w+");
    if( _file == NULL ) {
        cout << "Could not open " << filename << "!  Aborting save!";
        exit(EXIT_FAILURE);
    }

    int header_length = 32;
    char header[header_length];
    memset(header, 0x0, header_length);
    snprintf(header, header_length, "P6 %i %i %i ", px_width, px_height, PPM_MAX_VAL);

    // Write the ppm header.
    fwrite(header, sizeof(char), strlen(header), _file);
    // Write the data.
    for( int i = 0; i < (px_width * px_height); ++i) {
        uchar rgbdata[] = {
            data[i].get_rgb_red(),
            data[i].get_rgb_green(),
            data[i].get_rgb_blue()
        };
        fwrite(rgbdata, sizeof(rgbdata), 1, _file);
    }

    fclose(_file);

    int x, y;
    getyx(stdscr, y, x);
    mvprintw(y, 0, "Saved rendered image to %s", filename.c_str());
    refresh();
}
//}}}

