#include <cassert>
#include <cstdlib>

#include "camera.h"
#include "clock.h"
#include "controller.h"
#include "framework.h"
#include "render_stats.h"
#include "scene.h"
#include "view.h"
#include "wrap_cmath_inclusion.h"

Controller controller;

enum {
    MENU_FIRST, // placeholder so that menu entries start at 1 (leave alone!)

    // These options are always available.
    MENU_QUIT,
    // organize these by the PA in which they first appear, then alphabetically
    MENU_RESET_VIEW,
    MENU_TOGGLE_COORDINATE_AXES,
    MENU_TOGGLE_FULL_SCREEN,
    MENU_TOGGLE_STATS,
    MENU_TOGGLE_VIEW_HELP,
    MENU_TOGGLE_AMBIENT_REFLECTION,
    MENU_TOGGLE_BACK_FACE_CULLING,
    MENU_TOGGLE_DIFFUSE_REFLECTION,
    MENU_TOGGLE_GOURAUD_SHADING,
    MENU_TOGGLE_POLYGON_FILL,
    MENU_TOGGLE_SPECULAR_REFLECTION,
    MENU_USE_FACE_NORMALS,
    MENU_USE_VERTEX_NORMALS,
    MENU_TOGGLE_NORMAL_HEDGEHOG,
    MENU_CYCLE_LIGHT_HEDGEHOG,
    MENU_STEP,
    MENU_TOGGLE_ANIMATION,
    MENU_TOGGLE_FIRST_PERSON,
    MENU_TOGGLE_PERSPECTIVE,
    MENU_TOGGLE_TEXTURES,
};


static void onMenuSelection(int menuDescriptor);
static void onKeyboardKey(unsigned char key, int, int);
static void onMouseButton(int button, int state, int x, int y);
static void onSpecialKey(int key, int x, int y);


void Controller::init()
{
    framework.keyboardFunc(onKeyboardKey);
    framework.mouseFunc(onMouseButton);
    framework.specialFunc(onSpecialKey);

    // build the pop-up menu and attach it to the right mouse button
    framework.createMenu(onMenuSelection);
    // these commands should be in alphabetical order (except for "Quit")
    framework.addMenuEntry("[r]eset view",
                                             MENU_RESET_VIEW);
    framework.addMenuEntry("step [Space]",
                                             MENU_STEP);
    framework.addMenuEntry("toggle [1]st person",
                                             MENU_TOGGLE_FIRST_PERSON);
    framework.addMenuEntry("toggle [A]mbient reflection",
                                             MENU_TOGGLE_AMBIENT_REFLECTION);
    framework.addMenuEntry("toggle [a]nimation", // no conflict with antialias
                                             MENU_TOGGLE_ANIMATION);
    framework.addMenuEntry("toggle [b]ack face culling",
                                             MENU_TOGGLE_BACK_FACE_CULLING);
    framework.addMenuEntry("toggle [c]oordinate axes",
                                             MENU_TOGGLE_COORDINATE_AXES);
    framework.addMenuEntry("toggle [D]iffuse reflection",
                                             MENU_TOGGLE_DIFFUSE_REFLECTION);
    framework.addMenuEntry("toggle [F]ull screen",
                                             MENU_TOGGLE_FULL_SCREEN);
    framework.addMenuEntry("toggle [G]ouraud/Phong shading",
                                             MENU_TOGGLE_GOURAUD_SHADING);
    framework.addMenuEntry("toggle view [h]elp",
                                             MENU_TOGGLE_VIEW_HELP);
    framework.addMenuEntry("cycle [L]ight hedgehog",
                                             MENU_CYCLE_LIGHT_HEDGEHOG);
    framework.addMenuEntry("toggle [N]ormal hedgehog",
                                             MENU_TOGGLE_NORMAL_HEDGEHOG);
    framework.addMenuEntry("toggle [P]erspective",
                                             MENU_TOGGLE_PERSPECTIVE);
    framework.addMenuEntry("toggle [p]olygon fill",
                                             MENU_TOGGLE_POLYGON_FILL);
    framework.addMenuEntry("toggle [S]pecular reflection",
                                             MENU_TOGGLE_SPECULAR_REFLECTION);
    framework.addMenuEntry("toggle [s]tats",
                                             MENU_TOGGLE_STATS);
    framework.addMenuEntry("toggle [t]extures",
                                             MENU_TOGGLE_TEXTURES);
    framework.addMenuEntry("use [f]ace normals",
                                             MENU_USE_FACE_NORMALS);
    framework.addMenuEntry("use [v]ertex normals",
                                             MENU_USE_VERTEX_NORMALS);
    framework.addMenuEntry("[q]uit",
                                             MENU_QUIT);
    framework.attachMenu(FRAMEWORK_RIGHT_BUTTON);
}

