#include "check_gl.h"
#include "mesh.h"


void Mesh::allocateBuffers(void)
{
    //
    // Copy your previous (PA06) solution here.
    //
    CHECK_GL(glGenBuffers(1, &vertexPositionsBufferId));
    CHECK_GL(glGenBuffers(1, &vertexNormalBufferId));
    CHECK_GL(glGenBuffers(1, &faceNormalBufferId));
}


const void Mesh::createHedgehogs(Hedgehog *&faceHedgehog,
                                 Hedgehog *&vertexHedgehog) const
{
    faceHedgehog = new Hedgehog(faceCentroids, faceNormals, nFaces,
                                   yellowColor);
    vertexHedgehog = new Hedgehog(vertexPositions, vertexNormals,
                                     nVertices, yellowColor);
}


