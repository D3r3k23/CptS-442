#include "geometry.h"
#include "scene.h"
#include "shader_programs.h"
#include "texture.h"
#include "transform.h"
#include "wall.h"


Wall::Wall(const Image *image,
           const Point3 &pLL, const Point3 &pLR, const Point3 &pUL,
           int nI, int nJ,
           TexturedShaderProgram *texturedShaderProgram_)
    : SceneObject(),
      texturedShaderProgram(texturedShaderProgram_)
{
    rectangle = new Rectangle(image, pLL, pLR - pLL, pUL - pLL, nI, nJ);

    // We turn off linear filtering for the walls, as that makes the
    // black edges go away. TBD: Follow up on exactly why this is.
    texture = new Texture2D(image, false);
}


void Wall::display(const Transform &viewProjectionTransform,
                   Transform worldTransform)
{
    if (texturedShaderProgram) { // will be NULL in the template
        texturedShaderProgram->setModelViewProjectionMatrix(
            viewProjectionTransform * worldTransform);
        texturedShaderProgram->setImageTexture(texture);
        texturedShaderProgram->start();
    }

    rectangle->draw(this);
}
