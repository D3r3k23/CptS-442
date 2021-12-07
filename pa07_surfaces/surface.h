#ifndef INCLUDED_SURFACES

//
// The "surface" module provides the Surface class (see below).
//

#include "geometry.h"
#include "geometrical_object.h"
#include "regular_mesh.h"
#include "scene_object.h"
#include "shader_programs.h"

class Surface : public GeometricalObject
//
// represents a surface (a 2D manifold, possibly with a boundary)
//
// Technically, we deal only with *parametric* surfaces (not including
// SolidOfRotations), but we'll use the term "surface" as a shorthand
// until and if we need to render non-parametric (e.g. implicit)
// surfaces.
//
{
protected:
    // number of vertices ...
    int nI; // ... in the horizontal (topological) direction
    int nJ; // ... in the vertical (topological) direction

    // wrap vertices ...
    bool wrapI; // ... in the horizontal (topological) direction
    bool wrapJ; // ... in the vertical (topological) direction

public:
    // when the Surface is tessellated...
    RegularMesh *tessellationMesh;

    Surface(int nI_, int nJ_, int wrapI_, int wrapJ_)
        : nI(nI_), nJ(nJ_), wrapI(wrapI_), wrapJ(wrapJ_),
          tessellationMesh(NULL)
    { };

    virtual const Point3 operator()(const double u, const double v,
                                    Vector3 &tangentU, Vector3 &tangentV)
        const = 0;
    void draw(SceneObject *sceneObject);
    void tessellate(void);
};

#define INCLUDED_SURFACES
#endif // INCLUDED_SURFACES
