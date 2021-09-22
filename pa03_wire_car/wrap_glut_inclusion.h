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

#include <GL/glut.h>

#endif

#define WRAP_GLUT_INCLUSION_INCLUDED
#endif // WRAP_GLUT_INCLUSION_INCLUDED
