#ifndef INCLUDED_POLYLINE

//
// The "poly_line" module provides the PolyLine class (see below).
//

#include "geometry.h"
#include "shader_programs.h"
#include "tessellation.h"
#include "transform.h"

//
// Be careful when compiling under Windows to refer to the following
// as "PolyLines" (upper case "L"), as "windows.h" (which we include
// in as few places as necessary) defines a "Polyline" (lower case
// "l"), which is a function, not a class.
//

class PolyLine : public Tessellation
//
// a Tessellation consisting of a connected set of vertices
//
{
public:
    Point3 *vertexPositions;
    unsigned int vertexPositionsBufferId;
    int nVertices;
    //
    // A convention we will follow for polylines (and beyond) is that
    // it is up to the programmer to make sure that no two points
    // defining an object should have identical coordinates. Not
    // obeying this convention can cause bugs.
    //
    // One problem with the convention we need to work around is when
    // we want to draw a closed polyline (a loop). We'll take care of
    // this with `wrapI` which will indicate whether or not to repeat
    // the first index when drawing a polyline. That way, all the
    // points are still distinct.
    //
    bool wrapI;

public:
    PolyLine(Point3 *vertexPositions, int nI, bool wrapI);

    void allocateBuffers(void);
    // Transforms will be set in the draw() method.
    const void render(void);
    void updateBuffers(void);
};

#define INCLUDED_POLYLINE
#endif // INCLUDED_POLYLINE
