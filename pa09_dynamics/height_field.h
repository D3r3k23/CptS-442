#ifndef INCLUDED_HEIGHT_FIELD

//
// The "height_field" module implements the HeightField class (see below).
//

#include "surface.h"
#include "geometry.h"

class HeightField : public Surface
//
// a Surface that represents z = f(x,y)
//
{
private:
    const Point3 (*position)(const double u, const double v);

public:
    HeightField(const Point3 (*position_)(const double u, const double v),
                int nI, int nJ)
        : Surface(nI, nJ, false, false),
          position(position_)
    { };

    const Point3 operator()(const double u, const double v,
                            Vector3 &dp_du, Vector3 &dp_dv) const;
};


#define INCLUDED_HEIGHT_FIELD
#endif // INCLUDED_HEIGHT_FIELD
