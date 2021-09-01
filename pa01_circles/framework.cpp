#ifndef _WIN32 // on Windows, GLEW gets included in "wrap_gl_inclusion.h"
#include <GL/glew.h>
#endif

#include <string>
using namespace std;

#include "check_gl.h"
#include "framework.h"
#include "view.h"
#include "work_arounds.h"
#include "wrap_gl_inclusion.h"
#include "wrap_glut_inclusion.h"

Framework framework;

void *Framework::FRAMEWORK_BITMAP_9_BY_15 = GLUT_BITMAP_9_BY_15;




void View::init(int *argc, char **argv, string windowTitle)
//
// initializes the view. As it depends so much on GLUT, it's in this
// file rather than "view.cpp".
//
{
    unsigned int displayMode =
#ifdef __APPLE__
        // needed to get OpenGL 3.3 [sic] functionality
        GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB | GLUT_3_2_CORE_PROFILE;
#else
        GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB;
#endif

#ifdef WORK_AROUND_GLUT_GL_ERROR_BUG
    glutInit(argc, argv);
    glutInitDisplayMode(displayMode);
#else
    CHECK_GL(glutInit(argc, argv));
    CHECK_GL(glutInitDisplayMode(displayMode));
#endif

#ifndef WORK_AROUND_GLUT_INIT_BUG
    //
    // This is the way things *should* work, but some previous
    // versions of GLUT (predating freeglut) don't like
    // glutInitWindowSize() to be called before a window is created.
    //
    // Note, however, that this trick does not work with OpenGL 3.2
    // running under Ubuntu 12.04, but fortunately our original call
    // to glutInitWindowSize() does (assuming we use freeglut). See
    // "workarounds.h" for details.
    //
# ifdef WORK_AROUND_GLUT_GL_ERROR_BUG
    glutInitWindowSize(defaultCanvasWidth, defaultCanvasHeight);
# else
    CHECK_GL(glutInitWindowSize(defaultCanvasWidth, defaultCanvasHeight));
# endif
#endif
#ifdef WORK_AROUND_GLUT_GL_ERROR_BUG
    glutCreateWindow(windowTitle.c_str());
#else
    CHECK_GL(glutCreateWindow(windowTitle.c_str()));
#endif

#ifdef WORK_AROUND_GLUT_INIT_BUG
    //
    // The best alternative to not being able to call
    // glutInitWindowSize(): reshape the window after the fact.  It
    // may seem paradoxical to do this before we set a reshape
    // callback (below), but that callback is intended to be called
    // when the window is resized by the *user*, not from the
    // program, and is mainly to inform the program that the window
    // has been resized. (We already know this, since we're doing
    // it!)
    //
    // Oddly, this workaround can sometimes cause the newer (3.2)
    // freeglut/OpenGL combination to dump core on the first OpenGL
    // call (glClearColor() in this case).
    //
    CHECK_GL(glutReshapeWindow(defaultCanvasWidth, defaultCanvasHeight));
#endif

    CHECK_GL(glutDisplayFunc(display));

#ifdef WORK_AROUND_ENABLE_GLEW_EXPERIMENTAL
    glewExperimental = GL_TRUE;
#endif
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        fprintf(stderr, "GLEW error: %s -- exiting\n",
                glewGetErrorString(glewStatus));
        exit(EXIT_FAILURE);
    }
    glGetError(); // skip bogus "invalid enumerant" error caused by glewInit()

    //
    // Although Windows and Linux (NVIDIA) OpenGL implementations were
    // forgiving about this, according to OpenGL documentation we need
    // to have a vertex array object (which is not the same thing as a
    // vertex array) bound in order to set vertex array attributes.
    //
    // We could make this MACOS-specific, but it seems to do no harm
    // on the othe platforms and, in any case, seems to be expected.
    //
    GLuint vertexArrayObjectId;

    CHECK_GL(glGenVertexArrays(1, &vertexArrayObjectId));
    CHECK_GL(glBindVertexArray(vertexArrayObjectId));
}
