#include <assert.h>

#include "geometry.h"
#include "mesh.h"
#include "scene_object.h"
#include "surface.h"


void Surface::draw(SceneObject *sceneObject)
{
    if (!tessellationMesh) {
        // We've moved the updateBuffers() call here so that we can
        // call setTextureCoordinates() prior to that call.
        tessellate();
        tessellationMesh->setTextureCoordinates(1.0, 1.0);
        tessellationMesh->updateBuffers();
        sceneObject->addHedgehogs(tessellationMesh);
    }

    tessellationMesh->render();
}

void Surface::tessellate(void)
{
    //
    // Copy your previous (PA07) solution here.
    //

    return;
}
