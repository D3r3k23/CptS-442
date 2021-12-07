#include <assert.h>
#include <math.h> // for M_PI

#include "car.h"
#include "controller.h"
#include "scene.h"

//
// Our rollercoaster car design is a simple irregular mesh specified
// in a Wavefront "*.obj" file (default: "car.obj"). Feel free to
// modify that file.
//

string carFname = DEFAULT_CAR_FNAME;

Car::Car(const Rgb &baseRgb_, double initialU, const Curve *path_)
    :
      irregularMesh(NULL)
{
    coordinateAxes = new CoordinateAxes(); // add this line

    //
    // Copy your previous (PA08) solution here.
    //

    // Since the car's IrregularMesh doesn't need to be tessellated
    // (effectively), we can create the hedgehogs immediately.
    addHedgehogs(irregularMesh);
}

void Car::display(const Transform &viewProjectionTransform,
                  Transform worldTransform)
{
    //
    // Copy your previous (PA08) setting of `modelTransform` here.
    //

    if (scene->eadsShaderProgram) { // will be NULL in the template
        double specFrac = 0.25; // fraction of reflected power that's specular
        Rgb ambDiffBaseRgb = (1.0 - specFrac) * baseRgb;
        Transform identityTransform;

        scene->eadsShaderProgram->setEmittance(blackColor);
        scene->eadsShaderProgram->setAmbient(0.2 * ambDiffBaseRgb);
        scene->eadsShaderProgram->setDiffuse(0.8 * ambDiffBaseRgb);
        scene->eadsShaderProgram->setSpecular(
            Rgb(specFrac, specFrac, specFrac), 10.0);
        worldTransform *= modelTransform;
        scene->eadsShaderProgram->setModelViewProjectionMatrix(
            viewProjectionTransform * worldTransform);
        scene->eadsShaderProgram->setWorldMatrix(worldTransform);
        scene->eadsShaderProgram->setNormalMatrix(
            worldTransform.getNormalTransform());
        // We never use textures for the car, regardless of the
        // controller selection.
        scene->eadsShaderProgram->setTextureWeights(0.0, 0.0, 0.0);
        scene->eadsShaderProgram->start();
    }

    // `irregularMesh` will be NULL in the unmodified template.
    if (irregularMesh) {
        irregularMesh->render();
        const double quillLength = 0.04;
        displayHedgehogs(viewProjectionTransform,
            worldTransform, quillLength);
    }

    if (controller.axesEnabled)
        coordinateAxes->display(viewProjectionTransform, worldTransform);
}


const double Car::speed(const Track *track) const
{
    //
    // Copy your previous (PA09) solution here.
    //
    return 0.0; // replace (permits template to compile cleanly)
}


void Car::move(double dU)
{
    u += dU;
    // The next loop should only be executed once at most, but with
    // parametrics we should be careful.
    while (u > 1.0)
        u -= 1.0;
}

