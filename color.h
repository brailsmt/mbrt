#ifndef COLOR_H
#define COLOR_H

typedef unsigned char uchar;
//{{{
class Color {
    public:
        double red;
        double green;
        double blue;

        Color() : red(0.0), green(0.0), blue(0.0) {}
        Color(double _red, double _green, double _blue) : red(_red), green(_green), blue(_blue) {
            set_red(red);
            set_green(green);
            set_blue(blue);
        }

        Color(int _red, int _green, int _blue) : red(_red / 256.0), green(_green / 256.0), blue(_blue / 256.0) {
            set_red(red);
            set_green(green);
            set_blue(blue);
        }

        Color(const Color &other) : red(other.red), green(other.green), blue(other.blue) {
            set_red(red);
            set_green(green);
            set_blue(blue);
        }

        void operator=(const Color &other) {
            set_red ( other.red );
            set_green ( other.green );
            set_blue ( other.blue );
        }

        void operator*=(const Color &other) {
            set_red(red * other.red);
            set_green(green * other.green);
            set_blue(blue * other.blue);
        }
        Color operator* (const Color &other) { return Color(red * other.red, green * other.green, blue * other.blue); }

        void operator*=(double n) {
            set_red(red * n);
            set_green(green * n);
            set_blue(blue * n);
        }
        Color operator* (double n) { return Color(red * n, green * n, blue * n); }

        void operator+=(const Color &other) {
            set_red ( red + other.red );
            set_green( green + other.green );
            set_blue ( blue + other.blue );
        }

        Color operator+ (const Color &other) { return Color( red + other.red, green + other.green, blue + other.blue); }
        Color operator-(const Color &other) { return Color(red - other.red, green - other.green, blue - other.blue); }

        void set_red (double new_red) { red = (new_red < 1.0) ? new_red : 1.0; }
        void set_green (double new_green) { green = (new_green < 1.0) ? new_green : 1.0; }
        void set_blue (double new_blue) { blue = (new_blue < 1.0) ? new_blue : 1.0; }

        uchar get_rgb_red () const { return (uchar)(red * 255); }
        uchar get_rgb_green() const { return (uchar)(green * 255); }
        uchar get_rgb_blue () const { return (uchar)(blue * 255); }
};
//}}}

#endif
