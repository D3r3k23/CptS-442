#include <cassert>

#include <iostream>
#include <string>
using namespace std;

#include "check_gl.h"
#include "controller.h"
#include "framework.h"
#include "scene.h"
#include "view.h"
#include "work_arounds.h"

View view;

void View::display(void)
//
// redraws the view, starting from a blank screen
//
{
    CHECK_GL(glClearColor(0.5, 0.5, 1.0, 1.0));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT));
    scene->display();
    framework.swapBuffers();
}

