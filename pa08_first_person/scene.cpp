#include <cassert>

#include "camera.h"
#include "coordinate_axes.h"
#include "car.h"
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
#include "transform.h"
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
    // The argument to the draw() methods is the
    // "model-view-projection" transform (note the name change from
    // "view" transform), which is initially just the view-projection
    // transform. We'll pass this down the calling stack ultimately to
    // the render() methods, concatenating individual model transforms
    // to the modelview transform (by right-multiplying them) as we
    // descend the scene graph, which we implement in our call graph.
    //
    Transform viewProjectionTransform
        = camera.projectionTransform() * camera.viewTransform();
    renderStats.reset(); // for this frame
    for (unsigned int i = 0; i < sceneObjects.size(); i++) {
        Transform identityTransform; // world transform, initially

        sceneObjects[i]->display(viewProjectionTransform, identityTransform);
    }
    if (controller.axesEnabled) {
        Transform identityTransform;

        coordinateAxes->display(viewProjectionTransform, identityTransform);
    }
    if (controller.statsEnabled)
        renderStats.display();
    if (controller.viewHelpEnabled)
        view.displayViewHelp();
}


void Scene::step(double du)
//
// moves each of the cars and the camera (for use in first person
// mode) by `du`
//
{
    for (int i = 0; i < nCars; i++)
        cars[i]->move(du);
    camera.move(du);
}


Scene::Scene(const Layout layout, const string trackBsplineCvsFname)
{
    constantColorShaderProgram = new ConstantColorShaderProgram(
            "ConstantColorShaderProgram");
    eadsShaderProgram = new EadsShaderProgram();
    //
    // ASSIGNMENT (PA08)
    //
    // Delete the next two lines (which keep the unmodified template
    // from crashing).
    //
    // constantColorShaderProgram = NULL;
    // eadsShaderProgram = NULL;

    coordinateAxes = new CoordinateAxes();

    //
    // ASSIGNMENT (PA08)
    //
    // Modify your previous (PA07) solution here:
    //
    // - Increase the extent ("extent_") of the enviroment to 32 (so
    //   that there's a relatively distant horizon). In addtion to
    //   Ground::Ground(), pass the same value to camera.setExtent().
    //
    // - Don't add a Teapot to the scene if `layout` is LAYOUT_TRIG.
    //   (Actually, there's no real harm in doing this if you're
    //   prepared for your car to pass through the teapot!)
    //
    // - Allocate `cars` to be an array of `nCars` (pointers to) Cars:
    //
    //   * Space them equally in the `u` parameter by setting
    //     `initialU` to an integer multiple of `1.0 / nCars`.
    //
    //   * Choose an assortment of `baseRgb` values (perhaps from a
    //     table?) so you can distinguish one car from another.
    //
    //   * Add each Car to the scene (with addSceneObject()).
    //
    // - Set the camera's path to follow the track's guide curve.
    //
    // 40 lines in instructor solution (YMMV)
    //
    Ground* ground = new Ground(32.0);
    addSceneObject(ground);
    
    track = new Track(layout, trackBsplineCvsFname, ground);
    addSceneObject(track);

    #define N_CAR_COLORS 6
    const Rgb carColors[N_CAR_COLORS] = {
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0},
        {0.5, 0.5, 0.0},
        {0.1, 0.1, 0.1},
        {0.9, 0.9, 0.9}
    };

    cars = new Car*[nCars];
    for (int i = 0; i < nCars; i++)
    {
        Car* car = new Car(carColors[i % N_CAR_COLORS], (double)i / nCars, track->guideCurve);
        cars[i] = car;
        addSceneObject(car);
    }

    if (layout != LAYOUT_TRIG)
        addSceneObject(new Teapot());
    
    camera.setExtent(32.0);
    camera.setPath(track->guideCurve);

    addLight(new Light(whiteColor, {0.0, 0.0, -1.0})); // Sun
    addLight(new Light(whiteColor * 0.5, {0.0, 1.0, 0.0})); // Searchlight
}

