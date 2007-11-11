/* file name  : matrix.h
 * authors    : Michael Brailsford
 * created    : Sat Nov 03 15:24:52 -0500 2007
 * copyright  : (c) 2007 Michael Brailsford
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <string>
#include <cstdio>

#include "raytrace_defs.h"

/// Represents a 4x4 matrix and provides that operations required to compose
/// matrices for transformations in the 3D space.
//{{{
class Matrix {
    private:
        double matrix[4][4];

        /// Constructs a matrix with the values passed in.
        //{{{
        Matrix(double _00, double _01, double _02,
               double _10, double _11, double _12,
               double _20, double _21, double _22) {
            matrix[0][0] = _00; matrix[0][1] = _01; matrix[0][2] = _02; matrix[0][3] =   0;
            matrix[1][0] = _10; matrix[1][1] = _11; matrix[1][2] = _12; matrix[1][3] =   0;
            matrix[2][0] = _20; matrix[2][1] = _21; matrix[2][2] = _22; matrix[2][3] =   0;
            matrix[3][0] =   0; matrix[3][1] =   0; matrix[3][2] =   0; matrix[3][3] =   1;
        }
        //}}}

        /// Constructs a matrix with the values passed in.
        //{{{
        Matrix(double _00,     double _01,     double _02,     double _03,
               double _10,     double _11,     double _12,     double _13,
               double _20,     double _21,     double _22,     double _23,
               double _30 = 0, double _31 = 0, double _32 = 0, double _33 = 1) {
            matrix[0][0] = _00; matrix[0][1] = _01; matrix[0][2] = _02; matrix[0][3] = _03;
            matrix[1][0] = _10; matrix[1][1] = _11; matrix[1][2] = _12; matrix[1][3] = _13;
            matrix[2][0] = _20; matrix[2][1] = _21; matrix[2][2] = _22; matrix[2][3] = _23;
            matrix[3][0] = _30; matrix[3][1] = _31; matrix[3][2] = _32; matrix[3][3] = _33;
        }
        //}}}

    public:
        /// Constructs the identity matrix.
        //{{{
        Matrix() {
            matrix[0][0] = 1; matrix[0][1] = 0; matrix[0][2] = 0; matrix[0][3] = 0;
            matrix[1][0] = 0; matrix[1][1] = 1; matrix[1][2] = 0; matrix[1][3] = 0;
            matrix[2][0] = 0; matrix[2][1] = 0; matrix[2][2] = 1; matrix[2][3] = 0;
            matrix[3][0] = 0; matrix[3][1] = 0; matrix[3][2] = 0; matrix[3][3] = 1;
        }
        //}}}

        /// Constructs a rotation matrix to rotate points around the x-axis.
        //{{{
        static Matrix rotate_x(double angle) {
            return Matrix(1,          0,           0, 
                          0, cos(angle),  sin(angle), 
                          0, -sin(angle), cos(angle));
        }
        //}}}
        
        /// Constructs a rotation matrix to rotate points around the y-axis.
        //{{{
        static Matrix rotate_y(double angle) {
            return Matrix(
                    cos(angle), 0, -sin(angle), 
                    0,          1,           0, 
                    sin(angle), 0,  cos(angle)
                    );
        }
        //}}}
        
        /// Constructs a rotation matrix to rotate points around the z-axis.
        //{{{
        static Matrix rotate_z(double angle) {
            return Matrix( cos(angle), sin(angle), 0, 
                          -sin(angle), cos(angle), 0, 
                                    0,          0, 1);
        }
        //}}}

        /// Construct a matrix to uniformly scale.
        //{{{
        static Matrix scale(double scalar) {
            return scale(scalar, scalar, scalar);
        }
        //}}}
        
        /// Construct a matrix to scale.
        //{{{
        static Matrix scale(double scalar_x, double scalar_y, double scalar_z) {
            return Matrix(scalar_x,        0,        0, 
                                 0, scalar_y,        0,
                                 0,        0, scalar_z);
        }
        //}}}

        /// Construct a matrix to translate the camera.
        //{{{
        static Matrix translate(double dx, double dy, double dz) {
            return Matrix(1, 0, 0, dx,
                          0, 1, 0, dy,
                          0, 0, 1, dz,
                          0, 0, 0,  1);
        }
        //}}}
        
        /// Construct a matrix to translate the camera.
        //{{{
        static Matrix translate(Point3D pt) {
            return Matrix(1, 0, 0, pt.x,
                          0, 1, 0, pt.y,
                          0, 0, 1, pt.z,
                          0, 0, 0,    1);
        }
        //}}}

        // Matrix operations.
        //{{{
        Matrix operator*(const Matrix &B) const {
            Matrix rv;

            //log_info("here");
            for(int i = 0; i < 4; i++) {
                for(int j = 0; j < 4; j++) {
                    rv.matrix[i][j] = this->matrix[0][j] * B.matrix[i][0] + 
                                      this->matrix[1][j] * B.matrix[i][1] + 
                                      this->matrix[2][j] * B.matrix[i][2] + 
                                      this->matrix[3][j] * B.matrix[i][3];
                }
            }

            return rv;
        }
        //}}}
        //{{{
        void operator*=(const Matrix &B) {
            //log_info("here");
            for(int i = 0; i < 4; i++) {
                for(int j = 0; j < 4; j++) {
                    matrix[i][j] = matrix[0][j] * B.matrix[i][0] + 
                                   matrix[1][j] * B.matrix[i][1] + 
                                   matrix[2][j] * B.matrix[i][2] + 
                                   matrix[3][j] * B.matrix[i][3];
                }
            }
        }
        //}}}

        /// Transform the point passed in.
        //{{{
        Point3D transform(const Point3D &pt) const {
            return Point3D(pt.x * matrix[0][0] + pt.y * matrix[0][1] + pt.z * matrix[0][2] + matrix[0][3],
                           pt.x * matrix[1][0] + pt.y * matrix[1][1] + pt.z * matrix[1][2] + matrix[1][3],
                           pt.x * matrix[2][0] + pt.y * matrix[2][1] + pt.z * matrix[2][2] + matrix[2][3]);
        }
        //}}}


        //{{{
        void to_log() {
            //log_info("[%0-1.02f, %0-1.02f, %0-1.02f, %0-1.02f]", matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3]);
            //log_info("[%0-1.02f, %0-1.02f, %0-1.02f, %0-1.02f]", matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3]);
            //log_info("[%0-1.02f, %0-1.02f, %0-1.02f, %0-1.02f]", matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3]);
            //log_info("[%0-1.02f, %0-1.02f, %0-1.02f, %0-1.02f]", matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]);
        }
        //}}}
};
//}}}
#endif

