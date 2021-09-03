//
// This file will deal with all single-parametric curves.
//

#include "curve.h"
#include "geometry.h"
#include "poly_line.h"
#include "wrap_cmath_inclusion.h"


void TrigonometricCurve::draw(const Transform &viewTransform)
//
// If necessary, tessellate the Curve and then draw it (in
// the given color).
//
{
    //
    // ASSIGNMENT (PA02)
    //
    // Do the following:
    //
    // - If `tessellationPolyLine` is NULL, call `tessellate()` to
    //   create it.
    //
    // - Invoke the `render()` method on the tessellation.
    //
    // This will be very similar to Circle::draw(), but you'll need to
    // pass `viewTransform` to `render()` to implement 3D viewing. How
    // this actually works will be covered in a later unit.
    //
    // 3 lines in instructor solution (YMMV)
    //
}


const Point3 TrigonometricCurve::operator()(const double u) const
//
// returns ths value of the curve at the parameter u (0 <= u <= 1).
//
{
    //
    // ASSIGNMENT (PA02)
    //
    // Compute (the Vec3) angle 2*`M_PI`*(`freq` * `u` + `phase`) and
    // return a Point3 which is the value of computing the cosine of
    // each of its components, weighted by the corresponding
    // components of `mag`, then add `offset` to the returned result.
    //
    // To understand the convenience of operator overloading, use no
    // for loops to do this.
    //
    // `M_PI` comes from the standard math ("math.h") library. Never
    // declare your own value of pi (or e, for that matter).
    //
    // 5 lines in instructor solution (YMMV)
    //
    return Point3(); // replace (permits template to compile cleanly)
}


void TrigonometricCurve::tessellate(void)
//
// tessellates the trigonometric curve
//
{
    //
    // ASSIGNMENT (PA02)
    //
    // Add code to create an `nI`-element PolyLine using the curve.
    // Note that (*this)(u) returns a point along the curve and that
    // the curve is closed. (We don't want the car to fall off the
    // tracks!) Also, `u` varies from 0.0 to (`nI` - 1)/`nI` in steps
    // of 1.0/`nI`. (The point at `u` == 1.0 would be identical to
    // that at `u` == 0.0, and we avoid redundant positions throughout
    // coaster.)
    //
    // 9 lines in instructor solution (YMMV)
    //
}

