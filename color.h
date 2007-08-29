#ifndef COLOR_H
#define COLOR_H

#include <iostream>

typedef unsigned char uchar;
//{{{
class Color {
    public:
        float red;
        float green;
        float blue;

        Color() : red(0.0f), green(0.0f), blue(0.0f) {}
        Color(float _red, float _green, float _blue) : red(_red), green(_green), blue(_blue) {
            //std::cout << "float ctor" << std::endl;
            set_red(red);
            set_green(green);
            set_blue(blue);
        }

        Color(int _red, int _green, int _blue) : red(_red/256.0f), green(_green/256.0f), blue(_blue/256.0f) {
            //std::cout << "int ctor" << std::endl;
            set_red(red);
            set_green(green);
            set_blue(blue);
        }

        Color(const Color &other) : red(other.red), green(other.green), blue(other.blue) {
            //std::cout << "Copy ctor" << std::endl;
            set_red(red);
            set_green(green);
            set_blue(blue);
        }

        void operator=(const Color &other) {
            //std::cout << "operator=" << std::endl;
            set_red   ( other.red   );
            set_green ( other.green );
            set_blue  ( other.blue  );
        }

        void  operator*=(const Color &other) {
            //std::cout << "operator*=(Color)" << std::endl;
            set_red(red * other.red);
            set_green(green * other.green);
            set_blue(blue * other.blue);
        }
        Color operator* (const Color &other) { return Color(red * other.red, green * other.green, blue * other.blue); }

        void  operator*=(float n) {
            //std::cout << "operator*=(Color)" << std::endl;
            set_red(red * n);
            set_green(green * n);
            set_blue(blue * n);
        }
        Color operator* (float n) { return Color(red * n, green * n, blue * n); }

        void  operator+=(const Color &other) {
            //std::cout << "operator+=(Color)" << std::endl;
            set_red  ( red + other.red     );
            set_green( green + other.green );
            set_blue ( blue + other.blue   );
        }

        Color operator+ (const Color &other) { return Color( red + other.red, green + other.green, blue + other.blue); }
        Color operator-(const Color &other) { return Color(red - other.red, green - other.green, blue - other.blue); }

        void set_red   (float new_red)   { red   = (new_red   < 1.0f)? new_red:   1.0f; /*std::cout << "red = " << red << std::endl;*/ } 
        void set_green (float new_green) { green = (new_green < 1.0f)? new_green: 1.0f; /*std::cout << "green = " << green << std::endl;*/ } 
        void set_blue  (float new_blue)  { blue  = (new_blue  < 1.0f)? new_blue:  1.0f; /*std::cout << "blue = " << blue << std::endl;*/ } 

        uchar get_rgb_red  () const { return (uchar)(red   * 255); } 
        uchar get_rgb_green() const { return (uchar)(green * 255); } 
        uchar get_rgb_blue () const { return (uchar)(blue  * 255); } 
};
//}}}

#endif
