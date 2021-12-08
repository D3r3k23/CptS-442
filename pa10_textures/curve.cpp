//
// This file will deal with all single-parametric curves.
//

#include "curve.h"
#include "geometry.h"
#include "poly_line.h"
#include "scene.h"
#include "wrap_cmath_inclusion.h"


const double Curve::dS(const double u, const double du) const
//
// performs a single step of a Simpson's rule integration of the curve
// length differentials ("dp_du") over [ u, u+du ], returning the
// distance increment that corresponds to the parametric increment.
//
// This is about 50% less efficient than retaining previous kernel
// evaluations (see Curve::length()), but until C++ provides us with
// generators (for integration), it's the easiest way to do this.
//
{
    double h = du / 2.0;
    Vector3 dp_du[3];

    (*this)(u,       &dp_du[0]);
    (*this)(u +   h, &dp_du[1]);
    (*this)(u + 2*h, &dp_du[2]);
    //
    // Since the intergrand does not depend on the previous values of
    // itself (dp_du), Simpson's rule integration is the equivalent
    // of the ever-popular 4th order Runge-Kutta (aka "RK4").
    //
    return du * (dp_du[0].mag() + 4.0 * dp_du[1].mag() + dp_du[2].mag()) / 6.0;
}


const double Curve::length(void) const
//
// returns the length of the curve in NDC units
//
{
    double result = 0.0;
    int nIntervals = 512;
    // Simpson's rule uses approximately 2 samples per interval.
    int nSamples = 2 * nIntervals + 1; // guaranteed odd
    double h = 1.0 / (nSamples - 1);
    for (int i = 0; i < nSamples; i++) {
        // Simpson's rule should be good enough
        Vector3 dp_du;
        (*this)(i * h, &dp_du);
        double wt = ( (i == 0 || i == nSamples - 1)
                      ? 1.0 // at endpoints, otherwise...
                      : ( i % 2
                          ? 4.0 // at odd i
                          : 2.0 // at even i
                          ) );
        result += wt * dp_du.mag();
    }
    result *= h / 3.0;
    //
    // Enable the following to compare fast (this method) and slow
    // (dS()) integrations. (As we don't call length() very often,
    // this will cause very little overhead, but disable it if that
    // becomes untrue and for some reason you don't want to cache the
    // length() result.)
    //
    // This is also an example of how to use `dS()`.
    //
# if 0 // enable for testing
    double testResult = 0.0;
    double du = 1.0 / nIntervals;
    for (int i = 0; i < nIntervals; i++)
        testResult += dS(i * du, du);
    assert(fabs(result - testResult) < EPSILON);
# endif
    return result;
}


const double Curve::zMax(void) const
//
// returns the maximum z value along the curve
//
{
    //
    // Copy your previous (PA09) solution here.
    //
    const int nSteps = 2000;
    double zMax = std::numeric_limits<double>().min();

    const double step = 1.0 / nSteps;
    double u = 0.0;
    for (int i = 0; i < nSteps; i++, u += step)
    {
        double z = (*this)(u).g.z;
        if (z > zMax)
            zMax = z;
    }
    return zMax;
}


const Point3 BSplineCurve::operator()(const double u, Vector3 *dp_du,
    Vector3 *d2p_du2) const
{
    assert(0.0 <= u && u <= 1.0);
    int nKnot = ( isClosed ? nCvs : nCvs - 3 );

    //  map `u` to [ 0, nKnot ]
    double t = nKnot * u; // between 0 and nKnot, inclusive

    // `iKnot` is the integer part of t. It will determine the first
    // index of `cvs` that controls the part of the curve we're
    // interested in.
    int iKnot = (int) t; // truncates (always, since t >= 0)
    // Note that if `u` is 1.0, `iKnot` could now equal `nKnot` which
    // could cause trouble if we use it (with a value of 0 to 3,
    // inclusive added to it) to index `cvs`. The solution for this
    // depends on whether the curve is closed or not, and we'll take
    // care of it below.

    t -= iKnot; // `t` retains the fractional part

    if (!isClosed && iKnot == nKnot) {
        // If we don't (need to) use the modulus in the evaluation
        // loop below we really want `iKnot` less than `nKnot`, so if
        // iKnot is nCvs, decrement it and increment `t` (which is
        // almost certainly 0) by 1. The evaluation should be the same
        // because of continuity.
        assert(t == 1.0); // sanity check: the only time this should happen.
        iKnot = nKnot - 1;
        t = 1.0;
    }

    // compute the bases functions and, optionally, their derivatives
    double bs[4], db_dts[4], d2b_dt2s[4];
    basis(t, bs, ( dp_du ? &db_dts : NULL ), ( d2p_du2 ? &d2b_dt2s : NULL ));

    Point3 p(0,0,0), dp_dt(0,0,0), d2p_dt2(0,0,0);
    for (int i = 0; i < 4; i++) {
        int j;

        if (isClosed)
            j = (iKnot + i) % nCvs; // the modulus "wraps" the `cvs` indices
        else
            j = iKnot + i;

        assert(0 <= j && j < nCvs);
        p += bs[i] * cvs[j];
        if (dp_du)
            dp_dt += db_dts[i] * cvs[j];
        if (d2p_du2)
            d2p_dt2 += d2b_dt2s[i] * cvs[j];
    }
    if (dp_du) {
        // effectively, `nKnot` is dt/du
        *dp_du = Vector3(dp_dt.a) * nKnot; // derivative of spline is a vector
    }
    if (d2p_du2) {
        // effectively, `nKnot * nKnot` is d2t/du2

        // derivative of spline is a vector
        *d2p_du2 = Vector3(d2p_dt2.a) * nKnot * nKnot;
    }
    return p;
}


