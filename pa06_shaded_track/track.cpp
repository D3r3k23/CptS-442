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


//
//  track design parameters
//
//  Modifying any of these may change the tessellations and therefore
//  the statistics, but in the later PA's you're welcome to change
//  them to alter the aesthetics of the track. It's your
//  responsibility, though, to make sure that the supports don't
//  intersect (or come close) to the track.
//

// (x, y, z) frequencies of the trigonometric curve
const Vec3 Track::freq( 1.00,  2.00, 3.00);

// (x, y, z) magnitudes of trigonometric curve
const Vec3 Track::mag( 0.80,  0.80, 0.20);

// number of ties per support
const int Track::nTiesPerSupport = 8;

// number of tessellated rail segments per support
const int Track::nRailSegmentsPerSupport = 4 * nTiesPerSupport;

// number of tessellated rail segments
const int Track::nRailSegments = nSupports * nRailSegmentsPerSupport;

// number of supports
const int Track::nSupports = 15;

// = number of sides for all Tubes used for track
const int Track::nTheta = 6;


// (x, y, z) offsets of the trigonometric curve
const Point3 Track::offset(0.00,  0.00, Track::mag.g.z + 0.20);

// (x, y, z) phases of the trigonometric curve
const Vec3 Track::phase(-0.03,  0.69, 0.00);

// radius (in world coordinates) of all Tubes used for track
const double Track::radius = 0.005;

// separation of the two rails
const double Track::railSep = 10.0 * radius;


void Track::addSupports(const double maxHeight)
{
    //
    // ASSIGNMENT (PA06)
    //
    // Borrowing a bit from your PA02 solution, add code here to
    // create the track supports. There are `nSupports` of them,
    // uniformly spaced in `u` (by `dU`, perhaps). For each support,
    // add (append to `supportTubes`) a Tube of radius `radius` around
    // a LineSegment that extends from the `guideCurve` (evaluated at
    // parameter u) to the ground (z = 0).
    //
    // As these supports are vertical, you'll need an approximate
    // "never parallel" vector that is not (0,0,1). (1,0,0) (i.e.,
    // `vX`) works.
    //
    // To make the `nJ`-1 segments all about the same length (a
    // good idea), set `nJ` to
    //
    //   max(2, round(height * nJMax / maxHeight))
    //
    // where `height` is the height of the support, `nJMax` is an
    // arbitrary maximum value of `nJ` (I recommend 10) and the passed
    // parameter `maxHeight` is the maximum height of the track.
    //
    // 15 lines in instructor solution (YMMV)
    //
    //std::cout << "Creating support Tubes..." << '\n';
    double u = 0.0;
    const double uStep = 1.0 / nSupports;
    for (int i = 0; i < nSupports; i++, u += uStep)
    {
        Point3 point = (*guideCurve)(u);
        //std::cout << "supportTubes[" << i << "]: point = " << point << '\n';
        auto supportLine = new LineSegment({point.g.x, point.g.y, 0.0}, point, {1.0, 0, 0});

        const int nJ = max(2, (int)round(point.g.z * 10 / maxHeight));
        supportTubes.push_back(new Tube(supportLine, radius, nTheta, nJ, false));
    }
}


void Track::addTies(const Curve *leftRailCurve,
                    const Curve *rightRailCurve)
{
    //
    // ASSIGNMENT (PA06)
    //
    // Add code here to draw ties for the rails. There are
    // `nSupports`*`nTiesPerSupport` of them, uniformly spaced in `u`.
    // Each Tube in `tieTubes` uses a LineSegment that extends from
    // the `leftRailCurve` (evaluated at `u`) to the `rightRailCurve`
    // (also evaluated at `u`) and a radius `radius`. Use an `nJ` of 4
    // for these Tubes.
    //
    // 11 lines in instructor solution (YMMV)
    //
    const int nTies = nTiesPerSupport * nSupports;
    double u = 0.0;
    const double uStep = 1.0 / nTies;
    for (int i = 0; i < nTies; i++, u += uStep)
    {
        Point3 leftPoint  =  (*leftRailCurve)(u);
        Point3 rightPoint = (*rightRailCurve)(u);

        auto tieLine = new LineSegment(leftPoint, rightPoint, {0, 0, 1.0});
        tieTubes.push_back(new Tube(tieLine, radius, nTheta, 4, false));
    }
}


