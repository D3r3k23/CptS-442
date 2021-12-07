#ifndef INCLUDED_IMAGE

//
// The "image" module provides the Image class (see below).
//

#include <cstdlib> // for NULL

#include <string>
using namespace std;

#include "wrap_gl_inclusion.h"

class Image
//
// represents 2D images in a format acceptable to OpenGL
//
{
    Image *crop(int x, int y, int width, int height) const;

public:
    int width, height;
    int nBytesPerPixel;
    GLenum format;
    unsigned char *data;

Image(void)
    : width(0), height(0), nBytesPerPixel(0), format(GL_RGB), data(NULL)
    { };
    ~Image(void);

    Image *getSkyBoxFaceImage(unsigned int i0, unsigned int j0) const;
    static Image *readRgb(const string fname);
    void warp(const int warpTransform[2][3]);
};

#define INCLUDED_IMAGE
#endif // INCLUDED_IMAGE
