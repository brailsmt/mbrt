#ifndef COLOR_H
#define COLOR_H

typedef unsigned char uchar;
//{{{
/// This class represents a color.
class Color {
    public:
        /// Red component of the color
        double m_red;
        /// Green component of the color
        double m_green;
        /// Blue component of the color
        double m_blue;

        /// Default constructor.  Creates a pure black color.
        Color() : m_red(0.0), m_green(0.0), m_blue(0.0) {}

        /// Create a color from double components, ranging from 0.0 to 1.0
        Color(double _red, double _green, double _blue) : m_red(_red), m_green(_green), m_blue(_blue) {
            set_red(m_red);
            set_green(m_green);
            set_blue(m_blue);
        }

        /// Create a color from integer components, ranging from 0 to 256
        Color(int _red, int _green, int _blue) : m_red(_red / 256.0), m_green(_green / 256.0), m_blue(_blue / 256.0) {
            set_red(m_red);
            set_green(m_green);
            set_blue(m_blue);
        }

        /// Copy constructor
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

        /// Set a new red value
        void set_red (double new_red) { m_red = (new_red < 1.0) ? new_red : 1.0; }
        /// Set a new green value
        void set_green (double new_green) { m_green = (new_green < 1.0) ? new_green : 1.0; }
        /// Set a new blue value
        void set_blue (double new_blue) { m_blue = (new_blue < 1.0) ? new_blue : 1.0; }

        /// Return red component scaled to 0..255
        uchar get_rgb_red () const { return (uchar)(m_red * 255); }
        /// Return green component scaled to 0..255
        uchar get_rgb_green() const { return (uchar)(m_green * 255); }
        /// Return blue component scaled to 0..255
        uchar get_rgb_blue () const { return (uchar)(m_blue * 255); }
};
//}}}

#endif
