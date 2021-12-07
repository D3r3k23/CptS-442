#include "bezier_patch.h"
#include "controller.h"
#include "scene.h"
#include "shader_programs.h"
#include "teapot.h"
#include "teapot_cvs.h"


void Teapot::display(const Transform &viewTransform)
{
    Rgb veryRedRgb(0.976, 0.051, 0.008);

    if (scene->eadsShaderProgram) { // will be NULL in the template
        scene->eadsShaderProgram->setEmittance(blackColor);
        scene->eadsShaderProgram->setDiffuse(0.3*veryRedRgb);
        scene->eadsShaderProgram->setAmbient(0.3*veryRedRgb);
        scene->eadsShaderProgram->setSpecular(Rgb(0.3, 0.3, 0.3), 30.0);
        Transform identityTransform;
        scene->eadsShaderProgram->setViewMatrix(viewTransform);
        scene->eadsShaderProgram->setNormalMatrix(identityTransform);
        scene->eadsShaderProgram->start();
    }

    // draw the individual patches
    for (unsigned int i = 0; i < bezierPatches.size(); i++)
        bezierPatches[i]->draw(this);

    // draw their hedgehogs
    const double quillLength = 0.01;
    for (unsigned int i = 0; i < bezierPatches.size(); i++) {
        displayHedgehogs(viewTransform, quillLength);
    }
}


Teapot::Teapot(void)
{
    for (int i = 0; i < nTeapotBezierPatches; i++) {
        BezierPatch *bezierPatch = new BezierPatch(
            teapotBezierPatches[i], nI, nJ);
        bezierPatches.push_back(bezierPatch);
    }
}
