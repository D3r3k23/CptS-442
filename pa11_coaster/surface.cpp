#include <assert.h>

#include "geometry.h"
#include "mesh.h"
#include "scene_object.h"
#include "surface.h"


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
    // Copy your previous (PA07) solution here.
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
