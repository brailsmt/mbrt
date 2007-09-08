#ifndef POINT3D_H
#define POINT3D_H

#include <cmath>
#include <sstream>

class Point3D;
typedef Point3D Vector;

//{{{
class Point3D {
    public:
        double x;
        double y;
        double z;

        Point3D() : x(0.0), y(0.0), z(0.0) {}

        template <class _T>
        Point3D(_T _x, _T _y) : x((double)_x), y((double)_y), z(0.0) {}

        template <class _T>
        Point3D(_T _x, _T _y, _T _z) : x((double)_x), y((double)_y), z((double)_z) {}

        Point3D(const Point3D &other) : x(other.x), y(other.y), z(other.z) {}

        /// Returns the distance between this point and the other point.  This is defined by the
        /// following function, copied from http://en.wikipedia.org/wiki/Distance#Geometry.
        ///
        /// @f[ d=\sqrt{(\Delta x)^2+(\Delta y)^2+(\Delta z)^2}=\sqrt{(x_1-x_2)^2+(y_1-y_2)^2+(z_1-z_2)^2} @f]
        ///
        /// @param other The other point to which we are determining the distance.
        inline double distance_to(const Point3D &other);


        std::string to_string() {
            std::ostringstream rv;
            rv << "(" << x << ", " << y << ", " << z << ")";
            return rv.str();
        }

        template <class _T>
        void set(_T x, _T y, _T z) {
            this->x = (double)x;
            this->y = (double)y;
            this->z = (double)z;
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
//{{{
double Point3D::distance_to(const Point3D &other) {
    double dx = x - other.x;
    double dy = y - other.y;
    double dz = z - other.z;
    return sqrtf(dx*dx + dy*dy + dz*dz);
}
//}}}

/// Subtract the two vectors and return the new Vector.
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
//{{{
template <class _T>
inline Vector operator*(Vector &v, const _T n) {
    return Vector(v.x * (double)n, v.y * (double)n, v.z * (double)n);
}
//}}}

#endif
