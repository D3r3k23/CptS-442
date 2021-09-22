#ifndef INCLUDED_MESH

//
// The "mesh" module provides the Mesh (virtual) and IrregularMesh and
// RegularMesh (non-virtual) classes (see below).
//

#include <cassert>

#include "geometry.h"
#include "tessellation.h"


class Mesh : public Tessellation
//
// a 2D mesh of vertices that can be rendered as a wireframe or a set
// of filled triangles
//
{
public:
    Point3 *vertexPositions;       // there are nVertices of these
    int nVertices;

protected:
    int nFaces;
    unsigned int vertexPositionsBufferId;

public:
    void allocateBuffers(void);

};


#define INCLUDED_MESH
#endif // INCLUDED_MESH
