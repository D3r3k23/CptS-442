

#include "tie_fighter.h"
#include "controller.h"
#include "scene.h"


string tieFighterFname = "tieFighter.obj";

TieFighter::TieFighter(void)
  : irregularMesh(nullptr)
{
    coordinateAxes = new CoordinateAxes();

    baseRgb = {0.7, 0.7, 0.7};

    irregularMesh = IrregularMesh::read(tieFighterFname);
    addHedgehogs(irregularMesh);
}

void TieFighter::display(const Transform &viewProjectionTransform,
                  Transform worldTransform)
{
    Transform modelTransform{};
    modelTransform.scale(0.2, 0.2, 0.2);
    modelTransform.translate(0.0, 0.2, 0.0);

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
        // We never use textures for the tie fighter, regardless of the
        // controller selection.
        scene->eadsShaderProgram->setTextureWeights(0.0, 0.0, 0.0);
        scene->eadsShaderProgram->start();
    }

    irregularMesh->render();
    const double quillLength = 0.04;
    displayHedgehogs(viewProjectionTransform, worldTransform, quillLength);

    if (controller.axesEnabled)
        coordinateAxes->display(viewProjectionTransform, worldTransform);
}
