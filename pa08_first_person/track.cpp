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
const int Track::nTheta = 12;


// (x, y, z) offsets of the trigonometric curve
const Point3 Track::offset(0.00,  0.00, Track::mag.g.z + 0.20);

// (x, y, z) phases of the trigonometric curve
const Vec3 Track::phase(-0.03,  0.69, 0.00);

// radius (in world coordinates) of all Tubes used for track
const double Track::radius = 0.005;

// separation of the two rails
const double Track::railSep = 10.0 * radius;


const struct TagOfLayout tagOfLayout[] = {
    { LAYOUT_BSPLINE,         "bspline" },
    { LAYOUT_PLANAR_CIRCLE,   "circle" },
    { LAYOUT_TRIG,            "trig" },
};
const unsigned int nTagOfLayout = N_ELEM(tagOfLayout);


void Track::addSupports(const double maxHeight, const Ground *ground)
{
    //
    // Copy your previous (PA07) solution here.
    //
}


void Track::addTies(const Curve *leftRailCurve,
                    const Curve *rightRailCurve)
{
    //
    // Copy your previous (PA06) solution here.
    //
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
}


Track::Track(const Layout layout, const string trackBsplineCvsFname,
             const Ground *ground) : SceneObject()
{
    //
    // Copy your previous (PA07) solution here.
    //
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

