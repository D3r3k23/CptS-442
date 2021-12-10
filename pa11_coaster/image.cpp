//
// Read an SGI .sgi (aka .rgb) image file.
//
// Much of this code was borrowed from SGI's tk OpenGL toolkit for
// Brian Paul's "cubemap" demo (q.v.).
//
// Then it was converted to C++ and otherwise shamelessly hacked by
// Bob Lewis for use as part of the "coaster" project.
//

#include <cassert>
#include <stdio.h> // for fopen()
#include <cstdlib>
#include <cstring>

#include <iostream>
using namespace std;

#include "image.h"
#include "wrap_gl_inclusion.h"

struct RgbImage {
    unsigned short imagic;
    unsigned short type;
    unsigned short dim;
    unsigned short sizeX, sizeY, sizeZ;
    // The above entries are read directly from the file. Some of
    // those below might be there as well, but if so they're not read.
    unsigned long min, max;
    unsigned long wasteBytes;
    char name[80];
    unsigned long colorMap;
    // Below this point are entities definitely *not* in the file.
    FILE *file;
    unsigned char *tmp, *tmpR, *tmpG, *tmpB, *tmpA;
    unsigned long rleEnd;
    GLuint *rowStart;
    GLint *rowSize;
};

static void swapShorts(unsigned short *array, long length)
{
    unsigned long b1, b2;
    unsigned char *ptr;

    ptr = (unsigned char *) array;
    while (length--) {
        b1 = *ptr++;
        b2 = *ptr++;
        *array++ = (unsigned short) ((b1 << 8) | (b2));
    }
}

static void swapGluints(GLuint *array, long length)
{
    unsigned long b1, b2, b3, b4;
    unsigned char *ptr;

    ptr = (unsigned char *)array;
    while (length--) {
        b1 = *ptr++;
        b2 = *ptr++;
        b3 = *ptr++;
        b4 = *ptr++;
        *array++ = (b1 << 24) | (b2 << 16) | (b3 << 8) | (b4);
    }
}

static struct RgbImage *RgbImageOpen(const string fname)
{
    union {
        int testWord;
        char testByte[4];
    } endianTest;
    struct RgbImage *rgbImage;
    GLenum swapFlag;

    endianTest.testWord = 1;
    if (endianTest.testByte[0] == 1) {
        swapFlag = GL_TRUE;
    } else {
        swapFlag = GL_FALSE;
    }

    rgbImage = (struct RgbImage *) malloc(sizeof(struct RgbImage));
    if (rgbImage == NULL) {
        cerr << "Out of memory! -- exiting\n";
        return NULL;
    }
    if ((rgbImage->file = fopen(fname.c_str(), "rb")) == NULL) {
        perror(fname.c_str());
        return NULL;
    }

    //
    // Read the first 12 bytes of the file into an RgbImage struct.
    //
    fread(rgbImage, 1, 12, rgbImage->file);

    if (swapFlag)
        swapShorts(&rgbImage->imagic, 6);

    rgbImage->tmp  = (unsigned char *) malloc(rgbImage->sizeX*256);
    rgbImage->tmpR = (unsigned char *) malloc(rgbImage->sizeX*256);
    rgbImage->tmpG = (unsigned char *) malloc(rgbImage->sizeX*256);
    rgbImage->tmpB = (unsigned char *) malloc(rgbImage->sizeX*256);
    if (rgbImage->sizeZ == 4)
        rgbImage->tmpA = (unsigned char *) malloc(rgbImage->sizeX*256);
    if (rgbImage->tmp == NULL || rgbImage->tmpR == NULL
            || rgbImage->tmpG == NULL || rgbImage->tmpB == NULL) {
        cerr << "Out of memory! -- exiting\n";
        return NULL;
    }

    if ((rgbImage->type & 0xFF00) == 0x0100) {
        int nBytesInRow = rgbImage->sizeY * rgbImage->sizeZ * sizeof(GLuint);
        rgbImage->rowStart = (GLuint *) malloc(nBytesInRow);
        rgbImage->rowSize = (GLint *) malloc(nBytesInRow);
        if (rgbImage->rowStart == NULL || rgbImage->rowSize == NULL) {
            cerr << "Out of memory! -- exiting\n";
            return NULL;
        }
        rgbImage->rleEnd = 512 + (2 * nBytesInRow);
        fseek(rgbImage->file, 512, SEEK_SET);
        fread(rgbImage->rowStart, 1, nBytesInRow, rgbImage->file);
        fread(rgbImage->rowSize, 1, nBytesInRow, rgbImage->file);
        if (swapFlag) {
            swapGluints(rgbImage->rowStart,
                        (long) (nBytesInRow / sizeof(GLuint)));
            swapGluints((GLuint *) rgbImage->rowSize,
                        (long) (nBytesInRow / sizeof(GLint)));
        }
    }
    return rgbImage;
}

static void RgbImageClose(struct RgbImage *rgbImage)
{

    fclose(rgbImage->file);
    free(rgbImage->tmp);
    free(rgbImage->tmpR);
    free(rgbImage->tmpG);
    free(rgbImage->tmpB);
    if (rgbImage->sizeZ > 3)
        free(rgbImage->tmpA);
    free(rgbImage);
}

