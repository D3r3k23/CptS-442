#include <vector>
using namespace std;

#include "controller.h"
#include "curve.h"
#include "geometry.h"
#include "geometrical_object.h"
#include "n_elem.h"
#include "scene.h"
#include "track.h"

#include "shader_programs.h"

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

// number of tessellated rail segments per support
const int Track::nRailSegmentsPerSupport = 8;

// number of tessellated rail segments
const int Track::nRailSegments = nSupports * nRailSegmentsPerSupport;

// number of supports
const int Track::nSupports = 15;


// (x, y, z) offsets of the trigonometric curve
const Point3 Track::offset(0.00,  0.00, Track::mag.g.z + 0.20);

// (x, y, z) phases of the trigonometric curve
const Vec3 Track::phase(-0.03,  0.69, 0.00);


void Track::display(const Transform &viewTransform)
{
    scene->constantColorShaderProgram->setColor(whiteColor);
    scene->constantColorShaderProgram->start();

    supportLines->render(viewTransform);
    trackTrigonometricCurve->draw(viewTransform);
}


Track::Track(void)
{
    //
    // ASSIGNMENT (PA02)
    //
    // Add code here that creates
    //
    // - a trigonometric ("Lissajous"), closed parametric curve that
    //   will tessellate itself into an `nRailSegment`-segment
    //   PolyLine. Use the parameters at the start of this file.
    //
    // - `nSupports` PolyLine "supports" that extend from the
    //   "ground" (z = 0) to the curve. They should be equally-spaced
    //   in the parameter u, which will vary from 0 to 1. Be sure not
    //   to have supports at both the start (u = 0) and end (u = 1).
    //
    // and adds them to the model.
    //
    // The height of the track is the twice the magnitude of the
    // curve's z-component (2 * 0.20 = 0.40 in the above), so we've
    // adjusted `offset` (see above) to add in a correction so the
    // bottom of the curve is at z = 0.10 (the z-component of the
    // offset).
    //
    // 20 lines in instructor solution (YMMV)
    //
}

