#ifndef INCLUDED_TESSELLATION

//
// The "tessellation" module provides the Tessellation class (see below).
//


class Tessellation
//
// a virtual class for all entities OpenGL knows how to draw directly
//
{
public:
    virtual void allocateBuffers(void) = 0;
    virtual const void render(void) = 0;
    virtual void updateBuffers(void) = 0;
};

#define INCLUDED_TESSELLATION
#endif
