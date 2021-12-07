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
#include "teapot.h"
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


Scene::Scene(const Layout layout, const string trackBsplineCvsFname)
{
    constantColorShaderProgram = new ConstantColorShaderProgram(
            "ConstantColorShaderProgram");
    eadsShaderProgram = new EadsShaderProgram();

    //
    // ASSIGNMENT (PA07)
    //
    // Delete the next two lines (which keep the unmodified template
    // from crashing).
    //
    // constantColorShaderProgram = NULL;
    // eadsShaderProgram = NULL;

    coordinateAxes = new CoordinateAxes();

    //
    // ASSIGNMENT (PA07)
    //
    // Modify your previous (PA06) solution here to add a Ground
    // `ground`, a Track `track` (which, unlike `ground`, needs to be
    // an attribute of the scene), and a Teapot (they are both
    // SceneObjects). The extent of the ground should be 2.0, so that
    // it just barely fits in the initial viewport.
    //
    // Track::Track() now takes `layout` and `trackBsplineCvsFname`
    // (arguments passed to this constructor) and `ground` as
    // arguments.
    //
    // 8 lines in instructor solution (YMMV)
    //
    Ground* ground = new Ground(2.0);
    addSceneObject(ground);

    addSceneObject(new Track(layout, trackBsplineCvsFname, ground));
    addSceneObject(new Teapot());

    addLight(new Light(whiteColor, {0.0, 0.0, -1.0})); // Sun
    addLight(new Light(whiteColor * 0.5, {0.0, 1.0, 0.0})); // Searchlight
}

