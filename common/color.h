#ifndef COLOR_H
#define COLOR_H

#include "raytrace_defs.h"

typedef unsigned char uchar;
//{{{
/// This class represents a color.
class Color {
    private:
        /// Red component of the color
        double m_red;
        /// Green component of the color
        double m_green;
        /// Blue component of the color
        double m_blue;

        inline double get_largest_color_component() const {
            if(m_red >= m_green && m_red >= m_blue) {
                return m_red;
            }
            else if(m_green >= m_red && m_green >= m_blue) {
                return m_green;
            }
            else if(m_blue >= m_red && m_blue >= m_green) {
                return m_blue;
            }
        }
    public:

        /// Default constructor.  Creates a pure black color.
        Color() : m_red(0.0), m_green(0.0), m_blue(0.0) {}

        /// Create a color from an RGB triplet.
        Color(int rgb) : m_red(((unsigned char*)rgb)[0]/256.0), m_green(((unsigned char*)rgb)[1]/256.0), m_blue(((unsigned char*)rgb)[2]/256.0) {}

        /// Create a color from double components, ranging from 0.0 to 1.0
        Color(double _red, double _green, double _blue) : m_red(_red), m_green(_green), m_blue(_blue) {
            int a = 3;
        }

        /// Create a color from integer components, ranging from 0 to 256
        Color(int _red, int _green, int _blue) : m_red(_red / 256.0), m_green(_green / 256.0), m_blue(_blue / 256.0) {
        }

        /// Copy constructor
        Color(const Color &other) : m_red(other.m_red), m_green(other.m_green), m_blue(other.m_blue) {
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
        bool operator==(const Color &other) {
            double FUDGE_FACTOR = 0.0001;
            return (m_red >= other.m_red - FUDGE_FACTOR && m_red <= other.m_red + FUDGE_FACTOR)
                && (m_green >= other.m_green - FUDGE_FACTOR && m_green <= other.m_green + FUDGE_FACTOR)
                && (m_blue >= other.m_blue - FUDGE_FACTOR && m_blue <= other.m_blue + FUDGE_FACTOR);
        } 

        /// Set a new red value
        /// @param new_red  The new value for the red component
        void set_red (double new_red) { m_red = new_red; }
        /// Set a new green value
        /// @param new_green  The new value for the green component
        void set_green (double new_green) { m_green = new_green; }
        /// Set a new blue value
        /// @param new_blue  The new value for the blue component
        void set_blue (double new_blue) { m_blue = new_blue; }

        /// Return red component scaled to 0..255
        /// @return red component
        inline uchar get_rgb_red () const {
            double max = get_largest_color_component();
            if(max > 1.0) {
                return (uchar)((m_red/max) * 255); 
            }
            return (uchar)(m_red * 255);
        }

        /// Return green component scaled to 0..255
        /// @return green component
        inline uchar get_rgb_green() const {
            double max = get_largest_color_component();
            if(max > 1.0) {
                return (uchar)((m_green/max) * 255); 
            }
            return (uchar)(m_green * 255);
        }
        
        /// Return blue component scaled to 0..255
        /// @return blue component
        inline uchar get_rgb_blue () const {
            double max = get_largest_color_component();
            if(max > 1.0) {
                return (uchar)((m_blue/max) * 255); 
            }
            return (uchar)(m_blue * 255);
        }


        /// Return red component scaled to 0..255
        /// @return red component
        inline double get_red () const { return m_red; }

        /// Return green component scaled to 0..255
        /// @return green component
        inline double get_green() const { return m_green; }
        
        /// Return blue component scaled to 0..255
        /// @return blue component
        inline double get_blue () const { return m_blue; }

};
//}}}

#endif
