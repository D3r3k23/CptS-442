#include <string>
#include <sstream>

#include "controller.h"
#include "n_elem.h"
#include "render_stats.h"
#include "text.h"
#include "view.h"
#include "wrap_cmath_inclusion.h"


RenderStats renderStats;

void RenderStats::display()
{

    enum {
        FORMAT_YESNO,
        FORMAT_INT,
        FORMAT_FLOAT
    };
    struct {
        string tag;
        bool isValid;
        int format;
        void *value;
    } lineSpecs[] = {
        { "vertices",                        true,
          FORMAT_INT, &ctVertices },
        { "lines",                           true,
          FORMAT_INT, &ctLines },
        { "line strips",                     true,
          FORMAT_INT, &ctLineStrips },
        { "triangles (in irregular meshes)", true,
          FORMAT_INT, &ctTrianglesInIrregularMeshes },
        { "ambient reflection enabled?",     true,
          FORMAT_YESNO, &controller.ambientReflectionEnabled },
        { "diffuse reflection enabled?",     true,
          FORMAT_YESNO, &controller.diffuseReflectionEnabled },
        { "specular reflection enabled?",    true,
          FORMAT_YESNO, &controller.specularReflectionEnabled },
        { "triangles (in regular meshes)",
                                             true,
          FORMAT_INT, &ctTrianglesInRegularMeshes },
        { "triangle strips",                 true,
          FORMAT_INT, &ctTriangleStrips },
    };
    int maxLines = N_ELEM(lineSpecs);
    int nLines = 0;

    string *lines = new string[maxLines];
    for (int i = 0; i < maxLines; i++) {
        ostringstream sstrm;

        if (!lineSpecs[i].isValid)
            continue;
        sstrm << fixed;
        sstrm.width(10);
        switch (lineSpecs[i].format) {

        case FORMAT_YESNO:
            sstrm << ( *static_cast<bool *>(lineSpecs[i].value) ? "yes" : "no" );
            break;

        case FORMAT_INT:
            sstrm.precision(1);
            sstrm << *static_cast<int *>(lineSpecs[i].value);
            break;

        case FORMAT_FLOAT:
            sstrm.precision(1);
            sstrm << *static_cast<double *>(lineSpecs[i].value);
            break;

        }
        sstrm << " " << lineSpecs[i].tag;
        lines[nLines++] = sstrm.str();
    }

    text.renderAt(-0.90, 0.90, lines, nLines);
    delete [] lines;
}


void RenderStats::reset(void)
//
// Clear the all counts and mark the time. This is typically done at
// the start of a frame.
//
{
    ctVertices = 0;
    ctLines = 0;
    ctLineStrips = 0;
    ctTrianglesInIrregularMeshes = 0;
    ctTrianglesInRegularMeshes = 0;
    ctTriangleStrips = 0;
}
