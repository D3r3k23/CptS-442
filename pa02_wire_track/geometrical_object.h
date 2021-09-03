#ifndef INCLUDED_GEOMETRICAL_OBJECT

//
// The "geometrical_object" module provides the GeometricalObject
// class (see below).
//

#include "transform.h"

class GeometricalObject
//
// virtual class for objects defined by geometry
//
// GeometricalObjects need to be tessellated (into a Tessellation
// child class) in order to be drawn.
//
{
public:
    virtual void draw(const Transform &viewTransform) = 0;
    virtual void tessellate(void) = 0;
};

#define INCLUDED_GEOMETRICAL_OBJECT
#endif
