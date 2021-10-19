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
#include "tube.h"
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
    // ASSIGNMENT (PA06)
    //
    // Delete the next two lines (which keep the unmodified template
    // from crashing).
    //
    // constantColorShaderProgram = NULL;
    // eadsShaderProgram = NULL;

    coordinateAxes = new CoordinateAxes();

    //
    // ASSIGNMENT (PA06)
    //
    // Replace the Car you added in PA02 with a Track (as you did in
    // PA02).
    //
    // Add two Lights:
    //
    // - one of full irradiance (white) shining "down" (i.e., in the
    //   negative z direction). This is the "Sun".
    //
    // - add a second white light shining in the positive y direction
    //   (kind of a horizontal "searchlight"). Make the searchlight
    //   50% as bright as "the Sun", so you can distinguish the two
    //   sources.
    //
    // See the solution to PA05 for hints.
    //
    // 4 lines in instructor solution (YMMV)
    //
    addSceneObject(new Track());

    addLight(new Light(whiteColor, {0, 0, -1})); // Sun
    addLight(new Light(whiteColor*0.5, {0, 1, 0})); // Searchlight
}

