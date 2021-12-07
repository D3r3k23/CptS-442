#ifndef INCLUDED_CURVE

//
// The "curve" module provides the Curve class (see below).
//

#include <assert.h>
#include <vector>

#include "basis.h"
#include "color.h"
#include "geometry.h"
#include "poly_line.h"
#include "transform.h"
#include "vec.h"


class Curve
//
// virtual class implementing a 3D parametric curve
//
{
protected:
   Vector3 vNeverParallel;

public:

    // at least one older g++ compiler complains if this is missing
    virtual ~Curve() { };

    const void coordinateFrame(const double u, Point3 &p,
                         Vector3 &vU, Vector3 &vV, Vector3 &vW) const;

    virtual const Point3 operator()(const double u, Vector3 *dp_du = NULL)
        const = 0;
};


//
// a cubic (uniform, non-rational) B-Spline
//
class BSplineCurve : public Curve
{
    UniformCubicBSplineBasis basis; // set of polynomials that define the curve
    Point3 *cvs; // the control vertices
    int nCvs;
    bool isClosed; // curve closes on itself

public:
    BSplineCurve(const vector<Point3> cvs_,
                 const bool isClosed_, const Vector3 &vNeverParallel_)
        : isClosed(isClosed_)
    {
        nCvs = cvs_.size();
        assert((isClosed && nCvs >= 2) || (!isClosed && nCvs >= 4));

        vNeverParallel = vNeverParallel_;
        cvs = new Point3[nCvs];
        for (int i = 0; i < nCvs; i++)
            cvs[i] = cvs_[i];
    };

    // at least one older g++ compiler complains if this is missing
    virtual ~BSplineCurve() { };

    const Point3 operator()(const double u, Vector3 *dp_du = NULL) const;
};


//
// An offset curve starts from another curve, a "frame curve" and
// adds a constant offset to that curve in that curve's local
// coordinate system.
//
class OffsetCurve : public Curve
{
    Curve *frameCurve;
    Vector3 offset;

public:
    OffsetCurve(Curve *frameCurve_, Vector3 offset_, const Vector3
                &vNeverParallel_)
        : frameCurve(frameCurve_), offset(offset_)
    {
        vNeverParallel = vNeverParallel_;
    };

    // at least one older g++ compiler complains if this is missing
    virtual ~OffsetCurve() { };

    const Point3 operator()(const double u, Vector3 *dp_du = NULL) const;
};


//
// A LineSegment is just a linearly-interpolatable Curve between two
// endpoints. This is *not* the same thing as a PolyLine with two
// points or a Line with a single line, which are Tessellations (and
// do not need to be interpolated).
//
class LineSegment : public Curve
{
    Point3 p0, p1; // the endpoints

public:
    LineSegment(Point3 p0_, Point3 p1_, const Vector3 vNeverParallel_)
        : p0(p0_), p1(p1_)
    {
        assert((p1 - p0).mag() > EPSILON);
        vNeverParallel = vNeverParallel_;
    };

private:
    const Point3 operator()(const double u, Vector3 *dp_du = NULL) const;
};


class TrigonometricCurve : public Curve
{
    Vec3 mag, freq, phase;
    Point3 offset;

public:
    TrigonometricCurve(Vec3 mag_, Vec3 freq_, Vec3 phase_, Point3 offset_,
                       Vector3 vNeverParallel_)
        : mag(mag_), freq(freq_), phase(phase_), offset(offset_)
    {
        vNeverParallel = vNeverParallel_;
    };

    const Point3 operator()(const double u, Vector3 *dp_du = NULL) const;
};

#define INCLUDED_CURVE
#endif // INCLUDED_CURVE
