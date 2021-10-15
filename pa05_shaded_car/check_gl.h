#ifndef INCLUDED_CHECK_GL

//
// This header file defines the macro CHECK_GL() that should wrap all
// OpenGL (gl*() and glu*()) and GLUT (glut*()) calls. The latter is
// necessary because GLUT uses the OpenGL error mechanism.
//

#include <cstdlib> // for abort()
#include <cstdio> // for fprintf() and stderr

#include <string>
using namespace std;

#include "wrap_gl_inclusion.h"

//
// This is the only file that depends on GLU.
//
#if   defined(__APPLE__)
#include <OpenGL/glu.h>
#elif defined(_WIN32)
#include <windows.h>
#include <GL/glu.h>
#elif defined(__linux__)
#include <GL/glu.h>
#endif

enum { MAX_GL_ERROR_DEPTH = 3 };

inline void checkGL(string file, int line, string func, string stmt)
{
    GLenum err = glGetError();

    if (err == GL_NO_ERROR)
        return; // do nothing
    int depth = 1;
    do {
        fprintf(stderr,
                "%s:%d: (in %s())\n"
                "  OpenGL error \"%s\" (0x%04x) detected on call:\n"
                "  %s\n",
                file.c_str(), line, func.c_str(),
                gluErrorString(err), (unsigned int) err,
                stmt.c_str());
        err = glGetError(); // OpenGL errors are queued -- display them all
        if (++depth >= MAX_GL_ERROR_DEPTH) {
            fprintf(stderr,
                "  ... (maximum error depth exceeded -- probably OpenGL bug)\n"
                );
            break;
        }
    } while (err != GL_NO_ERROR);
    fprintf(stderr,
                "  -- exiting with core dump (where available)\n");
    abort();
}
//
// Placing any statement that makes an OpenGL call within this macro
// will (unless NDEBUG is defined) check for OpenGL errors after the
// call and, if found, print an error message to stdout and exit.
//
// Since it is not permitted to call glGetError() between glBegin()
// and glEnd(), be sure *not* to use this around those calls or
// glVertex*(), glNormal*(), glColor*(), or other gl*() calls that
// occur between them.
//
// (The "do { ... } while (0)" is an old C trick to avoid occasional
// clashes between C/C++ and cpp and make CHECK_GL() work just like a
// function call.)
//
// MSVC++ issues a warning about the "while (0)" expression below. We'll
// disable it with the MSVC++ __pragma() keyword there, but need to
// disable __pragma() on other platforms.
//
#if !defined(_WIN32) || defined(__MINGW32__)
#define __pragma(expr)
#endif

#ifndef NDEBUG
#define CHECK_GL(stmt) \
    do { stmt; checkGL(__FILE__, __LINE__, __FUNCTION__, #stmt); } \
    __pragma(warning(push)) \
    __pragma(warning(disable: 4127)) \
    while (0) \
    __pragma(warning(pop))
#else
#define CHECK_GL(stmt) \
    do { stmt;                                                   } \
    __pragma(warning(push)) \
    __pragma(warning(disable: 4127)) \
    while (0) \
    __pragma(warning(pop))
#endif

#define INCLUDED_CHECK_GL
#endif // INCLUDED_CHECK_GL
