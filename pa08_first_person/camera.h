#ifndef INCLUDED_CAMERA

//
// The "camera" module provides the Camera and (later) the
// ViewingVolume classes (see below).
//

#include "curve.h"
#include "geometry.h"
#include "transform.h"


class Camera {
//
// implements a camera viewing the scene
//
public:
    // These describe the camera viewing volume:


    //
    // These two structs contain states for first vs. third person
    // drawing.
    //
    struct {
        const Curve *path;
        // parametric position of camera along path
        double u;
        // in first person, these rotate the camera
        double roll, pitch, yaw;
    } firstPerson;
    struct {
        // in third person, these rotate the camera about the model
        double roll, pitch, yaw;
    } thirdPerson;

    //
    // These two structs contain states for orthographic
    // vs. perspective drawing.
    //
    // The `default_` structs in the following should be `const`, but
    // C++ won't permit us to initialize them(!)
    //
    // We use the "_" suffixes to distinguish reserved words (e.g in
    // Microsoft-compatible C++ compilers).
    //
    struct { // used in orthographic mode
        Vector3 towards;
        struct {
            double left, right, bottom, top, near_, far_;
        } default_, current;
    } orthographic;
    struct { // used in perspective mode
        struct {
            double verticalFieldOfView; // in degrees
            double aspect; // ratio of width to height
            //
            // Actually, `near` and `far` don't change for
            // perspective viewing, but it's convenient to put them
            // here.
            //
            double near_, far_;
        } default_, current;
    } perspective;

    Camera(void);

    Matrix4 lookAt(const Point3 &eye,
                   const Vector3 &viewDirection,
                   const Vector3 &upApprox);
    void magnify(double factor);
    void move(double dU);
    Transform projectionTransform(void);
    void reset(int canvasWidth, int canvasHeight);
    void reshape(const int newCanvasWidth, const int newCanvasHeight);
    void setAspect(double canvasAspect);
    void setExtent(double extent);
    void setPath(const Curve *curve);
    void shift(double xShift, double yShift);
    void twist(const double dRoll, const double dPitch, const double dYaw);
    Transform viewTransform(void);
    void zoom(double fraction);
};

extern Camera camera;

#define INCLUDED_CAMERA
#endif // INCLUDED_CAMERA
