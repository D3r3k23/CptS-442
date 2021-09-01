#include "check_gl.h"
#include "controller.h"
#include "geometry.h"
#include "poly_line.h"
#include "scene.h"
#include "shader_programs.h"
#include "wrap_gl_inclusion.h"


void PolyLine::allocateBuffers(void)
{
    // Allocate a buffer for the vertex coordinates ...
    CHECK_GL(glGenBuffers(1, &vertexPositionsBufferId));
}


void PolyLine::updateBuffers(void)
{
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, vertexPositionsBufferId));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, nVertices*sizeof(vertexPositions[0]),
                          vertexPositions, GL_STATIC_DRAW));
}


PolyLine::PolyLine(Point2 *vertexPositions_, int nVertices_, bool wrapI)
    : nVertices(nVertices_), wrapI(wrapI)
{
    vertexPositions = new Point2[nVertices];
    for (int i = 0; i < nVertices; i++)
        vertexPositions[i] = vertexPositions_[i];
    allocateBuffers();
    updateBuffers();
}


const void PolyLine::render(void)
{
    //
    // This controls line antialiasing.
    //
    if (controller.antialiasEnabled) {
        CHECK_GL(glEnable(GL_LINE_SMOOTH));
        CHECK_GL(glEnable(GL_BLEND));
        CHECK_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        CHECK_GL(glHint(GL_LINE_SMOOTH_HINT, GL_NICEST));
    } else {
        CHECK_GL(glDisable(GL_LINE_SMOOTH));
        CHECK_GL(glDisable(GL_BLEND));
    }


    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, vertexPositionsBufferId));
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glVertexAttribPointer(
                 0, // index of attribute
                 2, // # of elements per attribute
                 GL_DOUBLE, // type of each component
                 GL_FALSE,  // don't normalized fixed-point values
                 0, // offset between consecutive generic vertex attributes
                 BUFFER_OFFSET(0)));
    if (wrapI)
        CHECK_GL(glDrawArrays(GL_LINE_LOOP, 0, nVertices));
    else
        CHECK_GL(glDrawArrays(GL_LINE_STRIP, 0, nVertices));
}