const Transform Curve::coordinateFrame(const double u) const
{
    //
    // Copy your previous (PA09) solution here.
    //
    Point3 p;
    Vector3 dp_du, d2p_du2;
    p = (*this)(u, &dp_du, &d2p_du2);

    Vector3 d_vNeverParallel;
    if (frameIsDynamic)
    {
        double speed = scene->track->speed(u);
        double ds_du = dp_du.mag();
        Vector3 c = d2p_du2 * pow((speed / ds_du), 2);

        Vector3 g{0.0, 0.0, -gravAccel};
        d_vNeverParallel = c - g;
    }
    else
        d_vNeverParallel = vNeverParallel;

    Vector3 vW = dp_du.normalized();
    Vector3 vU = d_vNeverParallel.cross(vW).normalized();
    Vector3 vV = vW.cross(vU);

    return {
        vU.g.x, vV.g.x, vW.g.x, p.g.x,
        vU.g.y, vV.g.y, vW.g.y, p.g.y,
        vU.g.z, vV.g.z, vW.g.z, p.g.z,
        0.0,    0.0,    0.0,    1.0
    };
}


const Point3 LineSegment::operator()(const double u, Vector3 *dp_du,
    Vector3 *d2p_du2) const
{
    if (dp_du)
        (*dp_du) = p1 - p0;
    if (d2p_du2) // linear, so 2nd derivative is zero
        (*d2p_du2) = Vector3(0.0, 0.0, 0.0);
    // simple linear interpolation of the endpoints
    return p0 + u * (p1 - p0);
}


const Point3 OffsetCurve::operator()(const double u,
                                     Vector3 *dp_du, Vector3 *d2p_du2) const
{
    Point3 p;

    (*frameCurve)(u, dp_du, d2p_du2);
    Transform transform = frameCurve->coordinateFrame(u);
    // offset is a vector, but we want to transform it like a point,
    // so...
    return transform * (Point3(0,0,0) + offset);
}


const Point3 TrigonometricCurve::operator()(const double u, Vector3 *dp_du,
    Vector3 *d2p_du2) const
//
// returns ths position of the curve at the parameter `u` (0 <= `u`
// <= 1).  If `dp_du` is not NULL, it will be set to the path
// derivative vector at `u`. If `d2p_du2` is not NULL, it will be set
// the the second path derivative vector.
//
// Note: the path derivatives are *not* normalized. If the components
// of the path had dimensions of length (i.e. the path was a
// position) and `u` had dimensions of time, the path derivatives
// `dp_du` and `d2p_du2` would be a velocity and an acceleration,
// respectively. If the caller wants a unit-length (dimensionless)
// vector, it is up to them to normalize a derivative upon its
// return.
//
{
    //
    // Copy your previous (PA09) solution here.
    //
    auto calc_dp_du = [this](double u) -> Vector3
    {
        return (*this)(u + EPSILON) - (*this)(u - EPSILON);
    };

    if (dp_du)
    {
        *dp_du = calc_dp_du(u);
    }
    if (d2p_du2)
    {
        *d2p_du2 = calc_dp_du(u + EPSILON) - calc_dp_du(u - EPSILON);
    }
    Vec3 angle = 2 * M_PI * (freq * u + phase);
    return Point3(cos(angle.g.x), cos(angle.g.y), cos(angle.g.z)) * mag + offset;
}