static void onKeyboardKey(unsigned char key, int, int)
//
// handle a keyboard press (of a normal ASCII character)
//
{
    switch (key) {

    case '1':
        onMenuSelection(MENU_TOGGLE_FIRST_PERSON);
        break;

    case ' ':
        onMenuSelection(MENU_STEP);
        break;

    case 'A':
        onMenuSelection(MENU_TOGGLE_AMBIENT_REFLECTION);
        break;

    case 'a': // no conflict, as PA ranges are disjoint
        onMenuSelection(MENU_TOGGLE_ANIMATION);
        break;

    case 'b':
        onMenuSelection(MENU_TOGGLE_BACK_FACE_CULLING);
        break;

    case 'c':
        onMenuSelection(MENU_TOGGLE_COORDINATE_AXES);
        break;

    case 'D':
        onMenuSelection(MENU_TOGGLE_DIFFUSE_REFLECTION);
        break;

    case 'F':
        onMenuSelection(MENU_TOGGLE_FULL_SCREEN);
        break;

    case 'G':
        onMenuSelection(MENU_TOGGLE_GOURAUD_SHADING);
        break;

    case 'h':
        onMenuSelection(MENU_TOGGLE_VIEW_HELP);
        break;

    case 'f':
        onMenuSelection(MENU_USE_FACE_NORMALS);
        break;

    case 'L':
        onMenuSelection(MENU_CYCLE_LIGHT_HEDGEHOG);
        break;

    case 'N':
        onMenuSelection(MENU_TOGGLE_NORMAL_HEDGEHOG);
        break;

    case 'P':
        onMenuSelection(MENU_TOGGLE_PERSPECTIVE);
        break;

    case 'p':
        onMenuSelection(MENU_TOGGLE_POLYGON_FILL);
        break;

    case 'q':
    case '\027': // ESC: common alternative (was '\e', MSVC++ doesn't like that)
        onMenuSelection(MENU_QUIT);
        break;

    case 'r':
        onMenuSelection(MENU_RESET_VIEW);
        break;

    case 'S':
        onMenuSelection(MENU_TOGGLE_SPECULAR_REFLECTION);
        break;

    case 's':
        onMenuSelection(MENU_TOGGLE_STATS);
        break;

    case 't':
        onMenuSelection(MENU_TOGGLE_TEXTURES);
        break;

    case 'v':
        onMenuSelection(MENU_USE_VERTEX_NORMALS);
        break;

    case '-':
        onMouseButton(FRAMEWORK_WHEEL_DOWN_BUTTON, FRAMEWORK_UP, 0, 0);
        break;

    case '+':
        onMouseButton(FRAMEWORK_WHEEL_UP_BUTTON, FRAMEWORK_UP, 0, 0);
        break;

    }
    // ignore unknown keyboard keys
}

static void nextFrame(bool restartFrameClock)
//
// advances to and displays the next frame
//
{
    static double tPrev = -1.0;
    double dT;

    if (restartFrameClock) {
        tPrev = clock_.read();
        dT = 0.01;
    } else {
        double t = clock_.read();
        dT = t - tPrev;
        tPrev = t;
    }
    scene->step(dT);
    view.display();
}

static void idle(void)
{
    nextFrame(false);
}

