#ifndef INCLUDED_BEZIER_PATCH

//
// The "bezier_patch" module provides the BezierPatch class (see
// below).
//

#include "basis.h"
#include "geometry.h"
#include "surface.h"

class BezierPatch : public Surface
//
// a 3D cubic Bezier surface defined by a 4x4 grid of control vertices
//
{
    BezierBasis basis;
    Point3 cvs[4][4];

public:

    //
    // We are not allowing Bezier patches to wrap, which they seldom
    // do anyway.
    //
    BezierPatch(const Point3 cvs_[4][4], int nI_, int nJ_)
    : Surface(nI_, nJ_, false, false)
    {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)
                cvs[i][j] = cvs_[i][j];
        };
    }

    const Point3 operator()(const double u, const double v,
                            Vector3 &dp_du, Vector3 &dp_dv) const;

};

#define INCLUDED_BEZIER_PATCH
#endif // INCLUDED_BEZIER_PATCH
