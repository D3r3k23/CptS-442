#include "check_gl.h"
#include "mesh.h"


void Mesh::allocateBuffers(void)
{
    //
    // Copy your previous (PA06) solution here.
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


