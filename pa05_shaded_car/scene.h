#ifndef INCLUDED_SCENE
#include <cstdlib>
#include <vector>
using namespace std;

#include "coordinate_axes.h"
#include "color.h"
#include "geometry.h"
#include "light.h"
#include "mesh.h"
#include "poly_line.h"
#include "scene_object.h"
#include "shader_programs.h"


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

    Scene(void);
    void addLight(Light *light);
    void addSceneObject(SceneObject *sceneObject);
    void display(void);
};


extern Scene *scene;

#define INCLUDED_SCENE
#endif // INCLUDED_SCENE
