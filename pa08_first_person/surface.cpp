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

    return;
}
