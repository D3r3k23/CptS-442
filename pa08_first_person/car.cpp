#include <assert.h>
#include <math.h> // for M_PI

#include "car.h"
#include "controller.h"
#include "scene.h"

//
// Our rollercoaster car design is a simple irregular mesh specified
// in a Wavefront "*.obj" file (default: "car.obj"). Feel free to
// modify that file.
//

string carFname = DEFAULT_CAR_FNAME;

Car::Car(const Rgb &baseRgb_, double initialU, const Curve *path_)
    :
      irregularMesh(NULL)
{
    coordinateAxes = new CoordinateAxes(); // add this line

    //
    // ASSIGNMENT (PA08)
    //
    // Modify your previous (PA03) solution:
    //
    // - Set the car's `path` to the `path_` argument.
    //
    // - Set the car's parametric position `u` to `initialU`.
    //
    // - Set the car's `baseRgb` to `baseRgb_`.
    //
    // (These are very easy.)
    //
    // Note that adding the `baseRgb` attribute lets us have multiple
    // Cars with different colors.
    //
    // 13 lines in instructor solution (YMMV)
    //

    // Since the car's IrregularMesh doesn't need to be tessellated
    // (effectively), we can create the hedgehogs immediately.
    addHedgehogs(irregularMesh);
}

void Car::display(const Transform &viewProjectionTransform,
                  Transform worldTransform)
{
    //
    // ASSIGNMENT (PA08)
    //
    // Implement this in code:
    //
    // - If `path` is NULL (as it might be for debugging)...
    //
    //   * Set `modelTransform` to the identity transform.
    //
    // - If `path` is not NULL, set the car's `modelTransform` to
    //   place the car at the right size and orientation along the
    //   track:
    //
    //   a) Set `modelTransform` to the value returned by the car's
    //      path's coordinateFrame() at u.
    //
    //   b) Scale `modelTransform` by ~0.125 in x, y, and z.
    //
    //   c) Translate `modelTransform` by ~0.20 units in y to move the
    //      car up a bit so it appears more or less above the rails.
    //
    //   d) Rotate `modelTransform` by -pi/2 around the y axis.
    //
    //   e) Rotate `modelTransform` by -pi/2 around the x axis.
    //
    //   The order of nested transforms is important. You will set and
    //   modify `modelTransform`, which will later transform the
    //   vertices. (Do *not* modify the vertices themselves!) This
    //   means that the order in your code will look "backwards" from
    //   the order in which you want the transforms to be applied to
    //   the vertices:
    //
    //    T_model = T_frame * T_scale * T_translate * T_rotateY * T_rotateX
    //                 (a)       (b)        (c)          (d)         (e)
    //
    //   (Remember, the vertices will first be transformed by (e),
    //   (d), (c), then (b) then (a).)
    //

    if (scene->eadsShaderProgram) { // will be NULL in the template
        double specFrac = 0.25; // fraction of reflected power that's specular
        Rgb ambDiffBaseRgb = (1.0 - specFrac) * baseRgb;
        Transform identityTransform;

        scene->eadsShaderProgram->setEmittance(blackColor);
        scene->eadsShaderProgram->setAmbient(0.2 * ambDiffBaseRgb);
        scene->eadsShaderProgram->setDiffuse(0.8 * ambDiffBaseRgb);
        scene->eadsShaderProgram->setSpecular(
            Rgb(specFrac, specFrac, specFrac), 10.0);
        worldTransform *= modelTransform;
        scene->eadsShaderProgram->setModelViewProjectionMatrix(
            viewProjectionTransform * worldTransform);
        scene->eadsShaderProgram->setWorldMatrix(worldTransform);
        scene->eadsShaderProgram->setNormalMatrix(
            worldTransform.getNormalTransform());
        scene->eadsShaderProgram->start();
    }

    // `irregularMesh` will be NULL in the unmodified template.
    if (irregularMesh) {
        irregularMesh->render();
        const double quillLength = 0.04;
        displayHedgehogs(viewProjectionTransform,
            worldTransform, quillLength);
    }

    if (controller.axesEnabled)
        coordinateAxes->display(viewProjectionTransform, worldTransform);
}


void Car::move(double dU)
{
    u += dU;
    // The next loop should only be executed once at most, but with
    // parametrics we should be careful.
    while (u > 1.0)
        u -= 1.0;
}

