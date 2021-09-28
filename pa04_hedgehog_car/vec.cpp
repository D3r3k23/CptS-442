#include <cassert>
#include <iostream>
using namespace std;

#include "vec.h"
#include "wrap_cmath_inclusion.h"


//
// The following methods are useful for debugging.
//
// Notice that we are following the OpenGL matrix indexing
// conventions.
//

ostream& operator<<(ostream &out, const Vec3 t)
{
    out << "[ " << t.a[0] << " " << t.a[1] << " " << t.a[2] << " ]";
    return out;
}


ostream& operator<<(ostream &out, const Mat4 mat)
{
    for (int i = 0; i < 4; i++) {
        if (i == 0)
            out << "[ ";
        else
            out << "  ";
        out << "[ "
            << mat.a[mat.ij(i,0)] << " "
            << mat.a[mat.ij(i,1)] << " "
            << mat.a[mat.ij(i,2)] << " "
            << mat.a[mat.ij(i,3)] << " ]";
        if (i == 3)
            out << " ]\n";
        else
            out << "\n";
    }
    return out;
}
