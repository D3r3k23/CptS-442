
#include "geometry.h"
#include "wrap_cmath_inclusion.h"

#define CH_EOF (-1) // denotes end-of-file



const Matrix4 Matrix4::operator*(const Matrix4 &matrix) const
{
    Matrix4 result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.a[ij(i,j)] = 0.0;
            for (int k = 0; k < 4; k++)
                result.a[ij(i,j)] += a[ij(i,k)]*matrix.a[ij(k,j)];
        }
    }
    return result;
}

Matrix4 Matrix4::transpose(void) const
{
    Matrix4 result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.a[ij(i,j)] = a[ij(j,i)];
        }
    }
    return result;
}

