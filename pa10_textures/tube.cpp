#include <iostream>
using namespace std;

#include "curve.h"
#include "geometry.h"
#include "scene.h"
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
    // Copy your previous (PA08) solution here.
    //
    double theta = 2.0 * M_PI * u;
    Vector3 Q = {radius * cos(theta), radius * sin(theta), 0.0};

    Transform transform = curve->coordinateFrame(v);
    Point3 P = transform * (Point3{0.0, 0.0, 0.0} + Q);

    Vector3 vW{0.0, 0.0, 1.0};
    Vector3 vQ = Q.cross(vW).normalized();

    dp_du = transform * vQ;
    dp_dv = transform * vW;

    return P;
}