void Track::display(const Transform &viewTransform)
{
    // set matrix transform
    Transform identityTransform;

    scene->eadsShaderProgram->setViewMatrix(viewTransform);
    scene->eadsShaderProgram->setNormalMatrix(identityTransform);

    // set support attributes
    Rgb redRgb(1.0, 0.1, 0.1);
    Rgb whiteRgb(1.0, 1.0, 1.0);

    scene->eadsShaderProgram->setEmittance(blackColor);
    scene->eadsShaderProgram->setDiffuse(0.4 * redRgb);
    scene->eadsShaderProgram->setAmbient(0.4 * redRgb);
    scene->eadsShaderProgram->setSpecular(0.4 * whiteRgb, 40.0);
    scene->eadsShaderProgram->start();

    // draw supports
    //std::cout << "Drawing support Tubes..." << '\n';
    for (unsigned int i = 0; i < supportTubes.size(); i++)
    {
        //std::cout << "i=" << i << '\n';
        supportTubes[i]->draw(this);
    }

    // set tie attributes
    scene->eadsShaderProgram->setEmittance(blackColor);
    scene->eadsShaderProgram->setDiffuse(0.4 * redRgb);
    scene->eadsShaderProgram->setAmbient(0.4 * redRgb);
    scene->eadsShaderProgram->setSpecular(0.4 * whiteRgb, 40.0);
    scene->eadsShaderProgram->start();

    // draw ties
    //std::cout << "Drawing tie Tubes..." << '\n';
    for (unsigned int i = 0; i < tieTubes.size(); i++)
    {
        //std::cout << "i=" << i << '\n';
        tieTubes[i]->draw(this);
    }

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
    //std::cout << "Drawing left rail Tube..." << '\n';
    leftRailTube->draw(this);
    //std::cout << "Drawing right rail Tube..." << '\n';
    rightRailTube->draw(this);

    // draw hedgehogs

    // all track-related quills are drawn with this length
    double quillLength = 0.002;
    displayHedgehogs(viewTransform, quillLength);
}


void Track::setGuideCurve(void)
{
    //
    // ASSIGNMENT (PA06)
    //
    // Add code here to do the following:
    //
    // - Create a `guideCurve` identical to the one you did for PA02,
    //   except that you'll need to specify an "approximate up
    //   vector". Make it the z-direction: Vector(0,0,1). This will
    //   match the "never parallel" condition, since it is highly
    //   unlikely that your track will ever go straight up.
    //
    guideCurve = new TrigonometricCurve(mag, freq, phase, offset, {0, 0, 1.0});
}


Track::Track(void) : SceneObject()
{
    //
    // ASSIGNMENT (PA06)
    //
    // Modify and extend your PA02 code as follows:
    //
    // - Replace your `trigCurve` instance with a call to
    //   `Track::setGuideCurve()`.
    //
    // - Instance a `leftRailCurve`, an OffsetCurve offset from the
    //   guide curve by 0.5 * railSep in the U direction (i.e. in the
    //   direction of the ties).
    //
    // - Instance a Tube for the left rail whose frame curve is
    //   `leftRailCurve`, whose approximate up vector is (0,0,1)
    //   (i.e. vZ) and whose shader program is the one you instanced
    //   above.
    //
    // - Instance a `rightRailCurve`, an OffsetCurve offset from the
    //   guide curve by -0.5 * railSep in the U direction (i.e.
    //   on the other side of the guide curve from `leftRailCurve`).
    //
    // - Instance a Tube for the right rail whose frame curve is
    //   `rightRailCurve`, whose approximate up vector is (0,0,1)
    //   (i.e. vZ) and whose shader program is the one you instanced
    //   above.
    //
    // - Call addTies() to add ties using `leftRailCurve` and
    //   `rightRailCurve` and the above shader program.
    //
    // - Compute `maxSupportHeight` by adding twice the magnitude of
    //   the z-component of `mag` (a global) to the z-component of
    //   `offset` (another global).
    //
    // - Call addSupports() with `leftCurve`, `rightCurve`, and
    //   `maxSupportHeight`.
    //
    // 40 lines in instructor solution (YMMV)
    //
    setGuideCurve();

    auto leftRailCurve  = new OffsetCurve(guideCurve, { 0.5 * railSep, 0, 0}, {0, 0, 1.0});
    auto rightRailCurve = new OffsetCurve(guideCurve, {-0.5 * railSep, 0, 0}, {0, 0, 1.0});

    leftRailTube  = new Tube(leftRailCurve,  radius, nTheta, nRailSegments, true);
    rightRailTube = new Tube(rightRailCurve, radius, nTheta, nRailSegments, true);

    addTies(leftRailCurve, rightRailCurve);

    const int maxSupportHeight = 2 * mag.g.z + offset.g.z;
    addSupports(maxSupportHeight);
}