static void RgbImageGetRow(struct RgbImage *rgbImage, unsigned char *buf,
                           int y, int z)
{
    unsigned char *iPtr, *oPtr, pixel;
    int count, done = 0;

    if ((rgbImage->type & 0xFF00) == 0x0100) {
        fseek(rgbImage->file,
              (long) rgbImage->rowStart[y+z*rgbImage->sizeY], SEEK_SET);
        fread(rgbImage->tmp, 1,
              (unsigned int)rgbImage->rowSize[y+z*rgbImage->sizeY],
              rgbImage->file);

        iPtr = rgbImage->tmp;
        oPtr = buf;
        while (!done) {
            pixel = *iPtr++;
            count = (int)(pixel & 0x7F);
            if (!count) {
                done = 1;
                return;
            }
            if (pixel & 0x80) {
                while (count--)
                    *oPtr++ = *iPtr++;
            } else {
                pixel = *iPtr++;
                while (count--) {
                    *oPtr++ = pixel;
                }
            }
        }
    } else {
        fseek(rgbImage->file,
              512 + (y*rgbImage->sizeX) + (z*rgbImage->sizeX*rgbImage->sizeY),
              SEEK_SET);
        fread(buf, 1, rgbImage->sizeX, rgbImage->file);
    }
}


static void RgbImageGetData(struct RgbImage *rgbImage, unsigned char **data)
{
    unsigned char *ptr;
    int i, j;
    int nBytes = 4 * (rgbImage->sizeX+1) * (rgbImage->sizeY+1);

    (*data) = (unsigned char *) malloc(nBytes);
    if ((*data) == NULL) {
        cerr << "Out of memory! -- exiting\n";
        exit(EXIT_FAILURE);
    }

    ptr = *data;
    for (i = 0; i < (int)(rgbImage->sizeY); i++) {
        RgbImageGetRow(rgbImage, rgbImage->tmpR, i, 0);
        RgbImageGetRow(rgbImage, rgbImage->tmpG, i, 1);
        RgbImageGetRow(rgbImage, rgbImage->tmpB, i, 2);
        if (rgbImage->sizeZ > 3) {
            RgbImageGetRow(rgbImage, rgbImage->tmpA, i, 3);
        }
        for (j = 0; j < (int)(rgbImage->sizeX); j++) {
            *ptr++ = *(rgbImage->tmpR + j);
            *ptr++ = *(rgbImage->tmpG + j);
            *ptr++ = *(rgbImage->tmpB + j);
            if (rgbImage->sizeZ > 3) {
                *ptr++ = *(rgbImage->tmpA + j);
            }
        }
    }
    assert(ptr - (*data) <= nBytes);
}


Image::~Image(void)
{
    if (data)
        delete data;
}


Image *Image::crop(int xUL, int yUL, int newWidth, int newHeight) const
//
// extracts and returns a subimage from `originalImage`
//
{
    assert(xUL + newWidth <= width);
    assert(yUL + newHeight <= height);

    Image *image = new Image;
    image->width = newWidth;
    image->height = newHeight;
    image->nBytesPerPixel = nBytesPerPixel;

    int nBytes = image->nBytesPerPixel * image->width * image->height;
    image->data = new unsigned char[nBytes];
    if (image->data == NULL) {
        cerr << "Out of memory! -- exiting\n";
        exit(EXIT_FAILURE);
    }
    int ijNew = 0;
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            int ijOld = 3 * (width * (yUL + i) + (xUL + j));
            image->data[ijNew + 0] = data[ijOld + 0];
            image->data[ijNew + 1] = data[ijOld + 1];
            image->data[ijNew + 2] = data[ijOld + 2];
            ijNew += 3;
        }
    }
    assert(ijNew == nBytes); // sanity check

    return image;
}


Image *Image::getSkyBoxFaceImage(unsigned int iFace,
                                       unsigned int jFace) const
{
    //
    // Sky boxes must have dimensions of 4 * `wh` x 3 * `wh`, where
    // `wh` are the dimensions of each wall.
    //
    //  TBD: Make a `SkyBoxImage` subclass of `Image`.
    //
    int wh = width / 4;
    if (4 * wh != width || 3 * wh != height) {
        cerr << "the sky box image does not have proper dimensions"
             << "(4 * 2**N x 3 * 2**N) for a sky box -- exiting"
             << endl;
        exit(EXIT_FAILURE);
    }

    return crop(jFace*wh, iFace*wh, wh, wh); // order is (x, y, w, h)
}


void Image::warp(const int warpTransform[2][3])
{
    int nBytes = nBytesPerPixel * width * height;
    unsigned char *newData = new unsigned char[nBytes];
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int ijOld = 3 * (width * i + j);
            int iNew = warpTransform[0][0] * i
                + warpTransform[0][1] * j
                + (width-1) * warpTransform[0][2];
            int jNew = warpTransform[1][0] * i
                + warpTransform[1][1] * j
                + (height-1) * warpTransform[1][2];
            int ijNew = 3 * (width * iNew + jNew);
            newData[ijNew] = data[ijOld];
            newData[ijNew+1] = data[ijOld+1];
            newData[ijNew+2] = data[ijOld+2];
        }
    }
    delete data;
    data = newData;
}


Image *Image::readRgb(const string fname)
//
// reads an SGI .rgb file into an Image
//
{
    Image *image = new Image;
    struct RgbImage *rgbImage;

    rgbImage = RgbImageOpen(fname);
    if (!rgbImage) {
        cerr << "File not found -- exiting\n";
        exit(EXIT_FAILURE);
    }
    image->width = rgbImage->sizeX;
    image->height = rgbImage->sizeY;
    image->nBytesPerPixel = rgbImage->sizeZ;
    RgbImageGetData(rgbImage, &image->data);
    RgbImageClose(rgbImage);

    if (image->nBytesPerPixel == 3)
        image->format = GL_RGB;
    else if (image->nBytesPerPixel == 4)
        image->format = GL_RGBA;
    else {
        // not implemented
        cerr << "Error in LoadRGBImage " << image->nBytesPerPixel
             << "-component images not implemented" << endl;
        exit(EXIT_FAILURE);
    }
    return image;
}
