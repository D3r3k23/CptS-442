#ifndef INCLUDED_CAMERA

//
// The "camera" module provides the Camera and (later) the
// ViewingVolume classes (see below).
//

#include "geometry.h"
#include "transform.h"

class ViewingVolume {
//
// the volume of the world being viewed
//
public:
    double left, right, bottom, top, near_, far_;

};

class Camera {
//
// implements a camera viewing the scene
//
public:
    // These describe the camera viewing volume:

    ViewingVolume defaultViewingVolume;
    ViewingVolume currentViewingVolume;

    double roll, pitch, yaw;

    Vector3 towards;

    Camera(void);

    void reset(int canvasWidth, int canvasHeight);
    void setAspect(double canvasAspect);
    void setExtent(double extent);
    void twist(const double dRoll, const double dPitch, const double dYaw);
    Transform viewTransform(void);
};

extern Camera camera;

#define INCLUDED_CAMERA
#endif // INCLUDED_CAMERA
