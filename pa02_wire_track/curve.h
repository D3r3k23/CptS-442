#ifndef INCLUDED_CURVE

//
// The "curve" module provides the Curve class (see below).
//

#include <assert.h>
#include <vector>

#include "color.h"
#include "geometrical_object.h"
#include "geometry.h"
#include "poly_line.h"
#include "transform.h"
#include "vec.h"


class Curve
//
// virtual class implementing a 3D parametric curve
//
{
public:

    // at least one older g++ compiler complains if this is missing
    virtual ~Curve() { };

    const void coordinateFrame(const double u, Point3 &p,
                         Vector3 &vU, Vector3 &vV, Vector3 &vW) const;

    virtual const Point3 operator()(const double u) const = 0;
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
    LineSegment(Point3 p0_, Point3 p1_)
        : p0(p0_), p1(p1_)
    {
        assert((p1 - p0).mag() > EPSILON);
    };

private:
};


class TrigonometricCurve : public Curve
{
    Vec3 mag, freq, phase;
    Point3 offset;

    PolyLine *tessellationPolyLine;
    int nI;

public:
    Color color;

    TrigonometricCurve(Vec3 mag_, Vec3 freq_, Vec3 phase_, Point3 offset_,
                        int nI_)
        : mag(mag_), freq(freq_), phase(phase_), offset(offset_)
    {
        tessellationPolyLine = NULL;
        nI = nI_;
    }

    const Point3 operator()(const double u) const;
    void draw(const Transform &viewTransform);

private:
    void tessellate(void);
};

#define INCLUDED_CURVE
#endif // INCLUDED_CURVE
