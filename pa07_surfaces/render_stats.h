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
        frameNumber(0), ctVertices(0), ctLines(0), ctLineStrips(0)
        , ctTrianglesInIrregularMeshes(0)
        , ctTrianglesInRegularMeshes(0), ctTriangleStrips(0)
        { };

    void reset(void);
    void display(void);
};
extern RenderStats renderStats;

#define INCLUDED_RENDER_STATS
#endif // INCLUDED_RENDER_STATS
