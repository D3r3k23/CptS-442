#ifndef INCLUDED_CAR

//
// The "car" module implements the Car class (see below).
//

#include "coordinate_axes.h"
#include "curve.h"
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
    CoordinateAxes *coordinateAxes;

public:
    double u; // parametric position along track
    const Curve *path; // parametric path the car will follow
    Rgb baseRgb; // determines reflectance properties in ::draw()

    Car(const Rgb &baseRgb, double initialU, const Curve *path);

    void display(const Transform &viewProjectionTransform,
              Transform worldTransform);
    void move(double dU);
};

#define INCLUDED_CAR
#endif // INCLUDED_CAR
