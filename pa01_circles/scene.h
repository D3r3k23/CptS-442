#ifndef INCLUDED_SCENE
#include <cstdlib>
#include <vector>
using namespace std;

#include "geometrical_object.h"
#include "geometry.h"
#include "poly_line.h"
#include "shader_programs.h"


class Scene {

    // vector of GeometricalObjects in the scene
    vector<GeometricalObject *> geometricalObjects;

public:
    PassthruShaderProgram *passthruShaderProgram;


    Scene(void);
    void addGeometricalObject(GeometricalObject *geometricalObject);
    void display(void);
};


extern Scene *scene;

#define INCLUDED_SCENE
#endif // INCLUDED_SCENE
