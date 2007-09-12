#ifndef COLOR_H
#define COLOR_H

typedef unsigned char uchar;
//{{{
class Color {
    public:
        double m_red;
        double m_green;
        double m_blue;

        Color() : m_red(0.0), m_green(0.0), m_blue(0.0) {}
        Color(double _red, double _green, double _blue) : m_red(_red), m_green(_green), m_blue(_blue) {
            set_red(m_red);
            set_green(m_green);
            set_blue(m_blue);
        }

        Color(int _red, int _green, int _blue) : m_red(_red / 256.0), m_green(_green / 256.0), m_blue(_blue / 256.0) {
            set_red(m_red);
            set_green(m_green);
            set_blue(m_blue);
        }

        Color(const Color &other) : m_red(other.m_red), m_green(other.m_green), m_blue(other.m_blue) {
            set_red(m_red);
            set_green(m_green);
            set_blue(m_blue);
        }

        void operator=(const Color &other) {
            set_red ( other.m_red );
            set_green ( other.m_green );
            set_blue ( other.m_blue );
        }

        void operator*=(const Color &other) {
            set_red(m_red * other.m_red);
            set_green(m_green * other.m_green);
            set_blue(m_blue * other.m_blue);
        }
        Color operator* (const Color &other) { return Color(m_red * other.m_red, m_green * other.m_green, m_blue * other.m_blue); }

        void operator*=(double n) {
            set_red(m_red * n);
            set_green(m_green * n);
            set_blue(m_blue * n);
        }
        Color operator* (double n) { return Color(m_red * n, m_green * n, m_blue * n); }

        void operator+=(const Color &other) {
            set_red ( m_red + other.m_red );
            set_green( m_green + other.m_green );
            set_blue ( m_blue + other.m_blue );
        }

        Color operator+ (const Color &other) { return Color( m_red + other.m_red, m_green + other.m_green, m_blue + other.m_blue); }
        Color operator-(const Color &other) { return Color( m_red - other.m_red, m_green - other.m_green, m_blue - other.m_blue); }

        void set_red (double new_red) { m_red = (new_red < 1.0) ? new_red : 1.0; }
        void set_green (double new_green) { m_green = (new_green < 1.0) ? new_green : 1.0; }
        void set_blue (double new_blue) { m_blue = (new_blue < 1.0) ? new_blue : 1.0; }

        uchar get_rgb_red () const { return (uchar)(m_red * 255); }
        uchar get_rgb_green() const { return (uchar)(m_green * 255); }
        uchar get_rgb_blue () const { return (uchar)(m_blue * 255); }
};
//}}}

#endif
