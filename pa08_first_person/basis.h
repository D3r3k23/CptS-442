#ifndef INCLUDED_BASIS

//
// The "basis" module provides the Basis class (see below). It
// provides code for some well-known bases (families of polynomials).
// These are used for smooth curves and surfaces.
//

#include <cstddef> // for NULL

class Basis
//
// virtual class for bases
//
// Each basis only needs a single function, so it may seem overkill to
// define a class for each. A typedef would have worked, except that
// C++ doesn't permit defaulting arguments for typedefs. Besides, its
// an excellent example of using a virtual class.
//
{
public:
    virtual ~Basis() { };
    virtual const void operator()(
        const double u, double bs[4],
        double (*db_dus)[4] = NULL) const = 0;
};

class UniformCubicBSplineBasis : public Basis {
public:
    const void operator()(
        const double u, double bs[4],
        double (*db_dus)[4] = NULL) const;
};

class BezierBasis : public Basis {
public:
    const void operator()(
        const double u, double bs[4],
        double (*db_dus)[4] = NULL) const;
};

#define INCLUDED_BASIS
#endif // INCLUDED_BASIS
