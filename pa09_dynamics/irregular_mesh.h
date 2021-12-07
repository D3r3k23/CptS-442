#ifndef INCLUDED_IRREGULAR_MESH


#include "geometry.h"
#include "transform.h"
#include "mesh.h"


class IrregularMesh : public Mesh
//
// An IrregularMesh is a Mesh whose (interior) vertices don't
// necessarily share the same number of edges. It's topology is
// "irregular".
//
{
private:
    const void createFaceNormalsAndCentroids(void);
    const void renderTriangles(void) const;

public:
    IrregularMesh(Point3 *vertexPositions_,
                  Vector3 *vertexNormals_, int nVertices_);

    static IrregularMesh *read(const string fname);

public:
    const void render(void);
    void updateBuffers(void);
};

#define INCLUDED_IRREGULAR_MESH
#endif // INCLUDED_IRREGULAR_MESH
