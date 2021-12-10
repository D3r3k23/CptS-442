#ifndef INCLUDED_TRACK

//
// The "track" module provides the Track class (see below).
//

using namespace std;

#include "curve.h"
#include "ground.h"
#include "scene_object.h"
#include "tube.h"

typedef enum {
    LAYOUT_BSPLINE,            // default (after bsplines defined)
    LAYOUT_PLANAR_CIRCLE,      // mainly to validate banking computation
    LAYOUT_TRIG,               // the original figure-8 trigonometric curve
} Layout;

#define DEFAULT_TRACK_BSPLINE_CVS_FNAME "track_bspline_cvs.csv"

extern const struct TagOfLayout {
    Layout layout;
    string tag;
} tagOfLayout [];
extern const unsigned int nTagOfLayout;

extern const bool parseLayout(const string, Layout &layout);

class Track : public SceneObject
//
// a rollercoaster track
//
{
private:
    // These rail curves are now attributes.
    OffsetCurve *leftRailCurve;
    OffsetCurve *rightRailCurve;

    // components
    Tube *leftRailTube;
    Tube *rightRailTube;
    vector<Tube *> supportTubes;
    vector<Tube *> tieTubes;

    // track design parameters

    static const double approxRailSegmentLength;
    // = approximate rail segment length (in NDC units)
    static const double approxTieSep;
    // = approximate separation (in NDC units) between ties
    static const Vec3 freq;
    // = (x, y, z) frequencies of the trigonometric guide curve
    static const Vec3 mag;
    // = (x, y, z) magnitudes of the trigonometric guide curve
    static const int nSupports;
    // = number of supports
    static const int nRailSegmentsPerTie;
    // = number of segments between ties
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
    static const double speedAtTop; // speed at zMax of curve
public:
    void addTies(void);
    void addSupports(const Ground *ground);
    
private:
    void display(const Transform &viewProjectionTransform,
                 Transform worldTransform);

private:
    void setGuideCurve(const Layout layout, const string trackBsplineCvsFname);

public:
    Curve *guideCurve;
    const Ground* ground;
    double zMax; // maximum z value of track

    Track(const Layout layout, const string trackBsplineCvsFname, const Ground *ground);
    const double integrationStep(int &nU) const;
    const int numberOfTies(void) const;
    //
    // Assuming energy conservation, the speed of a car on the track
    // is a function of its position (ultimately, its height) along
    // the track.
    //
    const double speed(double u) const;
    const double supportSeparation(void) const;
    const double tieSeparation(void) const;
};

#define INCLUDED_TRACK
#endif // INCLUDED_TRACK
