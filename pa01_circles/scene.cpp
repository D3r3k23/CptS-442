#include <cassert>

#include "circle.h"
#include "controller.h"
#include "geometrical_object.h"
#include "n_elem.h"
#include "scene.h"
#include "shader_programs.h"
#include "wrap_cmath_inclusion.h"

#include <iostream>
using namespace std;

Scene *scene;


void Scene::addGeometricalObject(GeometricalObject *geometricalObject)
//
// adds `geometricalObject` to the scene
//
{
    geometricalObjects.push_back(geometricalObject);
}


void Scene::display(void)
//
// draws all objects in the scene
//
{
    passthruShaderProgram->start(); // this action will move elsewhere later

    for (unsigned int i = 0; i < geometricalObjects.size(); i++)
        geometricalObjects[i]->draw();
}


Scene::Scene(void)
{
    //
    // ASSIGNMENT (PA01)
    //
    // Replace the single Circle() code below with code to add 10
    // concentric Circles centered on (0, 0) of radii 0.1, 0.2, ...
    // 1.0 to the scene. Use Scene::addGeometricalObject() to add
    // them.
    //
    // 5 lines in instructor solution (YMMV)
    //
    passthruShaderProgram = new PassthruShaderProgram();
    for (int i = 1; i <= 10; i++)
    {
        double radius = 0.1 * i;
        auto circle = new Circle(Point2(0, 0), radius);
        addGeometricalObject(circle);
    }
}

