#ifndef WRAP_CMATH_INCLUSION_INCLUDED

//
// This include file takes care of including "cmath" on the
// appropriate platform. Any call to a math(3) function should include
// it.
//

#if   defined(__APPLE__)

#include <cmath>

#elif defined(_WIN32)

#define _USE_MATH_DEFINES // needed by Visual C++ to get (e.g.) M_PI
#include <cmath>
// On at least one platform, _USE_MATH_DEFINES doesn't get M_PI, soooo...
#ifndef M_PI
#include <math.h>
#endif

#elif defined(__linux__)

#include <cmath>

#endif

#define WRAP_CMATH_INCLUSION_INCLUDED
#endif // WRAP_CMATH_INCLUSION_INCLUDED
