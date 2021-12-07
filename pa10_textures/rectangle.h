#ifndef INCLUDED_RECTANGLE

//
// The "rectangle" module implements the Rectangle class (see below).
//

#include "geometry.h"
#include "image.h"
#include "surface.h"


class Rectangle : public Surface
//
// represents a rectangle defined by a point and two edge vectors
//
{
    Point3 pLL; // lower left corner
    Vector3 vU, vV; // u- and v- edge vectors

public:
    Rectangle(const Image *image,
         const Point3 &pLL_, const Vector3 &vU_, const Vector3 &vV_,
         int nI, int nJ)
    : Surface(nI, nJ, false, false),
        pLL(pLL_), vU(vU_), vV(vV_)
    { };

    const Point3 operator()(const double u, const double v,
                            Vector3 &tangentU, Vector3 &tangentV) const;
};


#define INCLUDED_RECTANGLE
#endif // INCLUDED_RECTANGLE
