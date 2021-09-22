#ifndef INCLUDED_CAR

//
// The "car" module implements the Car class (see below).
//

#include "irregular_mesh.h"
#include "n_elem.h"
#include "scene_object.h"
#include "shader_programs.h"

using namespace std;

extern string carFname; // name of the file containing the car model

#define DEFAULT_CAR_FNAME "car.obj"

class Car : public SceneObject
//
// a car riding on the coaster
//
{
private:
    IrregularMesh *irregularMesh;

public:
    Color color;

    Car(void);

    void display(const Transform &viewTransform);
};

#define INCLUDED_CAR
#endif // INCLUDED_CAR
