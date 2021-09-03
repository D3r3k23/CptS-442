#include <string>
#include <sstream>

#include "n_elem.h"
#include "render_stats.h"
#include "text.h"
#include "view.h"
#include "wrap_cmath_inclusion.h"


RenderStats renderStats;

void RenderStats::display()
{

    struct {
        string tag;
        bool isValid;
        int prec;      // -1 -> int, >= 0 -> = precision
        void *value;
    } lineSpecs[] = {
        { "vertices",                 true, -1, &ctVertices },
        { "lines",                    true, -1, &ctLines },
        { "line strips",              true, -1, &ctLineStrips },
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
        if (lineSpecs[i].prec >= 0) {
            sstrm.precision(lineSpecs[i].prec);
            sstrm << *static_cast<double *>(lineSpecs[i].value);
        } else {
            sstrm.precision(1);
            sstrm << *static_cast<int *>(lineSpecs[i].value);
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
}
