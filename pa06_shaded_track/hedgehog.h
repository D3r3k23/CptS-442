#ifndef INCLUDED_HEDGEHOG

//
// The "hedgehog" module provides the Hedgehog class (see below).
//

#include "color.h"
#include "lines.h"
#include "geometry.h"
#include "shader_programs.h"

class Hedgehog
//
// represents a set of positions and surface normal vectors associated
// with a GeometricalObject
//
// Because of this association, Hedgehogs don't really fall into the
// SceneObject-GeometricalObject-Tessellation taxonomy. They're a
// little of each.
//
{
    Point3 *positions;
    Vector3 *normals;
    int nQuills;

    Color color;

    Lines *lines;
    unsigned int vertexPositionsBufferId;
    ConstantColorShaderProgram *constantColorShaderProgram;
    void allocateBuffers(void);
    void updateBuffers(void);

public:
    Hedgehog(const Point3 *positions_, const Vector3 *normals_, int nVertices_,
             const Color &color);
    void draw(const Transform &viewTransform, const double quillLength);
};

#define INCLUDED_HEDGEHOG
#endif // INCLUDED_HEDGEHOG
