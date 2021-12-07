#include <assert.h>
#include <vector>

#include "geometry.h"
#include "mesh.h"
#include "scene_object.h"
#include "surface.h"

#include <iostream>

void Surface::draw(SceneObject *sceneObject)
{
    if (!tessellationMesh) {
        tessellate();
        sceneObject->addHedgehogs(tessellationMesh);
    }

    tessellationMesh->render();
}

void Surface::tessellate(void)
{
    //
    // ASSIGNMENT (PA07)
    //
    // Construct the `nI` x `nJ` grid of `vertexPositions` (Point3s)
    // and their `vertexNormals` (Vector3s) on the surface and set
    // `tessellationMesh` to a new RegularMesh using them. Remember
    // that the surface itself can be invoked parametrically (as
    // "(*this)(...)").
    //
    // To construct `vertexNormals`, note that "(*this)(...)" now
    // returns partial derivative Vector3's `dp_du` and `dp_dv` and
    // that the surface normal can be constructed from these. Also
    // remember that surface normals must be of unit length.
    //
    // Use the same criteria for `u` step size (depending on closed
    // vs. non-closed) that you used for Tube::tessellate() in PA06
    // (using `wrapJ` here, of course). Also use it for the `v` step
    // size.
    //
    // For consistency with the solution (although it's really
    // arbitrary), make `i` the most rapidly-varying index.
    //
    // 17 lines in instructor solution (YMMV)
    //
    std::vector<Point3>  vertexPositions;
    std::vector<Vector3> vertexNormals;

    double v = 0.0;
    const double vStep = 1.0 / (wrapJ ? nJ : nJ - 1);
    for (int j = 0; j < nJ; j++, v += vStep)
    {
        double u = 0.0;
        const double uStep = 1.0 / (wrapI ? nI : nI - 1);
        for (int i = 0; i < nI; i++, u += uStep)
        {
            Vector3 dp_du, dp_dv;
            Point3 position = (*this)(u, v, dp_du, dp_dv);

            Vector3 normal = dp_du.cross(dp_dv).normalized();

            vertexPositions.push_back(position);
            vertexNormals.push_back(normal);

            // std::cout << "j=" << j << " -> u=" << u << " i=" << i << " -> v=" << v << '\n';
            // std::cout << "position=" << position << '\n';
            // std::cout << "normal=" << normal << '\n';
        }
    }

    tessellationMesh = new RegularMesh(vertexPositions.data(), vertexNormals.data(), nI, nJ, wrapI, wrapJ);
}
