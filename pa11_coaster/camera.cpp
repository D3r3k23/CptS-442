#include "camera.h"
#include "controller.h"
#include "geometry.h"
#include "minmax.h"
#include "scene.h"
#include "transform.h"
#include "wrap_cmath_inclusion.h"

static const double RADIANS_PER_DEGREE = M_PI / 180.0;

Camera camera;

Camera::Camera(void)
{
    firstPerson.path  = NULL;
    firstPerson.u     = 0.0;
    firstPerson.roll  = 0.0;
    firstPerson.pitch = 0.0;
    firstPerson.yaw   = 0.0;

    thirdPerson.roll  = 0.0;
    thirdPerson.pitch = 0.0;
    thirdPerson.yaw   = 0.0;

    // 'near' and 'far' components are set in setExtent()
    orthographic.towards = Vector3(0.0, 0.0, 1.0); // OpenGL default
    orthographic.default_.left  = orthographic.default_.bottom = -1.0;
    orthographic.default_.right = orthographic.default_.top    =  1.0;

    perspective.default_.verticalFieldOfView = 45.0; // degrees
    perspective.default_.aspect = 1.0; // assume we start with a square window
    setExtent(2.0); // should be revised once model is defined
    orthographic.current = orthographic.default_;
    perspective.current = perspective.default_;
}

void Camera::setExtent(double extent_)
//
// sets the `extent` of the camera: the length of the longest edge of
// the bounding box of the entire model, including the surroundings
//
{
    //
    // This is the farthest the camera will be from the most distant
    // object in the mode, with a fudge factor of 10%.
    //
    double longestDiagonal = sqrt(3.0) * extent_ * 1.1;

    orthographic.default_.near_ = -longestDiagonal;
    orthographic.default_.far_  =  longestDiagonal;
    orthographic.current.near_ = -longestDiagonal;
    orthographic.current.far_  =  longestDiagonal;

    perspective.default_.near_ = 0.01;
    perspective.default_.far_  = longestDiagonal;
    perspective.current.near_ = 0.01;
    perspective.current.far_  = longestDiagonal;
}


void Camera::magnify(double factor)
{
    // the only time magnification makes sense
    assert(controller.useOrthographic);

    double left   = orthographic.current.left;
    double right  = orthographic.current.right;
    double top    = orthographic.current.top;
    double bottom = orthographic.current.bottom;
    double width  = (right - left) / factor;
    double height = (top - bottom) / factor;
    double centerX =  0.5 * (left + right);
    double centerY =  0.5 * (top + bottom);

    left   = centerX - 0.5 * width;
    right  = left + width;
    bottom = centerY - 0.5 * height;
    top    = bottom + height;

    orthographic.current.left   = left;
    orthographic.current.right  = right;
    orthographic.current.bottom = bottom;
    orthographic.current.top    = top;
}


void Camera::move(double dU)
{
    firstPerson.u += dU;
    // The next loop should only be executed once at most, but with
    // parametrics we should be careful.
    while (firstPerson.u > 1.0)
        firstPerson.u -= 1.0;
}


Transform Camera::projectionTransform(void)
{
    Transform transform;

    if (controller.useOrthographic) {
        double left = orthographic.current.left;
        double right = orthographic.current.right;
        double top = orthographic.current.top;
        double bottom = orthographic.current.bottom;
        double near_ = orthographic.current.near_;
        double far_ = orthographic.current.far_;
        return Transform(
              2.0/(right-left),
                         0,
                                    0,
                                                   -(right+left)/(right-left),
              0,
                         2.0/(top-bottom),
                                    0,
                                                   -(top+bottom)/(top-bottom),
              0,
                         0,
                                    -2.0/(far_-near_),
                                                   -(far_+near_)/(far_-near_),
              0,
                         0,
                                    0,
                                                   1
                             );
    } else {
        double alpha = RADIANS_PER_DEGREE
            * perspective.current.verticalFieldOfView / 2;
        double verticalScale = tan(alpha);
        double horizontalScale = perspective.current.aspect * verticalScale;
        double near_ = perspective.current.near_;
        double far_ = perspective.current.far_;
        double left   = -near_ * horizontalScale;
        double right  =  near_ * horizontalScale;
        double bottom = -near_ * verticalScale;
        double top    =  near_ * verticalScale;
        return Transform(
              2*near_/(right-left),
                         0,
                                    (right+left)/(right-left),
                                                   0,
              0,
                         2*near_/(top-bottom),
                                    (top+bottom)/(top-bottom),
                                                   0,
              0,
                         0,
                                    -(far_+near_)/(far_-near_),
                                                   -2*far_*near_/(far_-near_),
              0,
                         0,
                                    -1,
                                                   0
                             );
    }
}


void Camera::reset(int canvasWidth, int canvasHeight)
{
    //
    // We reset both orthographic and perspective projection and
    // viewing parameters.
    //
    orthographic.current = orthographic.default_;
    perspective.current  = perspective.default_;

    firstPerson.roll = 0.0;
    firstPerson.pitch = 0.0;
    firstPerson.yaw = 0.0;

    thirdPerson.roll = 0.0;
    thirdPerson.pitch = 0.0;
    thirdPerson.yaw = 0.0;
    setAspect(((double) canvasWidth) / canvasHeight);
}


