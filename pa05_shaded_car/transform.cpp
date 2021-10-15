#include "geometry.h"
#include "transform.h"
#include "wrap_cmath_inclusion.h"


const Point3 Transform::operator*(const Point3 &point3) const
{
    Point3 result;

    // We don't need i == 3 as 4th dimension of resulting point is
    // implicitly 1.
    for (int i = 0; i < 3; i++) {
        // As the 4th dimension of `point3` is implicitly 1. We can
        // initialize the sum to the last column of a[].
        result.a[i] = a[ij(i,3)];
        for (int j = 0; j < 3; j++)
            result.a[i] += a[ij(i,j)] * point3.a[j];
    }
    return result;
};


const Vector3 Transform::operator*(const Vector3 &vector3) const
{
    Vector3 result;

    // We don't need i == 3 as 4th dimension of resulting vector is
    // implicitly 0.
    for (int i = 0; i < 3; i++) {
        result.a[i] = 0.0;
        // We don't need j == 3 as 4th dimension of
        // `vector3`vector is implicitly 0.
        for (int j = 0; j < 3; j++)
            result.a[i] += a[ij(i,j)] * vector3.a[j];
    }
    return result;
};


void Transform::rotate(const double angle, const Vector3 &direction)
//
// rotates the transform by `angle` around `direction`
//
{
    //
    // This is the hardest of the `Transform::*()` methods. There's no
    // need to modify this method. Note that it operates on the
    // transform "in-place".
    //
    // Remember to test your results by compiling the "transform_t"
    // self test ("-DTEST"), code for which is included at the end of
    // this file.
    //
    Vector3 vU = direction.normalized();
    double cosA = cos(angle);
    double sinA = sin(angle);
    Matrix4 rotationMatrix;
    double s[3][3] = {
        {       0, -vU.g.z,  vU.g.y },
        {  vU.g.z,       0, -vU.g.x },
        { -vU.g.y,  vU.g.x,       0 },
    };

    // Construct the `rotationMatrix` according to the OpenGL
    // documentation.
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double I_ij = (i == j); // identity matrix
            if (i >= 3 || j >= 3) {
                rotationMatrix.a[ij(i, j)] = I_ij;
            } else {
                double u_uT_ij = vU.a[i]*vU.a[j];
                rotationMatrix.a[ij(i, j)] = u_uT_ij
                    + cosA * (I_ij - u_uT_ij) + sinA * s[i][j];
            };
        };
    };
    (*this) = (*this) * rotationMatrix;
}


void Transform::scale(const Vector3 &factor)
//
// scales the transform by `factor`
//
{
    //
    // Scale factors of (near) zero are almost certainly errors.
    // Remove these assertions if you ever find a good reason for
    // allowing them.
    //
    assert(fabs(factor.a[0]) > EPSILON);
    assert(fabs(factor.a[1]) > EPSILON);
    assert(fabs(factor.a[2]) > EPSILON);

    Matrix4 scaleMatrix(
        factor.a[0], 0.0,           0.0,           0.0,
        0.0,           factor.a[1], 0.0,           0.0,
        0.0,           0.0,           factor.a[2], 0.0,
        0.0,           0.0,           0.0,           1.0
        );
    (*static_cast<Matrix4 *>(this)) = (*this) * scaleMatrix;
}


void Transform::translate(const Vector3 offset) {
    Matrix4 translateMatrix(
        1.0, 0.0, 0.0, offset.a[0],
        0.0, 1.0, 0.0, offset.a[1],
        0.0, 0.0, 1.0, offset.a[2],
        0.0, 0.0, 0.0, 1.0
        );
    (*this) *= translateMatrix;
}


#ifdef TEST
#include <iostream>
using namespace std;
#include <cstdlib>
#include <cstring>

int main(int argc, char **argv)
{
    Transform t;

    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "-r") == 0) {
            // "r angle aX aY aZ" rotates by `angle` (in radians)
            // about (aX, aY, aZ)
            double angle = strtod(argv[i+1], NULL);
            double aX = strtod(argv[i+2], NULL);
            double aY = strtod(argv[i+3], NULL);
            double aZ = strtod(argv[i+4], NULL);
            i += 5;
            t.rotate(angle, aX, aY, aZ);
        } else if (strcmp(argv[i], "-s") == 0) {
            // "s sX sY sZ" scales the transform by (sX, sY, sZ)
            double sX = strtod(argv[i+1], NULL);
            double sY = strtod(argv[i+2], NULL);
            double sZ = strtod(argv[i+3], NULL);
            i += 4;
            t.scale(sX, sY, sZ);
        } else {
            cerr << "unknown operation '" << argv[i] << "' -- exiting\n";
            exit(EXIT_FAILURE);
        }
    }

    cout << "after transformations, the identity transform becomes:\n\n";

    cout << "t:\n";
    cout << t << "\n";



    cout << "t.transpose():\n";
    cout << t.transpose() << "\n";

}
#endif // TEST
