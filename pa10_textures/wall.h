#ifndef INCLUDED_WALL

//
// The "wall" module provides the Wall class (see below).
//

#include "image.h"
#include "geometry.h"
#include "rectangle.h"
#include "scene_object.h"
#include "shader_programs.h"
#include "texture.h"
#include "transform.h"

class Wall : public SceneObject
//
// SceneObject representing a rectangular wall
//
{
private:
    TexturedShaderProgram *texturedShaderProgram;
    Rectangle *rectangle;
    Texture2D *texture;

public:
    Wall(const Image *image,
         const Point3 &pLL_, const Point3 &pLR_, const Point3 &pUL_,
         int nI, int nJ,
         TexturedShaderProgram *texturedShaderProgram_);
    void display(const Transform &viewProjectionTransform,
                 Transform worldTransform);
};

#define INCLUDED_WALL
#endif
