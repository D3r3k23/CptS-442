#ifndef INCLUDED_SCENE
#include <cstdlib>
#include <vector>
using namespace std;

#include "car.h"
#include "coordinate_axes.h"
#include "color.h"
#include "geometry.h"
#include "light.h"
#include "mesh.h"
#include "poly_line.h"
#include "scene_object.h"
#include "shader_programs.h"
#include "track.h"

//
// NDC is measured in "length units", so each length unit corresponds
// to this many meters when we compute accelerations and
// velocities. (Time is always measured in real seconds and we're not
// worried about mass or charge. Yet.)
//
const double METERS_PER_LENGTH_UNIT = 50;
const double LENGTH_UNITS_PER_METER = 1.0 / METERS_PER_LENGTH_UNIT;

const double gravAccel = 9.8 // m / s**2
    * LENGTH_UNITS_PER_METER; // -> length (s) units per s**2

class Scene {

    // vector of SceneObjects in the scene
    vector<SceneObject *> sceneObjects;

public:
    ConstantColorShaderProgram *constantColorShaderProgram;
    static const Color skyColor;
    EadsShaderProgram *eadsShaderProgram;

    // `coordinateAxes` are a SceneObject, but we have to treat them
    // separately as their visibility can be turned on and off in the
    // GUI.
    CoordinateAxes *coordinateAxes;
    vector<Light *> lights; // (multiple) lights illuminating the scene
    Track *track;
    Ground* ground;
    static const int nCars = 6; // number of cars on the track
    Car **cars; // array of Car (pointers) to cars for later manipulation

    Scene(const Layout layout, const string trackBsplineCvsFname,
          const string skyBoxFname);
    const double cameraSpeed(void) const;
    void addLight(Light *light);
    void addSceneObject(SceneObject *sceneObject);
    void display(void);
    void step(double dT);
};


extern Scene *scene;

#define INCLUDED_SCENE
#endif // INCLUDED_SCENE
