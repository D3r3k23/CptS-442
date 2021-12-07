#include <math.h>

#include "color.h"
#include "controller.h"
#include "ground.h"
#include "height_field.h"
#include "n_elem.h"
#include "scene.h"
#include "shader_programs.h"
#include "transform.h"



const double gaussian(const double x, const double y,
                              const double sigma)
//
// a 2-D Gaussian (unnormalized)
//
{
    return exp((-pow(x, 2) - pow(y, 2)) / (2 * pow(sigma, 2)));
}


// ground global used in position()
static double extent_ = 2.0; // actually set in constructor

static struct {
    double u0, v0;
    double rHalf;
    double height;
} hills[] = {
    { 0.5, 0.5, 0.2, 0.22 }, // teapot base (do not modify)
};

static const Point3 position(const double u, const double v)
{
    //
    // Map parametric unit square to [-extent/2, extent/2] x
    // [-extent/2, extent/2] (NDC).
    //
    double x = extent_ * (u - 0.5);
    double y = extent_ * (v - 0.5);

    double zSum = 0.0;
    for (unsigned int i = 0; i < N_ELEM(hills); i++) {
        double sigma = hills[i].rHalf / log(2.0);
        double dx = x - extent_ * (hills[i].u0 - 0.5);
        double dy = y - extent_ * (hills[i].v0 - 0.5);
        zSum += hills[i].height * gaussian(dx, dy, sigma);
    }

    return Point3(x, y, zSum);
}


void Ground::display(const Transform &viewTransform)
{
    if (!heightField->tessellationMesh) {
        heightField->tessellate();
        addHedgehogs(heightField->tessellationMesh);
    }

    //
    // A grassy Ground should have a dark greenish Diffuse + Ambient
    // reflectance.
    //
    Rgb ambDiffBaseRgb(.1,.4,.1);
    if (scene->eadsShaderProgram) { // will be NULL in the template
        scene->eadsShaderProgram->setEmittance(blackColor);
        scene->eadsShaderProgram->setAmbient(0.2 * ambDiffBaseRgb);
        scene->eadsShaderProgram->setDiffuse(0.8 * ambDiffBaseRgb);
        scene->eadsShaderProgram->setSpecular(blackRgb, 0.0);
        Transform identityTransform;
        scene->eadsShaderProgram->setViewMatrix(viewTransform);
        scene->eadsShaderProgram->setNormalMatrix(identityTransform);
        scene->eadsShaderProgram->start();
    }
    heightField->tessellationMesh->render();

    const double quillLength = 0.02;
    displayHedgehogs(viewTransform, quillLength);
}


Ground::Ground(const double extent)
{
    //
    // Initially, we use a 21 x 21 vertex mesh, centered at (0, 0, 0).
    // This makes the wireframe useful for determining the positions
    // of things.
    //
    int nI = 21;
    int nJ = 21;
    extent_ = extent;

    heightField = new HeightField(position, nI, nJ);
}


const double Ground::height(const double x, const double y) const
{
    //
    // Undo the uv -> xy mapping that operator() does and then call
    // it to return the Point3 in NDC.
    //
    Vector3 dp_du, dp_dv; // ignored
    Point3 p = (*heightField)((x / extent_) + 0.5, (y / extent_) + 0.5,
                              dp_du, dp_dv);
    return p.g.z;
}
