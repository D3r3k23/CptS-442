#include <cassert>
#include <cstdlib>

#include "controller.h"
#include "framework.h"
#include "scene.h"
#include "view.h"

Controller controller;

enum {
    MENU_FIRST, // placeholder so that menu entries start at 1 (leave alone!)

    // These options are always available.
    MENU_QUIT,
    // organize these by the PA in which they first appear, then alphabetically
    MENU_TOGGLE_ANTIALIAS,
};


static void onMenuSelection(int menuDescriptor);
static void onKeyboardKey(unsigned char key, int, int);


void Controller::init()
{
    framework.keyboardFunc(onKeyboardKey);

    // build the pop-up menu and attach it to the right mouse button
    framework.createMenu(onMenuSelection);
    // these commands should be in alphabetical order (except for "Quit")
    framework.addMenuEntry("toggle [a]ntialiasing",
                                             MENU_TOGGLE_ANTIALIAS);
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

    case 'a':
        onMenuSelection(MENU_TOGGLE_ANTIALIAS);
        break;

    case 'q':
    case '\027': // ESC: common alternative (was '\e', MSVC++ doesn't like that)
        onMenuSelection(MENU_QUIT);
        break;

    }
    // ignore unknown keyboard keys
}

static void onMenuSelection(int menuDescriptor)
{

    switch (menuDescriptor) {

    case MENU_TOGGLE_ANTIALIAS:
        controller.antialiasEnabled = !controller.antialiasEnabled;
        view.display();
        break;

    case MENU_QUIT:
        exit(EXIT_SUCCESS);
        break;

    default:
        assert(0); // unexpected menu entry
    }
}

