#ifndef INCLUDED_SKY_BOX

//
// The "sky_box" module provides the SkyBox class.
//

#include <vector>
using namespace std;

#include "geometry.h"
#include "height_field.h"
#include "image.h"
#include "scene_object.h"
#include "shader_programs.h"
#include "surface.h"
#include "wall.h"

class SkyBox : public SceneObject
//
// a SceneObject that represents a set of textured walls surrounding
// the scene
//
// The viewer is intended to be *inside* the SkyBox, seeing a
// wall when there are no closer objects in the scene.
//
{
private:
    TexturedShaderProgram *texturedShaderProgram;

    // Under MACOS, there's a global "extent" we must avoid, hence the "_".
    double extent_;

    vector<Wall *> walls;

    void addWalls(void);

public:
    // This is the image used for wall textures. Each of the five
    // walls uses a different part of it.
    const Image *skyBoxImage;

    SkyBox(const double extent, const string skyBoxFname);
    void display(const Transform &viewProjectionTransform,
                 Transform worldTransform);
};

#define INCLUDED_SKY_BOX
#endif // INCLUDED_SKY_BOX
