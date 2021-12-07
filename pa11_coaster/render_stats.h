#ifndef INCLUDED_RENDER_STATS

//
// The "render_stats" module provides the RenderStats class (see below).
//

#include "color.h"
#include "minmax.h"

class RenderStats
//
// tracks rendering statistics
//
// This can include objects of interest (e.g. triangles) as well as
// frame numbers and times.
//
{
    double startTime;  // timestamp at start of data collection
    double meanFrameTime; // to "smooth" frame time calculations
    bool resetFrameTimer;

public:
    // statistics updated during model generation
    int frameNumber;
    int ctVertices;
    int ctLines;
    int ctLineStrips;
    int ctTrianglesInIrregularMeshes;
    int ctTrianglesInRegularMeshes;
    int ctTriangleStrips;

RenderStats()
    :
        startTime(0.0), meanFrameTime(0.0), resetFrameTimer(true),
        frameNumber(0), ctVertices(0), ctLines(0), ctLineStrips(0)
        , ctTrianglesInIrregularMeshes(0)
        , ctTrianglesInRegularMeshes(0), ctTriangleStrips(0)
        { };

    bool pendingFrameTimerReset(void) {
        return resetFrameTimer;
    };
    void reset(void);
    void resetTimeAveraging(void);
    void display(void);
};
extern RenderStats renderStats;

#define INCLUDED_RENDER_STATS
#endif // INCLUDED_RENDER_STATS
