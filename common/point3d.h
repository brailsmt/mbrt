#ifndef POINT3D_H
#define POINT3D_H

#include <cmath>
#include <sstream>
#include <algorithm>

class Point3D;
typedef Point3D Vector;

inline bool iswhitespace(char c) {
    return c == ' '
        || c == '\n'
        || c == '\r'
        || c == '\t';
}
/// A three-dimensional point in space.
//{{{
class Point3D {
    public:
        
        /// Distance from origin on the x axis
        double x;
        /// Distance from origin on the y axis
        double y;
        /// Distance from origin on the z axis
        double z;

        /// Default constructor.  Places point on the origin.
        Point3D() : x(0.0), y(0.0), z(0.0) {}

        /// Creates a point in three dimenisional space.
        Point3D(double _x, double _y, double _z = 0.0) : x(_x), y(_y), z(_z) {}

        /// Creates a point in three space by parsing a string of the form
        /// "(x, y, z)".
        Point3D(std::string str) {
            //syslog(LOG_INFO, "%s", str.c_str());
            // remove all whitespace
            std::string::iterator end;
            std::remove_if(str.begin(), end, iswhitespace);
            int openparen   = str.find('(');
            int closeparen  = str.find(')');
            int firstcomma  = str.find(',') + 1;
            int secondcomma = str.find(',', firstcomma) + 1;
            
            x = (double)strtod(str.substr(openparen+1, firstcomma - openparen).c_str(),     NULL);
            y = (double)strtod(str.substr(firstcomma, secondcomma - firstcomma).c_str(),  NULL);
            z = (double)strtod(str.substr(secondcomma, closeparen - secondcomma).c_str(), NULL);
        }

        /// Copy constructor
        Point3D(const Point3D &other) : x(other.x), y(other.y), z(other.z) {}

        /// Returns the distance between this point and the other point.  This is defined by the
        /// following function, copied from http://en.wikipedia.org/wiki/Distance#Geometry.
        ///
        /// @f[ d=\sqrt{(\Delta x)^2+(\Delta y)^2+(\Delta z)^2}=\sqrt{(x_1-x_2)^2+(y_1-y_2)^2+(z_1-z_2)^2} @f]
        ///
        /// @param other The other point to which we are determining the distance.
        inline double distance_to(const Point3D &other);

        /// Convert to a string representation
        /// @return string representation
        std::string to_string() {
            std::ostringstream rv;
            rv << "(" << x << ", " << y << ", " << z << ")";
            return rv.str();
        }

        /// Sets the coordinates of the point
        void set(double x, double y, double z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        /// Calculate the normalized vector from the origin to the destination.
        void normalize() {
            double norm = sqrtf(x * x + y * y + z * z);
            x /= norm;
            y /= norm;
            z /= norm;
        }

};
//}}}


// This is inline, it must be defined in the header.
/// Calculate the distant from this point to another point.
///
/// @param other The point to find the distance to
///@return distance to the point
//{{{
double Point3D::distance_to(const Point3D &other) {
    double dx = x - other.x;
    double dy = y - other.y;
    double dz = z - other.z;
    return sqrtf(dx*dx + dy*dy + dz*dz);
}
//}}}

/// Subtract the two vectors and return the new Vector.
/// @param A First vector
/// @param B Second vector
/// @return result of operation
//{{{
inline Vector operator-(const Vector &A, const Vector &B) {
    return Vector(A.x - B.x, A.y - B.y, A.z - B.z);
}
//}}}
/// Unary negation.
//{{{
inline Vector operator-(const Vector &A) {
    return Vector( -A.x, -A.y, -A.z);
}
//}}}

/// Add the two vectors and return the new Vector.
//{{{
inline Vector operator+(const Vector &A, const Vector &B) {
    return Vector(A.x + B.x, A.y + B.y, A.z + B.z);
}
//}}}
//{{{
inline Vector operator*(const Vector &A, double factor) {
    return Vector(A.x * factor, A.y * factor, A.z * factor);
}
//}}}

//{{{
inline bool operator==(const Vector &A, const Vector &B) {
    return (A.x == B.x) && (A.y == B.y) && (A.z == B.z);
}
//}}}
//{{{
inline bool operator!=(const Vector &A, const Vector &B) {
    return !(A == B);
}
//}}}

/// The dot product of two unit vectors provides the cosine of the angle between the two vectors.
//{{{
inline double dot_product(const Vector &A, const Vector &B) {
    return (A.x * B.x) + (A.y * B.y) + (A.z * B.z);
}
//}}}

/// The cross product of two vectors is the normal vector to
/// a plane defined by the two vectors
//{{{
inline 
Vector cross_product(const Vector& one, const Vector& two)
{
    return Vector(one.y * two.z - one.z * two.y,
                  one.z * two.x - one.x * two.z,
                  one.x * two.y - one.y * two.x);

}
//}}}
//{{{
inline Vector operator*(Vector &v, const double n) {
    return Vector(v.x * n, v.y * n, v.z * n);
}
//}}}


#endif
