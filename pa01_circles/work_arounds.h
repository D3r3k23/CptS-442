//
// This header file doesn't actually fix any bugs but it determines
// what "WORK_AROUND_*_BUG" #defines to set to enable various bug
// workarounds involving OpenGL (gl*() and glu*()) and GLUT (glut*())
// functions that don't behaved as advertised.
//
// This should be the only header file that cares about what platform
// (including GLUT/freeglut) we're compiling for. Any port to a new
// system will assume that no bugs are present, so modify this file
// if and when you find them.
//

#ifdef _WIN32
# if 1 // disable for testing
# define WORK_AROUND_GLUT_SWAP_NEEDS_GL_FLUSH_BUG
# endif
#endif

#ifdef __linux__
//
// The bug is present on *some* Ubuntu 10.04.2 systems, and I think
// it's there on 8.04, so we may need to be more specific.
//
//  machine   distro/OS     VM        VMdistro/OS  3dAcc  *glut     buggy?
//  =======   =========     ==        ===========  =====  =====     ======
//  peacock   Ubuntu 10.04  n/a       n/a          n/a    freeglut  no
//  stapledon Ubuntu 10.04  n/a       n/a          n/a    freeglut  yes
//
# if 1 // disable for testing
# define WORK_AROUND_GLUT_CHARS_NEED_FLUSH_BUG
# endif
#endif


#ifdef __linux__
# if 1 // disable for testing
# define WORK_AROUND_GLUT_INIT_BUG
# endif
#endif
#ifdef _WIN32 // assume MINGW
# if 1 // disable for testing
# define WORK_AROUND_GLUT_INIT_BUG
# endif
#endif

#ifdef __APPLE__
# if 1 // disable for testing
# define WORK_AROUND_GLUT_GL_ERROR_BUG
# endif
#endif

#if 1 // disable for testing
#define WORK_AROUND_CXX_RETURN_OVERLOADED_EXPRESSION_BUG
#endif

#ifdef __linux__
# if 1 // disable for testing
#define WORK_AROUND_FORCE_PTHREADS_LINK
# endif
#endif

#ifdef __APPLE__
# if 1 // disable for testing
#define WORK_AROUND_ENABLE_GLEW_EXPERIMENTAL
# endif
#endif
