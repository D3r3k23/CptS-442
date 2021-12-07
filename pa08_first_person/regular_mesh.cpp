#include <cassert>

#include "render_stats.h"
#include "regular_mesh.h"


void RegularMesh::allocateBuffers(void)
{
    //
    // Copy your previous (PA06) solution here.
    //
    CHECK_GL(glGenBuffers(1, &indexBufferId));
    Mesh::allocateBuffers();
}


void RegularMesh::updateBuffers(void)
{
    //
    // Copy your previous (PA06) solution here.
    //
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, vertexPositionsBufferId));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER,
        nVertices * sizeof(Point3),
        vertexPositions,
        GL_STATIC_DRAW
    ));
    
    CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId));
    CHECK_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        nVertexIndices * sizeof(unsigned int),
        vertexIndices,
        GL_STATIC_DRAW
    ));
    
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, vertexNormalBufferId));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER,
        nVertices * sizeof(Vector3),
        vertexNormals,
        GL_STATIC_DRAW
    ));
}


bool RegularMesh::pointsAreDistinct(void)
//
// helper: Make sure all vertices in a regular mesh are distinct.
//
{
    for (int iVertex = 0; iVertex < nVertices; iVertex++) {
        for (int jVertex = 0; jVertex < iVertex; jVertex++) {
            //
            // Interesting programming note: The body of this loop
            // originally looked like this:
            //
            //   Vector3 diff = vertexPositions[iVertex]
            //            - vertexPositions[jVertex];
            //   if (       fabs(diff.u.g.x) < EPSILON
            //           && fabs(diff.u.g.y) < EPSILON
            //           && fabs(diff.u.g.z) < EPSILON)
            //       return false;
            //
            // But rewriting it as below cut the time by 60%:
            //
            double dx = vertexPositions[iVertex].g.x
                - vertexPositions[jVertex].g.x;
            double dy = vertexPositions[iVertex].g.y
                - vertexPositions[jVertex].g.y;
            double dz = vertexPositions[iVertex].g.z
                - vertexPositions[jVertex].g.z;
            if (     -EPSILON < dx && dx < EPSILON
                  && -EPSILON < dy && dy < EPSILON
                  && -EPSILON < dz && dz < EPSILON)
                return false;
        }
    }
    return true;
}


void RegularMesh::quadBoundary(int iLeft, int jLower, Point3 p[4])
//
// possible helper: returns (in p[]) the four points bounding the two
// faces of the quad in CCW order. It takes account of wrapping in
// both the i and j directions.
//
{
    int iRight, jUpper;

    //
    // It is a bug to request the boundary of the "rightmost
    // rectangles" (i.e. those whose lower left corner is
    // `iLower` = `nI`-1) *unless* we're wrapping in i (i.e.
    // `wrapI` is true).
    //
    assert(0 <= iLeft && iLeft < nI - 1 + wrapI);
    //
    // Likewise for the "topmost" rectangle, `jLower`, `nI`, and
    // `wrapJ`, respectively.
    //
    assert(0 <= jLower && jLower < nJ - 1 + wrapJ);

    //
    // When wrapI is true, the iLeft = 0 vertices form the boundary of
    // the rightmost rectangles.
    //
    if (wrapI && iLeft == nI-1)
        iRight = 0;
    else
        iRight = iLeft+1;

    //
    // Likewise when wrapJ is true for the topmost rectangles.
    //
    if (wrapJ && jLower == nJ-1)
        jUpper = 0;
    else
        jUpper = jLower+1;

    p[0] = vertexPositions[vertexIndex(iLeft,  jLower)];
    p[1] = vertexPositions[vertexIndex(iRight, jLower)];
    p[2] = vertexPositions[vertexIndex(iRight, jUpper)];
    p[3] = vertexPositions[vertexIndex(iLeft,  jUpper)];
}


const void RegularMesh::render(void)
{
    //
    // Copy your previous (PA06) solution here.
    //
    GLint vpai = ShaderProgram::getCurrentAttributeIndex("vertexPosition");
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, vertexPositionsBufferId));
    CHECK_GL(glEnableVertexAttribArray(vpai));
    CHECK_GL(glVertexAttribPointer(
        vpai,
        3,
        GL_DOUBLE,
        GL_FALSE,
        0,
        BUFFER_OFFSET(0)
    ));


    GLint vnai = ShaderProgram::getCurrentAttributeIndex("vertexNormal");
    if (vnai != NO_SUCH_ATTRIBUTE)
    {
        CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, vertexNormalBufferId));
        CHECK_GL(glEnableVertexAttribArray(vnai));
        CHECK_GL(glVertexAttribPointer(
            vnai,
            3,
            GL_DOUBLE,
            GL_FALSE,
            0,
            BUFFER_OFFSET(0)
        ));
    }

    CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId));

    for (int j = 0; j < nJ - 1 + (int)wrapJ; j++)
        renderTriangleStrip(j);
}


