#include <vector>
using namespace std;

#include "controller.h"
#include "curve.h"
#include "geometry.h"
#include "geometrical_object.h"
#include "hedgehog.h"
#include "n_elem.h"
#include "scene_object.h"
#include "scene.h"
#include "track.h"
#include "tube.h"

#include "wrap_cmath_inclusion.h"

//
//  track design parameters
//
//  Modifying any of these may change the tessellations and therefore
//  the statistics, but in the later PA's you're welcome to change
//  them to alter the aesthetics of the track. It's your
//  responsibility, though, to make sure that the supports don't
//  intersect (or come close) to the track.
//

// approximate rail segment length (in NDC units)
const double Track::approxRailSegmentLength = 0.04;

// approximate separation (in NDC) between ties
const double Track::approxTieSep = railSep;

// number of rail segments between ties
const int Track::nRailSegmentsPerTie = 4;

// (x, y, z) frequencies of the trigonometric curve
const Vec3 Track::freq( 1.00,  2.00, 3.00);

// (x, y, z) magnitudes of trigonometric curve
const Vec3 Track::mag( 0.80,  0.80, 0.20);

// number of supports
const int Track::nSupports = 15;

// = number of sides for all Tubes used for track
const int Track::nTheta = 12;


// (x, y, z) offsets of the trigonometric curve
const Point3 Track::offset(0.00,  0.00, Track::mag.g.z + 0.20);

// (x, y, z) phases of the trigonometric curve
const Vec3 Track::phase(-0.03,  0.69, 0.00);

// radius (in world coordinates) of all Tubes used for track
const double Track::radius = 0.005;

// separation of the two rails
const double Track::railSep = 10.0 * radius;

const double Track::speedAtTop = 4.0 // m/s (about 8.8 mph)
        * LENGTH_UNITS_PER_METER; // speed at zMax of curve


const struct TagOfLayout tagOfLayout[] = {
    { LAYOUT_BSPLINE,         "bspline" },
    { LAYOUT_PLANAR_CIRCLE,   "circle" },
    { LAYOUT_TRIG,            "trig" },
};
const unsigned int nTagOfLayout = N_ELEM(tagOfLayout);


void Track::addSupports(const double maxHeight, const Ground *ground)
{
    //
    // ASSIGNMENT (PA09)
    //
    // Make a similar change to this method that you made to
    // `addTies()` (which you should do first) with these additional
    // modifications.
    //
    // - Maintain an `sNextSupport` as the value of `s` for the next
    //   support, just like `sNextTie`.
    //
    // - Call `supportSeparation()` to determine `dSSupport`, the
    //   distance between each support.
    //
    // - Make sure that each support always coincides with a tie.
    //
    // The recommended way to do this is to follow the same
    // integration steps as `addTies()`, but additionally keep track
    // of supports using `sNextSupport` and `dSSupport` as you did
    // `sNextTie` and `dSTie`. Whenever you would draw a tie in
    // `addTies()` don't do that, but test to see if it's time to draw
    // a support and, if so, do what you did in PA06.
    //
    const double dSTie = tieSeparation();
    const double dSSupport = supportSeparation();

    int nU;
    double dU = integrationStep(nU);

    double s = 0.0;
    double sNextTie = 0.0;
    double sNextSupport = 0.0;

    for (int i = 0; i < nU; i++)
    {
        double u = i * dU;
        if (s >= sNextTie)
        {
            if (s >= sNextSupport)
            {
                Point3 point = (*guideCurve)(u);
                double x = point.g.x;
                double y = point.g.y;
                double height = point.g.z;

                auto supportLine = new LineSegment({x, y, ground->height(x, y)}, point, {1.0, 0.0, 0.0});

                const int nJ = max(2, (int)round(height * 10 / maxHeight));
                supportTubes.push_back(new Tube(supportLine, radius, nTheta, nJ, false));

                sNextSupport += dSSupport;
            }
            sNextTie += dSTie;
        }
        s += guideCurve->dS(u, dU);
    }
}


