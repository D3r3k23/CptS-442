#include <cassert>

#include "check_gl.h"
#include "geometry.h"
#include "irregular_mesh.h"
#include "obj_io.h"
#include "render_stats.h"
#include "shader_programs.h"


// helper (could be static)
void fitInBbox(Point3 *p, const int nP,
                      const Point3 qMin, const Point3 qMax)
//
// scales (uniformly) and translates the points in `p` to fit in an
// axis-aligned bounding box defined by `qMin` and `qMax`.
//
{
    Point3 pMin, pMax;

    assert(nP > 0);
    pMin = pMax = p[0];
    for (int iP = 0; iP < nP; iP++) {
        for (int d = 0; d < 3; d++) {
            if (p[iP].a[d] < pMin.a[d])
                pMin.a[d] = p[iP].a[d];
            else if (p[iP].a[d] > pMax.a[d])
                pMax.a[d] = p[iP].a[d];
        }
    }
    // `pMin` and `pMax` now bound the points

    Point3 pCtr = (pMax + pMin) / 2.0;
    Point3 qCtr = (qMax + qMin) / 2.0;
    Vector3 dP = pMax - pCtr;
    Vector3 dQ = qMax - qCtr;

    // Find the smallest scale value.
    double scale = -1.0;
    for (int d = 0; d < 3; d++) {
        assert(dP.a[d] >= 0.0);
        if (dP.a[d] > 0.0) {
            double newScale = dQ.a[d] / dP.a[d];
            if (scale == -1.0 || newScale < scale)
                scale = newScale;
        }
    }
    if (scale == -1.0) // All points are identical, so don't scale.
        scale = 1.0;

    // Scale all coordinates in all dimensions by the smallest scale
    // value and add qCtr.
    for (int iP = 0; iP < nP; iP++)
        p[iP] = scale * (p[iP] - pCtr) + qCtr;
}


IrregularMesh::IrregularMesh(Point3 *vertexPositions_, int nVertices_)
{
    nVertices = nVertices_;
    vertexPositions = vertexPositions_;
    assert(nVertices % 3 == 0); // irregular mesh assumes 3 vertices/face
    nFaces = nVertices / 3;
    assert(nFaces * 3 == nVertices);


    allocateBuffers();
}


const void IrregularMesh::render(const Transform &viewTransform)
{
    //
    // ASSIGNMENT (PA03)
    //
    // Using the latter part of PolyLine::render() to give you the
    // general idea of what needs to be done, do the following:
    //
    // - Call ShaderProgram::getCurrentAttributeIndex() to get the
    //   current attribute index for "vertexPosition". We'll call this
    //   `vpai` ("vertex position attribute index") below.
    //
    // - Call glBindBuffer() to bind `vertexPositionsBufferId` to
    //   GL_ARRAY_BUFFER.
    //
    // - Call glEnableVertexAttribArray() to enable `vpai`.
    //
    // - Call glVertexAttribPointer() to set `vpai` with 3 double
    //   elements per attribute (i.e. vertex x, y, and z). Don't set
    //   it to normalize fixed-point values. There's a 0 offset
    //   between consecutive values and the buffer offset should be 0
    //   (use the BUFFER_OFFSET(0) macro). Be careful here: This call
    //   is particularly error-prone.
    //
    // At this point, you have "connected" the vertex positions in
    // their buffer with the "vertexPosition" attribute of the vertex
    // shader. Then,
    //
    // - Create a temporary array `savedPositions` (or whatever name
    //   you like) on the heap (not stack) of `nVertices` Point3's and
    //   copy `vertexPositions` to it.
    //
    // - Multiply all `vertexPositions` by `viewTransform` to
    //   transform them. (See Transform::operator*(Point3) to see
    //   what's really going on here: operator overloading.)
    //
    // - Call updateBuffers() to send `vertexPositions` to the GPU.
    //
    // - Copy `savedPositions` back to `vertexPositions`.
    //
    // - Delete `savedPositions`.
    //
    // - Call renderTriangles() to render the mesh as a collection of
    //   triangles.
    //
    // 24 lines in instructor solution (YMMV)
    //
    GLint vpai = ShaderProgram::getCurrentAttributeIndex("vertexPosition");
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, vertexPositionsBufferId));
    CHECK_GL(glEnableVertexAttribArray(vpai));
    CHECK_GL(glVertexAttribPointer(
        vpai, // index
        3, // elems per attrb
        GL_DOUBLE, // component type
        GL_FALSE, // don't normalize fixed-point values
        0, // offset
        BUFFER_OFFSET(0)
    ));

    Point3* savedPositions = new Point3[nVertices];
    for (int i = 0; i < nVertices; i++)
    {
        savedPositions[i] = vertexPositions[i];
        vertexPositions[i] = viewTransform * vertexPositions[i];
    }
    updateBuffers();

    for (int i =  0; i < nVertices; i++)
        vertexPositions[i] = savedPositions[i];
    delete[] savedPositions;

    renderTriangles();
}


