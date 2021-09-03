#include <string>

#include "check_gl.h"
#include "framework.h"
#include "shader_programs.h"
#include "text.h"

using namespace std;

Text text;


const void Text::renderAt(double xFrac, double yFrac,
                          string lines[], int nLines) const
{
    Color textColor = yellowColor;

    //
    // For now, we fall back on the default OpenGL shader program so
    // we can set the text color with glColor(). Eventually, we should
    // use a constant-color shader here, but that will require
    // knowledge of how (for instance) glutBitmapCharacter()
    // determines its pixel color.
    //
    ShaderProgram::disableCurrent();

    //
    // Enable a screen-specific coordinate system to work with
    // draws.  The modelview and projection transform matrix
    // push/pops are necessary to deal with the changing raster
    // position and viewing volume.
    //
    CHECK_GL(glRasterPos2d(0.0, 0.0));
    CHECK_GL(glMatrixMode(GL_MODELVIEW));
    CHECK_GL(glLoadIdentity());
    CHECK_GL(glColor3d(textColor.c.r, textColor.c.g, textColor.c.b));
    while (nLines--) {
        framework.bitmapStringAt(xFrac, yFrac, *lines);
        lines++;
        yFrac -= 0.05; // empirical: could be done with more font awareness
    }

	CHECK_GL(glMatrixMode(GL_MODELVIEW));
#ifdef WORK_AROUND_GLUT_CHARS_NEED_FLUSH_BUG
    framework.flushText();
#endif
}