void Track::addTies()
{
    //
    // ASSIGNMENT (PA09)
    //
    // Replace your previous solution (PA06) with the following:
    //
    // - Call tieSeparation() to determine the distance `dSTie`, the
    //   distance between each tie.
    //
    // - Call integrationStep() to determine `dU` and `nU`, the
    //   parameter step size between this frame and the next and the
    //   number of integration steps along the curve, respectively.
    //
    // - Initialize `s`, the length along the curve to 0.0.
    //
    // - Initialize a variable `sNextTie`, the value of `s` to put the
    //   next tie at, to 0.0.
    //
    // - For each integration step `i` from 0 to `nU - 1`, inclusive:
    //
    //   * Set `u` to `i * dU` the parametric value.
    //
    //   * If `s >= sNextTie`,
    //
    //     + Add a Tie as you did in PA06. (Note that `leftRailCurve`
    //       and `rightRailCurve` are now Track attributes, so replace
    //       the `leftCurve` and `rightCurve` parameters with them.)
    //
    //     + Increment `sNextTie` by `dSTie`.
    //
    //   * Increment `s` by `guideCurve->dS(u, dU)`.
    //
    const double dSTie = tieSeparation();

    int nU;
    double dU = integrationStep(nU);

    double s = 0.0;
    double sNextTie = 0.0;

    for (int i = 0; i < nU; i++)
    {
        double u = i * dU;
        if (s >= sNextTie)
        {
            Point3 leftPoint  =  (*leftRailCurve)(u);
            Point3 rightPoint = (*rightRailCurve)(u);

            auto tieLine = new LineSegment(leftPoint, rightPoint, {0.0, 0.0, 1.0});
            tieTubes.push_back(new Tube(tieLine, radius, nTheta, 4, false));

            sNextTie += dSTie;
        }
        s += guideCurve->dS(u, dU);
    }
}


void Track::display(const Transform &viewProjectionTransform,
                    Transform worldTransform)
{
    // set matrix transform
    scene->eadsShaderProgram->setModelViewProjectionMatrix(
        viewProjectionTransform * worldTransform);
    scene->eadsShaderProgram->setWorldMatrix(worldTransform);
    scene->eadsShaderProgram->setNormalMatrix(
        worldTransform.getNormalTransform());

    // set support attributes
    Rgb redRgb(1.0, 0.1, 0.1);
    Rgb whiteRgb(1.0, 1.0, 1.0);

    scene->eadsShaderProgram->setEmittance(blackColor);
    scene->eadsShaderProgram->setDiffuse(0.4 * redRgb);
    scene->eadsShaderProgram->setAmbient(0.4 * redRgb);
    scene->eadsShaderProgram->setSpecular(0.4 * whiteRgb, 40.0);
    scene->eadsShaderProgram->start();

    // draw supports
    for (unsigned int i = 0; i < supportTubes.size(); i++)
        supportTubes[i]->draw(this);

    // set tie attributes
    scene->eadsShaderProgram->setEmittance(blackColor);
    scene->eadsShaderProgram->setDiffuse(0.4 * redRgb);
    scene->eadsShaderProgram->setAmbient(0.4 * redRgb);
    scene->eadsShaderProgram->setSpecular(0.4 * whiteRgb, 40.0);
    scene->eadsShaderProgram->start();

    // draw ties
    for (unsigned int i = 0; i < tieTubes.size(); i++)
        tieTubes[i]->draw(this);

    // set rail attributes

    // Here are some reflectance choices for metallic-looking
    // rails. Pick one (or make up your own):
#  if   1 // chrome
    const Rgb kAmbient(0.25, 0.25,   0.25);
    const Rgb kDiffuse(0.4,  0.4,  0.4);
    const Rgb kSpecular(0.75, 0.75, 0.75);
    const double expoSpecular = 75.0;
#  elif 0 // brass
    const Rgb kAmbient(0.329412, 0.223529, 0.027451);
    const Rgb kDiffuse(0.780392, 0.568627, 0.027451);
    const Rgb kSpecular(0.992157, 0.941176, 0.807843);
    const double expoSpecular = 27.8974;
#  elif 0 // copper
    const Rgb kAmbient(0.19125, 0.0735,   0.0225);
    const Rgb kDiffuse(0.7038,  0.27048,  0.0828);
    const Rgb kSpecular(0.25677, 0.137622, 0.086014);
    const double expoSpecular = 12.8;
#  endif
    scene->eadsShaderProgram->setEmittance(blackColor);
    scene->eadsShaderProgram->setDiffuse(kDiffuse);
    scene->eadsShaderProgram->setAmbient(kAmbient);
    scene->eadsShaderProgram->setSpecular(kSpecular, expoSpecular);
    scene->eadsShaderProgram->start();

    // draw rail(s)
    leftRailTube->draw(this);
    rightRailTube->draw(this);

    // draw hedgehogs

    // all track-related quills are drawn with this length
    double quillLength = 0.002;
    displayHedgehogs(viewProjectionTransform, worldTransform, quillLength);
}


