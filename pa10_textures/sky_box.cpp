#include <string>

#include "geometry.h"
#include "ground.h"
#include "n_elem.h"
#include "scene.h"
#include "shader_programs.h"
#include "sky_box.h"
#include "texture.h"
#include "wall.h"


SkyBox::SkyBox(const double extent, const string skyBoxFname)
{
    texturedShaderProgram = new TexturedShaderProgram();
    skyBoxImage = Image::readRgb(skyBoxFname);
    extent_ = extent;
    addWalls();
}


void SkyBox::addWalls(void)
{
    Point3 p_xyz = Point3(-extent_/2, -extent_/2, -extent_/2);
    Point3 p_xyZ = Point3(-extent_/2, -extent_/2,  extent_/2);
    Point3 p_xYz = Point3(-extent_/2,  extent_/2, -extent_/2);
    Point3 p_xYZ = Point3(-extent_/2,  extent_/2,  extent_/2);
    Point3 p_Xyz = Point3( extent_/2, -extent_/2, -extent_/2);
    Point3 p_XyZ = Point3( extent_/2, -extent_/2,  extent_/2);
    Point3 p_XYz = Point3( extent_/2,  extent_/2, -extent_/2);
    Point3 p_XYZ = Point3( extent_/2,  extent_/2,  extent_/2);

    struct WallSpec {
        Point3 pLL, pLR, pUL;

        // row and column of subimage within 3 x 4 subdivision
        int iFace, jFace;
    } wallSpecs[] = {
        { p_xyz, p_xYz, p_xyZ, 1, 3 }, // back
        { p_XYz, p_Xyz, p_XYZ, 1, 1 }, // front
        { p_xYz, p_XYz, p_xYZ, 1, 0 }, // left
        { p_Xyz, p_xyz, p_XyZ, 1, 2 }, // right
        { p_XYZ, p_XyZ, p_xYZ, 2, 1 }, // top
#if 0 // if needed (bottom is filled by Ground)
        { p_xYz, p_xyz, p_XYz, 0, 0 }, // bottom
#endif
    };

    for (unsigned int i = 0; i < N_ELEM(wallSpecs); i++) {
        const Image *wallImage = skyBoxImage->getSkyBoxFaceImage(
            wallSpecs[i].iFace, wallSpecs[i].jFace);
        Wall *wall = new Wall(wallImage,
                wallSpecs[i].pLL, wallSpecs[i].pLR, wallSpecs[i].pUL,
                2, 2, texturedShaderProgram);
        walls.push_back(wall);
        // TBD: delete "wallImage"
    }
    // TBD: delete "image"
}


void SkyBox::display(const Transform &viewProjectionTransform,
                          Transform worldTransform)
{
    // draw the walls
    for (unsigned int i = 0; i < walls.size(); i++)
        walls[i]->display(viewProjectionTransform, worldTransform);
}
