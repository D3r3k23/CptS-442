#include <sys/time.h>
#include <cassert>

#include "clock.h"

double Clock::read()
{
    struct timeval tv;
    // default the timezone, as we don't need it
    int status = gettimeofday(&tv, 0);

    assert(status == 0);
    return tv.tv_sec + 1.0e-6 * tv.tv_usec;
}
Clock clock_;

#ifdef TEST
#include <iostream>
#include <iomanip>
using namespace std;

int main(int argc, char **argv)
{
    cout << setprecision(12) << clock_.read() << endl;
}
#endif