void Track::setGuideCurve(const Layout layout,
                          const string trackBsplineCvsFname)
{
    //
    // Copy your previous (PA07) solution here.
    //
    switch (layout)
    {
        case LAYOUT_BSPLINE:
        {
            vector<Point3> cvs_ = readPoint3s(trackBsplineCvsFname);
            guideCurve = new BSplineCurve(cvs_, true, vZ);
            break;
        }
        case LAYOUT_PLANAR_CIRCLE:
        {
            //
            // This is a circle of radius 1.0 which is 0.2 NDC units off
            // the ground. These parameters are distinct from those used
            // for the trig layout.
            //
            const Vec3       mag( 1.0,  1.0,  0.0);
            const Vec3      freq( 1.0, -1.0,  0.0);
            const Point3  offset( 0.0,  0.0,  0.2);
            const Vec3     phase( 0.0,  0.25, 0.0);
            guideCurve = new TrigonometricCurve(mag, freq, phase, offset, vZ);
            break;
        }
        case LAYOUT_TRIG:
        {
            guideCurve = new TrigonometricCurve(mag, freq, phase, offset, vZ);
            break;
        }
        default:
            // should not be reached (bad track numbers should be caught in main())
            assert(false);
    }
}


const double Track::speed(double u) const
{
    //
    // ASSIGNMENT (PA09)
    //
    // Assuming energy conservation, return the speed of a car (or
    // anything riding on the track) at position `u` along the track.
    // Remember that `speedAtTop` is its speed when it's `z`
    // coordinate is at `zMax`.
    //
    // There's a global `gravAccel` that's g (the gravitational
    // acceleration at the Earth's surface) in NDC units.
    //
    double v_top = speedAtTop;
    double z_top = zMax;
    double z = (*guideCurve)(u).g.z;
    double v = sqrt(pow(v_top, 2) + 2 * (z_top - z));
    return v;
}


Track::Track(const Layout layout, const string trackBsplineCvsFname,
             const Ground *ground)
 : SceneObject()
{
    //
    // ASSIGNMENT (PA09)
    //
    // Modify your PA07 solution to do the following:
    //
    // - Set zMax to be the maximum z value for the guide curve.
    //
    // - Enable the guide curve to use a dynamic (banked) coordinate
    //   frame.
    //
    // - Make the left and right rail curves you instance in the
    //   constructor attributes `leftRailCurve` and `rightRailCurve`.
    //   Be sure they're attributes, not local variables.
    //
    // - Remove the call to addTies(), as it will now be done *after*
    //   the Scene has been instanced (a hack to be corrected later).
    //   Long story.
    //
    setGuideCurve(layout, trackBsplineCvsFname);
    guideCurve->enableDynamicFrame();
    zMax = guideCurve->zMax();

    leftRailCurve  = new OffsetCurve(guideCurve, { 0.5 * railSep, 0.0, 0.0}, {0.0, 0.0, 1.0});
    rightRailCurve = new OffsetCurve(guideCurve, {-0.5 * railSep, 0.0, 0.0}, {0.0, 0.0, 1.0});

    const int nRailSegments = nRailSegmentsPerTie * numberOfTies();

    leftRailTube  = new Tube(leftRailCurve,  radius, nTheta, nRailSegments, true);
    rightRailTube = new Tube(rightRailCurve, radius, nTheta, nRailSegments, true);

    const int maxSupportHeight = 2 * mag.g.z + offset.g.z;
    addSupports(maxSupportHeight, ground);
}


const double Track::integrationStep(int &nU) const
//
// returns the integration step size (in parametric units) along the
// track
//
{
    int nTies = numberOfTies();

    nU = 100 * nRailSegmentsPerTie * nTies; // use small increment
    // should be enough unless curve is truly bizarre

    return 1.0 / nU;
}


const int Track::numberOfTies(void) const
//
// returns the total number of ties
//
{
    //
    // The total number of ties to draw is determined by the ratio of
    // the total length of the curve to the desired approximate
    // separation between ties.
    //
    double sTotal = guideCurve->length(); // total curve length
    return (int) ceil(sTotal / approxTieSep); // at least one
}


const double Track::supportSeparation(void) const
//
// returns the separation along the track between two supports in NDC
// units
//
{
    double sTotal = guideCurve->length(); // total curve length
    return sTotal / nSupports;
}


const double Track::tieSeparation(void) const
//
// returns the separation along the track between two ties in NDC units
//
{
    //
    // The value returned is the exact value corresponding to
    // `approxTieSep`.
    //
    double sTotal = guideCurve->length(); // total curve length
    return sTotal / numberOfTies();
}


extern const bool parseLayout(const string tag, Layout &layout)
{
    for (unsigned int i = 0; i < nTagOfLayout; i++) {
        if (tag == tagOfLayout[i].tag) {
            layout = tagOfLayout[i].layout;
            return true;
        }
    }
    return false;
}