static void onMenuSelection(int menuDescriptor)
{

    switch (menuDescriptor) {

    case MENU_CYCLE_LIGHT_HEDGEHOG:
        {
        controller.lightHedgehogIndex++;
        //
        // If we've gone through all possible lights, turn the light
        // hedgehog off.
        //
        if (controller.lightHedgehogIndex
                >= static_cast<int>(scene->lights.size()))
            controller.lightHedgehogIndex = LIGHT_HEDGEHOG_DISABLED;
        controller.normalHedgehogEnabled = false;
        renderStats.resetTimeAveraging();
        renderStats.reset();
        view.display();
        }
        break;

    case MENU_RESET_VIEW:
        renderStats.resetTimeAveraging();
        view.reset(); // also resets camera
        // a reset turns off animation
        controller.animationEnabled = false;
        framework.idleFunc(NULL);
        view.display();
        break;

    case MENU_STEP:
        // a step turns off animation
        controller.animationEnabled = false;
        nextFrame(true);
        framework.idleFunc(NULL);
        view.display();
        break;

    case MENU_TOGGLE_AMBIENT_REFLECTION:
        controller.ambientReflectionEnabled
            = !controller.ambientReflectionEnabled;
        renderStats.resetTimeAveraging();
        renderStats.reset();
        view.display();
        break;

    case MENU_TOGGLE_ANIMATION:
        controller.animationEnabled = !controller.animationEnabled;
        nextFrame(true);
        framework.idleFunc(
            controller.animationEnabled
            ? idle
            : NULL
            );
        break;

    case MENU_TOGGLE_BACK_FACE_CULLING:
        controller.backFaceCullingEnabled = !controller.backFaceCullingEnabled;
        renderStats.resetTimeAveraging();
        renderStats.reset();
        view.display();
        break;

    case MENU_TOGGLE_COORDINATE_AXES:
        controller.axesEnabled = !controller.axesEnabled;
        view.display();
        break;

    case MENU_TOGGLE_DIFFUSE_REFLECTION:
        controller.diffuseReflectionEnabled
            = !controller.diffuseReflectionEnabled;
        renderStats.resetTimeAveraging();
        renderStats.reset();
        view.display();
        break;

    case MENU_TOGGLE_NORMAL_HEDGEHOG:
        controller.normalHedgehogEnabled
            = !controller.normalHedgehogEnabled;
        if (controller.normalHedgehogEnabled)
            controller.lightHedgehogIndex = LIGHT_HEDGEHOG_DISABLED;
        renderStats.resetTimeAveraging();
        renderStats.reset();
        view.display();
        break;

    case MENU_TOGGLE_FIRST_PERSON:
        controller.useFirstPerson = !controller.useFirstPerson;
        //
        // Usually, first person means perspective and third person
        // means orthographic.  (The user can always toggle
        // perspective later.)
        //
        controller.useOrthographic = !controller.useFirstPerson;
        renderStats.resetTimeAveraging();
        renderStats.reset();
        view.display();
        break;

    case MENU_TOGGLE_FULL_SCREEN:
        controller.fullScreenEnabled = !controller.fullScreenEnabled;
        view.setFullScreen(controller.fullScreenEnabled);
        renderStats.resetTimeAveraging();
        renderStats.reset();
        view.display();
        break;

    case MENU_TOGGLE_GOURAUD_SHADING:
        controller.useGouraudShading = !controller.useGouraudShading;
        renderStats.resetTimeAveraging();
        renderStats.reset();
        view.display();
        break;

    case MENU_TOGGLE_PERSPECTIVE:
        controller.useOrthographic = !controller.useOrthographic;
        renderStats.resetTimeAveraging();
        renderStats.reset();
        view.display();
        break;

    case MENU_TOGGLE_POLYGON_FILL:
        controller.fillPolygons = !controller.fillPolygons;
        renderStats.resetTimeAveraging();
        renderStats.reset();
        view.display();
        break;

    case MENU_TOGGLE_SPECULAR_REFLECTION:
        controller.specularReflectionEnabled
            = !controller.specularReflectionEnabled;
        renderStats.resetTimeAveraging();
        renderStats.reset();
        view.display();
        break;

    case MENU_TOGGLE_STATS:
        controller.statsEnabled = !controller.statsEnabled;
        view.display();
        break;

    case MENU_TOGGLE_TEXTURES:
        controller.useTextures = !controller.useTextures;
        renderStats.resetTimeAveraging();
        renderStats.reset();
        view.display();
        break;

    case MENU_TOGGLE_VIEW_HELP:
        controller.viewHelpEnabled = !controller.viewHelpEnabled;
        view.display();
        break;

    case MENU_USE_FACE_NORMALS:
        controller.useVertexNormals = false;
        renderStats.resetTimeAveraging();
        renderStats.reset();
        view.display();
        break;

    case MENU_USE_VERTEX_NORMALS:
        controller.useVertexNormals = true;
        renderStats.resetTimeAveraging();
        renderStats.reset();
        view.display();
        break;

    case MENU_QUIT:
        exit(EXIT_SUCCESS);
        break;

    default:
        assert(0); // unexpected menu entry
    }
}


