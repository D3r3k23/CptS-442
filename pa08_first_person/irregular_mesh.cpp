#include <cassert>

#include "controller.h"
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


IrregularMesh::IrregularMesh(Point3 *vertexPositions_, Vector3 *vertexNormals_,
                             int nVertices_)
{
    nVertices = nVertices_;
    vertexPositions = vertexPositions_;
    vertexNormals = vertexNormals_;
    assert(nVertices % 3 == 0); // irregular mesh assumes 3 vertices/face
    nFaces = nVertices / 3;
    assert(nFaces * 3 == nVertices);

    createFaceNormalsAndCentroids();

    allocateBuffers();
    //
    // Since we're handling transforms in the vertex shader, we only
    // need to download the buffers once, here in the constructor,
    // rather than in IrregularMesh::render().
    //
    updateBuffers();
}


const void IrregularMesh::render(void)
{
    //
    // Copy your previous (PA05) solution here.
    //
}


const void IrregularMesh::renderTriangles(void) const
{
    //
    // Copy your previous (PA03) solution here.
    //
}



const void IrregularMesh::createFaceNormalsAndCentroids(void)
{
    int iVertex = 0;

    assert(nFaces * 3 == nVertices);
    faceNormals = new Vector3[nFaces];
    faceCentroids = new Point3[nFaces];
    for (int iFace = 0; iFace < nFaces; iFace++) {
        faceNormals[iFace] = faceNormal(
            vertexPositions[iVertex],
            vertexPositions[iVertex + 1],
            vertexPositions[iVertex + 2]);
        faceCentroids[iFace] = (
            vertexPositions[iVertex]
            + vertexPositions[iVertex + 1]
            + vertexPositions[iVertex + 2]) / 3.0;
        iVertex += 3;
    }
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
    Vector3 *vertexNormals = new Vector3[nVertices];

    int iVertex = 0;
    for (int iFace = 0; iFace < nFaces; iFace++) {
        Face face = facesVector[iFace];
        vertexPositions[iVertex] = vertexPositionsVector[
            face.faceVertex0.positionIndex];
        assert(face.faceVertex0.normalIndex != OBJ_INDEX_DEFAULTED);
        vertexNormals[iVertex] = vertexNormalsVector[
                face.faceVertex0.normalIndex];
        iVertex++;
        vertexPositions[iVertex] = vertexPositionsVector[
            face.faceVertex1.positionIndex];
        assert(face.faceVertex1.normalIndex != OBJ_INDEX_DEFAULTED);
        vertexNormals[iVertex] = vertexNormalsVector[
                face.faceVertex1.normalIndex];
        iVertex++;
        vertexPositions[iVertex] = vertexPositionsVector[
            face.faceVertex2.positionIndex];
        assert(face.faceVertex2.normalIndex != OBJ_INDEX_DEFAULTED);
        vertexNormals[iVertex] = vertexNormalsVector[
                face.faceVertex2.normalIndex];
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
        vertexPositions, vertexNormals, nVertices);

    return irregularMesh;
}


void IrregularMesh::updateBuffers(void)
{
    //
    // Copy your previous (PA05) solution here.
    //
}
