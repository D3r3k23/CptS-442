#ifndef INCLUDED_TUBE

//
// The "tube" module provides the Tube class (see below).
//

#include "curve.h"
#include "geometrical_object.h"
#include "regular_mesh.h"
#include "scene_object.h"
#include "shader_programs.h"

class Tube : public GeometricalObject
//
// a tubular extrusion along a given path with a circular cross-section
//
{
    Curve *curve;
    double radius;
    bool isClosed; // tube is closed
    int nI; // vertices in the longitudinal direction
    int nJ; // vertices in the axial direction
public:

Tube(Curve *curve_, double radius_, int nI_, int nJ_, bool isClosed_)
    : curve(curve_), radius(radius_), isClosed(isClosed_),
        nI(nI_), nJ(nJ_), tessellationMesh(NULL)
    { };

    void draw(SceneObject *sceneObject);
public:
    RegularMesh *tessellationMesh;
private:
    void tessellate(void);
};

#define INCLUDED_TUBE
#endif // INCLUDED_TUBE

