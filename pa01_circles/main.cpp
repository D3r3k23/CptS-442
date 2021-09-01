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
        ;
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

    while ((ch = getopt(argc, argv, "h")) != -1) {
        switch (ch) {

        case 'h':
            help(argv[0]);
            exit(EXIT_SUCCESS);
            break;

        default:
            cerr << "unknown command line option -- exiting\n";
            exit(EXIT_FAILURE);
        }
    }

    view.init(&argc, argv, argv[0]);
    controller.init();
    scene = new Scene();
    framework.mainLoop();
}
