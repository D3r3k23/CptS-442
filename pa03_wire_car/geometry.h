#ifndef INCLUDED_GEOMETRY

//
// The "geometry" module provides objects and functions that know
// about geometry, but nothing about graphics. Tecnically, they could
// be considered part of the model, but are generally low-level. This
// is where things like vector, point, and matrix operations belong,
// but absolutely *no* OpenGL calls.
//

#include <assert.h>
#include <vector>

#include "vec.h"
#include "wrap_cmath_inclusion.h"


class Point2 {
//
// 2D point
//
public:
    union {
        struct {
            double x, y;
        } g;         // use as individual [g]eometric components
        double a[2]; // use as [a]rray
    };

    Point2(void)
    { g.x = 0.0; g.y = 0.0; };

    Point2(double x, double y)
    { g.x = x;   g.y = y; };

    Point2(double b[2])
    { a[0] = b[0]; a[1] = b[1]; };

};


//
// We use the classes "Point3" and "Vector3" to denote points and
// vectors being used in a geometric sense, as opposed to "Vec3",
// which simply applies to indicate a 3-tuple of scalar values and
// might also, for instance, be applied to an RGB tuple.
//

class Point3 : public Vec3
//
// 3D point
//
{

public:

    Point3(void)
    : Vec3() {};

    Point3(double x, double y, double z)
    : Vec3(x, y, z) {};

    Point3(double a[3])
    : Vec3(a) {};

    // Point3 + Point3 -> Point3 (e.g. for computing midpoint)
    // compound operator
    Point3 operator+=(const Point3 &p)
    {
        a[0] += p.a[0];
        a[1] += p.a[1];
        a[2] += p.a[2];
        return *this;
    };
    // binary operator
    const Point3 operator+(const Point3 &p) const
    {
        return Point3(*this) += p;
    };

    // Point3 * Vec3 -> Point3
    // compound operator
    Point3 operator*=(const Vec3 &vec)
    {
        a[0] *= vec.a[0];
        a[1] *= vec.a[1];
        a[2] *= vec.a[2];
        return *this;
    };
    // binary operator
    const Point3 operator*(const Vec3 &vec) const
    {
        return Point3(*this) *= vec;
    };

    // Point3 - offset -> Point3
    // compound operator
    Point3 operator-=(const double offset)
    {
        a[0] -= offset;
        a[1] -= offset;
        a[2] -= offset;
        return *this;
    };
    // binary operator
    const Point3 operator-(const double offset) const
    {
        return Point3(*this) -= offset;
    };

    // Point3 * scalar -> Point3 (e.g. for computing centroid)
    Point3 operator*=(const double scale)
    // compound operator
    {
        a[0] *= scale;
        a[1] *= scale;
        a[2] *= scale;
        return *this;
    };
    // binary operator
    const Point3 operator*(const double scale) const
    {
        return Point3(*this) *= scale;
    };

    // Point3 / scalar -> Point3 (e.g. for computing centroid)
    // compound operator
    Point3 operator/=(const double scale)
    {
        a[0] /= scale;
        a[1] /= scale;
        a[2] /= scale;
        return *this;
    };
    // binary operator
    const Point3 operator/(const double scale) const
    {
        return Point3(*this) /= scale;
    };
};


class Vector3 : public Vec3
//
// 3D vector
//
{

public:

    Vector3()
    : Vec3()
    { };

    Vector3(double x, double y, double z)
    : Vec3(x, y, z)
    { };

    Vector3(double a[3])
    : Vec3(a)
    { };

    // Vector3 + Point3 -> Point3
    // binary operator
    const Point3 operator+(const Point3 &p) const
    {
        return Point3(
            g.x + p.g.x,
            g.y + p.g.y,
            g.z + p.g.z);
    };

    // Vector3 + Vector3 -> Vector3
    // compound operator
    Vector3 operator+=(const Vector3 &v)
    {
        a[0] += v.a[0];
        a[1] += v.a[1];
        a[2] += v.a[2];
        return *this;
    };
    // binary operator
    const Vector3 operator+(const Vector3 &v) const
    {
        return Vector3(*this) += v;
    };

    // Vector3 - Vector3 -> Vector3
    // compound operator
    Vector3 operator-=(const Vector3 &v)
    {
        a[0] -= v.a[0];
        a[1] -= v.a[1];
        a[2] -= v.a[2];
        return *this;
    };
    // binary operator
    const Vector3 operator-(const Vector3 &v) const
    {
        return Vector3(*this) -= v;
    };

    // -Vector3 -> Vector3
    const Vector3 operator-(void) const
    // unary operator
    {
        return Vector3(
            -g.x,
            -g.y,
            -g.z);
    };

    // Vector3 * Vec3 -> Vector3
    // compound operator
    Vector3 operator*=(const Vec3 &vec)
    {
        a[0] *= vec.a[0];
        a[1] *= vec.a[1];
        a[2] *= vec.a[2];
        return *this;
    };
    // binary operator
    const Vector3 operator*(const Vec3 &vec) const
    {
        return Vector3(*this) *= vec;
    };

    // Vector3 * scalar -> Vector3
    // compound operator
    Vector3 operator*=(const double scale)
    {
        a[0] *= scale;
        a[1] *= scale;
        a[2] *= scale;
        return *this;
    };
    // binary operator
    const Vector3 operator*(const double scale) const
    {
        return Vector3(*this) *= scale;
    };

    // Vector3 / scalar -> Vector3
    // compound operator
    Vector3 operator/=(const double scale)
    {
        a[0] /= scale;
        a[1] /= scale;
        a[2] /= scale;
        return *this;
    };
    // binary operator
    const Vector3 operator/(const double scale) const
    {
        return Vector3(*this) /= scale;
    };


    const double dot(const Vector3 &other) const
    {
        return g.x*other.g.x + g.y*other.g.y + g.z*other.g.z;
    };

    const double mag(void) const
    {
        // C++ oddness: remember that dot(*this) is actually this->dot(*this)
        return sqrt(dot(*this));
    };

    const Vector3 normalized(void) const
    {
        //
        // Normalizing a near-zero vector in a graphics program is
        // usually a sign of an error in program logic. This will almost
        // certainly show errors in your code before they propagate to
        // places where they are harder to isolate. You might even consider
        // propagating the assertion itself up the calling stack.
        //
        double mag_ = mag();
        assert(mag_ > EPSILON); // otherwise, something is likely to be wrong

        // C++ oddness: remember that mag() is actually this->mag()
        return *this / mag_;
    };

};



