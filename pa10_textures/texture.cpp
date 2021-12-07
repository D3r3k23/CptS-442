#include <cassert>
#include <string>
#include <iostream>
using namespace std;

#include "check_gl.h"
#include "image.h"
#include "n_elem.h"
#include "controller.h"
#include "texture.h"
#include "wrap_gl_inclusion.h"


static bool isPowerOf2(unsigned int i)
//
// returns TRUE iff i is a (positive) power of 2
//
{
    if (i == 0)
        return false;
    while (!(i & 0x1))
        i >>= 1;
    return i == 1; // only one bit set
}


Texture2D::Texture2D(const Image *image, bool wrap)
{
//
// ASSIGNMENT (PA10)
//
// Initialize a 2D texture (GL_TEXTURE_2D) using `image`.  These are
// the steps:
//
// - (optional) Check to make sure that the image's width and height
//   are powers of two. (For portability, but even when OpenGL permits
//   otherwise, it's more efficient.) Exit with an error message if
//   they are not.
//
// - Call glGenTextures() to get a (texture) `id` (which is an
//   attribute of Texture).  (Observe type conventions.)
//
// - Call glBindTexture() to make the `id` the current
//   `GL_TEXTURE_2D`.
//
// - Make calls to glTexParameteri() to set necessary texture
//   parameters, depending on `wrap`:
//
//   + If `wrap` is true, set s and t wrapping to repeat.
//
//   + If `wrap` is false, set s and t wrapping to clamp at the
//     edge (GL_CLAMP_TO_EDGE). (You might try GL_CLAMP to see the
//     difference.)
//
// - Call glTexParameteri() to set the "pixel area < texel" and "pixel
//   area > texel" filters to linearly interpolate pixels. If you know
//   what a mipmap is, don't worry: You don't need to use one.
//
// - Call `glTexImage2D()` to define the texture `GL_TEXTURE_2D` from
//   the image data. The parameters should be:
//
//   + a mipmap level of 0 (suitable when there's no mipmap)
//
//   + internal format should be GL_RGB8
//
//   + image width and height
//
//   + a border of 0 (for modern OpenGL this is *always* what you
//     want)
//
//   + a pixel data format of GL_RGB
//
//   + a pixel data type of GL_UNSIGNED_BYTE
//
//   + the image data itself
//
// You'll want to pay special attention to OpenGL man pages here.
//
}
