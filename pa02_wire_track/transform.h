#ifndef INCLUDED_TRANSFORM

//
// The "transform" module provides the Transform class (see below).
//

#include "geometry.h"
#include "vec.h"

class Transform : public Matrix4
//
// linear transforms of Point3's and Vector3's
//
{
public:

    Transform(void)
    {
        // When instanced by default, Transforms differ from Matrix4's
        // in that all transform matrices start off as identity
        // matrices.
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                a[ij(i,j)] = (i == j);
            };
        };
    };
    Transform(const Matrix4 &matrix4)
    {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                a[ij(i,j)] = matrix4.a[ij(i, j)];
            };
        };
    }
    // see comment for the similar Matrix4 constructor.
    Transform(
        double m_1, double m_5, double  m_9, double m_13,
        double m_2, double m_6, double m_10, double m_14,
        double m_3, double m_7, double m_11, double m_15,
        double m_4, double m_8, double m_12, double m_16)
    {
        a[0] = m_1; a[4] = m_5; a[8]  =  m_9; a[12] = m_13;
        a[1] = m_2; a[5] = m_6; a[9]  = m_10; a[13] = m_14;
        a[2] = m_3; a[6] = m_7; a[10] = m_11; a[14] = m_15;
        a[3] = m_4; a[7] = m_8; a[11] = m_12; a[15] = m_16;
    };

    const Transform operator*(const Matrix4 &matrix4) const
    {
        return Transform(*static_cast<const Matrix4 *>(this) * matrix4);
    };

    const Transform operator*=(const Matrix4 &matrix4)
    {
        return (*this) = (*this) * matrix4;
    };

    const Vector3 operator*(const Vector3 &vector3) const;

    const Point3 operator*(const Point3 &point3) const;

    void rotate(const double angle, const Vector3 &direction);
    void rotate(const double angle,
                const double aX, const double aY, const double aZ) {
        rotate(angle, Vector3(aX, aY, aZ));
    };

    void scale(const Vector3 &factor);
    void scale(const double sX, const double sY, const double sZ) {
        scale(Vector3(sX, sY, sZ));
    }

    void translate(const Vector3 offset);
    void translate(const double offX, const double offY, const double offZ)
    {
        translate(Vector3(offX, offY, offZ));
    };
};


#define INCLUDED_TRANSFORM
#endif // INCLUDED_TRANSFORM
