#include "height_field.h"


const Point3 HeightField::operator()(const double u, const double v,
                                    Vector3 &dp_du, Vector3 &dp_dv) const
{
    //
    // Eventually, we should allow for a direct computation of the
    // partial derivatives, but this should be good enough for now.
    //
    double h = 1.0e-6;
    dp_du = ((*position)(u + h, v) - (*position)(u - h, v)) / (2 * h);
    dp_dv = ((*position)(u, v + h) - (*position)(u, v - h)) / (2 * h);

    return (*position)(u, v);
}


