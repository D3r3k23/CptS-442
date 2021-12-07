#ifndef INCLUDED_COORDINATE_AXES

//
// The "coordinate_axes" module provides the CoordinateAxes class
// (see below).
//

#include "poly_line.h"
#include "scene_object.h"
#include "shader_programs.h"
#include "transform.h"


class CoordinateAxes : public SceneObject
//
// a SceneObject showing the x, y, and z axes in the current
// coordinate frame
//
{
private:
    PolyLine *xAxis;
    PolyLine *yAxis;
    PolyLine *zAxis;

public:
    CoordinateAxes(void);
    void display(const Transform &viewTransform);
};


#define INCLUDED_COORDINATE_AXES
#endif // INCLUDED_COORDINATE_AXES
