#ifndef WRAP_GL_INCLUSION_INCLUDED

//
// This include file takes care of including "glew.h" and "gl.h" in
// the appropriate way on the appropriate platform. Any call to a
// glew*() or gl*() function should include it.
//
// It does *not* take care of "glut.h" (i.e., glut*()) and "glu.h"
// (i.e., glu*()) inclusion, as those are only done infrequently (at
// this writing, one distinct file each).
//

#if   defined(__APPLE__)

//
// It is is (apparently) unnecessary to include an OpenGL header file
// (I think it's "gl3.h") under MACOS.
//
#include <GL/glew.h>

#elif defined(_WIN32)

#include <windows.h>
#include <GL/glew.h>

#elif defined(__linux__)

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#endif

#define WRAP_GL_INCLUSION_INCLUDED
#endif // WRAP_GL_INCLUSION_INCLUDED
