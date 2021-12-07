#include "bezier_patch.h"

const Point3 BezierPatch::operator()(const double u, const double v,
                                     Vector3 &dp_du, Vector3 &dp_dv) const
{
    //
    // Copy your previous (PA07) solution here.
    //
    double ub[4], db_du[4];
    basis(u, ub, &db_du);

    double vb[4], db_dv[4];
    basis(v, vb, &db_dv);
    
    Point3 position{0.0, 0.0, 0.0};
    dp_du = Vector3{0.0, 0.0, 0.0};
    dp_dv = Vector3{0.0, 0.0, 0.0};
    
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            position += cvs[i][j] * ub[j] * vb[i];
            dp_du += Vector3(cvs[i][j].g.x, cvs[i][j].g.y, cvs[i][j].g.z) * db_du[j] * vb[i];
            dp_dv += Vector3(cvs[i][j].g.x, cvs[i][j].g.y, cvs[i][j].g.z) * ub[j] * db_dv[i];
        }

    return position;
}
