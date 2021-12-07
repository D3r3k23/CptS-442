#include <cassert>

#include "render_stats.h"
#include "regular_mesh.h"


void RegularMesh::allocateBuffers(void)
{
    //
    // Copy your previous (PA06) solution here.
    //
    Mesh::allocateBuffers();
}


void RegularMesh::updateBuffers(void)
{
    //
    // ASSIGNMENT (PA10)
    //
    // In addition to your previous (PA06) solution, do the following:
    //
    // - If `textureCoordinates` is not NULL:
    //
    //   * Call glBindBuffer() to bind `textureCoordinatesBufferId` to
    //     GL_ARRAY_BUFFER, treating `textureCoordinates` similarly
    //     to what you did with vertex positions and vertex normals.
    //
    //   * Call glBufferData() to associate it with
    //     `textureCoordinates`.
    //
    // As before, for maximum efficiency make it GL_STATIC_DRAW.
    // Remember that glBufferData() always expects buffer sizes in
    // bytes.
    //
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
    // ASSIGNMENT (PA10)
    //
    // Modify your previous (PA06) solution here as follows:
    //
    // - If and only if `textureCoordinates` is not NULL, configure
    //   and enable texture coordinates. This will be similar to what
    //   you did to configure the vertex normal buffer (in PA06), with
    //   these exceptions:
    //
    //   * Call glBindBuffer() using `textureCoordinatesBufferId`
    //     instead of `vertexNormalBufferId`.
    //
    //   * Call ShaderProgram::getCurrentAttributeIndex() to get
    //     `textureCoordinatesAttributeIndex`, the attribute index (i.e.
    //     "handle") of the "textureCoordinates" attribute in the current
    //     shader program.
    //
    //   * Use `textureCoodinatesAttrbuteIndex` in the
    //     glEnableVertexAttribArray() and glVertexAttribPointer()
    //     calls instead of `vnai`. Remember
    //     that vertex coordinates consist of two double values per
    //     vertex. As before, be careful here.
    //
    //   The logical place to do this is right after you enable the
    //   vertex normal buffer.
    //
}


void RegularMesh::setTextureCoordinates(const double uScale,
                                        const double vScale)
//
// Uses the topological (`nI x nJ`) mesh to set the RegularMesh's
// `textureCoordinates`.
//
{
    int iTextureVertex = 0;
    textureCoordinates = new Point2[nVertices];
    double dU = uScale / (nI - 1);
    double dV = vScale / (nJ - 1);
    for (int j = 0; j < nJ; j++) {
        for (int i = 0; i < nI; i++) {
            textureCoordinates[iTextureVertex].g.x = i * dU;
            textureCoordinates[iTextureVertex].g.y = j * dV;
            iTextureVertex++;
        }
    }
    assert(iTextureVertex == nVertices);
}


const void RegularMesh::renderTriangleStrip(const int j) const
{
    //
    // Copy your previous (PA06) solution here.
    //
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
    textureCoordinates = NULL;

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
}
