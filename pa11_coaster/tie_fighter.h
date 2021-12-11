#ifndef INCLUDED_FIE_FIGHTER

#include "coordinate_axes.h"
#include "curve.h"
#include "irregular_mesh.h"
#include "n_elem.h"
#include "scene_object.h"
#include "shader_programs.h"
#include "track.h"

using namespace std;

extern string tieFighterFname;

#define DEFAULT_CAR_FNAME "car.obj"

class TieFighter : public SceneObject
{
private:
    IrregularMesh *irregularMesh;
    CoordinateAxes *coordinateAxes;

public:
    Rgb baseRgb; // determines reflectance properties in ::draw()

    TieFighter();

    void display(const Transform &viewProjectionTransform,
              Transform worldTransform);
};

#define INCLUDED_FIE_FIGHTER
#endif // INCLUDED_FIE_FIGHTER
