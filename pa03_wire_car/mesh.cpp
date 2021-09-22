#include "check_gl.h"
#include "mesh.h"


void Mesh::allocateBuffers(void)
{
    //
    // ASSIGNMENT (PA03)
    //
    // Do the following:
    //
    // - Call glGenBuffers() to create `vertexPositionsBufferId`.
    //
    CHECK_GL(glGenBuffers(1, &vertexPositionsBufferId));
}
