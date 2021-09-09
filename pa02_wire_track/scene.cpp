#include <cassert>

#include "camera.h"
#include "coordinate_axes.h"
#include "color.h"
#include "controller.h"
#include "curve.h"
#include "n_elem.h"
#include "render_stats.h"
#include "scene.h"
#include "scene_object.h"
#include "shader_programs.h"
#include "track.h"
#include "view.h"
#include "wrap_cmath_inclusion.h"

#include <iostream>
using namespace std;

const Color Scene::skyColor(0.5, 0.5, 1.0);

Scene *scene;


void Scene::addSceneObject(SceneObject *sceneObject)
//
// adds `sceneObject` to the scene
//
{
    sceneObjects.push_back(sceneObject);
}


void Scene::display(void)
//
// draws all objects in the scene
//
{
    //
    // The argument to draw() methods is the "view" transform, We'll
    // pass this down the calling stack ultimately to the render()
    // methods. It's trivial now, but will become much more important
    // when we start using modeling transforms to create complex
    // scenes in PA08.
    //
    Transform viewTransform = camera.viewTransform();
    renderStats.reset(); // for this frame
    for (unsigned int i = 0; i < sceneObjects.size(); i++) {
        sceneObjects[i]->display(viewTransform);
    }
    if (controller.axesEnabled) {
        coordinateAxes->display(viewTransform);
    }
    if (controller.statsEnabled)
        renderStats.display();
    if (controller.viewHelpEnabled)
        view.displayViewHelp();
}


Scene::Scene(void)
{
    coordinateAxes = new CoordinateAxes();
    constantColorShaderProgram = new ConstantColorShaderProgram(
            "ConstantColorShaderProgram");

    //
    // ASSIGNMENT (PA02)
    //
    // Delete the next line (which keeps the unmodified template from
    // crashing).
    //
    // constantColorShaderProgram = NULL;

    //
    // ASSIGNMENT (PA02)
    //
    // This constructor sets up the scene.
    //
    // - Add a new Track to the scene (with addSceneObject(), of
    //   course). (This is very easy, since all the real work is in
    //   Track::Track().)
    //
    // 3 lines in instructor solution (YMMV)
    //
    addSceneObject(new Track);
}

