#include <cassert>

#include "camera.h"
#include "coordinate_axes.h"
#include "car.h"
#include "color.h"
#include "controller.h"
#include "n_elem.h"
#include "render_stats.h"
#include "scene.h"
#include "scene_object.h"
#include "shader_programs.h"
#include "view.h"
#include "wrap_cmath_inclusion.h"

#include <iostream>
using namespace std;

const Color Scene::skyColor(0.5, 0.5, 1.0);

Scene *scene;


void Scene::addLight(Light *light)
//
// adds `light` to the scene
//
{
    lights.push_back(light);
}


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
    constantColorShaderProgram = new ConstantColorShaderProgram(
            "ConstantColorShaderProgram");
    eadsShaderProgram = new EadsShaderProgram();

    //
    // ASSIGNMENT (PA05)
    //
    // Delete the next two lines (which keep the unmodified template
    // from crashing).
    //
    // constantColorShaderProgram = NULL;
    // eadsShaderProgram = NULL;

    coordinateAxes = new CoordinateAxes();

    addLight(new Light(whiteColor, Vector3(-1, -1, -1)));
    addSceneObject(new Car());
}

