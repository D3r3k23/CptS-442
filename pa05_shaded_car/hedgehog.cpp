#include "check_gl.h"
#include "controller.h"
#include "hedgehog.h"
#include "light.h"
#include "render_stats.h"
#include "scene.h"
#include "shader_programs.h"
#include "wrap_gl_inclusion.h"

//
// Surfaces whose normals (face or vertex) dotted into the light
// direction result in values larger than this have their light
// hedgehog quills drawn.
//
static const double NORMAL_DOT_THRESHOLD = -5.0e-2;


Hedgehog::Hedgehog(const Point3 *positions_, const Vector3 *normals_,
                   int nQuills_, const Color &color_)
  : nQuills(nQuills_),
    color(color_)
{
    constantColorShaderProgram = new ConstantColorShaderProgram(
        "ConstantColorShaderProgram for hedgehog");

    // save positions and normals
    positions = new Point3[nQuills];
    normals = new Vector3[nQuills];
    for (int i = 0; i < nQuills; i++) {
        positions[i] = positions_[i];
        normals[i] = normals_[i];
    }

    // initialize the Lines object
    Point3 (*vertexPositions)[2] = new Point3[nQuills][2];
    vertexPositions = new Point3[nQuills][2];
    for (int i = 0; i < nQuills; i++) {
        // These will be set at draw time.
        vertexPositions[i][0] = positions_[i];
        vertexPositions[i][1] = positions_[i];
    }
    lines = new Lines(vertexPositions, nQuills);
    delete [] vertexPositions;
}


static bool showQuillVector(const Vector3 &quillVector, const Vector3 &normal)
//
// returns true iff a quill vector should be shown
//
// If all compilers permitted nested functions (like gcc does), this
// would be declared in Hedgehog::render().
//
{
    if (controller.normalHedgehogEnabled) {
        return true; // always show normal vectors
    } else {
        // When the quill vector is a light vector and it does not have a
        // positive projection on the normal, the light doesn't contribute
        // so don't show it.
        //
        // We compare to NORMAL_DOT_THRESHOLD to deny showing
        // near-tangential quills, allowing for roundoff.
        return normal.dot(quillVector) >= NORMAL_DOT_THRESHOLD;
    }
}

void Hedgehog::draw(const Transform &viewTransform, const double quillLength)
{
    Vector3 quillVector;

    scene->constantColorShaderProgram->setViewMatrix(
        viewTransform);

    if (controller.normalHedgehogEnabled) {
        // The hedgehog is displaying normals, so use the intrinsic
        // hedgehog color.
        scene->constantColorShaderProgram->setColor(color);
    } else {
        // If the hedgehog is not displaying normals, it's displaying
        // the light vector given by the `iLight`th light, which we'll
        // use to initialize `quillVector`. We'll also set the color
        // of the quills to the irradiance of the light.
        Light *light = scene->lights[controller.lightHedgehogIndex];

        // Set the quill color to the light color, overriding the
        // intrinsic hedgehog color.
        scene->constantColorShaderProgram->setColor(light->irradiance);

        // Draw quills pointing towards the light.
        quillVector = light->towards();
        // make sure it's unit length
        quillVector = quillVector.normalized();
    }

    //
    // Since the directions of the quill vectors can change from frame
    // to frame, we need to update the buffers on every redraw, unlike
    // other objects.
    //
    for (int iQuill = 0; iQuill < nQuills; iQuill++) {
        Vector3 quillNormal = normals[iQuill];

        // If we're looking at normals, set `quillVector` accordingly.
        // Otherwise, we'll use the value of `quillVector` we set
        // above for the light.
        if (controller.normalHedgehogEnabled)
            quillVector = quillNormal.normalized();

#if 0 // TBD: Enable when transforms have inverses to preserve quill length.
        quillVector = quillVector.normalized(); // may not be necessary
        quillVector /= (viewTransform.inverse() * quillVector).mag();
#endif
        const Point3 start = positions[iQuill];
        const Point3 end = start + quillLength * quillVector;
        lines->vertexPositions[iQuill][0] = start;

        // To not show a quill vector, set the quill to zero length.
        if (showQuillVector(quillVector, quillNormal))
            lines->vertexPositions[iQuill][1] = end;
        else
            lines->vertexPositions[iQuill][1] = start; // i.e. zero length
    }
    lines->updateBuffers();

    scene->constantColorShaderProgram->start();
    lines->render();

    return;
}