static void onMouseButton(int button, int state, int x, int y)
{
    // number of clicks required to zoom or magnify by a factor of 2
    int nClicksPerDyad = 16;
    double factor = pow(2.0, 1.0/nClicksPerDyad);

    //
    // Mouse wheel events occur in sequential pairs of state =
    // FRAMEWORK_DOWN (0 for GLUT), then state = FRAMEWORK_UP (1 for
    // GLUT), so we only need to pay attention to one of them. We'll
    // choose the FRAMEWORK_UP one.
    //
    if (state == FRAMEWORK_UP) {
        switch (button) {

        case FRAMEWORK_WHEEL_DOWN_BUTTON:
            if (controller.useOrthographic)
                camera.magnify(1.0 / factor);
            else
                camera.zoom(1.0 / factor);
            view.display();
            break;

        case FRAMEWORK_WHEEL_UP_BUTTON:
            if (controller.useOrthographic)
                camera.magnify(factor);
            else
                camera.zoom(factor);
            view.display();
            break;

        // ignore other buttons
        }
    }
}


static void onSpecialKey(int key, int x, int y)
//
// handle a keyboard press (of a non-ASCII key)
//
{
    int modifiers = framework.getKeyModifiers();
    // fraction of the viewing volume to move on a single shift key press
    double shiftFraction = 0.01;

    switch (key) {

    case FRAMEWORK_KEY_UP:
        if (modifiers & FRAMEWORK_ACTIVE_SHIFT) {
            //
            // Ignore shifting in perspective viewing mode, as it
            // makes little sense.
            //
            if (controller.useOrthographic)
                camera.shift(0.0, -shiftFraction);
        } else
            camera.twist(-1.0, 0.0, 0.0);
        view.display();
        break;

    case FRAMEWORK_KEY_DOWN:
        if (modifiers & FRAMEWORK_ACTIVE_SHIFT) {
            if (controller.useOrthographic) // (as above)
                camera.shift(0, +shiftFraction);
        } else
            camera.twist(+1.0, 0.0, 0.0);
        view.display();
        break;

    case FRAMEWORK_KEY_LEFT:
        if (modifiers & FRAMEWORK_ACTIVE_SHIFT) {
            if (controller.useOrthographic) // (as above)
                camera.shift(+shiftFraction, 0);
        } else if (modifiers & FRAMEWORK_ACTIVE_CTRL)
            camera.twist(0.0, 0.0, -1.0);
        else
            camera.twist(0.0, -1.0, 0.0);
        view.display();
        break;

    case FRAMEWORK_KEY_RIGHT:
        if (modifiers & FRAMEWORK_ACTIVE_SHIFT) {
            if (controller.useOrthographic) // (as above)
                camera.shift(-shiftFraction, 0);
        } else if (modifiers & FRAMEWORK_ACTIVE_CTRL)
            camera.twist(0.0, 0.0, 1.0);
        else
            camera.twist(0.0, 1.0, 0.0);
        view.display();
        break;
    }
    // ignore unknown special keys
}
