#ifndef INCLUDED_MESH

//
// The "mesh" module provides the Mesh (virtual) and IrregularMesh and
// RegularMesh (non-virtual) classes (see below).
//

#include <cassert>

#include "geometry.h"
#include "hedgehog.h"
#include "poly_line.h"
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
    Point3 *faceCentroids;   // there are nFaces of these
    Vector3 *vertexNormals;  // there are nVertices of these
    Vector3 *faceNormals;    // there are nFaces of these
    unsigned int vertexPositionsBufferId;
    unsigned int vertexNormalBufferId;
    unsigned int faceNormalBufferId;

    static const Point3 triangleCentroid(Point3 p0, Point3 p1, Point3 p2)
    //
    // helper: returns the centroid of the (p0, p1, p2) triangle
    //
    {
        return (p0 + p1 + p2) / 3;
    }


public:
    void allocateBuffers(void);

public:
    const void createHedgehogs(Hedgehog *&faceHedgehog,
                               Hedgehog *&vertexHedgehog) const;

protected:
    const virtual void createFaceNormalsAndCentroids(void) = 0;
};


#define INCLUDED_MESH
#endif // INCLUDED_MESH
