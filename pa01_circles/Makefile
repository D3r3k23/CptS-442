PROG = $(shell basename $$PWD)

# formerly, this was just
#
#  SRCS = $(wildcard *.cpp)
#
# but this way will compile the most recent (and therefore probably
# buggiest) source first:
SRCS = $(shell ls -c *.cpp)

HDRS = $(wildcard *.h)

# UNAME is defined as the output of uname(1) in upper case, but under
# MinGW it's kind of lengthy and includes version information, which
# is not our intent, so let's trim it to "MINGW32" in that case and
# hope for broad compatibility.
UNAME = $(shell uname | sed 's/_.*//' | tr a-z A-Z)

BIN_EXTN = $(BIN_EXTN_$(UNAME))
BIN_EXTN_DARWIN =
BIN_EXTN_LINUX =
BIN_EXTN_MINGW32 = .exe

OBJS = $(SRCS:.cpp=.o)

# Set these to "-D" to enable or "-U" to disable the topic
OPTIONAL_TOPICS_FLAGS = \
	$(OPTIONAL_TOPICS_FLAGS_$(UNAME))

OPTIONAL_TOPICS_FLAGS_LINUX = \
	-UCOVER_ENVIRONMENT_MAPPING

OPTIONAL_TOPICS_FLAGS_DARWIN = \
	-UCOVER_ENVIRONMENT_MAPPING

CXXFLAGS = -g -Wall $(CXXFLAGS_$(UNAME))

# MACOS CXXFLAGS notes:
#
# - As of MacOS 10.9.5 (and maybe earlier), the GLUT library is
#   deprecated. To avoid the compile noise resulting from warnings, we
#   set the "-Wno-deprecated-declarations" option.
#
# - The macports package system puts its headers in
#   /opt/local/include, so we need to include that in the "#include"
#   search path.
#
CXXFLAGS_DARWIN = -Wno-deprecated-declarations -I/opt/local/include

CXXFLAGS_LINUX =

GLEW_TOP_DIR = \
	"C:\Program Files (x86)\NVIDIA Corporation\NVIDIA OpenGL SDK 10"

# Where GLEW's "GL" directory containing header files resides on Windows.
GLEW_INCLUDE_DIR = $(GLEW_TOP_DIR)"\common\GLEW\include"


GLUT_TOP_DIR = \
	"C:\Program Files (x86)\NVIDIA Corporation\NVIDIA OpenGL SDK 10"

# Where GLUT's "GL" directory containing header files resides on Windows.
GLUT_INCLUDE_DIR = $(GLUT_TOP_DIR)"\external\include"


# use this to get the glut and glew headers
CXXFLAGS_MINGW32 = \
	-I$(GLEW_INCLUDE_DIR) \
	-I$(GLUT_INCLUDE_DIR)


LDFLAGS = -g

LIBS =

LIBFLAGS = $(LIBFLAGS_$(UNAME))

# There are two possible GLUT libraries under MacOS, each selectable
# by uncommenting one (and only one) of the following LIBFLAGS_DARWIN
# macros...

# 1. The official one from Apple that's available as the GLUT
#    Framework.  This is an old, but official GLUT that doesn't, for
#    instance, support mouse wheels. If you want to (or must) use it,
#    enable this macro:
LIBFLAGS_DARWIN = -lGLEW -framework GLUT -framework OpenGL

# 2. The updated "freeglut" is available for MacOS, but must be
#    installed via "fink" (or maybe "macports"?). While it does
#    provide mouse wheel support, it also requires (and starts, if
#    needed) the Apple X11 server, which is a minor drawback. Assuming
#    it's in the default "fink" /sw/lib directory, you can select it
#    by selecting this macro:
#LIBFLAGS_DARWIN = -L/sw/lib -lfreeglut -framework OpenGL

#LIBFLAGS_LINUX = -lglut -lGLU

# In Ubuntu 12.04, the concluding "-lGL" was not necessary. In 13.04,
# it is. In any case, it doesn't hurt to include it. The "-pthread" is
# needed to work around a Ubuntu 14.04 bug (see "work_arounds.h").
LIBFLAGS_LINUX = -lGLEW -lglut -lGLU -lGL -pthread

LIBFLAGS_MINGW32 = -lglew32 -lglu32 -lopengl32

BIN_FILE = $(PROG)$(BIN_EXTN)

default: $(BIN_FILE)

all: $(BIN_FILE)

$(BIN_FILE): $(OBJS) $(LIBS)
	$(CXX) $(LDFLAGS) $^ $(LIBFLAGS) -o $@

# TODO: Figure out a way to remove *.obj (compiled) files on Windows
# that aren't OBJ model files.
clean:
	rm -rf *.o *~ \#*\# a.out core core.* gmon.out

immaculate: clean
	rm -rf $(BIN_FILE) $(BIN_FILE)_gprof

profile:
	$(MAKE) clean
	$(MAKE) CXXFLAGS="-pg $(CXXFLAGS)" LDFLAGS="-pg $(LDFLAGS)" $(BIN_FILE) \
	    && mv $(BIN_FILE) $(BIN_FILE)_gprof
	$(MAKE) clean

$(OBJS): $(HDRS)

