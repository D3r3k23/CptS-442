#ifndef INCLUDED_VIEW

//
// The "view" module provides the View class (see below).
//

#include "camera.h"
#include "geometry.h"

#include <string>
using namespace std;

class View
//
// provides view (e.g. viewport, canvas) functionality
//
{
    // We want the drawing canvas to be square and small enough to fit
    // on a typical LCD projector resolution of 1024x768 with window
    // and desktop decorations.
    static const int defaultCanvasWidth  = 600,
                     defaultCanvasHeight = 600; // of drawing area, in pixels

    int canvasWidth, canvasHeight;
    // used when toggling full screen mode
    int savedCanvasWidth, savedCanvasHeight;

public:
View(void)
    :   canvasWidth(      defaultCanvasWidth),
        canvasHeight(     defaultCanvasHeight),
        savedCanvasWidth( defaultCanvasWidth),
        savedCanvasHeight(defaultCanvasHeight)
    { };
    void init(int *argc, char **argv, string windowTitle);
    static void display(void); // "static" allows this to be used as a callback
    void displayViewHelp(void);
    void reset(void);
    static void reshape(int width, int height); // likewise
    void setFullScreen(bool makeItFull);

private:
    static void initTransforms(void);
};

extern View view;

#define INCLUDED_VIEW
#endif // INCLUDED_VIEW
