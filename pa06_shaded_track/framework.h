#ifndef INCLUDED_FRAMEWORK

//
// This header file provides the Framework class (see below).
//

#include <string>
using namespace std;

#include "check_gl.h"
#include "work_arounds.h"
#include "wrap_gl_inclusion.h"

// This is the only module (both header and source) that should depend
// on GLUT.
#include "wrap_glut_inclusion.h"

enum {
    FRAMEWORK_ACTIVE_CTRL       = GLUT_ACTIVE_CTRL,
    FRAMEWORK_ACTIVE_SHIFT      = GLUT_ACTIVE_SHIFT,
    FRAMEWORK_KEY_UP            = GLUT_KEY_UP,
    FRAMEWORK_KEY_DOWN          = GLUT_KEY_DOWN,
    FRAMEWORK_KEY_LEFT          = GLUT_KEY_LEFT,
    FRAMEWORK_KEY_RIGHT         = GLUT_KEY_RIGHT,
    FRAMEWORK_RIGHT_BUTTON      = GLUT_RIGHT_BUTTON,
    FRAMEWORK_UP                = GLUT_UP,

    //
    // Mouse wheel support is in freeglut, but in original GLUT (we
    // need to support both) it is only "sorta" there, delivering
    // wheel events as undocumented mouse "buttons".  Hopefully,
    // either a future GLUT release (unlikely) or the universal
    // availability of freeglut (more likely) will make appropriate,
    // identically-named definitions and make these #defines
    // unnecessary.
    //
    // Nevertheless, for the time being the buttons are as follows:
    //
# ifdef GLUT_WHEEL_DOWN_BUTTON
    FRAMEWORK_WHEEL_DOWN_BUTTON = GLUT_WHEEL_DOWN_BUTTON,
# else
    FRAMEWORK_WHEEL_DOWN_BUTTON = 0x0004,
# endif
# ifdef GLUT_WHEEL_UP_BUTTON
    FRAMEWORK_WHEEL_UP_BUTTON   = GLUT_WHEEL_UP_BUTTON,
# else
    FRAMEWORK_WHEEL_UP_BUTTON   = 0x0003,
# endif

};

class Framework {
//
// This class abstracts all interfacs to the windowing system,
// especially including event handlers and non-OpenGL functionality
// such as menus and text.
//

public:
    static void *FRAMEWORK_BITMAP_9_BY_15;

    void addMenuEntry(string label, int value)
    {
        glutAddMenuEntry(label.c_str(), value);
    };

    void attachMenu(int button)
    {
        glutAttachMenu(button);
    };

    void bitmapStringAt(double xFrac, double yFrac, string s);

    void createMenu(void (*onMenuSelection)(int menuDescriptor))
    {
        glutCreateMenu(onMenuSelection);
    };

# ifdef WORK_AROUND_GLUT_CHARS_NEED_FLUSH_BUG
    void flushText(void);

# endif
    void fullScreen(void)
    {
        glutFullScreen();
    };

    int getKeyModifiers(void)
    {
        return glutGetModifiers();
    };

    void mainLoop(void)
    {
        glutMainLoop();
    };

    void reshapeWindow(int width, int height)
    {
        glutReshapeWindow(width, height);
    }

    void swapBuffers(void)
    {
#ifdef WORK_AROUND_GLUT_SWAP_NEEDS_GL_FLUSH_BUG
        glFlush();
#endif
        glutSwapBuffers();
    };

    void keyboardFunc(void (*onKeyboardKey)(unsigned char key, int x, int y))
    {
        glutKeyboardFunc(onKeyboardKey);
    };


    void mouseFunc(void (*onMouseButton)(int button, int state, int x, int y))
    {
        glutMouseFunc(onMouseButton);
    };


    void specialFunc(void (*onSpecialKey)(int key, int x, int y))
    {
        glutSpecialFunc(onSpecialKey);
    }
};

extern Framework framework;

#define INCLUDED_FRAMEWORK
#endif // INCLUDED_FRAMEWORK
