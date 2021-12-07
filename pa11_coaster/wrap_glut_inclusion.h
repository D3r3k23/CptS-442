#ifndef WRAP_GLUT_INCLUSION_INCLUDED

//
// This include file takes care of including "glut.h" in the
// appropriate way on the appropriate platform. Any call to a glut*()
// function should include it.
//

#if   defined(__APPLE__)

#include <GLUT/glut.h>

#elif defined(_WIN32)

#define GLUT_STATIC_LIB
#include <GL/glut.h>

#elif defined(__linux__)

#include <GL/freeglut.h>

#endif

// glutInitContextVersion() and glutInitContextProfile() are two
// relatively new additions to freeglut that specify the glut context.
// On some systems, calls to them make the glut menus appear black.
#ifndef WORK_AROUND_GLUT_BLACK_POPUP_MENU
// The following are passed to glutInitContextVersion() to select the
// OpenGL context. Eventually, this might be something more modern
// like (4, 6).
#define MAJOR_OPENGL_CONTEXT_VERSION 3
#define MINOR_OPENGL_CONTEXT_VERSION 3
#endif

#define WRAP_GLUT_INCLUSION_INCLUDED
#endif // WRAP_GLUT_INCLUSION_INCLUDED
