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


void Framework::bitmapStringAt(double xFrac, double yFrac, string s) {
    void *font = FRAMEWORK_BITMAP_9_BY_15;

    CHECK_GL(glRasterPos2d(xFrac, yFrac));
    for (unsigned int i = 0; s[i]; i++) {
        glutBitmapCharacter(font, s.c_str()[i]);
    }
};


# ifdef WORK_AROUND_GLUT_CHARS_NEED_FLUSH_BUG
void Framework::flushText(void)
{
    //
    // We get OpenGL to think it's going to draw a triangle strip and
    // then fake it out by not providing enough vertices. This makes
    // the preceding output characters appear while guaranteeing not
    // to add any additional graphics output.
    //
    CHECK_GL();
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3d(0,0,0);
    glVertex3d(0,0,0);
    glEnd();
    CHECK_GL();
}
# endif



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

#ifndef WORK_AROUND_GLUT_BLACK_POPUP_MENU
    // The following sets the graphical context.
    //
    // (Warning: These only seem to be available in recent freeglut
    // releases.)
    glutInitContextVersion(MAJOR_OPENGL_CONTEXT_VERSION,
                           MINOR_OPENGL_CONTEXT_VERSION);
    glutInitContextProfile(GLUT_CORE_PROFILE);
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
    // glutInitWindowSize(): reshape the window after the fact.
    //
    CHECK_GL(glutReshapeWindow(defaultCanvasWidth, defaultCanvasHeight));
#endif

    CHECK_GL(glutDisplayFunc(display));
    CHECK_GL(glutReshapeFunc(reshape));

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
