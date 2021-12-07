#include <string>
#include <sstream>

#include "clock.h"
#include "controller.h"
#include "n_elem.h"
#include "render_stats.h"
#include "text.h"
#include "view.h"
#include "wrap_cmath_inclusion.h"


RenderStats renderStats;

void RenderStats::display()
{
    double thisFrameTime = clock_.read() - startTime; // in seconds
    //
    // If frame time averaging is used, assign exponential weights to
    // previous frames:
    //
    // tMean[i] = a*t[i] + a*w * t[i-1] + a*w**2 * t[i-2] + ....
    //
    // which is just
    //
    //          = a*t[i] + w*tMean[i-1]
    //
    // Given that sum_{i=0}^inf (a * w**i) must == 1, we get
    //
    //     a = (1 - w)
    //
    // assuming 0 < w < 1. To define w, we choose a value of n for
    // which the relative contribution to the sum is 1/2, i.e.
    //
    //     w**n = 1/2
    //
    // and solve (approximately) for w (`weightPrevious`).
    //
    const int halfWeightFrame = 100;
    double weightPrevious = pow(0.5, 1.0 / halfWeightFrame);

    double meanFrameTimeUsec; // in microseconds
    double frameRate;
    double triangleRate;

    if (resetFrameTimer) {
        meanFrameTime = thisFrameTime;
        resetFrameTimer = false;
    } else {
        meanFrameTime =
            (1 - weightPrevious) * thisFrameTime  // = a*t[i]
            + weightPrevious * meanFrameTime;     // = w*tMean[i-1]
    }
    meanFrameTimeUsec = 1.e6 * meanFrameTime; // in microseconds
    frameRate = 1.0 / meanFrameTime;
    triangleRate = (ctTrianglesInIrregularMeshes + ctTrianglesInRegularMeshes)
        / meanFrameTime;

    enum {
        FORMAT_YESNO,
        FORMAT_INT,
        FORMAT_FLOAT
    };
    struct {
        string tag;
        bool isValid;
        int format;
        void *value;
    } lineSpecs[] = {
        { "frame number:",                    true,
          FORMAT_INT, &frameNumber },
        { "vertices:",                        true,
          FORMAT_INT, &ctVertices },
        { "lines:",                           true,
          FORMAT_INT, &ctLines },
        { "line strips:",                     true,
          FORMAT_INT, &ctLineStrips },
        { "triangles (in irregular meshes):", true,
          FORMAT_INT, &ctTrianglesInIrregularMeshes },
        { "ambient reflection enabled?",      true,
          FORMAT_YESNO, &controller.ambientReflectionEnabled },
        { "diffuse reflection enabled?",      true,
          FORMAT_YESNO, &controller.diffuseReflectionEnabled },
        { "specular reflection enabled?",     true,
          FORMAT_YESNO, &controller.specularReflectionEnabled },
        { "triangles (in regular meshes):",
                                              true,
          FORMAT_INT, &ctTrianglesInRegularMeshes },
        { "triangle strips:",                 true,
          FORMAT_INT, &ctTriangleStrips },
        { "mean frame time (usec):",          true,
          FORMAT_FLOAT, &meanFrameTimeUsec },
        { "frames/sec:",                      true,
          FORMAT_FLOAT, &frameRate },
        { "triangles/sec:",                   true,
          FORMAT_FLOAT, &triangleRate },
    };
    int maxLines = N_ELEM(lineSpecs);
    int nLines = 0;

    string *lines = new string[maxLines];
    for (int i = 0; i < maxLines; i++) {
        ostringstream sstrm;

        if (!lineSpecs[i].isValid)
            continue;
        sstrm << fixed;
        sstrm.width(35);
        sstrm << lineSpecs[i].tag;
        sstrm.width(11);
        switch (lineSpecs[i].format) {

        case FORMAT_YESNO:
            sstrm << ( *static_cast<bool *>(lineSpecs[i].value) ? "yes" : "no" );
            break;

        case FORMAT_INT:
            sstrm.precision(1);
            sstrm << *static_cast<int *>(lineSpecs[i].value);
            break;

        case FORMAT_FLOAT:
            sstrm.precision(1);
            sstrm << *static_cast<double *>(lineSpecs[i].value);
            break;

        default:
            assert(false); // unknown line format
        }
        lines[nLines++] = sstrm.str();
    }

    text.renderAt(-0.90, 0.90, lines, nLines);
    delete [] lines;
}


void RenderStats::resetTimeAveraging(void)
{
    resetFrameTimer = true;
}


void RenderStats::reset(void)
//
// Clear the all counts and mark the time. This is typically done at
// the start of a frame.
//
{
    ctVertices = 0;
    ctLines = 0;
    ctLineStrips = 0;
    ctTrianglesInIrregularMeshes = 0;
    ctTrianglesInRegularMeshes = 0;
    ctTriangleStrips = 0;
    startTime = clock_.read();
}
