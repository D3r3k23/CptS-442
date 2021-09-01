#ifndef INCLUDED_MINMAX

//
// This header file provides convenient (and generic) macros MAX() and
// MIN().
//

//
// To avoid any possible side effects. `a` and `b` should always be
// constant expressions or variable names.
//
#define MIN(a, b) ( (a) <= (b) ? (a) : (b) )
#define MAX(a, b) ( (a) >= (b) ? (a) : (b) )

#define INCLUDED_MINMAX
#endif // INCLUDED_MINMAX