// not a legal OBJ index (OK for textureIndex and normalIndex to be this)
#define OBJ_INDEX_DEFAULTED -1

class FaceVertex {
public:
    int positionIndex;
    int textureIndex;
    int normalIndex;
    FaceVertex(int positionIndex_, int textureIndex_)
        : positionIndex(positionIndex_),
          textureIndex(textureIndex_)
    {}; // just copy arguments to attributes
    FaceVertex(void) {};
};

class Face {
public:
    // We make all faces triangular by tessellating anything larger.
    // These vertices should appear in CCW order when viewed from the
    // "outside" of the face.
    FaceVertex faceVertex0, faceVertex1, faceVertex2;
    Face(const FaceVertex faceVertex0_, const FaceVertex faceVertex1_,
           const FaceVertex faceVertex2_)
        : faceVertex0(faceVertex0_),
          faceVertex1(faceVertex1_),
          faceVertex2(faceVertex2_)
    { }; // set attributes from arguments -- normals must be set later
    Face(void) {};
};


// Useful shorthand.
const Vector3 vX(1, 0, 0);
const Vector3 vY(0, 1, 0);
const Vector3 vZ(0, 0, 1);


//
// In order to permit commutativity, we must make overloaded
// operators where the left operand is not a Point3 or Vector3
// functions, not methods.
//
// We make use of commutativity wherever possible (and note it).
//

// Vec3 * Point3 -> Point3
inline Point3 operator*(const Vec3 vec, const Point3 p)
{
    return p * vec; // commutativity
}

// Point3 + Vector3 -> Point3
inline Point3 operator+(const Point3 p, const Vector3 v)
{
    // Because we define Point3 before Vector3, I can't figure out a
    // way to make this a member of Point3.
    return Point3(p.a[0] + v.a[0],
                  p.a[1] + v.a[1],
                  p.a[2] + v.a[2]);
}

// Point3 - Vector3 -> Point3
inline Point3 operator-(const Point3 p, const Vector3 v)
{
    // Because we define Point3 before Vector3, I can't figure out a
    // way to make this a member of Point3.
    return Point3(p.a[0] - v.a[0],
                  p.a[1] - v.a[1],
                  p.a[2] - v.a[2]);
}

// Point3 - Point3 -> Vector3
inline Vector3 operator-(const Point3 p0, const Point3 p1)
{
    // Because we define Point3 before Vector3, I can't figure out a
    // way to make this a member of Point3.
    //
    // vector math says that the difference of two points is a vector
    return Vector3(p0.a[0] - p1.a[0],
                   p0.a[1] - p1.a[1],
                   p0.a[2] - p1.a[2]);
}

// scalar * Point3 -> Point3
inline Point3 operator*(const double scale, const Point3 p)
{
    return p * scale; // commutativity
}

// scalar * Vector3 -> Vector3
inline Vector3 operator*(const double scale, const Vector3 v)
{
    return v * scale; // commutativity
}

vector<Point2> readPoint2s(string fname);


class Matrix4 : public Mat4
//
// a 4x4 matrix
//
{

public:

Matrix4()
    : Mat4()
    { };

    // This constructor assumes matrices are in OpenGL order
    Matrix4(const double a_[16]) {
        for (int i = 0; i < 16; i++)
            a[i] = a_[i];
    };

    // Notice that this prototype follows the OpenGL naming convention
    // and allows you to enter matrices from OpenGL matrices verbatim,
    // which normal C/C++ code does not.
    Matrix4(
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

    const Matrix4 operator*(const Matrix4 &matrix) const;
    Matrix4 transpose(void) const;
};


#define INCLUDED_GEOMETRY
#endif // INCLUDED_GEOMETRY
