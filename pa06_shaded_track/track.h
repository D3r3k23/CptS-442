#ifndef INCLUDED_TRACK

//
// The "track" module provides the Track class (see below).
//

using namespace std;

#include "curve.h"
#include "scene_object.h"
#include "tube.h"


class Track : public SceneObject
//
// a rollercoaster track
//
{
private:
    // components
    Tube *leftRailTube;
    Tube *rightRailTube;
    vector<Tube *> supportTubes;
    vector<Tube *> tieTubes;

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
    static const int nTiesPerSupport;
    // = number of ties per support
    static const int nTheta;
    // = number of sides for all Tubes used for track
    static const Point3 offset;
    // = (x, y, z) offsets of the trigonometric guide curve
    static const Vec3 phase;
    // = (x, y, z) phases of the trigonometric guide curve
    static const double radius;
    // = radius (in NDC) of all Tubes used for track
    static const double railSep;
    // = separation of the two rails

    void addSupports(const double maxHeight);
    void addTies(const Curve *leftCurve, const Curve *rightCurve);
    void display(const Transform &viewTransform);

private:
    void setGuideCurve(void);

public:
    Curve *guideCurve;

    Track(void);
};

#define INCLUDED_TRACK
#endif // INCLUDED_TRACK
