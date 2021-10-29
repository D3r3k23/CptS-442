#include <iostream>
#include <vector>
using namespace std;

#include "curve.h"
#include "geometry.h"
#include "tube.h"
#include "wrap_cmath_inclusion.h"


void Tube::draw(SceneObject *sceneObject)
{
    //
    // ASSIGNMENT (PA06)
    //
    // Add about code that:
    //
    // - checks if `tessellationMesh` is NULL and, if it is,
    //
    //   + calls `tessellate()` to create it
    //
    //   + calls `sceneObject->addHedgehogs(tessellationMesh)`
    //
    // - renders the tessellation
    //
    // Use TrigonometricCurve::draw() as a guideline.
    //
    // Note: This method will go away when it is more generalized in
    // the next assignment.
    //
    // 4 lines in instructor solution (YMMV)
    //
    if (!tessellationMesh)
    {
        tessellate();
        sceneObject->addHedgehogs(tessellationMesh);
    }
    tessellationMesh->render();
}

struct Frame
{
    Point3 P;
    Vector3 U, V, W;
    Frame(Curve* curve, double u)
        { curve->coordinateFrame(u, P, U, V, W); }
};

void Tube::tessellate(void)
{
    //
    // ASSIGNMENT (PA06)
    //
    // Add code to create an `nI` x `nJ` mesh using a trigonometric
    // parameterization. `nI` is along the angular dimension and `nJ`
    // is along the path. Iff `closed` is true, the tube is
    // continuous.
    //
    // Use `curve->coordinateFrame()` to get the position `p` and
    // `vU`, `vV`, and `vW` basis vectors and generate the
    // `vertexPositions[]` Point3's and vertexNormals[] Vector3's from that
    // based on the extrusion formula we derived in class. Remember
    // that the normal to tube with a circular cross section is just
    // the (unit) radial vector.
    //
    // If the tube is closed, use a `u` step size of 1/`nJ`. If is
    // open, use a `u` step size of 1/(`nJ`-1). The former ensures
    // that the triangles defining the last segment of the tube will
    // not be degenerate.
    //
    // Once `vertexPositions[]` are set, return a RegularMesh
    // constructed from them.
    //
    // Note: This method will go away when it is more generalized in
    // the next assignment.
    //
    // 16 lines in instructor solution (YMMV)
    //
    //std::cout << "Tesselating Tube" << '\n';

    std::vector<Point3>  vertexPositions;
    std::vector<Vector3> vertexNormals;

    double u = 0.0;
    const double uStep = 1.0 / (isClosed ? nJ : nJ - 1);
    for (int j = 0; j < nJ; j++, u += uStep)
    {
        //std::cout << "j=" << j << ", u=" << u << '\n';
        assert(0.0 <= u && u <= 1.0);
        Frame frame(curve, u);

        double theta = 0.0;
        const double thetaStep = 2.0 * M_PI / nI;
        for (int i = 0; i < nI; i++, theta += thetaStep)
        {
            //std::cout << "theta = " << theta << '\n';

            Vector3 vertexNormal   = (radius * cos(theta) * frame.U) + (radius * sin(theta) * frame.V);
            Point3  vertexPosition = frame.P + vertexNormal;

            //std::cout << "vertexPosition[" << i << "]=" << vertexPosition << '\n';
            vertexPositions.push_back(vertexPosition);
            vertexNormals.push_back(vertexNormal);
        }
    }
    tessellationMesh = new RegularMesh(vertexPositions.data(), vertexNormals.data(), nI, nJ, true, isClosed);
}

