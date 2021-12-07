#include "controller.h"
#include "scene_object.h"
#include "hedgehog.h"
#include "transform.h"

#if PA >= PA04_HEDGHOG_CAR

const void SceneObject::addHedgehogs(Mesh *mesh)
//
// creates face and vertex hedgehogs for the Tube and appends them to
// the `faceHedgehog` and `vertexHedgehog` vectors.
//
{
    Hedgehog *faceHedgehog;
    Hedgehog *vertexHedgehog;

    if (mesh) { // will be NULL in the template
        mesh->createHedgehogs(faceHedgehog, vertexHedgehog);
        faceHedgehogs.push_back(faceHedgehog);
        vertexHedgehogs.push_back(vertexHedgehog);
    }
}

#endif // PA >= PA04_HEDGEHOG_CAR

const void SceneObject::displayHedgehogs(
    const Transform &viewTransform,
    const double quillLength) const
{
    if (controller.normalHedgehogEnabled
            || controller.lightHedgehogIndex != LIGHT_HEDGEHOG_DISABLED) {
        if (controller.useVertexNormals) {
            for (unsigned int i = 0; i < vertexHedgehogs.size(); i++) {
                Hedgehog *hedgehog = vertexHedgehogs[i];
                hedgehog->draw(viewTransform, quillLength);
            }
        } else {
            for (unsigned int i = 0; i < faceHedgehogs.size(); i++) {
                Hedgehog *hedgehog = faceHedgehogs[i];
                hedgehog->draw(viewTransform, quillLength);
            }
        }
    }
}