void Camera::reshape(const int canvasWidth, const int canvasHeight)
//
// adjust the camera (probably) in response to a resize request
//
{
    setAspect(((double) canvasWidth) / canvasHeight);
}


void Camera::setAspect(double canvasAspect)
//
// adjusts the aspect of the Camera's "world" to match `canvasAspect`
//
{
    double left = orthographic.current.left;
    double right = orthographic.current.right;
    double bottom = orthographic.current.bottom;
    double top = orthographic.current.top;
    double worldWidth  = right - left;
    double worldHeight = top - bottom;
    double worldCenterX = 0.5 * (right + left);
    double worldCenterY = 0.5 * (top + bottom);

    if (canvasAspect > 1.0) {
        // preserve world height
        worldWidth = worldHeight * canvasAspect;
        left = worldCenterX - 0.5 * worldWidth;
        right = left + worldWidth;
    } else {
        // preserve world width
        worldHeight = worldWidth / canvasAspect;
        bottom = worldCenterY - 0.5 * worldHeight;
        top = bottom + worldHeight;
    }
    orthographic.current.left = left;
    orthographic.current.right = right;
    orthographic.current.bottom = bottom;
    orthographic.current.top = top;
    perspective.current.aspect = canvasAspect;
}


void Camera::setPath(const Curve *path)
{
    firstPerson.path = path;
}


void Camera::shift(double xShift, double yShift)
//
// shifts the camera viewing volume in x or y (or both, in
// principle).  xShift and yShift are fractions of the window width
// and height, respectfully.
//
{
    assert(controller.useOrthographic); // only makes sense in orthographic mode
    double dx = xShift
        * (orthographic.current.right - orthographic.current.left);
    double dy = yShift
        * (orthographic.current.top   - orthographic.current.bottom);

    orthographic.current.bottom += dy;
    orthographic.current.top    += dy;
    orthographic.current.left   += dx;
    orthographic.current.right  += dx;
}


void Camera::twist(const double dRoll, const double dPitch, const double dYaw)
{
    if (controller.useFirstPerson) {
        firstPerson.roll += dRoll;
        firstPerson.pitch += dPitch;
        firstPerson.yaw += dYaw;
    } else {
        thirdPerson.roll += dRoll;
        thirdPerson.pitch += dPitch;
        thirdPerson.yaw += dYaw;
    }
}


Matrix4 Camera::lookAt(const Point3 &eye,
                       const Vector3 &viewDirection,
                       const Vector3 &upApprox)
//
// returns the "look at" matrix: a small variation on gluLookAt()
//
{
    Vector3 upApproxNorm = upApprox.normalized();
    Vector3 uePerp = viewDirection.cross(upApproxNorm).normalized();
    Vector3 up = uePerp.cross(viewDirection);
    Matrix4 mRot(
        uePerp.g.x, uePerp.g.y, uePerp.g.z, 0.0,
        up.g.x,     up.g.y,     up.g.z,     0.0,
        -viewDirection.g.x,
                      -viewDirection.g.y,
                                    -viewDirection.g.z,
                                                  0.0,
        0.0,          0.0,          0.0,          1.0);
    Matrix4 mTrans(
        1.0,          0.0,          0.0,          -eye.g.x,
        0.0,          1.0,          0.0,          -eye.g.y,
        0.0,          0.0,          1.0,          -eye.g.z,
        0.0,          0.0,          0.0,          1.0);
    return mRot * mTrans;
}


Transform Camera::viewTransform(void)
// returns Transform that converts world to camera-relative
// coordinates
{
    Point3 eye;
    Vector3 viewDirection;
    Vector3 upApprox;
    Transform transform;

    if (controller.useFirstPerson) {
        // rotate the camera around its origin
        transform.rotate(firstPerson.roll * RADIANS_PER_DEGREE,  vX);
        transform.rotate(firstPerson.pitch * RADIANS_PER_DEGREE, vY);
        transform.rotate(firstPerson.yaw * RADIANS_PER_DEGREE,   vZ);

        // rider is above and slightly forward of car
        transform.translate(0.0, -0.07, 0.02);

        transform.rotate(0.3, vX); // look downwards (by default)
        transform.rotate(M_PI, vY); // look in the direction of travel
        Transform coordinateFrame
            = firstPerson.path->coordinateFrame(firstPerson.u);
        transform = transform * coordinateFrame.inverse();
    } else {
        eye = Point3(0.0, 0.0, 1.0);
        Point3 center(0.0, 0.0, 0.0);
        viewDirection = (center - eye).normalized();
        upApprox = Vector3(0.0, 1.0, 0.0);
        transform = lookAt(eye, viewDirection, upApprox);

        // rotate the model around its origin
        transform.rotate(thirdPerson.roll * RADIANS_PER_DEGREE,  vX);
        transform.rotate(thirdPerson.pitch * RADIANS_PER_DEGREE, vY);
        transform.rotate(thirdPerson.yaw * RADIANS_PER_DEGREE,   vZ);
    }

    return transform;
}


void Camera::zoom(double factor)
{
    assert(factor > 0.0);
    assert(!controller.useOrthographic); // only makes sense in perspective mode
    double hToD = tan(0.5 * RADIANS_PER_DEGREE
                      * perspective.current.verticalFieldOfView);
    perspective.current.verticalFieldOfView = 2.0 * atan(factor * hToD)
        / RADIANS_PER_DEGREE; // convert atan() result back to degrees
}

