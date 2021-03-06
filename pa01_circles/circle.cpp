#include "circle.h"
#include "n_elem.h"
#include "poly_line.h"
#include "wrap_cmath_inclusion.h"
#include <vector>


void Circle::draw(void)
{
    // If the circle has not been tessellated, tessellate it.
    if (!tessellationPolyLine)
        tessellate();

    // Now draw it.
    tessellationPolyLine->render();
}


// Calculates the number of vertices for a tesselated poly line circle based on:
// P: Screen resolution in NDC coordinates per pixel
// r: circle radius
static int number_vertices(double P, double r)
{
    double D = P / 2;
    double n = M_PI / (std::acos(1 - D / r));
    return std::ceil(n);
}


void Circle::tessellate(void)
//
// tessellates the circle
//
{
    //
    // ASSIGNMENT (PA01)
    //
    // Replace the square PolyLine code below with code that uses the
    // formula you derived to set `tessellationPolyLine` to a PolyLine
    // representing a polygon with the minimum number of sides to be
    // within a half-pixel of a circle of the given `radius`.
    //
    // PolyLine() copies `vertexPosition`, so if you use dynamic
    // storage (and you should), remember to clean up after yourself.
    //
    // 19 lines in instructor solution (YMMV)
    //

    const double P = 0.00119; // Screen resolution - NDC coordinates per pixel
    int n = number_vertices(P, radius);

    std::vector<Point2> vertices;
    for (int i = 0; i < n; i++)
    {
        double theta = (2 * M_PI / n) * i; // radians
        double x = radius * std::sin(theta) + center.g.x;
        double y = radius * std::cos(theta) + center.g.y;
        vertices.push_back(Point2(x, y));
    }
    tessellationPolyLine = new PolyLine(vertices.data(), vertices.size(), true);
}
