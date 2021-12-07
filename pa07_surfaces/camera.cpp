#include "camera.h"
#include "controller.h"
#include "geometry.h"
#include "minmax.h"
#include "wrap_cmath_inclusion.h"

static const double RADIANS_PER_DEGREE = M_PI / 180.0;

Camera camera;

Camera::Camera(void)
{
    roll  = 0.0;
    pitch = 0.0;
    yaw   = 0.0;
    // 'near' and 'far' components are set in setExtent()
    towards = Vector3(0.0, 0.0, 1.0); // OpenGL default
    defaultViewingVolume.left  = defaultViewingVolume.bottom = -1.0;
    defaultViewingVolume.right = defaultViewingVolume.top    =  1.0;
    setExtent(2.0); // should be revised once model is defined
    currentViewingVolume = defaultViewingVolume;
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

    defaultViewingVolume.near_ = -longestDiagonal;
    defaultViewingVolume.far_  =  longestDiagonal;
}


void Camera::magnify(double factor)
{
    double left   = currentViewingVolume.left;
    double right  = currentViewingVolume.right;
    double top    = currentViewingVolume.top;
    double bottom = currentViewingVolume.bottom;
    double width  = (right - left) / factor;
    double height = (top - bottom) / factor;
    double centerX =  0.5 * (left + right);
    double centerY =  0.5 * (top + bottom);

    left   = centerX - 0.5 * width;
    right  = left + width;
    bottom = centerY - 0.5 * height;
    top    = bottom + height;

    currentViewingVolume.left   = left;
    currentViewingVolume.right  = right;
    currentViewingVolume.bottom = bottom;
    currentViewingVolume.top    = top;
}


void Camera::reset(int canvasWidth, int canvasHeight)
{
    currentViewingVolume = defaultViewingVolume;

    roll = 0.0;
    pitch = 0.0;
    yaw = 0.0;
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
    double left = currentViewingVolume.left;
    double right = currentViewingVolume.right;
    double bottom = currentViewingVolume.bottom;
    double top = currentViewingVolume.top;
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
    currentViewingVolume.left = left;
    currentViewingVolume.right = right;
    currentViewingVolume.bottom = bottom;
    currentViewingVolume.top = top;
}


void Camera::shift(double xShift, double yShift)
//
// shifts the camera viewing volume in x or y (or both, in
// principle).  xShift and yShift are fractions of the window width
// and height, respectfully.
//
{
    double dx = xShift
        * (currentViewingVolume.right - currentViewingVolume.left);
    double dy = yShift
        * (currentViewingVolume.top   - currentViewingVolume.bottom);

    currentViewingVolume.bottom += dy;
    currentViewingVolume.top    += dy;
    currentViewingVolume.left   += dx;
    currentViewingVolume.right  += dx;
}


void Camera::twist(const double dRoll, const double dPitch, const double dYaw)
{
    roll += dRoll;
    pitch += dPitch;
    yaw += dYaw;
}


Transform Camera::viewTransform(void)
// returns Transform that converts world to camera-relative
// coordinates
{
    Point3 eye;
    Vector3 viewDirection;
    Vector3 upApprox;
    Transform transform;

    // shift the model
    double dx = 0.5 * (currentViewingVolume.right + currentViewingVolume.left);
    double dy = 0.5 * (currentViewingVolume.top + currentViewingVolume.bottom);
    double dz = 0.5 * (currentViewingVolume.far_ + currentViewingVolume.near_);

    transform.translate(dx, dy, dz);

    // Scale x and y so that the world retains its aspect ratio.
    double scaleX = 2.0
        / (currentViewingVolume.right - currentViewingVolume.left);
    double scaleY = 2.0
        / (currentViewingVolume.top - currentViewingVolume.bottom);

    // Negate z so that depth increases into the display.
    double scaleZ = -2.0
        / (currentViewingVolume.far_ - currentViewingVolume.near_);

    Vector3 scaleToNdc(scaleX, scaleY, scaleZ);
    transform.scale(scaleToNdc);

    // rotate the model around its origin
    transform.rotate(roll * RADIANS_PER_DEGREE,  vX);
    transform.rotate(pitch * RADIANS_PER_DEGREE, vY);
    transform.rotate(yaw * RADIANS_PER_DEGREE,   vZ);

    return transform;
}

