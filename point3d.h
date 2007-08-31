#ifndef POINT3D_H
#define POINT3D_H

#include <cmath>
#include <sstream>

class Point3D;
typedef Point3D Vector;

//{{{
class Point3D {
    public:
        float x;
        float y;
        float z;

        Point3D() : x(0.0f), y(0.0f), z(0.0f) {}

        template <class _T>
        Point3D(_T _x, _T _y) : x((float)_x), y((float)_y), z(0.0f) {}

        template <class _T>
        Point3D(_T _x, _T _y, _T _z) : x((float)_x), y((float)_y), z((float)_z) {}

        Point3D(const Point3D &other) : x(other.x), y(other.y), z(other.z) {}

        /// Returns the distance between this point and the other point.  This is defined by the
        /// following function, copied from http://en.wikipedia.org/wiki/Distance#Geometry.
        ///
        /// @f[ d=\sqrt{(\Delta x)^2+(\Delta y)^2+(\Delta z)^2}=\sqrt{(x_1-x_2)^2+(y_1-y_2)^2+(z_1-z_2)^2} @f]
        ///
        /// @param other The other point to which we are determining the distance.
        inline float distance_to(const Point3D &other);


        std::string to_string() {
            std::ostringstream rv;
            rv << "(" << x << ", " << y << ", " << z << ")";
            return rv.str();
        }

        template <class _T>
        void set(_T x, _T y, _T z) {
            this->x = (float)x;
            this->y = (float)y;
            this->z = (float)z;
        }

        /// Calculate the normalized vector from the origin to the destination.
        void normalize() {
            float norm = sqrtf(x * x + y * y + z * z);
            x /= norm;
            y /= norm;
            z /= norm;
        }

};
//}}}

// This is inline, it must be defined in the header.
//{{{
float Point3D::distance_to(const Point3D &other) {
    float dx = x - other.x;
    float dy = y - other.y;
    float dz = z - other.z;
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
inline Vector operator*(const Vector &A, float factor) {
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
inline float dot_product(const Vector &A, const Vector &B) {
    return (A.x * B.x) + (A.y * B.y) + (A.z * B.z);
}
//}}}
//{{{
template <class _T>
inline Vector operator*(Vector &v, const _T n) {
    return Vector(v.x * (float)n, v.y * (float)n, v.z * (float)n);
}
//}}}

#endif
