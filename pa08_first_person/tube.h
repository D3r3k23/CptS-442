#ifndef INCLUDED_TUBE

//
// The "tube" module provides the Tube class (see below).
//

#include "curve.h"
#include "surface.h"
#include "regular_mesh.h"
#include "shader_programs.h"

class Tube : public Surface
//
// a tubular extrusion along a given path with a circular cross-section
//
{
    Curve *curve;
    double radius;
public:

Tube(Curve *curve_, double radius_, int nI_, int nJ_, bool isClosed_)
    : Surface(nI_, nJ_, true, isClosed_), curve(curve_), radius(radius_)
    {
        tessellationMesh = NULL;
    };

    const Point3 operator()(const double u, const double v,
                                    Vector3 &dp_du, Vector3 &dp_dv) const;
};

#define INCLUDED_TUBE
#endif // INCLUDED_TUBE

