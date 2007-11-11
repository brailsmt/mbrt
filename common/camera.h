/* file name  : camera.h
 * authors    : Michael Brailsford
 * created    : Mon Nov 05 21:13:34 -0600 2007
 * copyright  : (c) 2007 Michael Brailsford
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "matrix.h"

/// This class represents the camera that is recording the image.
class Camera {
	protected:
        /// This is the location in 3 space where the camera is location.
        Point3D location;

        /// This is vector which points up from the camera.
        Vector up;

        /// This is vector which points to the right from the camera.
        Vector right;

        /// This is vector which points to the front of the camera.
        Vector front;

        /// This is the matrix transform required to rotate the camera.
        Matrix xfrm;

	public:
        /// Constructs the default camera located at the origin with up along
        /// the -y axis, right along the +x axis, and front along the +z axis.
		Camera()
            : location()
            , up(0.0, -1.0, 0.0)
            , right(1.0, 0.0, 0.0)
            , front(0.0, 0.0, 1.0)
            , xfrm() {}

        Camera(Point3D loc) : location(loc) {}

        Camera(Point3D loc, Point3D look_at) : location(loc) {
            // Determine the amount of rotation about the x-axis.
            xfrm *= Matrix::rotate_x(0);//-sin((direction.x - coords.x)/(direction.y - coords.y)));
            // Determine the amount of rotation about the y-axis.
            xfrm *= Matrix::rotate_y(0);//+sin((direction.y - coords.y)/(direction.z - coords.z)));
            // Determine the amount of rotation about the z-axis.
            xfrm *= Matrix::rotate_z(0);//-sin((direction.z - coords.z)/(direction.x - coords.x)));
        }

		virtual ~Camera() {}

        virtual Ray ray_through(Point3D pt) const {
            //return Ray(location, pt-location);
            return Ray(location, xfrm.transform(pt)-location);
        }
};
#endif