const void RegularMesh::renderTriangleStrip(const int j) const
{
    //
    // Copy your previous (PA06) solution here.
    //
    const int nIndicesInStrip = 2 * (nI + wrapI);

    const unsigned bufferOffset = j * nIndicesInStrip * sizeof(unsigned int);

    CHECK_GL(glDrawElements(
        GL_TRIANGLE_STRIP,
        nIndicesInStrip,
        GL_UNSIGNED_INT,
        BUFFER_OFFSET(bufferOffset)
    ));

    const int nTrianglesInStrip = nIndicesInStrip - 2 + (int)wrapI;
    renderStats.ctTrianglesInRegularMeshes += nTrianglesInStrip;
    renderStats.ctTriangleStrips++;
    renderStats.ctVertices += nIndicesInStrip;
}


void RegularMesh::createVertexIndices(void)
{
    int nIndicesPerTriangleStrip = 2 * (nI + wrapI);
    int nTriangleStrips = nJ - 1 + wrapJ;
    nVertexIndices = nIndicesPerTriangleStrip * nTriangleStrips;
    vertexIndices = new unsigned int [nVertexIndices];
    int iVertexIndices = 0;
    for (int j = 0; j < nJ - 1 + wrapJ; j++) {
        int jTop = j + 1;
        if (jTop >= nJ) {
            assert(jTop == nJ && wrapJ); // should be the only time this happens
            jTop = 0;
        }
        for (int i = 0; i < nI; i++) {
            vertexIndices[iVertexIndices++] = vertexIndex(i, jTop);
            vertexIndices[iVertexIndices++] = vertexIndex(i, j);
        }
        if (wrapI) {
            vertexIndices[iVertexIndices++] = vertexIndex(0, jTop);
            vertexIndices[iVertexIndices++] = vertexIndex(0, j);
        }
    }
    assert(iVertexIndices == nVertexIndices);
}


RegularMesh::RegularMesh(Point3 *vertexPositions_, Vector3 *vertexNormals_,
           int nI_, int nJ_, bool wrapI_, bool wrapJ_)
    : nI(nI_), nJ(nJ_), wrapI(wrapI_), wrapJ(wrapJ_)
{
    nVertices = nI * nJ;
    vertexPositions = new Point3[nVertices];
    vertexNormals = new Vector3[nVertices];
    for (int i = 0; i < nVertices; i++) {
        vertexPositions[i] = vertexPositions_[i];
        vertexNormals[i] = vertexNormals_[i];
    }

    // This enforces our requirement for distinct mesh points and thus
    // prevents later trouble.

    // For regular meshes, we create face normals for all the
    // vertices. If the vertices of a face aren't distinct, however,
    // the normals are undefined. Hence, we have a check here to make
    // sure they're distinct. (We don't do this for irregular meshes,
    // since it's possible for vertices to be repeated.)
    assert(pointsAreDistinct());

    createVertexIndices();
    createFaceNormalsAndCentroids();

    allocateBuffers();
    //
    // Since we're handling transforms in the vertex shader, we only
    // need to download the buffers once, here in the constructor,
    // rather than in RegularMesh::render().
    //
    updateBuffers();
}


const void RegularMesh::createFaceNormalsAndCentroids(void)
{
    //
    // Copy your previous (PA06) solution here.
    //
    const int nQuads = (nI - 1 + (int)wrapI) * (nJ - 1 + (int)wrapJ);
    nFaces = 2 * nQuads;

    faceNormals  = new Vector3[nFaces];
    faceCentroids = new Point3[nFaces];

    int iFace = 0;
    for (int j = 0; j < nJ - 1 + (int)wrapJ; j++)
        for (int i = 0; i < nI - 1 + (int)wrapI; i++)
        {
            Point3 quadVertices[4];
            quadBoundary(i, j, quadVertices);

            faceNormals[iFace] = faceNormal(quadVertices[0], quadVertices[2], quadVertices[3]);
            faceCentroids[iFace++] = (quadVertices[0] + quadVertices[2] + quadVertices[3]) / 3;
            
            faceNormals[iFace] = faceNormal(quadVertices[0], quadVertices[1], quadVertices[2]);
            faceCentroids[iFace++] = (quadVertices[0] + quadVertices[1] + quadVertices[2]) / 3;
        }
}
