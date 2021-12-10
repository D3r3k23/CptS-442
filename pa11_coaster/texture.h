#ifndef INCLUDED_TEXTURE

//
// The "texture" module provides the Texture2D and, later,
// CubeMapFaceSpec classes (see below).
//

#include <string>

#include "image.h"
#include "wrap_gl_inclusion.h" // for GLenum


class Texture2D
//
// a 2-dimensional image texture
//
{
public:
    unsigned int id;

    Texture2D(const Image *image, bool wrap);
    bool useTextureVertices(void) const { return true; };
};


#define INCLUDED_TEXTURE
#endif // INCLUDED_TEXTURE
