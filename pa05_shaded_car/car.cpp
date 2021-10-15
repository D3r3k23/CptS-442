#include <assert.h>

#include "car.h"
#include "controller.h"
#include "scene.h"

//
// Our rollercoaster car design is a simple irregular mesh specified
// in a Wavefront "*.obj" file (default: "car.obj"). Feel free to
// modify that file.
//

string carFname = DEFAULT_CAR_FNAME;

Car::Car(void)
{

    //
    // Copy your previous (PA03) solution here.
    //
    irregularMesh = IrregularMesh::read(carFname);

    // Since the car's IrregularMesh doesn't need to be tessellated
    // (effectively), we can create the hedgehogs immediately.
    addHedgehogs(irregularMesh);
}

void Car::display(const Transform &viewTransform)
{

    if (scene->eadsShaderProgram) { // will be NULL in the template
        double specFrac = 0.25; // fraction of reflected power that's specular
        Rgb baseRgb(0,.5,1);
        Rgb ambDiffBaseRgb = (1.0 - specFrac) * baseRgb;
        Transform identityTransform;

        scene->eadsShaderProgram->setEmittance(blackColor);
        scene->eadsShaderProgram->setAmbient(0.2 * ambDiffBaseRgb);
        scene->eadsShaderProgram->setDiffuse(0.8 * ambDiffBaseRgb);
        scene->eadsShaderProgram->setSpecular(
            Rgb(specFrac, specFrac, specFrac), 10.0);
        scene->eadsShaderProgram->setViewMatrix(viewTransform);
        scene->eadsShaderProgram->setNormalMatrix(identityTransform);
        scene->eadsShaderProgram->start();
    }

    // `irregularMesh` will be NULL in the unmodified template.
    if (irregularMesh) {
        irregularMesh->render();
        const double quillLength = 0.04;
        displayHedgehogs(viewTransform, quillLength);
    }

}

