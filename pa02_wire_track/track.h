#ifndef INCLUDED_TRACK

//
// The "track" module provides the Track class (see below).
//

using namespace std;

#include "curve.h"
#include "lines.h"
#include "scene_object.h"


class Track : public SceneObject
//
// a rollercoaster track
//
{
private:
    // components
    TrigonometricCurve *trackTrigonometricCurve;
    Lines *supportLines;

    // track design parameters

    static const Vec3 freq;
    // = (x, y, z) frequencies of the trigonometric guide curve
    static const Vec3 mag;
    // = (x, y, z) magnitudes of the trigonometric guide curve
    static const int nSupports;
    // = number of supports
    static const int nRailSegmentsPerSupport;
    // = number of tessellated rail segments per support
    static const int nRailSegments;
    // = number of tesselated rail segments
    static const Point3 offset;
    // = (x, y, z) offsets of the trigonometric guide curve
    static const Vec3 phase;
    // = (x, y, z) phases of the trigonometric guide curve

    void display(const Transform &viewTransform);

public:

    Track(void);
};

#define INCLUDED_TRACK
#endif // INCLUDED_TRACK