const void IrregularMesh::renderTriangles(void) const
{
    //
    // ASSIGNMENT (PA03)
    //
    // Add code to draw all triangles in the irregular mesh:
    //
    // - Call glDrawArrays() to draw a `3 * nFaces` vertices as
    //   (`nFaces`) triangles.
    //
    // - Increment "renderStats" field(s) as needed. Remember that
    //   each face is a triangle. Note: The grader will compare the
    //   "stats" on your submission with those on the official
    //   submission to check your code.
    //
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, nVertices));
    renderStats.ctVertices += nVertices;
    renderStats.ctTrianglesInIrregularMeshes += nFaces;
}



IrregularMesh *IrregularMesh::read(const string fname)
{
    vector<Point3> vertexPositionsVector;
    vector<Vector3> vertexNormalsVector;
    vector<Point2> textureCoordinatesVector;
    vector<Face> facesVector;

    if (!readObj(fname, vertexPositionsVector, vertexNormalsVector,
                 textureCoordinatesVector, facesVector)) {
        cerr << "Unable to read OBJ file \"" << fname << "\" -- exiting\n";
        exit(EXIT_FAILURE);
    }

    //
    // The vectors readObj() returns follow the OBJ format, but this
    // is not entirely compatible with what the OpenGL array-drawing
    // routines. In particular, OpenGL (indirect) indexing cannot
    // handle the (up to) 3 distinct indices (position, texture,
    // normal) in OBJ face specifications, so we must eliminate the
    // indirection of the OBJ indices.
    //

    int nFaces = facesVector.size();
    int nVertices = 3 * nFaces; // 3 vertices / (triangular) face
    Point3 *vertexPositions = new Point3[nVertices];

    int iVertex = 0;
    for (int iFace = 0; iFace < nFaces; iFace++) {
        Face face = facesVector[iFace];
        vertexPositions[iVertex] = vertexPositionsVector[
            face.faceVertex0.positionIndex];
        iVertex++;
        vertexPositions[iVertex] = vertexPositionsVector[
            face.faceVertex1.positionIndex];
        iVertex++;
        vertexPositions[iVertex] = vertexPositionsVector[
            face.faceVertex2.positionIndex];
        iVertex++;
    }
    assert(iVertex == nVertices);
    Face *faces = new Face[nFaces];
    copy(facesVector.begin(), facesVector.end(), faces);

    // make the mesh fit in a 1.5 x 1.5 x 1.5 bounding box
    fitInBbox(vertexPositions, nVertices,
              Point3(-0.75, -0.75, -0.75),
              Point3( 0.75,  0.75,  0.75)
        );

    IrregularMesh *irregularMesh = new IrregularMesh(
        vertexPositions, nVertices);

    return irregularMesh;
}


void IrregularMesh::updateBuffers(void)
{
    //
    // ASSIGNMENT (PA03)
    //
    // Do the following:
    //
    // - Call glBindBuffer() to bind `vertexPositionsBufferId` to
    //   GL_ARRAY_BUFFER.
    //
    // - Call glBufferData() to associate that buffer with
    //   `vertexPositions`. For maximum efficiency, make it
    //   GL_STATIC_DRAW. Remember that glBufferData() always expects
    //   buffer sizes in bytes. The "sizeof()" operator can help here,
    //   and remember to use the CHECK_GL() macro so errors can be
    //   immediately detected.
    //
    // 4 lines in instructor solution (YMMV)
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, vertexPositionsBufferId));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER,
        nVertices * sizeof(Point3),
        vertexPositions,
        GL_STATIC_DRAW
    ));
}
