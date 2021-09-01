#ifndef INCLUDED_CIRCLE

//
// The "circle" module provides the Circle class (see below).
//

#include "geometrical_object.h"
#include "geometry.h"
#include "poly_line.h"
#include "shader_programs.h"

class Circle : public GeometricalObject
//
// a 2D GeometricalObject subclass that represents circles.
//
{
    Point2 center;
    double radius;

public:
    Circle(Point2 center_, double radius_)
    : center(center_), radius(radius_), tessellationPolyLine(NULL)
    { };
    void draw();

private:
    PolyLine *tessellationPolyLine;
    void tessellate(void);
};

#define INCLUDED_CIRCLE
#endif // INCLUDED_CIRCLE
