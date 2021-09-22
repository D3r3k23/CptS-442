#include <cassert>

#include <iostream>
#include <string>
using namespace std;

#include "check_gl.h"
#include "color.h"
#include "controller.h"
#include "framework.h"
#include "render_stats.h"
#include "scene.h"
#include "text.h"
#include "view.h"
#include "work_arounds.h"

View view;

void View::display(void)
//
// redraws the view, starting from a blank screen
//
{
    CHECK_GL(glClearColor(
                 static_cast<GLclampf>(scene->skyColor.c.r),
                 static_cast<GLclampf>(scene->skyColor.c.g),
                 static_cast<GLclampf>(scene->skyColor.c.b), 1.0));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT));
    CHECK_GL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    scene->display();
    renderStats.frameNumber++;
    framework.swapBuffers();
}


#include "n_elem.h"
void View::displayViewHelp(void)
{
    string orthographicHelpLines[] = {
        "key             camera motion",
        "---             ----------------------",
        "r               reset view",
        "Up              rotate +1 deg around x",
        "Down            rotate -1 deg around x",
        "Left            rotate -1 deg around y",
        "Right           rotate +1 deg around y",
        "Ctrl-Left       rotate +1 deg around z",
        "Ctrl-Right      rotate -1 deg around z",
        "Shift-Left      move left",
        "Shift-Right     move right",
        "Shift-Up        move up",
        "Shift-Down      move down",
        "Wheel-Up or +   zoom in",
        "Wheel-Down or - zoom out",
    };
    int nOrthographicHelpLines = N_ELEM(orthographicHelpLines);

    text.renderAt(-0.90, 0.0, orthographicHelpLines, nOrthographicHelpLines);
}


void View::reset(void)
{
    camera.reset(canvasWidth, canvasHeight);
}


void View::reshape(int newCanvasWidth, int newCanvasHeight)
//
// responds to a resize request
//
{
    camera.reshape(newCanvasWidth, newCanvasHeight);
    view.canvasWidth  = newCanvasWidth;
    view.canvasHeight = newCanvasHeight;
    CHECK_GL(glViewport(0, 0, view.canvasWidth, view.canvasHeight));
}


void View::setFullScreen(bool makeItFull)
{
    if (makeItFull) {
        savedCanvasWidth = canvasWidth;
        savedCanvasHeight = canvasHeight;
        framework.fullScreen();
    } else
        framework.reshapeWindow(savedCanvasWidth, savedCanvasHeight);
}

