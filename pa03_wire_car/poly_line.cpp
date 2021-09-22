#include "check_gl.h"
#include "color.h"
#include "controller.h"
#include "geometry.h"
#include "poly_line.h"
#include "render_stats.h"
#include "scene.h"
#include "shader_programs.h"
#include "transform.h"
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


PolyLine::PolyLine(Point3 *vertexPositions_, int nVertices_, bool wrapI_)
    : nVertices(nVertices_), wrapI(wrapI_)
{
    vertexPositions = new Point3[nVertices];
    for (int i = 0; i < nVertices; i++)
        vertexPositions[i] = vertexPositions_[i];
    allocateBuffers();
    updateBuffers();
}


const void PolyLine::render(const Transform &viewTransform)
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
    Point3 *savedCoordinates = new Point3[nVertices];
    for (int i = 0; i < nVertices; i++) {
        savedCoordinates[i] = vertexPositions[i];
        vertexPositions[i] = viewTransform * vertexPositions[i];
    }
    updateBuffers();
    // Restore the saved (untransformed) vertexPositions.
    for (int i = 0; i < nVertices; i++) {
        vertexPositions[i] = savedCoordinates[i];
    }
    delete [] savedCoordinates;
    if (wrapI)
        CHECK_GL(glDrawArrays(GL_LINE_LOOP, 0, nVertices));
    else
        CHECK_GL(glDrawArrays(GL_LINE_STRIP, 0, nVertices));
    renderStats.ctLines += nVertices - (wrapI == 0);
    renderStats.ctLineStrips++;
    renderStats.ctVertices += nVertices;
}
