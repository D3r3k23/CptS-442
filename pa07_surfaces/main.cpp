#include <iostream>
#include <cstdlib>
using namespace std;
#include <getopt.h>

#include "controller.h"
#include "framework.h"
#include "scene.h"
#include "view.h"
#include "work_arounds.h"

#ifdef WORK_AROUND_FORCE_PTHREADS_LINK
#include <pthread.h>
#endif



static void help(char *progname)
{
    cout <<
        "syntax: " << progname << " [<option>]*\n"
        "where <option> is any of:\n"
        "  -h             Display (this) help message.\n"
        "  -l <name>      Select the track layout, where <name> is one of:\n"
        ;
    cout << "                 ";
    for (unsigned int i = 0; i < nTagOfLayout; i++) {
        if (i == nTagOfLayout-1)
            cout << "or ";
        cout << "\"" << tagOfLayout[i].tag << "\"";
        if (i != nTagOfLayout-1)
            cout << ", ";
    }
    cout << "\n";
    cout <<
        "  -t <filename>  Read the track bspline control vertices"
        " from <filename> (CSV format).\n"
        "                 This implies \"-l bspline\"."
        " (default: \"" DEFAULT_TRACK_BSPLINE_CVS_FNAME "\")\n";
}

#ifdef WORK_AROUND_FORCE_PTHREADS_LINK

void _do_not_call_me()
// dummy function to force libpthreads linkage (for bug fix)
{
    pthread_getconcurrency();
};

#endif

int main(int argc, char **argv)
{
    int ch;
    int status;
    Layout layout = LAYOUT_BSPLINE; // the default
    string trackBsplineCvsFname = DEFAULT_TRACK_BSPLINE_CVS_FNAME;

    while ((ch = getopt(argc, argv, "c:hl:t:")) != -1) {
        switch (ch) {

        case 'h':
            help(argv[0]);
            exit(EXIT_SUCCESS);
            break;

        case 'l':
            status = parseLayout(optarg, layout);
            assert(status);
            break;

        case 't':
            trackBsplineCvsFname = optarg;
            layout = LAYOUT_BSPLINE;
            break;

        default:
            cerr << "unknown command line option -- exiting\n";
            exit(EXIT_FAILURE);
        }
    }

    view.init(&argc, argv, argv[0]);
    controller.init();
    scene = new Scene(layout, trackBsplineCvsFname);
    framework.mainLoop();
}
