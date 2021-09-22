#include "check_gl.h"
#include "color.h"
#include "controller.h"
#include "geometry.h"
#include "lines.h"
#include "render_stats.h"
#include "scene.h"
#include "shader_programs.h"
#include "wrap_gl_inclusion.h"


void Lines::allocateBuffers(void)
{
    // Allocate a buffer for the vertex coordinates ...
    CHECK_GL(glGenBuffers(1, &vertexPositionsBufferId));
}


void Lines::updateBuffers(void)
{
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, vertexPositionsBufferId));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, nI * sizeof(vertexPositions[0]),
                          vertexPositions, GL_STATIC_DRAW));
}


Lines::Lines(Point3 (*vertexPositions_)[2], int nI_)
    : nI(nI_)
{
    vertexPositions = new Point3[nI][2];
    for (int i = 0; i < nI; i++) {
        vertexPositions[i][0] = vertexPositions_[i][0];
        vertexPositions[i][1] = vertexPositions_[i][1];
    }
    allocateBuffers();
    updateBuffers();
}


const void Lines::render(const Transform &viewTransform)
{
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, vertexPositionsBufferId));
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glVertexAttribPointer(
                 0, // index of attribute
                 3, // # of elements per attribute
                 GL_DOUBLE, // type of each component
                 GL_FALSE,  // don't normalized fixed-point values
                 0, // offset between consecutive generic vertex attributes
                 BUFFER_OFFSET(0)));
    // Until we can perform the transform in the vertex shader, we'll
    // do it here.
    Point3 (*savedPositions)[2] = new Point3[nI][2];
    for (int i = 0; i < nI; i++) {
        savedPositions[i][0] = vertexPositions[i][0];
        savedPositions[i][1] = vertexPositions[i][1];
        vertexPositions[i][0] = viewTransform * vertexPositions[i][0];
        vertexPositions[i][1] = viewTransform * vertexPositions[i][1];
    }

    // only needed here when we transform the vertex coordinates
    updateBuffers();

    // Restore the saved (untransformed) vertex coordinates.
    for (int i = 0; i < nI; i++) {
        vertexPositions[i][0] = savedPositions[i][0];
        vertexPositions[i][1] = savedPositions[i][1];
    }
    delete [] savedPositions;
    CHECK_GL(glDrawArrays(GL_LINES, 0, 2*nI));
    renderStats.ctLines += nI;
    renderStats.ctVertices += 2*nI;
}
