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
#include "sky_box.h"
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


const double Scene::cameraSpeed(void) const
{
    //
    // Copy your previous (PA09) solution here.
    //
    return !cars ? 0.0 : cars[0]->speed(track);
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


void Scene::step(double dtReq)
//
// moves each of the cars and the camera (for use in first person
// mode) by `du`, computing the distance `du` traveled by each car in
// time `dtReq`
//
// also moves the camera by the same amount as the first car
//
{
    //
    // Move the cars and camera by no more than this time step. If
    // `dtReq` is smaller, use it instead. If `dtReq` is larger, take
    // steps no longer than this value.
    //

    // dtMax which is a negative power of 2 reduces the likelihood of
    // roundoff problems
    const double dtMax = 1.0 / 1024.0; // resolution of ~1ms

    double dtSum = 0.0;
    while (dtSum < dtReq) {
        double dt = dtReq - dtSum; // step size
        if (dt > dtMax)
            dt = dtMax;
        for (int i = 0; i < nCars; i++) {
            Vector3 dp_du;
            (*cars[i]->path)(cars[i]->u, &dp_du);
            double ds_du = dp_du.mag();
            // A car's speed is ds/dt, which is equal to ds/du * du/dt, so...
            double du = cars[i]->speed(track) * dt / ds_du;
            if (i == 0) // assume the camera is at car[0]
                camera.move(du);
            cars[i]->move(du);
        }
        dtSum += dt;
    }
}


Scene::Scene(const Layout layout, const string trackBsplineCvsFname,
             const string skyBoxFname)
{
    constantColorShaderProgram = new ConstantColorShaderProgram(
            "ConstantColorShaderProgram");
    eadsShaderProgram = new EadsShaderProgram();

    //
    // ASSIGNMENT (PA10)
    //
    // Delete the next two lines (which keep the unmodified template
    // from crashing).
    //
    // constantColorShaderProgram = NULL;
    // eadsShaderProgram = NULL;

    coordinateAxes = new CoordinateAxes();

    //
    // Modify your previous (PA08) solution to instance an SkyBox
    // with `extent` and `skyBoxFname` and add it to the scene.
    //
    Ground* ground = new Ground(32.0);
    addSceneObject(ground);

    addSceneObject(new SkyBox(32.0, skyBoxFname));

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

