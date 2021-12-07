#ifndef INCLUDED_REGULAR_MESH

#include "check_gl.h"
#include "mesh.h"


class RegularMesh : public Mesh
//
// A RegularMesh is a Mesh, all of whose (interior) vertices share the
// same number of edges. (In this case, 6.)
//
{
    // number of vertices ...
    int nI; // ... in the horizontal (topological) direction
    int nJ; // ... in the vertical (topological) direction

    // wrap mesh ...
    bool wrapI; // ... in the horizontal (topological) direction
    bool wrapJ; // ... in the vertical (topological) direction

    unsigned int indexBufferId;

    int nVertexIndices;
    unsigned int *vertexIndices;

    // A regular mesh vertex has this many (at most) incident vertices.
    static const int MAX_INCIDENT_FACES_ON_VERTEX = 6;
    int incidentFaceIndices(const int i, const int j,
                            int iFs[MAX_INCIDENT_FACES_ON_VERTEX]);
    void quad(int i, int j, Point3 pQuad[4]);

    const int vertexIndex(int i, int j) const
    //
    // returns the index into any vertex-related arrays corresponding
    // to vertex (i, j)
    //
    {
        assert(0 <= i && i < nI);
        assert(0 <= j && j < nJ);
        //
        // Copy your previous (PA06) solution here.
        //
        return j * nI + i;
    };

public:
    void allocateBuffers(void);
    RegularMesh(Point3 *vertexPositions_, Vector3 *vertexNormals_,
        int nI, int nJ, bool wrapI, bool wrapJ);

    const void render(void);
    void updateBuffers(void);

private:
    const void createFaceNormalsAndCentroids(void);
    void createVertexIndices(void);
    bool pointsAreDistinct(void);
    void quadBoundary(int i, int j, Point3 p[4]);
    const void renderTriangleStrip(const int j) const;
};

#define INCLUDED_REGULAR_MESH
#endif // INCLUDED_REGULAR_MESH
