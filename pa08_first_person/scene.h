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
    static const int nCars = 6; // number of cars on the track
    Car **cars; // array of Car (pointers) to cars for later manipulation

    Scene(const Layout layout, const string trackBsplineCvsFname);
    void addLight(Light *light);
    void addSceneObject(SceneObject *sceneObject);
    void display(void);
    void step(double dU);
};


extern Scene *scene;

#define INCLUDED_SCENE
#endif // INCLUDED_SCENE
