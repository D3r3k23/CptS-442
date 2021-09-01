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



#define INCLUDED_GEOMETRY
#endif // INCLUDED_GEOMETRY
