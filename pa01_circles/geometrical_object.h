#ifndef INCLUDED_GEOMETRICAL_OBJECT

//
// The "geometrical_object" module provides the GeometricalObject
// class (see below).
//


class GeometricalObject
//
// virtual class for objects defined by geometry
//
// GeometricalObjects need to be tessellated (into a Tessellation
// child class) in order to be drawn.
//
{
public:
    virtual void draw(void) = 0;
    virtual void tessellate(void) = 0;
};

#define INCLUDED_GEOMETRICAL_OBJECT
#endif
