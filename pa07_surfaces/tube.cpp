#include <iostream>
using namespace std;

#include "curve.h"
#include "geometry.h"
#include "tube.h"
#include "wrap_cmath_inclusion.h"


const Point3 Tube::operator()(const double u, const double v,
        Vector3 &dp_du, Vector3 &dp_dv) const
//
// returns a Point3 on the surface of the tube. `u` maps to the
// aziumuthal angle around the tube. `v` maps to the axial position
// along the curve determined by the of the guiding curve. As
// parameters. They both vary from 0 to 1.
//
{
    //
    // ASSIGNMENT (PA07)
    //
    // Now that the (new) Surface class incorporates the draw()
    // method, we no longer need individual draw() methods for its
    // children. We *do* need the operator() methods (like this one)
    // to map `u` and `v` to the surface properties we need: the
    // position (which the method returns) and the tangents `dp_du`
    // and `dp_dv`.
    //
    // Use curve->coordinateFrame() to get the frame at `v`
    // and then displace the origin of the frame by `radius` in the
    // direction determined by 2pi * `u`.
    //
    // You can borrow a lot of code for this from your PA06 solution
    // for Tube::tessellate().
    //
    // 10 lines in instructor solution (YMMV)
    //
    Point3 P;
    Vector3 vU, vV, vW;
    curve->coordinateFrame(v, P, vU, vV, vW);

    double theta = 2.0 * M_PI * u;
    Vector3 Q = (radius * cos(theta) * vU) + (radius * sin(theta) * vV);

    Point3 position = P + Q;

    dp_du = Q.cross(vW).normalized();
    dp_dv = vW;

    return position;
}

