#ifndef INCLUDED_N_ELEM

//
// This header file provides a useful little macro to compute the
// number of elements in an array. It can be used as a constant to
// initialize even a static const int to the length (number of
// elements) in an accompanying array.
//
// WARNING: This only works on an array whose size is known at
// compile time. If passed a pointer -- even a pointer to such an
// array -- it will always return "1", which is likely not to be what
// you want!
//
// USE WITH CAUTION!
//
#define N_ELEM(a) (sizeof(a)/sizeof(a[0]))

#define INCLUDED_N_ELEM
#endif // INCLUDED_N_ELEM
