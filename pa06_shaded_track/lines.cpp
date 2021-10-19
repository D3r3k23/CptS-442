#include "check_gl.h"
#include "color.h"
#include "controller.h"
#include "geometry.h"
#include "lines.h"
#include "render_stats.h"
#include "scene.h"
#include "shader_programs.h"
#include "view.h"
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


const void Lines::render(void)
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
    CHECK_GL(glDrawArrays(GL_LINES, 0, 2*nI));
    renderStats.ctLines += nI;
    renderStats.ctVertices += 2*nI;
}
