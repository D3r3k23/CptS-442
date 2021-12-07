#ifndef INCLUDED_TEAPOT

//
// The "teapot" module implements the Teapot class (see below).
//

using namespace std;

#include "bezier_patch.h"
#include "coordinate_axes.h"
#include "geometry.h"
#include "scene_object.h"
#include "shader_programs.h"
#include "surface.h"

class Teapot : public SceneObject
//
// SceneObject representing the classic Newell teapot
//
{
    // A Teapot is made up of a collection of BezierPatches.
    vector<BezierPatch *> bezierPatches;

    // Each patch will be made of the same material and will get an nI
    // x nJ tessellation.
    static const int nI = 8;
    static const int nJ = 8;
    Vector3 offset; // displacement from the modeling coordinate origin

public:
    Teapot(void);

    void display(const Transform &viewTransform);
};

#define INCLUDED_TEAPOT
#endif // INCLUDED_TEAPOT
