#include "check_gl.h"
#include "mesh.h"


void Mesh::allocateBuffers(void)
{
    //
    // ASSIGNMENT (PA10)
    //
    // In addition to your previous (PA05) solution, do the following:
    //
    // - Call glGenBuffers() to create `textureCoordinatesBufferId`.
    //
}


const void Mesh::createHedgehogs(Hedgehog *&faceHedgehog,
                                 Hedgehog *&vertexHedgehog) const
{
    faceHedgehog = new Hedgehog(faceCentroids, faceNormals, nFaces,
                                   yellowColor);
    vertexHedgehog = new Hedgehog(vertexPositions, vertexNormals,
                                     nVertices, yellowColor);
}


