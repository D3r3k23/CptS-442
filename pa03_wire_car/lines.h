#ifndef INCLUDED_LINES

//
// The "lines" module provides the Lines class (see below).
//

#include "geometry.h"
#include "tessellation.h"
#include "transform.h"

#include "shader_programs.h"

class Lines : public Tessellation
//
// draws a set of line segments
//
{
public:
    // array of line segments, defined by start and stop vertex coordinates
    Point3 (*vertexPositions)[2];

    unsigned int vertexPositionsBufferId;

    int nI; // number of line segments

public:
    Lines(Point3 (*vertexPositions)[2], int nI);

    void allocateBuffers(void);
    const void render(const Transform &viewTransform);
    void updateBuffers(void);
};

#define INCLUDED_LINES
#endif // INCLUDED_LINES
